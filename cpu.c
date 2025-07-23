#include "oslabs.h"
#include <stdlib.h>

// Helper function to initialize a NULLPCB
struct PCB null_pcb() {
    struct PCB null = {0, 0, 0, 0, 0, 0, 0};
    return null;
}

// Priority Scheduling: Handle process arrival
struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    // If no current process (NULLPCB), execute the new process
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    // If current process exists, compare priorities
    if (new_process.process_priority > current_process.process_priority) {
        // Preempt current process: add it back to the queue
        current_process.remaining_bursttime = current_process.total_bursttime - (timestamp - current_process.execution_starttime);
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;
        // Execute new process
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        // Add new process to the queue
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

// Priority Scheduling: Handle process completion
struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    // Find the process with the highest priority
    int max_priority_index = 0;
    int max_priority = ready_queue[0].process_priority;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority > max_priority) {
            max_priority = ready_queue[i].process_priority;
            max_priority_index = i;
        }
    }
    // Select the process
    struct PCB next_process = ready_queue[max_priority_index];
    // Remove from queue
    for (int i = max_priority_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    // Set execution times
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

// Shortest Remaining Time Next: Handle process arrival
struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    // If no current process (NULLPCB), execute the new process
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    // Calculate remaining time of current process
    int current_remaining = current_process.remaining_bursttime - (timestamp - current_process.execution_starttime);
    if (new_process.total_bursttime < current_remaining) {
        // Preempt current process: add it back to the queue
        current_process.remaining_bursttime = current_remaining;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;
        // Execute new process
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        // Add new process to the queue
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

// Shortest Remaining Time Next: Handle process completion
struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    // Find the process with the shortest remaining burst time
    int min_remaining_index = 0;
    int min_remaining_time = ready_queue[0].remaining_bursttime;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < min_remaining_time) {
            min_remaining_time = ready_queue[i].remaining_bursttime;
            min_remaining_index = i;
        }
    }
    // Select the process
    struct PCB next_process = ready_queue[min_remaining_index];
    // Remove from queue
    for (int i = min_remaining_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    // Set execution times
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

// Round-Robin: Handle process arrival
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    // If no current process (NULLPCB), execute the new process
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(new_process.total_bursttime, time_quantum);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    // Add new process to the queue (no preemption in RR on arrival)
    ready_queue[*queue_cnt] = new_process;
    (*queue_cnt)++;
    return current_process;
}

// Round-Robin: Handle process completion
struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    // Select the first process in the queue (FIFO)
    struct PCB next_process = ready_queue[0];
    // Remove from queue
    for (int i = 0; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    // Set execution times
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + MIN(next_process.remaining_bursttime, time_quantum);
    return next_process;
}