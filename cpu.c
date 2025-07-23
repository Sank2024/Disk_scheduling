#include "oslabs.h"

struct RCB NULLRCB = {0, 0, 0, 0, 0};

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    if (*queue_cnt == 0) {
        return NULLRCB;
    }

    int earliest_index = 0;
    for (int i = 1; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < request_queue[earliest_index].arrival_timestamp) {
            earliest_index = i;
        }
    }

    struct RCB next_request = request_queue[earliest_index];

    for (int i = earliest_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next_request;
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    if (*queue_cnt == 0) {
        return NULLRCB;
    }

    int selected_index = 0;
    int min_seek = abs(request_queue[0].cylinder - current_cylinder);

    for (int i = 1; i < *queue_cnt; i++) {
        int seek_time = abs(request_queue[i].cylinder - current_cylinder);
        if (seek_time < min_seek || (seek_time == min_seek && request_queue[i].arrival_timestamp < request_queue[selected_index].arrival_timestamp)) {
            min_seek = seek_time;
            selected_index = i;
        }
    }

    struct RCB next_request = request_queue[selected_index];

    for (int i = selected_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next_request;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    } else {
        request_queue[*queue_cnt] = new_request;
        (*queue_cnt)++;
        return current_request;
    }
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) {
        return NULLRCB;
    }

    int selected_index = -1;
    int min_seek = 1 << 30;

    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (selected_index == -1 || request_queue[i].arrival_timestamp < request_queue[selected_index].arrival_timestamp) {
                selected_index = i;
            }
        }
    }

    if (selected_index == -1) {
        for (int i = 0; i < *queue_cnt; i++) {
            int direction_ok = (scan_direction == 1 && request_queue[i].cylinder > current_cylinder) || (scan_direction == 0 && request_queue[i].cylinder < current_cylinder);
            if (direction_ok) {
                int seek_time = abs(request_queue[i].cylinder - current_cylinder);
                if (seek_time < min_seek || (seek_time == min_seek && request_queue[i].arrival_timestamp < request_queue[selected_index].arrival_timestamp)) {
                    selected_index = i;
                    min_seek = seek_time;
                }
            }
        }
    }

    if (selected_index == -1) {
        for (int i = 0; i < *queue_cnt; i++) {
            int seek_time = abs(request_queue[i].cylinder - current_cylinder);
            if (seek_time < min_seek || (seek_time == min_seek && request_queue[i].arrival_timestamp < request_queue[selected_index].arrival_timestamp)) {
                selected_index = i;
                min_seek = seek_time;
            }
        }
    }

    struct RCB next_request = request_queue[selected_index];

    for (int i = selected_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;

    return next_request;
}