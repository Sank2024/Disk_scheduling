#include "oslabs.h"
#include <stdlib.h>

struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt) {
    if (*queue_cnt == 0) {
        struct RCB null_rcb = {0, 0, 0, 0, 0};
        return null_rcb;
    }
    
    int min_index = 0;
    int min_arrival_time = request_queue[0].arrival_timestamp;
    
    for (int i = 1; i < *queue_cnt; i++) {
        if (request_queue[i].arrival_timestamp < min_arrival_time) {
            min_arrival_time = request_queue[i].arrival_timestamp;
            min_index = i;
        }
    }
    
    struct RCB next_request = request_queue[min_index];
    
    for (int i = min_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;
    
    return next_request;
}

struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder) {
    if (*queue_cnt == 0) {
        struct RCB null_rcb = {0, 0, 0, 0, 0};
        return null_rcb;
    }
    
    int min_distance = abs(request_queue[0].cylinder - current_cylinder);
    int min_index = 0;
    int min_arrival_time = request_queue[0].arrival_timestamp;
    
    for (int i = 1; i < *queue_cnt; i++) {
        int distance = abs(request_queue[i].cylinder - current_cylinder);
        if (distance < min_distance || 
            (distance == min_distance && request_queue[i].arrival_timestamp < min_arrival_time)) {
            min_distance = distance;
            min_index = i;
            min_arrival_time = request_queue[i].arrival_timestamp;
        }
    }
    
    struct RCB next_request = request_queue[min_index];
    
    for (int i = min_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;
    
    return next_request;
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (current_request.request_id == 0) {
        return new_request;
    }
    request_queue[*queue_cnt] = new_request;
    (*queue_cnt)++;
    return current_request;
}

struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction) {
    if (*queue_cnt == 0) {
        struct RCB null_rcb = {0, 0, 0, 0, 0};
        return null_rcb;
    }
    
    int min_arrival_time = -1;
    int min_index = -1;
    
    for (int i = 0; i < *queue_cnt; i++) {
        if (request_queue[i].cylinder == current_cylinder) {
            if (min_arrival_time == -1 || request_queue[i].arrival_timestamp < min_arrival_time) {
                min_arrival_time = request_queue[i].arrival_timestamp;
                min_index = i;
            }
        }
    }
    
    if (min_index != -1) {
        struct RCB next_request = request_queue[min_index];
        for (int i = min_index; i < *queue_cnt - 1; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        (*queue_cnt)--;
        return next_request;
    }
    
    int min_distance = -1;
    min_arrival_time = -1;
    min_index = -1;
    
    for (int i = 0; i < *queue_cnt; i++) {
        if ((scan_direction == 1 && request_queue[i].cylinder > current_cylinder) ||
            (scan_direction == 0 && request_queue[i].cylinder < current_cylinder)) {
            int distance = abs(request_queue[i].cylinder - current_cylinder);
            if (min_distance == -1 || distance < min_distance) {
                min_distance = distance;
                min_index = i;
                min_arrival_time = request_queue[i].arrival_timestamp;
            }
        }
    }
    
    if (min_index == -1) {
        for (int i = 0; i < *queue_cnt; i++) {
            int distance = abs(request_queue[i].cylinder - current_cylinder);
            if (min_distance == -1 || distance < min_distance) {
                min_distance = distance;
                min_index = i;
                min_arrival_time = request_queue[i].arrival_timestamp;
            }
        }
    }
    
    struct RCB next_request = request_queue[min_index];
    
    for (int i = min_index; i < *queue_cnt - 1; i++) {
        request_queue[i] = request_queue[i + 1];
    }
    (*queue_cnt)--;
    
    return next_request;
}