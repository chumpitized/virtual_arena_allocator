#include <assert.h>
#include <cstring>
#include <iostream>
#include <windows.h>

#ifndef DEFAULT_ALIGNMENT
#define DEFAULT_ALIGNMENT sizeof(void *)
#endif

#ifndef PAGE_SIZE
#define PAGE_SIZE 4096
#endif

typedef struct Arena Arena;
struct Arena {
	unsigned char *buffer;
	size_t committed;
	size_t length;
	size_t curr_offset;
};

int main() {
	std::cout << "hello" << std::endl;
	return 0;
}