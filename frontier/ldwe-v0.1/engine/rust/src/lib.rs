#![forbid(unsafe_code)]

pub const FEATURE_MAX: u16 = u16::MAX;
pub const WAVE_COUNT: usize = 7;
pub const JOINT_COUNT: usize = 9;

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u8)]
pub enum Source {
    Simulated = 0,
    RecordedReplay = 1,
    LiveWlan = 2,
    StaleWlan = 3,
}

impl Source {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Simulated => "SIMULATED",
            Self::RecordedReplay => "RECORDED_REPLAY",
            Self::LiveWlan => "LIVE_WLAN",
            Self::StaleWlan => "STALE_WLAN",
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
#[repr(u8)]
pub enum Behavior {
    Calm = 0,
    Attune = 1,
    Storm = 2,
    Stale = 3,
}

impl Behavior {
    pub fn as_str(self) -> &'static str {
        match self {
            Self::Calm => "CALM",
            Self::Attune => "ATTUNE",
            Self::Storm => "STORM",
            Self::Stale => "STALE",
        }
    }
}

#[derive(Clone, Copy, Debug)]
pub struct InputFrame {
    pub sequence: u64,
    pub timestamp_ms: u64,
    pub strength_q16: u16,
    pub variability_q16: u16,
    pub novelty_q16: u16,
    pub freshness_ms: u32,
    pub valid: bool,
    pub source: Source,
}

#[derive(Clone, Copy, Debug)]
pub struct Config {
    pub stale_after_ms: u32,
    pub attune_enter_q16: u16,
    pub attune_exit_q16: u16,
    pub storm_variability_enter_q16: u16,
    pub storm_novelty_enter_q16: u16,
    pub storm_variability_exit_q16: u16,
    pub storm_novelty_exit_q16: u16,
    pub dwell_attune_ticks: u16,
    pub dwell_storm_ticks: u16,
    pub dwell_calm_ticks: u16,
    pub dwell_recover_ticks: u16,
}

impl Default for Config {
    fn default() -> Self {
        Self {
            stale_after_ms: 1500,
            attune_enter_q16: 16_384,
            attune_exit_q16: 10_486,
            storm_variability_enter_q16: 36_044,
            storm_novelty_enter_q16: 39_321,
            storm_variability_exit_q16: 24_903,
            storm_novelty_exit_q16: 26_214,
            dwell_attune_ticks: 8,
            dwell_storm_ticks: 6,
            dwell_calm_ticks: 24,
            dwell_recover_ticks: 20,
        }
    }
}

#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub struct PoseFrame {
    pub sequence: u64,
    pub tick: u64,
    pub timestamp_ms: u64,
    pub frame_hash: u64,
    pub freshness_ms: u32,
    pub strength_q16: u16,
    pub variability_q16: u16,
    pub novelty_q16: u16,
    pub valid: bool,
    pub source: Source,
    pub behavior: Behavior,
    pub root_y_um: i32,
    pub joint_mdeg: [i32; JOINT_COUNT],
}

impl PoseFrame {
    pub fn to_json(self) -> String {
        format!(
            concat!(
                "{\"schema\":\"legion.pose/0.1\",",
                "\"sequence\":{},\"tick\":{},\"timestamp_unix_ms\":{},",
                "\"source\":\"{}\",\"valid\":{},\"freshness_ms\":{},",
                "\"behavior\":\"{}\",",
                "\"features_q16\":{\"strength\":{},\"variability\":{},\"novelty\":{}},",
                "\"pose\":{\"root_y_um\":{},\"root_roll_mdeg\":{},",
                "\"neck1_pitch_mdeg\":{},\"neck2_pitch_mdeg\":{},",
                "\"head_yaw_mdeg\":{},\"wing_l_roll_mdeg\":{},",
                "\"wing_r_roll_mdeg\":{},\"tail1_yaw_mdeg\":{},",
                "\"tail2_yaw_mdeg\":{},\"tail3_yaw_mdeg\":{}},",
                "\"frame_hash\":\"{:016x}\"}"
            ),
            self.sequence,
            self.tick,
            self.timestamp_ms,
            self.source.as_str(),
            self.valid,
            self.freshness_ms,
            self.behavior.as_str(),
            self.strength_q16,
            self.variability_q16,
            self.novelty_q16,
            self.root_y_um,
            self.joint_mdeg[0],
            self.joint_mdeg[1],
            self.joint_mdeg[2],
            self.joint_mdeg[3],
            self.joint_mdeg[4],
            self.joint_mdeg[5],
            self.joint_mdeg[6],
            self.joint_mdeg[7],
            self.joint_mdeg[8],
            self.frame_hash
        )
    }
}

pub const fn q16_ratio(numerator: u32, denominator: u32) -> u16 {
    if denominator == 0 || numerator >= denominator {
        u16::MAX
    } else {
        (((numerator as u64 * 65_535) + (denominator as u64 / 2)) / denominator as u64) as u16
    }
}

const SIN_QUARTER_Q30: [i32; 257] = [
    0, 6588356, 13176464, 19764076, 26350943, 32936819, 39521455, 46104602,
    52686014, 59265442, 65842639, 72417357, 78989349, 85558366, 92124163, 98686491,
    105245103, 111799753, 118350194, 124896179, 131437462, 137973796, 144504935, 151030634,
    157550647, 164064728, 170572633, 177074115, 183568930, 190056834, 196537583, 203010932,
    209476638, 215934457, 222384147, 228825464, 235258165, 241682010, 248096755, 254502159,
    260897982, 267283981, 273659918, 280025552, 286380643, 292724951, 299058239, 305380268,
    311690799, 317989595, 324276419, 330551034, 336813204, 343062693, 349299266, 355522689,
    361732726, 367929144, 374111709, 380280190, 386434353, 392573967, 398698801, 404808624,
    410903207, 416982319, 423045732, 429093217, 435124548, 441139496, 447137835, 453119340,
    459083786, 465030947, 470960600, 476872522, 482766489, 488642281, 494499676, 500338453,
    506158392, 511959275, 517740883, 523502998, 529245404, 534967884, 540670223, 546352205,
    552013618, 557654248, 563273883, 568872310, 574449320, 580004702, 585538248, 591049748,
    596538995, 602005783, 607449906, 612871159, 618269338, 623644239, 628995660, 634323400,
    639627258, 644907034, 650162530, 655393548, 660599890, 665781362, 670937767, 676068911,
    681174602, 686254647, 691308855, 696337036, 701339000, 706314559, 711263525, 716185713,
    721080937, 725949013, 730789757, 735602987, 740388522, 745146182, 749875788, 754577161,
    759250125, 763894504, 768510122, 773096806, 777654384, 782182683, 786681534, 791150767,
    795590213, 799999706, 804379079, 808728167, 813046808, 817334838, 821592095, 825818421,
    830013654, 834177638, 838310216, 842411232, 846480531, 850517961, 854523370, 858496606,
    862437520, 866345964, 870221790, 874064853, 877875009, 881652112, 885396022, 889106597,
    892783698, 896427186, 900036924, 903612776, 907154608, 910662286, 914135678, 917574653,
    920979082, 924348837, 927683790, 930983817, 934248793, 937478595, 940673101, 943832191,
    946955747, 950043650, 953095785, 956112036, 959092290, 962036435, 964944360, 967815955,
    970651112, 973449725, 976211688, 978936898, 981625251, 984276646, 986890984, 989468165,
    992008094, 994510675, 996975812, 999403415, 1001793390, 1004145648, 1006460100, 1008736660,
    1010975242, 1013175761, 1015338134, 1017462281, 1019548121, 1021595575, 1023604567, 1025575020,
    1027506862, 1029400018, 1031254418, 1033069992, 1034846671, 1036584389, 1038283080, 1039942680,
    1041563127, 1043144360, 1044686319, 1046188946, 1047652185, 1049075980, 1050460278, 1051805027,
    1053110176, 1054375676, 1055601479, 1056787540, 1057933813, 1059040255, 1060106826, 1061133483,
    1062120190, 1063066909, 1063973603, 1064840240, 1065666786, 1066453210, 1067199483, 1067905576,
    1068571464, 1069197120, 1069782521, 1070327646, 1070832474, 1071296985, 1071721163, 1072104991,
    1072448455, 1072751542, 1073014240, 1073236540, 1073418433, 1073559913, 1073660973, 1073721611,
    1073741824
];

const BASE_PHASE_STEP: [u32; WAVE_COUNT] = [
    20_043_181, 13_600_730, 85_899_346, 50_107_952, 50_107_952, 50_107_952, 536_870_912,
];
const INITIAL_PHASE: [u32; WAVE_COUNT] = [
    0, 536_870_912, 0, 0, 3_987_458_440, 3_681_291_408, 1_073_741_824,
];
const STATE_FREQ_Q16: [u32; 4] = [65_535, 104_858, 235_930, 32_768];
const AMP_MDEG: [[i32; WAVE_COUNT]; 4] = [
    [1200, 1000, 2500, 4200, 5000, 5800, 350],
    [1800, 3600, 8500, 7200, 8600, 9800, 900],
    [3000, 6500, 22000, 13000, 15500, 18000, 3500],
    [500, 700, 1800, 1600, 1900, 2200, 0],
];
const BIND_Q15: [[i32; WAVE_COUNT]; JOINT_COUNT] = [
    [4096, 16384, 0, 0, 0, 0, 4096],
    [12288, 24576, 0, 0, 0, 0, 4096],
    [8192, -16384, 0, 0, 0, 0, 3072],
    [0, 32768, 0, 0, 0, 0, 6144],
    [0, 0, 32768, 0, 0, 0, 4096],
    [0, 0, -32768, 0, 0, 0, -4096],
    [0, 0, 0, 32768, 0, 0, 1024],
    [0, 0, 0, 0, 32768, 0, 1536],
    [0, 0, 0, 0, 0, 32768, 2048],
];
const JOINT_LIMIT_MDEG: [i32; JOINT_COUNT] =
    [8000, 18000, 18000, 22000, 36000, 36000, 32000, 36000, 40000];

fn sin_q30(phase: u32) -> i32 {
    let quadrant = phase >> 30;
    let within = phase & 0x3fff_ffff;
    let index = (within >> 22).min(255) as usize;
    let fraction = (within & 0x003f_ffff) as i64;
    let (a, b) = if quadrant == 0 || quadrant == 2 {
        (SIN_QUARTER_Q30[index] as i64, SIN_QUARTER_Q30[index + 1] as i64)
    } else {
        (SIN_QUARTER_Q30[256 - index] as i64, SIN_QUARTER_Q30[255 - index] as i64)
    };
    let mut value = a + (((b - a) * fraction) >> 22);
    if quadrant >= 2 { value = -value; }
    value as i32
}

#[derive(Clone, Debug)]
pub struct Engine {
    tick: u64,
    phase_q32: [u32; WAVE_COUNT],
    behavior: Behavior,
    candidate: Behavior,
    candidate_ticks: u16,
}

impl Default for Engine {
    fn default() -> Self {
        Self {
            tick: 0,
            phase_q32: INITIAL_PHASE,
            behavior: Behavior::Calm,
            candidate: Behavior::Calm,
            candidate_ticks: 0,
        }
    }
}

impl Engine {
    fn candidate_for(&self, cfg: &Config, input: &InputFrame) -> Behavior {
        if !input.valid || input.freshness_ms > cfg.stale_after_ms { return Behavior::Stale; }
        let storm_enter = input.variability_q16 >= cfg.storm_variability_enter_q16 && input.novelty_q16 >= cfg.storm_novelty_enter_q16;
        let storm_hold = input.variability_q16 >= cfg.storm_variability_exit_q16 && input.novelty_q16 >= cfg.storm_novelty_exit_q16;
        let attune_enter = input.variability_q16 >= cfg.attune_enter_q16 || input.novelty_q16 >= cfg.attune_enter_q16;
        let calm_region = input.variability_q16 <= cfg.attune_exit_q16 && input.novelty_q16 <= cfg.attune_exit_q16;

        match self.behavior {
            Behavior::Storm => if storm_hold { Behavior::Storm } else { Behavior::Attune },
            Behavior::Attune => if storm_enter { Behavior::Storm } else if calm_region { Behavior::Calm } else { Behavior::Attune },
            Behavior::Stale => Behavior::Calm,
            Behavior::Calm => if storm_enter { Behavior::Storm } else if attune_enter { Behavior::Attune } else { Behavior::Calm },
        }
    }

    fn dwell_for(&self, cfg: &Config, candidate: Behavior) -> u16 {
        match candidate {
            Behavior::Stale => 1,
            Behavior::Storm => cfg.dwell_storm_ticks,
            Behavior::Attune => cfg.dwell_attune_ticks,
            Behavior::Calm if self.behavior == Behavior::Stale => cfg.dwell_recover_ticks,
            Behavior::Calm => cfg.dwell_calm_ticks,
        }
    }

    pub fn step(&mut self, cfg: &Config, input: InputFrame) -> PoseFrame {
        let candidate = self.candidate_for(cfg, &input);
        if candidate != self.candidate {
            self.candidate = candidate;
            self.candidate_ticks = 1;
        } else {
            self.candidate_ticks = self.candidate_ticks.saturating_add(1);
        }
        if candidate != self.behavior && self.candidate_ticks >= self.dwell_for(cfg, candidate) {
            self.behavior = candidate;
            self.candidate_ticks = 0;
        }

        let state = self.behavior as usize;
        let activity = 65_535u32 + (input.variability_q16 as u32 >> 1) + (input.novelty_q16 as u32 >> 1);
        let strength_gain = 32_767u32 + (input.strength_q16 as u32 >> 1);
        let mut waves = [0i32; WAVE_COUNT];

        for i in 0..WAVE_COUNT {
            let step = (((BASE_PHASE_STEP[i] as u64 * STATE_FREQ_Q16[state] as u64) >> 16) * activity as u64) >> 16;
            self.phase_q32[i] = self.phase_q32[i].wrapping_add(step.min(u32::MAX as u64) as u32);
            let mut amp = (AMP_MDEG[state][i] as i64 * strength_gain as i64) >> 16;
            if i == 2 || i == 6 { amp += (amp * input.novelty_q16 as i64) >> 17; }
            if (3..=5).contains(&i) { amp += (amp * input.variability_q16 as i64) >> 17; }
            waves[i] = ((sin_q30(self.phase_q32[i]) as i64 * amp) >> 30) as i32;
        }

        let root_amp = [26_000i64, 45_000, 75_000, 12_000][state];
        let mut root_y_um = ((sin_q30(self.phase_q32[0]) as i64 * root_amp) >> 30) as i32;
        if self.behavior == Behavior::Stale { root_y_um -= 24_000; }

        let mut joints = [0i32; JOINT_COUNT];
        for j in 0..JOINT_COUNT {
            let mut sum = 0i64;
            for i in 0..WAVE_COUNT { sum += (BIND_Q15[j][i] as i64 * waves[i] as i64) >> 15; }
            if self.behavior == Behavior::Attune {
                if j == 1 { sum -= 4500; }
                if j == 4 { sum += 5000; }
                if j == 5 { sum -= 5000; }
            } else if self.behavior == Behavior::Stale {
                if j == 1 || j == 2 { sum += 8500; }
                if j == 4 { sum -= 12000; }
                if j == 5 { sum += 12000; }
            }
            joints[j] = sum.clamp(-(JOINT_LIMIT_MDEG[j] as i64), JOINT_LIMIT_MDEG[j] as i64) as i32;
        }

        let mut pose = PoseFrame {
            sequence: input.sequence,
            tick: self.tick,
            timestamp_ms: input.timestamp_ms,
            frame_hash: 0,
            freshness_ms: input.freshness_ms,
            strength_q16: input.strength_q16,
            variability_q16: input.variability_q16,
            novelty_q16: input.novelty_q16,
            valid: input.valid,
            source: input.source,
            behavior: self.behavior,
            root_y_um,
            joint_mdeg: joints,
        };
        pose.frame_hash = pose_hash(&pose);
        self.tick = self.tick.wrapping_add(1);
        pose
    }
}

fn fnv_u64(mut hash: u64, mut value: u64) -> u64 {
    for _ in 0..8 {
        hash ^= value & 0xff;
        hash = hash.wrapping_mul(1_099_511_628_211);
        value >>= 8;
    }
    hash
}

pub fn pose_hash(p: &PoseFrame) -> u64 {
    let mut h = 1_469_598_103_934_665_603u64;
    for value in [
        p.sequence, p.tick, p.timestamp_ms, p.freshness_ms as u64,
        p.strength_q16 as u64, p.variability_q16 as u64, p.novelty_q16 as u64,
        p.valid as u64, p.source as u8 as u64, p.behavior as u8 as u64,
        p.root_y_um as u32 as u64,
    ] {
        h = fnv_u64(h, value);
    }
    for angle in p.joint_mdeg { h = fnv_u64(h, angle as u32 as u64); }
    h
}

#[cfg(test)]
mod tests {
    use super::*;

    #[test]
    fn deterministic() {
        let cfg = Config::default();
        let mut a = Engine::default();
        let mut b = Engine::default();
        for i in 0..1000 {
            let input = InputFrame {
                sequence: i,
                timestamp_ms: 1000 + i * 16,
                strength_q16: 45_875,
                variability_q16: if i > 200 { 52_428 } else { 3_277 },
                novelty_q16: if i > 200 { 58_982 } else { 2_621 },
                freshness_ms: 10,
                valid: true,
                source: Source::Simulated,
            };
            assert_eq!(a.step(&cfg, input), b.step(&cfg, input));
        }
    }
}
