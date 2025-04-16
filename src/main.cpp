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

typedef uintptr_t uptr;

typedef struct Arena Arena;
struct Arena {
	unsigned char *buffer;
	size_t committed;
	size_t length;
	size_t curr_offset;
};

void arena_init(Arena *a, size_t length) {
	a->buffer 		= (unsigned char *)VirtualAlloc(NULL, length, MEM_COMMIT, PAGE_READWRITE);
	a->committed 	= 0;
	a->length 		= length;
	a->curr_offset 	= 0;
}

bool is_power_of_two(size_t x) {
	return (x & (x - 1)) == 0;
}

uptr align_forward(uptr ptr, size_t align) {
	assert(is_power_of_two(align));

	uptr a = (uptr)align;
	uptr modulo = ptr & (a - 1);

	if (modulo != 0) {
		ptr += a - modulo;
	}

	return ptr;
}

size_t commit_memory(Arena *a, size_t size) {
	size_t total_alloc = a->curr_offset + size;

	if (total_alloc <= a->committed) return -1;

	size_t diff = total_alloc - a->committed;

	//use bitwise operators to avoid conditional branch...
	return (diff + PAGE_SIZE - 1) & ~(PAGE_SIZE - 1);

	//if (diff % PAGE_SIZE == 0) {
	//	return diff;
	//} else {
	//	return ((diff / PAGE_SIZE) + 1) * PAGE_SIZE;
	//}
}

void *arena_alloc(Arena *a, size_t size, size_t align) {
	uptr curr_ptr = (uptr)a->buffer + (uptr)a->curr_offset;
	uptr offset = align_forward(curr_ptr, align);
	offset -= (uptr)a->buffer;

	if (offset + size <= a->length) {
		void *ptr = &a->buffer[offset];

		size_t to_commit = commit_memory(a, size);
		if (to_commit) {
			VirtualAlloc(&a->buffer[a->curr_offset], to_commit, MEM_COMMIT, PAGE_READWRITE);
			a->committed += to_commit;
		}

		a->curr_offset = offset + size;

		return memset(ptr, 0, size);
	}

	return NULL;
}

void arena_clear(Arena *a) {
	a->curr_offset = 0;
}

int main() {
	std::cout << "hello" << std::endl;
	return 0;
}