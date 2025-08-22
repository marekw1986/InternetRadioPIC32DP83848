/*
 * ringbuffer.h
 *
 *  Created on: 27 wrz 2022
 *      Author: marek
 */
#include <stdint.h>

#ifndef INC_RINGBUFFER_H_
#define INC_RINGBUFFER_H_

#define RING_BUFFER_SIZE 8192

void ringbuffer_clear(void);
void write_byte_to_ringbuffer(uint8_t data);
uint16_t write_array_to_ringbuffer(const uint8_t* data, uint16_t len);
uint16_t read_array_from_ringbuffer(uint8_t* data, uint16_t len);
uint16_t get_remaining_space_in_ringbuffer();
uint16_t get_num_of_bytes_in_ringbuffer();

#endif /* INC_RINGBUFFER_H_ */
