#ifndef WORLD_ENGINE_H
#define WORLD_ENGINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define WORLD_ENGINE_VERSION "world-engine/0.1.0"
#define WORLD_FEATURE_MAX UINT16_C(65535)
#define WORLD_WAVE_COUNT 7
#define WORLD_JOINT_COUNT 9

typedef enum {
    WORLD_SOURCE_SIMULATED = 0,
    WORLD_SOURCE_RECORDED_REPLAY = 1,
    WORLD_SOURCE_LIVE_WLAN = 2,
    WORLD_SOURCE_STALE_WLAN = 3
} world_source_t;

typedef enum {
    WORLD_BEHAVIOR_CALM = 0,
    WORLD_BEHAVIOR_ATTUNE = 1,
    WORLD_BEHAVIOR_STORM = 2,
    WORLD_BEHAVIOR_STALE = 3
} world_behavior_t;

typedef struct {
    uint64_t sequence;
    uint64_t timestamp_ms;
    uint16_t strength_q16;
    uint16_t variability_q16;
    uint16_t novelty_q16;
    uint32_t freshness_ms;
    uint8_t valid;
    uint8_t source;
} world_input_frame_t;

typedef struct {
    uint32_t stale_after_ms;
    uint16_t attune_enter_q16;
    uint16_t attune_exit_q16;
    uint16_t storm_variability_enter_q16;
    uint16_t storm_novelty_enter_q16;
    uint16_t storm_variability_exit_q16;
    uint16_t storm_novelty_exit_q16;
    uint16_t dwell_attune_ticks;
    uint16_t dwell_storm_ticks;
    uint16_t dwell_calm_ticks;
    uint16_t dwell_recover_ticks;
} world_config_t;

typedef struct {
    uint64_t tick;
    uint64_t last_sequence;
    uint32_t phase_q32[WORLD_WAVE_COUNT];
    uint16_t candidate_ticks;
    uint8_t behavior;
    uint8_t candidate;
} world_engine_t;

typedef struct {
    uint64_t sequence;
    uint64_t tick;
    uint64_t timestamp_ms;
    uint64_t frame_hash;
    uint32_t freshness_ms;
    uint16_t strength_q16;
    uint16_t variability_q16;
    uint16_t novelty_q16;
    uint8_t valid;
    uint8_t source;
    uint8_t behavior;
    int32_t root_y_um;
    int32_t joint_mdeg[WORLD_JOINT_COUNT];
} world_pose_frame_t;

void world_default_config(world_config_t *config);
void world_engine_init(world_engine_t *engine);
void world_engine_step(
    world_engine_t *engine,
    const world_config_t *config,
    const world_input_frame_t *input,
    world_pose_frame_t *output
);

const char *world_behavior_name(world_behavior_t behavior);
const char *world_source_name(world_source_t source);
uint64_t world_pose_hash(const world_pose_frame_t *pose);
int world_pose_to_json(
    const world_pose_frame_t *pose,
    char *buffer,
    size_t capacity
);

#ifdef __cplusplus
}
#endif

#endif
