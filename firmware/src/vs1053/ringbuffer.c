#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "ringbuffer.h"

// Must be power of 2
#ifndef RING_BUFFER_SIZE
#define RING_BUFFER_SIZE 1024
#endif

static uint8_t buffer[RING_BUFFER_SIZE];
static volatile uint16_t head = 0;
static volatile uint16_t tail = 0;

#define RB_MASK (RING_BUFFER_SIZE - 1)

// --- Internal helpers ---
static inline bool rb_is_full(void) {
    return ((head + 1) & RB_MASK) == tail;
}

static inline bool rb_is_empty(void) {
    return head == tail;
}

// --- API implementation ---

void ringbuffer_clear(void) {
    head = 0;
    tail = 0;
    // No need to memset buffer (optional)
}

uint16_t get_num_of_bytes_in_ringbuffer(void) {
    return (head - tail) & RB_MASK;
}

uint16_t get_remaining_space_in_ringbuffer(void) {
    return (tail - head - 1) & RB_MASK;
}

void write_byte_to_ringbuffer(uint8_t data) {
    if (!rb_is_full()) {
        buffer[head] = data;
        head = (head + 1) & RB_MASK;
    } else {
        // Policy: overwrite oldest
        // advance tail first
        tail = (tail + 1) & RB_MASK;
        buffer[head] = data;
        head = (head + 1) & RB_MASK;
    }
}

uint16_t write_array_to_ringbuffer(const uint8_t *data, uint16_t len) {
    int written = 0;
    for (int i = 0; i < len; i++) {
        if (rb_is_full()) {
            // overwrite oldest
            tail = (tail + 1) & RB_MASK;
        }
        buffer[head] = data[i];
        head = (head + 1) & RB_MASK;
        written++;
    }
    return written;
}

uint8_t read_byte_from_ringbuffer(void) {
    if (rb_is_empty()) {
        return 0; // Or some sentinel
    }
    uint8_t val = buffer[tail];
    tail = (tail + 1) & RB_MASK;
    return val;
}

uint16_t read_array_from_ringbuffer(uint8_t *data, uint16_t len) {
    int read = 0;
    while (!rb_is_empty() && read < len) {
        data[read] = buffer[tail];
        tail = (tail + 1) & RB_MASK;
        read++;
    }
    return read;
}