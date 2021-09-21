#pragma warning(disable:4996) //disabling warning

#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef struct chunk {
	uint8_t data[64];
	uint16_t size;
	uint16_t crc;
} CHUNK;

//CRC-calculation
uint16_t crc16(const uint8_t *data_p, unsigned int length);

int main() {
	
	return 0;
}
//CRC-calculation
uint16_t crc16(const uint8_t *data_p, unsigned int length) {
	uint8_t x;
	uint16_t crc = 0xFFFF;
	while (length--) {
		x = crc >> 8 ^ *data_p++;
		x ^= x >> 4;
		crc = (crc << 8) ^ ((uint16_t)(x << 12)) ^ ((uint16_t)(x << 5)) ^ ((uint16_t)x);
	}
	return crc;
}