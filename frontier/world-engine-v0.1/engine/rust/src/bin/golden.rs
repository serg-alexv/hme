use std::env;
use std::fs::{self, File};
use std::io::{self, BufWriter, Write};
use std::path::Path;
use world_engine::{Config, Engine, InputFrame, Source};

fn invalid_data(message: impl Into<String>) -> io::Error {
    io::Error::new(io::ErrorKind::InvalidData, message.into())
}

fn source_from_code(code: u8) -> io::Result<Source> {
    match code {
        0 => Ok(Source::Simulated),
        1 => Ok(Source::RecordedReplay),
        2 => Ok(Source::LiveWlan),
        3 => Ok(Source::StaleWlan),
        _ => Err(invalid_data(format!("unknown source code {code}"))),
    }
}

fn main() -> io::Result<()> {
    let output_path = env::args()
        .nth(1)
        .unwrap_or_else(|| "build/golden_pose.jsonl".to_string());
    let schedule_path = env::args().nth(2).unwrap_or_else(|| {
        format!(
            "{}/../fixtures/golden_schedule.csv",
            env!("CARGO_MANIFEST_DIR")
        )
    });
    if let Some(parent) = Path::new(&output_path).parent() {
        fs::create_dir_all(parent)?;
    }

    let schedule = fs::read_to_string(&schedule_path)?;
    let mut output = BufWriter::new(File::create(&output_path)?);
    let config = Config::default();
    let mut engine = Engine::default();
    let mut expected_sequence = 0u64;
    for (line_index, line) in schedule.lines().enumerate() {
        if line.is_empty() || line.starts_with('#') {
            continue;
        }
        let fields: Vec<&str> = line.split(',').collect();
        if fields.len() != 8 {
            return Err(invalid_data(format!(
                "schedule line {} has {} fields",
                line_index + 1,
                fields.len()
            )));
        }
        let start = fields[0]
            .parse::<u64>()
            .map_err(|_| invalid_data(format!("invalid start on line {}", line_index + 1)))?;
        let end = fields[1]
            .parse::<u64>()
            .map_err(|_| invalid_data(format!("invalid end on line {}", line_index + 1)))?;
        let strength_q16 = fields[2]
            .parse::<u16>()
            .map_err(|_| invalid_data(format!("invalid strength on line {}", line_index + 1)))?;
        let variability_q16 = fields[3]
            .parse::<u16>()
            .map_err(|_| invalid_data(format!("invalid variability on line {}", line_index + 1)))?;
        let novelty_q16 = fields[4]
            .parse::<u16>()
            .map_err(|_| invalid_data(format!("invalid novelty on line {}", line_index + 1)))?;
        let freshness_ms = fields[5]
            .parse::<u32>()
            .map_err(|_| invalid_data(format!("invalid freshness on line {}", line_index + 1)))?;
        let valid = match fields[6] {
            "0" => false,
            "1" => true,
            _ => {
                return Err(invalid_data(format!(
                    "invalid validity on line {}",
                    line_index + 1
                )))
            }
        };
        let source =
            source_from_code(fields[7].parse::<u8>().map_err(|_| {
                invalid_data(format!("invalid source on line {}", line_index + 1))
            })?)?;
        if start != expected_sequence || end < start {
            return Err(invalid_data(format!(
                "non-contiguous range on line {}",
                line_index + 1
            )));
        }

        for sequence in start..=end {
            let input = InputFrame {
                sequence,
                timestamp_ms: 1_700_000_000_000 + (sequence * 1_000) / 60,
                strength_q16,
                variability_q16,
                novelty_q16,
                freshness_ms,
                valid,
                source,
            };
            writeln!(output, "{}", engine.step(&config, input).to_json())?;
        }
        expected_sequence = end + 1;
    }
    if expected_sequence != 720 {
        return Err(invalid_data(format!(
            "schedule produced {expected_sequence} frames, expected 720"
        )));
    }
    output.flush()?;
    eprintln!("Wrote 720 frames to {output_path}");
    Ok(())
}
