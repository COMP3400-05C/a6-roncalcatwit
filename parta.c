#include "parta.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Initializes an array of PCBs based on the given bursts.
 * Each PCB contains a PID, the burst time left, and the wait time (initially 0).
 *
 * @param bursts An array of CPU burst times.
 * @param blen The length of the bursts array.
 * @return A pointer to the array of PCBs.
 */
struct pcb* init_procs(int* bursts, int blen) {
    // Allocate memory for the PCBs
    struct pcb* procs = (struct pcb*)malloc(blen * sizeof(struct pcb));
    if (!procs) {
        perror("Failed to allocate memory for PCBs");
        return NULL;
    }

    // Initialize each PCB with its PID, burst_left, and wait time
    for (int i = 0; i < blen; i++) {
        procs[i].pid = i;
        procs[i].burst_left = bursts[i];
        procs[i].wait = 0;
    }

    return procs;
}

/**
 * Prints the details of all PCBs in the array.
 *
 * @param procs The array of PCBs.
 * @param plen The number of PCBs in the array.
 */
void printall(struct pcb* procs, int plen) {
    printf("PID\tBurst Left\tWait\n");
    for (int i = 0; i < plen; i++) {
        printf("%d\t%d\t\t%d\n", procs[i].pid, procs[i].burst_left, procs[i].wait);
    }
}

/**
 * Runs the current process by reducing its burst_left by the given amount.
 * Increases the wait time of all other processes by the same amount.
 *
 * @param procs The array of PCBs.
 * @param plen The number of PCBs in the array.
 * @param current The index of the current process.
 * @param amount The amount of burst time to reduce.
 */
void run_proc(struct pcb* procs, int plen, int current, int amount) {
    if (procs[current].burst_left > 0) {
        procs[current].burst_left -= amount;
        if (procs[current].burst_left < 0) {
            procs[current].burst_left = 0; // Ensure no negative burst time
        }

        // Increment wait time for all other processes
        for (int i = 0; i < plen; i++) {
            if (i != current && procs[i].burst_left > 0) {
                procs[i].wait += amount;
            }
        }
    }
}

/**
 * Implements the First-Come-First-Serve (FCFS) scheduling algorithm.
 * Runs each process to completion in the order they appear.
 *
 * @param procs The array of PCBs.
 * @param plen The number of PCBs in the array.
 * @return The total time taken to complete all processes.
 */
int fcfs_run(struct pcb* procs, int plen) {
    int total_time = 0;

    for (int i = 0; i < plen; i++) {
        if (procs[i].burst_left > 0) {
            procs[i].wait = total_time; // Set wait time for the process
            total_time += procs[i].burst_left; // Add burst time to total time
            procs[i].burst_left = 0; // Process is completed
        }
    }

    return total_time;
}

/**
 * Determines the next process to run in a round-robin fashion.
 * Skips processes that are already completed.
 *
 * @param current The index of the current process.
 * @param procs The array of PCBs.
 * @param plen The number of PCBs in the array.
 * @return The index of the next process to run, or -1 if all are completed.
 */
int rr_next(int current, struct pcb* procs, int plen) {
    for (int i = 1; i <= plen; i++) {
        int next = (current + i) % plen;
        if (procs[next].burst_left > 0) {
            return next;
        }
    }
    return -1; // All processes are completed
}

/**
 * Implements the Round-Robin (RR) scheduling algorithm.
 * Runs each process for a time quantum or until completion, whichever comes first.
 *
 * @param procs The array of PCBs.
 * @param plen The number of PCBs in the array.
 * @param quantum The time quantum for the round-robin scheduler.
 * @return The total time taken to complete all processes.
 */
int rr_run(struct pcb* procs, int plen, int quantum) {
    int total_time = 0;
    int current = 0;

    while (1) {
        current = rr_next(current, procs, plen);
        if (current == -1) {
            break; // All processes are completed
        }

        int run_time = (procs[current].burst_left > quantum) ? quantum : procs[current].burst_left;
        run_proc(procs, plen, current, run_time);
        total_time += run_time;
    }

    return total_time;
}

/**
 * Main function for testing the implemented functions.
 */
// int main() {
//     int bursts[] = {5, 8, 2};
//     int plen = sizeof(bursts) / sizeof(bursts[0]);

//     // Initialize processes
//     struct pcb* procs = init_procs(bursts, plen);
//     if (!procs) {
//         return 1;
//     }

//     printf("Initial PCBs:\n");
//     printall(procs, plen);

//     // Test FCFS
//     printf("\nRunning FCFS:\n");
//     int fcfs_time = fcfs_run(procs, plen);
//     printall(procs, plen);
//     printf("Total time (FCFS): %d\n", fcfs_time);

//     // Reinitialize processes for RR
//     free(procs);
//     procs = init_procs(bursts, plen);

//     // Test RR
//     printf("\nRunning RR (Quantum = 4):\n");
//     int rr_time = rr_run(procs, plen, 4);
//     printall(procs, plen);
//     printf("Total time (RR): %d\n", rr_time);

//     free(procs);
//     return 0;
// }
