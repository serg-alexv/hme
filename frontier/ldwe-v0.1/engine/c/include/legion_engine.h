#ifndef LEGION_ENGINE_H
#define LEGION_ENGINE_H

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

#define LEGION_ENGINE_VERSION "ldwe/0.1.0"
#define LEGION_FEATURE_MAX UINT16_C(65535)
#define LEGION_WAVE_COUNT 7
#define LEGION_JOINT_COUNT 9

typedef enum {
    LEGION_SOURCE_SIMULATED = 0,
    LEGION_SOURCE_RECORDED_REPLAY = 1,
    LEGION_SOURCE_LIVE_WLAN = 2,
    LEGION_SOURCE_STALE_WLAN = 3
} legion_source_t;

typedef enum {
    LEGION_BEHAVIOR_CALM = 0,
    LEGION_BEHAVIOR_ATTUNE = 1,
    LEGION_BEHAVIOR_STORM = 2,
    LEGION_BEHAVIOR_STALE = 3
} legion_behavior_t;

typedef struct {
    uint64_t sequence;
    uint64_t timestamp_ms;
    uint16_t strength_q16;
    uint16_t variability_q16;
    uint16_t novelty_q16;
    uint32_t freshness_ms;
    uint8_t valid;
    uint8_t source;
} legion_input_frame_t;

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
} legion_config_t;

typedef struct {
    uint64_t tick;
    uint64_t last_sequence;
    uint32_t phase_q32[LEGION_WAVE_COUNT];
    uint16_t candidate_ticks;
    uint8_t behavior;
    uint8_t candidate;
} legion_engine_t;

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
    int32_t joint_mdeg[LEGION_JOINT_COUNT];
} legion_pose_frame_t;

void legion_default_config(legion_config_t *config);
void legion_engine_init(legion_engine_t *engine);
void legion_engine_step(
    legion_engine_t *engine,
    const legion_config_t *config,
    const legion_input_frame_t *input,
    legion_pose_frame_t *output
);

const char *legion_behavior_name(legion_behavior_t behavior);
const char *legion_source_name(legion_source_t source);
uint64_t legion_pose_hash(const legion_pose_frame_t *pose);
int legion_pose_to_json(
    const legion_pose_frame_t *pose,
    char *buffer,
    size_t capacity
);

#ifdef __cplusplus
}
#endif

#endif
