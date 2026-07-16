use legion_engine::{Config, Engine, InputFrame, Source};
use std::env;
use std::io::{self, Write};
use std::net::TcpStream;
use std::process::Command;
use std::thread;
use std::time::{Duration, Instant, SystemTime, UNIX_EPOCH};

fn now_ms() -> u64 {
    SystemTime::now()
        .duration_since(UNIX_EPOCH)
        .unwrap_or_default()
        .as_millis() as u64
}

fn parse_first_percent(text: &str) -> Option<u16> {
    text.lines().find_map(|line| {
        let trimmed = line.trim();
        if !trimmed.ends_with('%') {
            return None;
        }
        let value = trimmed.rsplit_once(':')?.1.trim().trim_end_matches('%');
        value.parse::<u16>().ok().filter(|v| *v <= 100)
    })
}

fn read_windows_quality() -> Result<u16, String> {
    let output = Command::new("netsh")
        .args(["wlan", "show", "interfaces"])
        .output()
        .map_err(|e| format!("netsh launch failed: {e}"))?;
    if !output.status.success() {
        return Err(String::from_utf8_lossy(&output.stderr).to_string());
    }
    let text = String::from_utf8_lossy(&output.stdout);
    parse_first_percent(&text).ok_or_else(|| "signal quality not found".to_string())
}

fn isqrt_u64(value: u64) -> u32 {
    if value == 0 {
        return 0;
    }
    let mut x = value;
    let mut y = (x + 1) >> 1;
    while y < x {
        x = y;
        y = (x + value / x) >> 1;
    }
    x.min(u32::MAX as u64) as u32
}

#[derive(Default)]
struct Window {
    values: [u16; 16],
    len: usize,
    cursor: usize,
    previous: Option<u16>,
}

impl Window {
    fn push(&mut self, value: u16) -> (u16, u16) {
        self.values[self.cursor] = value;
        self.cursor = (self.cursor + 1) % self.values.len();
        self.len = self.len.saturating_add(1).min(self.values.len());

        let mean = self.values[..self.len]
            .iter()
            .map(|v| *v as i64)
            .sum::<i64>() / self.len as i64;
        let variance = self.values[..self.len]
            .iter()
            .map(|v| {
                let d = *v as i64 - mean;
                d * d
            })
            .sum::<i64>() / self.len as i64;
        let standard_deviation = isqrt_u64(variance as u64);
        let variability = standard_deviation.saturating_mul(4).min(65_535) as u16;
        let novelty = self.previous
            .map(|p| value.abs_diff(p).saturating_mul(4))
            .unwrap_or(0);
        self.previous = Some(value);
        (variability, novelty)
    }
}

fn main() -> io::Result<()> {
    let address = env::args()
        .nth(1)
        .unwrap_or_else(|| "127.0.0.1:19331".to_string());
    let simulate = env::args().any(|a| a == "--simulate");
    let mut socket = TcpStream::connect(&address)?;
    socket.set_nodelay(true)?;

    let cfg = Config::default();
    let mut engine = Engine::default();
    let mut window = Window::default();
    let mut sequence = 0u64;
    let mut current_quality = 80u16;
    let mut valid = simulate;
    let mut last_probe = Instant::now() - Duration::from_secs(1);

    loop {
        if last_probe.elapsed() >= Duration::from_millis(500) {
            if simulate {
                let phase = (sequence % 600) as u32;
                let triangle = if phase < 300 { phase } else { 600 - phase };
                current_quality = (32_768u32 + (triangle * 43_690u32) / 300).min(65_535) as u16;
                valid = true;
            } else {
                match read_windows_quality() {
                    Ok(percent) => {
                        current_quality = ((percent as u32 * 65535) / 100) as u16;
                        valid = true;
                    }
                    Err(error) => {
                        eprintln!("{error}");
                        valid = false;
                    }
                }
            }
            last_probe = Instant::now();
        }

        let (variability, novelty) = window.push(current_quality);
        let input = InputFrame {
            sequence,
            timestamp_ms: now_ms(),
            strength_q16: current_quality,
            variability_q16: variability,
            novelty_q16: novelty,
            freshness_ms: if valid { 10 } else { 999_999 },
            valid,
            source: if simulate {
                Source::Simulated
            } else if valid {
                Source::LiveWlan
            } else {
                Source::StaleWlan
            },
        };
        let line = engine.step(&cfg, input).to_json();
        socket.write_all(line.as_bytes())?;
        socket.write_all(b"\n")?;
        sequence = sequence.wrapping_add(1);
        thread::sleep(Duration::from_millis(16));
    }
}
