#include "world_engine.h"

#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv) {
    const char *output_path = argc > 1 ? argv[1] : "build/golden_pose.jsonl";
    const char *schedule_path = argc > 2 ? argv[2] : "../fixtures/golden_schedule.csv";
    FILE *schedule = fopen(schedule_path, "rb");
    if (!schedule) {
        perror(schedule_path);
        return 2;
    }
    FILE *output = fopen(output_path, "wb");
    if (!output) {
        perror(output_path);
        fclose(schedule);
        return 2;
    }

    world_config_t config;
    world_engine_t engine;
    world_default_config(&config);
    world_engine_init(&engine);

    char row[512];
    char line[2048];
    uint64_t expected_sequence = 0;
    while (fgets(row, sizeof(row), schedule)) {
        if (row[0] == '#' || row[0] == '\n' || row[0] == '\r') continue;

        uint64_t start;
        uint64_t end;
        unsigned strength;
        unsigned variability;
        unsigned novelty;
        unsigned freshness;
        unsigned valid;
        unsigned source;
        const int fields = sscanf(
            row,
            "%" SCNu64 ",%" SCNu64 ",%u,%u,%u,%u,%u,%u",
            &start, &end, &strength, &variability, &novelty, &freshness, &valid, &source
        );
        if (fields != 8 || start != expected_sequence || end < start ||
            strength > UINT16_MAX || variability > UINT16_MAX || novelty > UINT16_MAX ||
            valid > 1 || source > WORLD_SOURCE_STALE_WLAN) {
            fprintf(stderr, "Invalid golden schedule row: %s", row);
            fclose(schedule);
            fclose(output);
            return 3;
        }

        for (uint64_t sequence = start; sequence <= end; ++sequence) {
            world_input_frame_t input;
            memset(&input, 0, sizeof(input));
            input.sequence = sequence;
            input.timestamp_ms = UINT64_C(1700000000000) + (sequence * UINT64_C(1000)) / UINT64_C(60);
            input.strength_q16 = (uint16_t)strength;
            input.variability_q16 = (uint16_t)variability;
            input.novelty_q16 = (uint16_t)novelty;
            input.freshness_ms = freshness;
            input.valid = (uint8_t)valid;
            input.source = (uint8_t)source;

            world_pose_frame_t pose;
            world_engine_step(&engine, &config, &input, &pose);
            const int length = world_pose_to_json(&pose, line, sizeof(line));
            if (length < 0 || (size_t)length >= sizeof(line)) {
                fclose(schedule);
                fclose(output);
                return 4;
            }
            fprintf(output, "%s\n", line);
        }
        expected_sequence = end + 1;
    }
    fclose(schedule);
    if (expected_sequence != 720) {
        fprintf(stderr, "Golden schedule produced %" PRIu64 " frames, expected 720\n", expected_sequence);
        fclose(output);
        return 5;
    }
    fclose(output);
    fprintf(stderr, "Wrote 720 frames to %s\n", output_path);
    return 0;
}
