#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

struct __attribute__ ((__packed__)) sdshdr8 {
    uint8_t len;
    uint8_t alloc;
    unsigned char flags;
    char buf[];
};

struct __attribute__ ((__packed__)) sdshdr16 {
    uint16_t len;
    uint16_t alloc;
    unsigned char flags;
    char buf[];
};

int main()
{
	printf("size of struct: %d\n", sizeof(struct sdshdr8));
	printf("size of struct: %d\n", sizeof(struct sdshdr16));
	return 0;
}

