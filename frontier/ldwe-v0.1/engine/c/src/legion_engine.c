#include "legion_engine.h"

#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define Q16_ONE UINT32_C(65535)

enum {
    W_BREATH = 0,
    W_ORIENT = 1,
    W_WING = 2,
    W_TAIL1 = 3,
    W_TAIL2 = 4,
    W_TAIL3 = 5,
    W_TREMOR = 6
};

enum {
    J_ROOT_ROLL = 0,
    J_NECK1_PITCH = 1,
    J_NECK2_PITCH = 2,
    J_HEAD_YAW = 3,
    J_WING_L_ROLL = 4,
    J_WING_R_ROLL = 5,
    J_TAIL1_YAW = 6,
    J_TAIL2_YAW = 7,
    J_TAIL3_YAW = 8
};

static const int32_t SIN_QUARTER_Q30[257] = {
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
};

static const uint32_t BASE_PHASE_STEP[LEGION_WAVE_COUNT] = {
    UINT32_C(20043181), UINT32_C(13600730), UINT32_C(85899346),
    UINT32_C(50107952), UINT32_C(50107952), UINT32_C(50107952),
    UINT32_C(536870912)
};

static const uint32_t INITIAL_PHASE[LEGION_WAVE_COUNT] = {
    UINT32_C(0), UINT32_C(536870912), UINT32_C(0), UINT32_C(0),
    UINT32_C(3987458440), UINT32_C(3681291408), UINT32_C(1073741824)
};

static const int32_t AMP_MDEG[4][LEGION_WAVE_COUNT] = {
    { 1200, 1000, 2500, 4200, 5000, 5800, 350 },
    { 1800, 3600, 8500, 7200, 8600, 9800, 900 },
    { 3000, 6500, 22000, 13000, 15500, 18000, 3500 },
    { 500, 700, 1800, 1600, 1900, 2200, 0 }
};

static const uint32_t STATE_FREQ_Q16[4] = {
    UINT32_C(65535), UINT32_C(104858), UINT32_C(235930), UINT32_C(32768)
};

static const int32_t BIND_Q15[LEGION_JOINT_COUNT][LEGION_WAVE_COUNT] = {
    { 4096, 16384, 0, 0, 0, 0, 4096 },
    { 12288, 24576, 0, 0, 0, 0, 4096 },
    { 8192, -16384, 0, 0, 0, 0, 3072 },
    { 0, 32768, 0, 0, 0, 0, 6144 },
    { 0, 0, 32768, 0, 0, 0, 4096 },
    { 0, 0, -32768, 0, 0, 0, -4096 },
    { 0, 0, 0, 32768, 0, 0, 1024 },
    { 0, 0, 0, 0, 32768, 0, 1536 },
    { 0, 0, 0, 0, 0, 32768, 2048 }
};

static const int32_t JOINT_LIMIT_MDEG[LEGION_JOINT_COUNT] = {
    8000, 18000, 18000, 22000, 36000, 36000, 32000, 36000, 40000
};

enum {
    LEGION_Q16_ATTUNE_ENTER = 16384,
    LEGION_Q16_ATTUNE_EXIT = 10486,
    LEGION_Q16_STORM_VARIABILITY_ENTER = 36044,
    LEGION_Q16_STORM_NOVELTY_ENTER = 39321,
    LEGION_Q16_STORM_VARIABILITY_EXIT = 24903,
    LEGION_Q16_STORM_NOVELTY_EXIT = 26214
};

static int32_t clamp_i32(int64_t value, int32_t low, int32_t high) {
    if (value < low) return low;
    if (value > high) return high;
    return (int32_t)value;
}

static int32_t sin_q30(uint32_t phase) {
    const uint32_t quadrant = phase >> 30;
    const uint32_t within = phase & UINT32_C(0x3fffffff);
    uint32_t index = within >> 22;
    uint32_t fraction = within & UINT32_C(0x003fffff);
    if (index > 255) index = 255;

    int64_t a;
    int64_t b;
    if (quadrant == 0 || quadrant == 2) {
        a = SIN_QUARTER_Q30[index];
        b = SIN_QUARTER_Q30[index + 1];
    } else {
        a = SIN_QUARTER_Q30[256 - index];
        b = SIN_QUARTER_Q30[255 - index];
    }
    int64_t value = a + (((b - a) * (int64_t)fraction) >> 22);
    if (quadrant >= 2) value = -value;
    return (int32_t)value;
}

static legion_behavior_t candidate_for(legion_behavior_t current, const legion_config_t *config, const legion_input_frame_t *input) {
    if (!input->valid || input->freshness_ms > config->stale_after_ms) return LEGION_BEHAVIOR_STALE;

    const bool storm_enter = input->variability_q16 >= config->storm_variability_enter_q16 && input->novelty_q16 >= config->storm_novelty_enter_q16;
    const bool storm_hold = input->variability_q16 >= config->storm_variability_exit_q16 && input->novelty_q16 >= config->storm_novelty_exit_q16;
    const bool attune_enter = input->variability_q16 >= config->attune_enter_q16 || input->novelty_q16 >= config->attune_enter_q16;
    const bool calm_region = input->variability_q16 <= config->attune_exit_q16 && input->novelty_q16 <= config->attune_exit_q16;

    if (current == LEGION_BEHAVIOR_STORM) return storm_hold ? LEGION_BEHAVIOR_STORM : LEGION_BEHAVIOR_ATTUNE;
    if (storm_enter) return LEGION_BEHAVIOR_STORM;
    if (current == LEGION_BEHAVIOR_ATTUNE) return calm_region ? LEGION_BEHAVIOR_CALM : LEGION_BEHAVIOR_ATTUNE;
    if (current == LEGION_BEHAVIOR_STALE) return LEGION_BEHAVIOR_CALM;
    return attune_enter ? LEGION_BEHAVIOR_ATTUNE : LEGION_BEHAVIOR_CALM;
}

static uint16_t dwell_for(legion_behavior_t current, legion_behavior_t candidate, const legion_config_t *config) {
    if (candidate == LEGION_BEHAVIOR_STALE) return 1;
    if (candidate == LEGION_BEHAVIOR_STORM) return config->dwell_storm_ticks;
    if (candidate == LEGION_BEHAVIOR_ATTUNE) return config->dwell_attune_ticks;
    if (current == LEGION_BEHAVIOR_STALE) return config->dwell_recover_ticks;
    return config->dwell_calm_ticks;
}

static uint64_t fnv1a64_u64(uint64_t hash, uint64_t value) {
    for (unsigned i = 0; i < 8; ++i) {
        hash ^= (uint8_t)(value & UINT64_C(0xff));
        hash *= UINT64_C(1099511628211);
        value >>= 8;
    }
    return hash;
}

static uint64_t fnv1a64_u32(uint64_t hash, uint32_t value) {
    return fnv1a64_u64(hash, value);
}

void legion_default_config(legion_config_t *config) {
    if (!config) return;
    config->stale_after_ms = 1500;
    config->attune_enter_q16 = LEGION_Q16_ATTUNE_ENTER;
    config->attune_exit_q16 = LEGION_Q16_ATTUNE_EXIT;
    config->storm_variability_enter_q16 = LEGION_Q16_STORM_VARIABILITY_ENTER;
    config->storm_novelty_enter_q16 = LEGION_Q16_STORM_NOVELTY_ENTER;
    config->storm_variability_exit_q16 = LEGION_Q16_STORM_VARIABILITY_EXIT;
    config->storm_novelty_exit_q16 = LEGION_Q16_STORM_NOVELTY_EXIT;
    config->dwell_attune_ticks = 8;
    config->dwell_storm_ticks = 6;
    config->dwell_calm_ticks = 24;
    config->dwell_recover_ticks = 20;
}

void legion_engine_init(legion_engine_t *engine) {
    if (!engine) return;
    memset(engine, 0, sizeof(*engine));
    engine->behavior = LEGION_BEHAVIOR_CALM;
    engine->candidate = LEGION_BEHAVIOR_CALM;
    for (size_t i = 0; i < LEGION_WAVE_COUNT; ++i) engine->phase_q32[i] = INITIAL_PHASE[i];
}

const char *legion_behavior_name(legion_behavior_t behavior) {
    switch (behavior) {
        case LEGION_BEHAVIOR_CALM: return "CALM";
        case LEGION_BEHAVIOR_ATTUNE: return "ATTUNE";
        case LEGION_BEHAVIOR_STORM: return "STORM";
        case LEGION_BEHAVIOR_STALE: return "STALE";
        default: return "UNKNOWN";
    }
}

const char *legion_source_name(legion_source_t source) {
    switch (source) {
        case LEGION_SOURCE_SIMULATED: return "SIMULATED";
        case LEGION_SOURCE_RECORDED_REPLAY: return "RECORDED_REPLAY";
        case LEGION_SOURCE_LIVE_WLAN: return "LIVE_WLAN";
        case LEGION_SOURCE_STALE_WLAN: return "STALE_WLAN";
        default: return "UNKNOWN";
    }
}

uint64_t legion_pose_hash(const legion_pose_frame_t *pose) {
    uint64_t hash = UINT64_C(1469598103934665603);
    hash = fnv1a64_u64(hash, pose->sequence);
    hash = fnv1a64_u64(hash, pose->tick);
    hash = fnv1a64_u64(hash, pose->timestamp_ms);
    hash = fnv1a64_u32(hash, pose->freshness_ms);
    hash = fnv1a64_u32(hash, pose->strength_q16);
    hash = fnv1a64_u32(hash, pose->variability_q16);
    hash = fnv1a64_u32(hash, pose->novelty_q16);
    hash = fnv1a64_u32(hash, pose->valid);
    hash = fnv1a64_u32(hash, pose->source);
    hash = fnv1a64_u32(hash, pose->behavior);
    hash = fnv1a64_u32(hash, (uint32_t)pose->root_y_um);
    for (size_t i = 0; i < LEGION_JOINT_COUNT; ++i) hash = fnv1a64_u32(hash, (uint32_t)pose->joint_mdeg[i]);
    return hash;
}

void legion_engine_step(legion_engine_t *engine, const legion_config_t *config, const legion_input_frame_t *input, legion_pose_frame_t *output) {
    if (!engine || !config || !input || !output) return;

    legion_behavior_t current = (legion_behavior_t)engine->behavior;
    legion_behavior_t candidate = candidate_for(current, config, input);

    if ((uint8_t)candidate != engine->candidate) {
        engine->candidate = (uint8_t)candidate;
        engine->candidate_ticks = 1;
    } else if (engine->candidate_ticks < UINT16_MAX) {
        engine->candidate_ticks++;
    }

    const uint16_t required = dwell_for(current, candidate, config);
    if (engine->candidate_ticks >= required && candidate != current) {
        engine->behavior = (uint8_t)candidate;
        engine->candidate_ticks = 0;
        current = candidate;
    } else {
        current = (legion_behavior_t)engine->behavior;
    }

    int32_t wave_mdeg[LEGION_WAVE_COUNT] = {0};
    const uint32_t state_freq = STATE_FREQ_Q16[current];
    const uint32_t activity = Q16_ONE + ((uint32_t)input->variability_q16 >> 1) + ((uint32_t)input->novelty_q16 >> 1);
    const uint32_t strength_gain = (Q16_ONE / 2) + ((uint32_t)input->strength_q16 >> 1);

    for (size_t i = 0; i < LEGION_WAVE_COUNT; ++i) {
        uint64_t step = (uint64_t)BASE_PHASE_STEP[i] * state_freq;
        step = (step >> 16) * activity;
        step >>= 16;
        if (step > UINT32_MAX) step = UINT32_MAX;
        engine->phase_q32[i] += (uint32_t)step;

        int64_t amplitude = AMP_MDEG[current][i];
        amplitude = (amplitude * strength_gain) >> 16;
        if (i == W_WING || i == W_TREMOR) amplitude += (amplitude * input->novelty_q16) >> 17;
        if (i >= W_TAIL1 && i <= W_TAIL3) amplitude += (amplitude * input->variability_q16) >> 17;
        wave_mdeg[i] = (int32_t)(((int64_t)sin_q30(engine->phase_q32[i]) * amplitude) >> 30);
    }

    memset(output, 0, sizeof(*output));
    output->sequence = input->sequence;
    output->tick = engine->tick;
    output->timestamp_ms = input->timestamp_ms;
    output->freshness_ms = input->freshness_ms;
    output->strength_q16 = input->strength_q16;
    output->variability_q16 = input->variability_q16;
    output->novelty_q16 = input->novelty_q16;
    output->valid = input->valid;
    output->source = input->source;
    output->behavior = (uint8_t)current;

    const int32_t root_amp_um[4] = {26000, 45000, 75000, 12000};
    output->root_y_um = (int32_t)(((int64_t)sin_q30(engine->phase_q32[W_BREATH]) * root_amp_um[current]) >> 30);
    if (current == LEGION_BEHAVIOR_STALE) output->root_y_um -= 24000;

    for (size_t joint = 0; joint < LEGION_JOINT_COUNT; ++joint) {
        int64_t sum = 0;
        for (size_t wave = 0; wave < LEGION_WAVE_COUNT; ++wave) sum += ((int64_t)BIND_Q15[joint][wave] * wave_mdeg[wave]) >> 15;

        if (current == LEGION_BEHAVIOR_ATTUNE) {
            if (joint == J_NECK1_PITCH) sum -= 4500;
            if (joint == J_WING_L_ROLL) sum += 5000;
            if (joint == J_WING_R_ROLL) sum -= 5000;
        } else if (current == LEGION_BEHAVIOR_STALE) {
            if (joint == J_NECK1_PITCH || joint == J_NECK2_PITCH) sum += 8500;
            if (joint == J_WING_L_ROLL) sum -= 12000;
            if (joint == J_WING_R_ROLL) sum += 12000;
        }

        output->joint_mdeg[joint] = clamp_i32(sum, -JOINT_LIMIT_MDEG[joint], JOINT_LIMIT_MDEG[joint]);
    }

    output->frame_hash = legion_pose_hash(output);
    engine->last_sequence = input->sequence;
    engine->tick++;
}

int legion_pose_to_json(const legion_pose_frame_t *pose, char *buffer, size_t capacity) {
    if (!pose || !buffer || capacity == 0) return -1;
    return snprintf(
        buffer, capacity,
        "{\"schema\":\"legion.pose/0.1\","
        "\"sequence\":%" PRIu64 ",\"tick\":%" PRIu64 ","
        "\"timestamp_unix_ms\":%" PRIu64 ","
        "\"source\":\"%s\",\"valid\":%s,"
        "\"freshness_ms\":%" PRIu32 ",\"behavior\":\"%s\","
        "\"features_q16\":{\"strength\":%" PRIu16 ",\"variability\":%" PRIu16 ",\"novelty\":%" PRIu16 "},"
        "\"pose\":{\"root_y_um\":%" PRId32 ",\"root_roll_mdeg\":%" PRId32 ","
        "\"neck1_pitch_mdeg\":%" PRId32 ",\"neck2_pitch_mdeg\":%" PRId32 ","
        "\"head_yaw_mdeg\":%" PRId32 ",\"wing_l_roll_mdeg\":%" PRId32 ","
        "\"wing_r_roll_mdeg\":%" PRId32 ",\"tail1_yaw_mdeg\":%" PRId32 ","
        "\"tail2_yaw_mdeg\":%" PRId32 ",\"tail3_yaw_mdeg\":%" PRId32 "},"
        "\"frame_hash\":\"%016" PRIx64 "\"}",
        pose->sequence, pose->tick, pose->timestamp_ms,
        legion_source_name((legion_source_t)pose->source), pose->valid ? "true" : "false",
        pose->freshness_ms, legion_behavior_name((legion_behavior_t)pose->behavior),
        pose->strength_q16, pose->variability_q16, pose->novelty_q16,
        pose->root_y_um, pose->joint_mdeg[J_ROOT_ROLL], pose->joint_mdeg[J_NECK1_PITCH],
        pose->joint_mdeg[J_NECK2_PITCH], pose->joint_mdeg[J_HEAD_YAW],
        pose->joint_mdeg[J_WING_L_ROLL], pose->joint_mdeg[J_WING_R_ROLL],
        pose->joint_mdeg[J_TAIL1_YAW], pose->joint_mdeg[J_TAIL2_YAW],
        pose->joint_mdeg[J_TAIL3_YAW], pose->frame_hash
    );
}
