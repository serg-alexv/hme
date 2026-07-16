#include "legion_engine.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static legion_input_frame_t input_for(uint64_t sequence) {
    legion_input_frame_t input;
    memset(&input, 0, sizeof(input));
    input.sequence = sequence;
    input.timestamp_ms = UINT64_C(1700000000000) + sequence * UINT64_C(16);
    input.freshness_ms = 10;
    input.valid = 1;
    input.source = LEGION_SOURCE_SIMULATED;

    const uint64_t phase = sequence / 180;
    if (phase == 0) {
        input.strength_q16 = UINT16_C(57671);
        input.variability_q16 = UINT16_C(3932);
        input.novelty_q16 = UINT16_C(2621);
    } else if (phase == 1) {
        input.strength_q16 = UINT16_C(47185);
        input.variability_q16 = UINT16_C(22282);
        input.novelty_q16 = UINT16_C(31457);
    } else if (phase == 2) {
        input.strength_q16 = UINT16_C(27525);
        input.variability_q16 = UINT16_C(55049);
        input.novelty_q16 = UINT16_C(57671);
    } else {
        input.valid = 0;
        input.source = LEGION_SOURCE_STALE_WLAN;
        input.freshness_ms = 999999;
    }
    return input;
}

int main(int argc, char **argv) {
    const char *output_path = argc > 1 ? argv[1] : "build/golden_pose.jsonl";
    FILE *output = fopen(output_path, "wb");
    if (!output) {
        perror(output_path);
        return 2;
    }

    legion_config_t config;
    legion_engine_t engine;
    legion_default_config(&config);
    legion_engine_init(&engine);

    char line[2048];
    for (uint64_t sequence = 0; sequence < 720; ++sequence) {
        const legion_input_frame_t input = input_for(sequence);
        legion_pose_frame_t pose;
        legion_engine_step(&engine, &config, &input, &pose);
        const int length = legion_pose_to_json(&pose, line, sizeof(line));
        if (length < 0 || (size_t)length >= sizeof(line)) {
            fclose(output);
            return 3;
        }
        fprintf(output, "%s\n", line);
    }
    fclose(output);
    fprintf(stderr, "Wrote 720 frames to %s\n", output_path);
    return 0;
}
