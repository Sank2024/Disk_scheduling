#include "oslabs.h"
#include <stdlib.h>

struct PCB null_pcb() {
    struct PCB null = {0, 0, 0, 0, 0, 0, 0};
    return null;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.remaining_bursttime;
        return new_process;
    }
    // Lower priority value means higher priority
    if (new_process.process_priority < current_process.process_priority) {
        // Update current process's remaining_bursttime
        if (current_process.execution_starttime > 0) {
            current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        }
        current_process.execution_endtime = 0; // Set EET to 0 when queued
        ready_queue[*queue_cnt] = current_process; // Preserve EST
        (*queue_cnt)++;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.remaining_bursttime;
        return new_process;
    } else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    // Select process with lowest priority value (highest priority)
    int min_priority_index = 0;
    int min_priority = ready_queue[0].process_priority;
    int min_arrival_time = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].process_priority < min_priority ||
            (ready_queue[i].process_priority == min_priority && ready_queue[i].arrival_timestamp < min_arrival_time)) {
            min_priority = ready_queue[i].process_priority;
            min_priority_index = i;
            min_arrival_time = ready_queue[i].arrival_timestamp;
        }
    }
    struct PCB next_process = ready_queue[min_priority_index];
    for (int i = min_priority_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.remaining_bursttime;
        return new_process;
    }
    if (new_process.remaining_bursttime < current_process.remaining_bursttime ||
        (new_process.remaining_bursttime == current_process.remaining_bursttime && new_process.arrival_timestamp < current_process.arrival_timestamp)) {
        // Update current process's remaining_bursttime
        if (current_process.execution_starttime > 0) {
            current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        }
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = current_process;
        (*queue_cnt)++;
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.remaining_bursttime;
        return new_process;
    } else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        ready_queue[*queue_cnt] = new_process;
        (*queue_cnt)++;
        return current_process;
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    int min_remaining_index = 0;
    int min_remaining_time = ready_queue[0].remaining_bursttime;
    int min_arrival_time = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].remaining_bursttime < min_remaining_time ||
            (ready_queue[i].remaining_bursttime == min_remaining_time && ready_queue[i].arrival_timestamp < min_arrival_time)) {
            min_remaining_time = ready_queue[i].remaining_bursttime;
            min_remaining_index = i;
            min_arrival_time = ready_queue[i].arrival_timestamp;
        }
    }
    struct PCB next_process = ready_queue[min_remaining_index];
    for (int i = min_remaining_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + MIN(new_process.remaining_bursttime, time_quantum);
        return new_process;
    }
    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    int i;
    for (i = *queue_cnt; i > 0 && ready_queue[i-1].arrival_timestamp > new_process.arrival_timestamp; i--) {
        ready_queue[i] = ready_queue[i-1];
    }
    ready_queue[i] = new_process;
    (*queue_cnt)++;
    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (*queue_cnt == 0) {
        return null_pcb();
    }
    // Select process with earliest arrival_timestamp
    int min_arrival_index = 0;
    int min_arrival_time = ready_queue[0].arrival_timestamp;
    for (int i = 1; i < *queue_cnt; i++) {
        if (ready_queue[i].arrival_timestamp < min_arrival_time) {
            min_arrival_time = ready_queue[i].arrival_timestamp;
            min_arrival_index = i;
        }
    }
    struct PCB next_process = ready_queue[min_arrival_index];
    for (int i = min_arrival_index; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + MIN(next_process.remaining_bursttime, time_quantum);
    return next_process;
}