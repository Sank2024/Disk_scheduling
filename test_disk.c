#include <stdio.h>
#include <string.h>
#include "oslabs.h"

// Helper function to compare two RCBs
int compare_rcb(struct RCB r1, struct RCB r2) {
    return r1.request_id == r2.request_id &&
           r1.arrival_timestamp == r2.arrival_timestamp &&
           r1.cylinder == r2.cylinder &&
           r1.address == r2.address &&
           r1.process_id == r2.process_id;
}

// Helper function to print RCB
void print_rcb(struct RCB rcb) {
    printf("[RID:%d, AT:%d, CYL:%d, ADDR:%d, PID:%d]\n",
           rcb.request_id, rcb.arrival_timestamp, rcb.cylinder, rcb.address, rcb.process_id);
}

// Helper function to print queue
void print_queue(struct RCB request_queue[QUEUEMAX], int queue_cnt) {
    printf("Queue: ");
    if (queue_cnt == 0) {
        printf("Empty\n");
    } else {
        for (int i = 0; i < queue_cnt; i++) {
            printf("[RID:%d, AT:%d, CYL:%d, ADDR:%d, PID:%d] ",
                   request_queue[i].request_id, request_queue[i].arrival_timestamp,
                   request_queue[i].cylinder, request_queue[i].address, request_queue[i].process_id);
        }
        printf("\n");
    }
}

// Test handle_request_arrival_fcfs
void test_handle_request_arrival_fcfs() {
    printf("\nTesting handle_request_arrival_fcfs:\n");
    
    // Test Case 1: Sample case (disk busy)
    struct RCB request_queue[QUEUEMAX] = {0};
    int queue_cnt = 0;
    struct RCB current_request = {51, 1, 53, 53, 51};
    struct RCB new_request = {52, 2, 54, 54, 52};
    int timestamp = 2;
    struct RCB expected_rcb = {51, 1, 53, 53, 51};
    struct RCB expected_queue[QUEUEMAX] = {{52, 2, 54, 54, 52}};
    int expected_queue_cnt = 1;
    
    struct RCB result = handle_request_arrival_fcfs(request_queue, &queue_cnt, current_request, new_request, timestamp);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(request_queue[0], expected_queue[0]);
    printf("Test Case 1 (Sample, disk busy): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue, expected_queue_cnt);
        printf("Got queue: "); print_queue(request_queue, queue_cnt);
    }
    
    // Test Case 2: Disk free
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    current_request = (struct RCB){0, 0, 0, 0, 0};
    new_request = (struct RCB){1, 1, 10, 10, 1};
    timestamp = 1;
    expected_rcb = (struct RCB){1, 1, 10, 10, 1};
    expected_queue_cnt = 0;
    
    result = handle_request_arrival_fcfs(request_queue, &queue_cnt, current_request, new_request, timestamp);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Disk free): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
}

// Test handle_request_completion_fcfs
void test_handle_request_completion_fcfs() {
    printf("\nTesting handle_request_completion_fcfs:\n");
    
    // Test Case 1: Sample case
    struct RCB request_queue[QUEUEMAX] = {{1, 10, 124323, 124323, 1}};
    int queue_cnt = 1;
    struct RCB expected_rcb = {1, 10, 124323, 124323, 1};
    int expected_queue_cnt = 0;
    
    struct RCB result = handle_request_completion_fcfs(request_queue, &queue_cnt);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 1 (Sample): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
    
    // Test Case 2: Empty queue
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    expected_rcb = (struct RCB){0, 0, 0, 0, 0};
    expected_queue_cnt = 0;
    
    result = handle_request_completion_fcfs(request_queue, &queue_cnt);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Empty queue): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
    
    // Test Case 3: Multiple requests
    struct RCB temp_queue[QUEUEMAX] = {{1, 5, 100, 100, 1}, {2, 3, 200, 200, 2}, {3, 7, 300, 300, 3}};
    queue_cnt = 3;
    expected_rcb = (struct RCB){2, 3, 200, 200, 2};
    struct RCB expected_queue[QUEUEMAX] = {{1, 5, 100, 100, 1}, {3, 7, 300, 300, 3}};
    expected_queue_cnt = 2;
    
    result = handle_request_completion_fcfs(temp_queue, &queue_cnt);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt &&
           compare_rcb(temp_queue[0], expected_queue[0]) && compare_rcb(temp_queue[1], expected_queue[1]);
    printf("Test Case 3 (Multiple requests): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue, expected_queue_cnt);
        printf("Got queue: "); print_queue(temp_queue, queue_cnt);
    }
}

// Test handle_request_arrival_sstf
void test_handle_request_arrival_sstf() {
    printf("\nTesting handle_request_arrival_sstf:\n");
    
    // Test Case 1: Sample case (disk busy)
    struct RCB request_queue[QUEUEMAX] = {0};
    int queue_cnt = 0;
    struct RCB current_request = {51, 1, 53, 53, 51};
    struct RCB new_request = {52, 2, 54, 54, 52};
    int timestamp = 2;
    struct RCB expected_rcb = {51, 1, 53, 53, 51};
    struct RCB expected_queue[QUEUEMAX] = {{52, 2, 54, 54, 52}};
    int expected_queue_cnt = 1;
    
    struct RCB result = handle_request_arrival_sstf(request_queue, &queue_cnt, current_request, new_request, timestamp);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(request_queue[0], expected_queue[0]);
    printf("Test Case 1 (Sample, disk busy): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue, expected_queue_cnt);
        printf("Got queue: "); print_queue(request_queue, queue_cnt);
    }
    
    // Test Case 2: Disk free
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    current_request = (struct RCB){0, 0, 0, 0, 0};
    new_request = (struct RCB){1, 1, 10, 10, 1};
    timestamp = 1;
    expected_rcb = (struct RCB){1, 1, 10, 10, 1};
    expected_queue_cnt = 0;
    
    result = handle_request_arrival_sstf(request_queue, &queue_cnt, current_request, new_request, timestamp);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Disk free): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
}

// Test handle_request_completion_sstf
void test_handle_request_completion_sstf() {
    printf("\nTesting handle_request_completion_sstf:\n");
    
    // Test Case 1: Sample case
    struct RCB request_queue[QUEUEMAX] = {{1, 72, 45, 45, 1}, {2, 71, 47, 47, 2}, {3, 73, 43, 43, 3}};
    int queue_cnt = 3;
    int current_cylinder = 48;
    struct RCB expected_rcb = {2, 71, 47, 47, 2};
    struct RCB expected_queue1[QUEUEMAX] = {{1, 72, 45, 45, 1}, {3, 73, 43, 43, 3}};
    int expected_queue_cnt = 2;
    
    struct RCB result = handle_request_completion_sstf(request_queue, &queue_cnt, current_cylinder);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt &&
               compare_rcb(request_queue[0], expected_queue1[0]) && compare_rcb(request_queue[1], expected_queue1[1]);
    printf("Test Case 1 (Sample): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue1, expected_queue_cnt);
        printf("Got queue: "); print_queue(request_queue, queue_cnt);
    }
    
    // Test Case 2: Empty queue
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    current_cylinder = 50;
    expected_rcb = (struct RCB){0, 0, 0, 0, 0};
    expected_queue_cnt = 0;
    
    result = handle_request_completion_sstf(request_queue, &queue_cnt, current_cylinder);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Empty queue): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
    
    // Test Case 3: Same cylinder, different arrival times
    struct RCB temp_queue[QUEUEMAX] = {{1, 10, 50, 50, 1}, {2, 5, 50, 50, 2}};
    queue_cnt = 2;
    current_cylinder = 48;
    expected_rcb = (struct RCB){2, 5, 50, 50, 2};
    struct RCB expected_queue2[QUEUEMAX] = {{1, 10, 50, 50, 1}};
    expected_queue_cnt = 1;
    
    result = handle_request_completion_sstf(temp_queue, &queue_cnt, current_cylinder);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(temp_queue[0], expected_queue2[0]);
    printf("Test Case 3 (Same cylinder): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue2, expected_queue_cnt);
        printf("Got queue: "); print_queue(temp_queue, queue_cnt);
    }
}

// Test handle_request_arrival_look
void test_handle_request_arrival_look() {
    printf("\nTesting handle_request_arrival_look:\n");
    
    // Test Case 1: Sample case (disk busy)
    struct RCB request_queue[QUEUEMAX] = {0};
    int queue_cnt = 0;
    struct RCB current_request = {51, 1, 53, 53, 51};
    struct RCB new_request = {52, 2, 54, 54, 52};
    int timestamp = 2;
    struct RCB expected_rcb = {51, 1, 53, 53, 51};
    struct RCB expected_queue[QUEUEMAX] = {{52, 2, 54, 54, 52}};
    int expected_queue_cnt = 1;
    
    struct RCB result = handle_request_arrival_look(request_queue, &queue_cnt, current_request, new_request, timestamp);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(request_queue[0], expected_queue[0]);
    printf("Test Case 1 (Sample, disk busy): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue, expected_queue_cnt);
        printf("Got queue: "); print_queue(request_queue, queue_cnt);
    }
    
    // Test Case 2: Disk free
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    current_request = (struct RCB){0, 0, 0, 0, 0};
    new_request = (struct RCB){1, 1, 10, 10, 1};
    timestamp = 1;
    expected_rcb = (struct RCB){1, 1, 10, 10, 1};
    expected_queue_cnt = 0;
    
    result = handle_request_arrival_look(request_queue, &queue_cnt, current_request, new_request, timestamp);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Disk free): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
}

// Test handle_request_completion_look
void test_handle_request_completion_look() {
    printf("\nTesting handle_request_completion_look:\n");
    
    // Test Case 1: Sample case
    struct RCB request_queue[QUEUEMAX] = {{1, 52, 58, 58, 1}, {2, 51, 58, 58, 2}, {3, 53, 58, 58, 3}};
    int queue_cnt = 3;
    int current_cylinder = 58;
    int scan_direction = 1;
    struct RCB expected_rcb = {2, 51, 58, 58, 2};
    struct RCB expected_queue1[QUEUEMAX] = {{1, 52, 58, 58, 1}, {3, 53, 58, 58, 3}};
    int expected_queue_cnt = 2;
    
    struct RCB result = handle_request_completion_look(request_queue, &queue_cnt, current_cylinder, scan_direction);
    int pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt &&
               compare_rcb(request_queue[0], expected_queue1[0]) && compare_rcb(request_queue[1], expected_queue1[1]);
    printf("Test Case 1 (Sample): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue1, expected_queue_cnt);
        printf("Got queue: "); print_queue(request_queue, queue_cnt);
    }
    
    // Test Case 2: Empty queue
    memset(request_queue, 0, sizeof(request_queue));
    queue_cnt = 0;
    current_cylinder = 50;
    scan_direction = 1;
    expected_rcb = (struct RCB){0, 0, 0, 0, 0};
    expected_queue_cnt = 0;
    
    result = handle_request_completion_look(request_queue, &queue_cnt, current_cylinder, scan_direction);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt;
    printf("Test Case 2 (Empty queue): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
    }
    
    // Test Case 3: Scan direction 1, requests in direction
    struct RCB temp_queue[QUEUEMAX] = {{1, 10, 60, 60, 1}, {2, 5, 70, 70, 2}};
    queue_cnt = 2;
    current_cylinder = 50;
    scan_direction = 1;
    expected_rcb = (struct RCB){1, 10, 60, 60, 1};
    struct RCB expected_queue2[QUEUEMAX] = {{2, 5, 70, 70, 2}};
    expected_queue_cnt = 1;
    
    result = handle_request_completion_look(temp_queue, &queue_cnt, current_cylinder, scan_direction);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(temp_queue[0], expected_queue2[0]);
    printf("Test Case 3 (Scan direction 1): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue2, expected_queue_cnt);
        printf("Got queue: "); print_queue(temp_queue, queue_cnt);
    }
    
    // Test Case 4: Scan direction 1, no requests in direction
    struct RCB temp_queue2[QUEUEMAX] = {{1, 10, 40, 40, 1}, {2, 5, 30, 30, 2}};
    queue_cnt = 2;
    current_cylinder = 50;
    scan_direction = 1;
    expected_rcb = (struct RCB){1, 10, 40, 40, 1};
    struct RCB expected_queue3[QUEUEMAX] = {{2, 5, 30, 30, 2}};
    expected_queue_cnt = 1;
    
    result = handle_request_completion_look(temp_queue2, &queue_cnt, current_cylinder, scan_direction);
    pass = compare_rcb(result, expected_rcb) && queue_cnt == expected_queue_cnt && compare_rcb(temp_queue2[0], expected_queue3[0]);
    printf("Test Case 4 (Scan direction 1, no requests in direction): %s\n", pass ? "PASS" : "FAIL");
    if (!pass) {
        printf("Expected RCB: "); print_rcb(expected_rcb);
        printf("Got RCB: "); print_rcb(result);
        printf("Expected queue_cnt: %d, Got: %d\n", expected_queue_cnt, queue_cnt);
        printf("Expected queue: "); print_queue(expected_queue3, expected_queue_cnt);
        printf("Got queue: "); print_queue(temp_queue2, queue_cnt);
    }
}

int main() {
    test_handle_request_arrival_fcfs();
    test_handle_request_completion_fcfs();
    test_handle_request_arrival_sstf();
    test_handle_request_completion_sstf();
    test_handle_request_arrival_look();
    test_handle_request_completion_look();
    return 0;
}