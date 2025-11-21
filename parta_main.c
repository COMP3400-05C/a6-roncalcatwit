#include "parta.h"
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>

/**
 * Main function to process command-line arguments and execute the specified scheduling algorithm.
 * 
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Exit status of the program (0 for success, 1 for error).
 */
int main(int argc, char* argv[]) {
    // Check if there are enough arguments
    if (argc < 3) {
        fprintf(stderr, "ERROR: Missing arguments\n");
        return 1;
    }

    // Determine the scheduling algorithm
    char* algorithm = argv[1];
    if (strcmp(algorithm, "fcfs") == 0) {
        // FCFS algorithm
        printf("Using FCFS\n");

        // Parse CPU bursts
        int num_bursts = argc - 2;
        int* bursts = malloc(num_bursts * sizeof(int));
        if (!bursts) {
            fprintf(stderr, "ERROR: Memory allocation failed\n");
            return 1;
        }

        for (int i = 0; i < num_bursts; i++) {
            bursts[i] = atoi(argv[i + 2]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        // Initialize processes and calculate average wait time
        struct pcb* procs = init_procs(bursts, num_bursts);
        int total_wait_time = fcfs_run(procs, num_bursts);
        double avg_wait_time = (double)total_wait_time / num_bursts;

        printf("Average wait time: %.2f\n", avg_wait_time);

        // Cleanup
        free(bursts);
        free(procs);
    } else if (strcmp(algorithm, "rr") == 0) {
        // RR algorithm
        if (argc < 4) {
            fprintf(stderr, "ERROR: Missing arguments\n");
            return 1;
        }

        int quantum = atoi(argv[2]);
        printf("Using RR(%d)\n", quantum);

        // Parse CPU bursts
        int num_bursts = argc - 3;
        int* bursts = malloc(num_bursts * sizeof(int));
        if (!bursts) {
            fprintf(stderr, "ERROR: Memory allocation failed\n");
            return 1;
        }

        for (int i = 0; i < num_bursts; i++) {
            bursts[i] = atoi(argv[i + 3]);
            printf("Accepted P%d: Burst %d\n", i, bursts[i]);
        }

        // Initialize processes and calculate average wait time
        struct pcb* procs = init_procs(bursts, num_bursts);
        int total_wait_time = rr_run(procs, num_bursts, quantum);
        double avg_wait_time = (double)total_wait_time / num_bursts;

        printf("Average wait time: %.2f\n", avg_wait_time);

        // Cleanup
        free(bursts);
        free(procs);
    } else {
        // Invalid algorithm
        fprintf(stderr, "ERROR: Invalid algorithm specified\n");
        return 1;
    }

    return 0;
}
