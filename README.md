# Virtual Arena Allocator

This implementation of a simple arena uses `VirtualAlloc` from the `windows.h` header file to reserve a large block of memory for future allocations. Reserved memory is _not_ committed memory, which means it does not represent memory in use by your process. Instead, it refers to _virtual memory_ set aside for allocations within the arena. Committed memory, in contrast, refers to allocations in physical memory.

When the requested allocation size exceeds the amount of committed memory, the arena will attempt to commit more memory (by some multiple of the system's page size, in this example `4096`) from the reserved pool.
