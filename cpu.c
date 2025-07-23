#include <stdio.h>
#include "oslabs.h"

// Helper function to insert PCB into the ready queue based on priority (lower number = higher priority)
void insert_by_priority(struct PCB ready_queue[], int *queue_cnt, struct PCB new_process) {
    int i = *queue_cnt - 1;
    while (i >= 0 && ready_queue[i].process_priority > new_process.process_priority) {
        ready_queue[i + 1] = ready_queue[i];
        i--;
    }
    ready_queue[i + 1] = new_process;
    (*queue_cnt)++;
}

// Helper function to insert PCB into the ready queue based on remaining burst time (SRTF)
void insert_by_remaining_time(struct PCB ready_queue[], int *queue_cnt, struct PCB new_process) {
    int i = *queue_cnt - 1;
    while (i >= 0 && ready_queue[i].remaining_bursttime > new_process.remaining_bursttime) {
        ready_queue[i + 1] = ready_queue[i];
        i--;
    }
    ready_queue[i + 1] = new_process;
    (*queue_cnt)++;
}

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    if (new_process.process_priority < current_process.process_priority) {
        new_process.execution_starttime = timestamp;
        new_process.remaining_bursttime = new_process.total_bursttime;
        insert_by_priority(ready_queue, queue_cnt, current_process);
        return new_process;
    } else {
        insert_by_priority(ready_queue, queue_cnt, new_process);
        return current_process;
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        struct PCB null_process = {0};
        return null_process;
    }
    struct PCB next_process = ready_queue[0];
    for (int i = 0; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    next_process.execution_starttime = timestamp;
    next_process.execution_endtime = timestamp + next_process.remaining_bursttime;
    return next_process;
}

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    new_process.remaining_bursttime = new_process.total_bursttime;
    if (current_process.process_id == 0) {
        new_process.execution_starttime = timestamp;
        return new_process;
    }
    if (new_process.total_bursttime < current_process.remaining_bursttime) {
        current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        insert_by_remaining_time(ready_queue, queue_cnt, current_process);
        new_process.execution_starttime = timestamp;
        return new_process;
    } else {
        insert_by_remaining_time(ready_queue, queue_cnt, new_process);
        return current_process;
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if (*queue_cnt == 0) {
        struct PCB null_process = {0};
        return null_process;
    }
    struct PCB next_process = ready_queue[0];
    for (int i = 0; i < *queue_cnt - 1; i++) {
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
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    } else {
        insert_by_priority(ready_queue, queue_cnt, new_process);
        return current_process;
    }
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if (*queue_cnt == 0) {
        struct PCB null_process = {0};
        return null_process;
    }
    struct PCB next_process = ready_queue[0];
    for (int i = 0; i < *queue_cnt - 1; i++) {
        ready_queue[i] = ready_queue[i + 1];
    }
    (*queue_cnt)--;
    next_process.execution_starttime = timestamp;
    return next_process;
}