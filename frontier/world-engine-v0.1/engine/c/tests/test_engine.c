#include "world_engine.h"

#include <assert.h>
#include <stdio.h>
#include <string.h>

static world_input_frame_t calm_input(uint64_t sequence) {
    world_input_frame_t input;
    memset(&input, 0, sizeof(input));
    input.sequence = sequence;
    input.timestamp_ms = 1000 + (sequence * UINT64_C(1000)) / UINT64_C(60);
    input.strength_q16 = UINT16_C(52428);
    input.variability_q16 = UINT16_C(3277);
    input.novelty_q16 = UINT16_C(2621);
    input.freshness_ms = 10;
    input.valid = 1;
    input.source = WORLD_SOURCE_SIMULATED;
    return input;
}

static void test_determinism(void) {
    world_config_t config;
    world_engine_t a;
    world_engine_t b;
    world_default_config(&config);
    world_engine_init(&a);
    world_engine_init(&b);

    for (uint64_t i = 0; i < 600; ++i) {
        world_input_frame_t input = calm_input(i);
        if (i > 150 && i < 350) {
            input.variability_q16 = UINT16_C(45875);
            input.novelty_q16 = UINT16_C(52428);
        }
        world_pose_frame_t pa;
        world_pose_frame_t pb;
        world_engine_step(&a, &config, &input, &pa);
        world_engine_step(&b, &config, &input, &pb);
        assert(memcmp(&pa, &pb, sizeof(pa)) == 0);
    }
}

static void test_stale_is_immediate(void) {
    world_config_t config;
    world_engine_t engine;
    world_default_config(&config);
    world_engine_init(&engine);

    world_input_frame_t input = calm_input(0);
    input.valid = 0;
    input.freshness_ms = 999999;
    input.source = WORLD_SOURCE_STALE_WLAN;

    world_pose_frame_t pose;
    world_engine_step(&engine, &config, &input, &pose);
    assert(pose.behavior == WORLD_BEHAVIOR_STALE);
}

static void test_stale_recovery_is_calm_first(void) {
    world_config_t config;
    world_engine_t engine;
    world_default_config(&config);
    world_engine_init(&engine);

    world_input_frame_t input = calm_input(0);
    input.valid = 0;
    input.freshness_ms = 999999;
    input.source = WORLD_SOURCE_STALE_WLAN;

    world_pose_frame_t pose;
    world_engine_step(&engine, &config, &input, &pose);
    assert(pose.behavior == WORLD_BEHAVIOR_STALE);

    for (uint64_t i = 0; i < config.dwell_recover_ticks; ++i) {
        input = calm_input(i + 1);
        input.strength_q16 = WORLD_FEATURE_MAX;
        input.variability_q16 = WORLD_FEATURE_MAX;
        input.novelty_q16 = WORLD_FEATURE_MAX;
        world_engine_step(&engine, &config, &input, &pose);
        if (i + 1 < config.dwell_recover_ticks) assert(pose.behavior == WORLD_BEHAVIOR_STALE);
    }
    assert(pose.behavior == WORLD_BEHAVIOR_CALM);
}

static void test_limits(void) {
    world_config_t config;
    world_engine_t engine;
    world_default_config(&config);
    world_engine_init(&engine);

    const int32_t limits[WORLD_JOINT_COUNT] = {
        8000, 18000, 18000, 22000, 36000, 36000, 32000, 36000, 40000
    };

    for (uint64_t i = 0; i < 5000; ++i) {
        world_input_frame_t input = calm_input(i);
        input.strength_q16 = WORLD_FEATURE_MAX;
        input.variability_q16 = WORLD_FEATURE_MAX;
        input.novelty_q16 = WORLD_FEATURE_MAX;
        world_pose_frame_t pose;
        world_engine_step(&engine, &config, &input, &pose);
        for (size_t j = 0; j < WORLD_JOINT_COUNT; ++j) {
            assert(pose.joint_mdeg[j] >= -limits[j]);
            assert(pose.joint_mdeg[j] <= limits[j]);
        }
    }
}

static void test_input_changes_output(void) {
    world_config_t config;
    world_engine_t a;
    world_engine_t b;
    world_default_config(&config);
    world_engine_init(&a);
    world_engine_init(&b);

    world_input_frame_t low = calm_input(0);
    world_input_frame_t high = calm_input(0);
    high.strength_q16 = WORLD_FEATURE_MAX;
    high.variability_q16 = WORLD_FEATURE_MAX;
    high.novelty_q16 = WORLD_FEATURE_MAX;

    world_pose_frame_t pa;
    world_pose_frame_t pb;
    world_engine_step(&a, &config, &low, &pa);
    world_engine_step(&b, &config, &high, &pb);
    assert(pa.frame_hash != pb.frame_hash);
}

static void test_attune_can_enter_storm(void) {
    world_config_t config;
    world_engine_t engine;
    world_default_config(&config);
    world_engine_init(&engine);

    world_pose_frame_t pose = {0};
    for (uint64_t i = 0; i < config.dwell_attune_ticks; ++i) {
        world_input_frame_t input = calm_input(i);
        input.variability_q16 = UINT16_C(22282);
        input.novelty_q16 = UINT16_C(31457);
        world_engine_step(&engine, &config, &input, &pose);
    }
    assert(pose.behavior == WORLD_BEHAVIOR_ATTUNE);

    for (uint64_t i = 0; i < config.dwell_storm_ticks; ++i) {
        world_input_frame_t input = calm_input(UINT64_C(100) + i);
        input.variability_q16 = UINT16_C(55049);
        input.novelty_q16 = UINT16_C(57671);
        world_engine_step(&engine, &config, &input, &pose);
    }
    assert(pose.behavior == WORLD_BEHAVIOR_STORM);
}

int main(void) {
    test_determinism();
    test_stale_is_immediate();
    test_stale_recovery_is_calm_first();
    test_limits();
    test_input_changes_output();
    test_attune_can_enter_storm();
    puts("world_engine: all C tests passed");
    return 0;
}
