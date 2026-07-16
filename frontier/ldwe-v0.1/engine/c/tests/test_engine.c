#include "legion_engine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static legion_input_frame_t calm_input(uint64_t sequence) {
    legion_input_frame_t input;
    memset(&input, 0, sizeof(input));
    input.sequence = sequence;
    input.timestamp_ms = 1000 + sequence * 16;
    input.strength_q16 = UINT16_C(52428);
    input.variability_q16 = UINT16_C(3277);
    input.novelty_q16 = UINT16_C(2621);
    input.freshness_ms = 10;
    input.valid = 1;
    input.source = LEGION_SOURCE_SIMULATED;
    return input;
}

static void test_determinism(void) {
    legion_config_t config;
    legion_engine_t a;
    legion_engine_t b;
    legion_default_config(&config);
    legion_engine_init(&a);
    legion_engine_init(&b);

    for (uint64_t i = 0; i < 600; ++i) {
        legion_input_frame_t input = calm_input(i);
        if (i > 150 && i < 350) {
            input.variability_q16 = UINT16_C(45875);
            input.novelty_q16 = UINT16_C(52428);
        }
        legion_pose_frame_t pa;
        legion_pose_frame_t pb;
        legion_engine_step(&a, &config, &input, &pa);
        legion_engine_step(&b, &config, &input, &pb);
        assert(memcmp(&pa, &pb, sizeof(pa)) == 0);
    }
}

static void test_stale_is_immediate(void) {
    legion_config_t config;
    legion_engine_t engine;
    legion_default_config(&config);
    legion_engine_init(&engine);

    legion_input_frame_t input = calm_input(0);
    input.valid = 0;
    input.freshness_ms = 999999;
    input.source = LEGION_SOURCE_STALE_WLAN;

    legion_pose_frame_t pose;
    legion_engine_step(&engine, &config, &input, &pose);
    assert(pose.behavior == LEGION_BEHAVIOR_STALE);
}

static void test_limits(void) {
    legion_config_t config;
    legion_engine_t engine;
    legion_default_config(&config);
    legion_engine_init(&engine);

    const int32_t limits[LEGION_JOINT_COUNT] = {
        8000, 18000, 18000, 22000, 36000, 36000, 32000, 36000, 40000
    };

    for (uint64_t i = 0; i < 5000; ++i) {
        legion_input_frame_t input = calm_input(i);
        input.strength_q16 = LEGION_FEATURE_MAX;
        input.variability_q16 = LEGION_FEATURE_MAX;
        input.novelty_q16 = LEGION_FEATURE_MAX;
        legion_pose_frame_t pose;
        legion_engine_step(&engine, &config, &input, &pose);
        for (size_t j = 0; j < LEGION_JOINT_COUNT; ++j) {
            assert(pose.joint_mdeg[j] >= -limits[j]);
            assert(pose.joint_mdeg[j] <= limits[j]);
        }
    }
}

static void test_input_changes_output(void) {
    legion_config_t config;
    legion_engine_t a;
    legion_engine_t b;
    legion_default_config(&config);
    legion_engine_init(&a);
    legion_engine_init(&b);

    legion_input_frame_t low = calm_input(0);
    legion_input_frame_t high = calm_input(0);
    high.strength_q16 = LEGION_FEATURE_MAX;
    high.variability_q16 = LEGION_FEATURE_MAX;
    high.novelty_q16 = LEGION_FEATURE_MAX;

    legion_pose_frame_t pa;
    legion_pose_frame_t pb;
    legion_engine_step(&a, &config, &low, &pa);
    legion_engine_step(&b, &config, &high, &pb);
    assert(pa.frame_hash != pb.frame_hash);
}

int main(void) {
    test_determinism();
    test_stale_is_immediate();
    test_limits();
    test_input_changes_output();
    puts("legion_engine: all C tests passed");
    return 0;
}
