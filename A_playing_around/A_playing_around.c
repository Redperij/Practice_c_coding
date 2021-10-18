#pragma warning(disable:4996) //disabling warning

/*
You need to write two functions:

Function called //register_slip_frame_callback that takes one parameter:
pointer to a function that takes a const unsigned char pointer and an integer as parameters and returns no value.
register_slip_frame_callback does not return a value.
The function stores the callback function pointer to a global variable.

Function called process_slip_data that takes one parameter:
pointer to a function that does not take any parameters and returns an integer.
Process_slip_data does not return a value.

Process_slip_data uses the function given as parameter to read bytes that carry SLIP framed data.
The function reads and decodes data until the reader function return EOF to indicate end of input.
The data that reader function returns contains SLIP framed data where the maximum data length is 40 bytes.
Note that because of escaping the number of received bytes before frame is completetely received can be much longer (82 bytes in the worst case before decoding).
The length of decoded data never exceed 40 bytes. When a frame is completely received (SLIP_END received) then data is passed to the callback function for processing.
If two SLIP_ENDs are received with no data between them the callback function is not called because there is no data to process.

The test bench first registers a callback function and then calls process_slip_data. Both callback function and reader function are implemented in the test bench.
*/

//NON-TASK

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdint.h>

#define SLIP_END 0xC0
#define SLIP_ESC 0xDB
#define SLIP_ESC_END 0xDC
#define SLIP_ESC_ESC 0xDD

int read_byte = -1;

int bytes[] = { 0xC0, 0x01, 0xDD, 0xDB, 0x49, 0xDC, 0xDB, 0x15, 0xC0 };

int read_bytes();

void callback(unsigned char *ip_frame, int size);

void register_slip_frame_callback(void (*callback_func)(unsigned char *, int));

void process_slip_data(int (*read_bytes)());

int main(int arcg, char **argv)
{
	
	return 0;
}

//TASK

void register_slip_frame_callback(void (*callback_func)(unsigned char *, int)) {

}

void process_slip_data(int (*read_bytes)()) {

}

//NON-TASK

int read_bytes() {
	read_byte++;
	return bytes[read_byte];
}

void callback(unsigned char *ip_frame, int size) {
	return;
}