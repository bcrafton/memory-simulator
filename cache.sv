// system verilog implementation of a cache
// direct map, set associative, fully associative
// this will be either a direct mapped or set associative cache.
// learning towards direct mapped because it is easier and is traditional cache.
// okay so start with direct mapped cache.

// 32 byte direct mapped cache. 128 slots?
// https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Memory/direct.html

// this means we need to change our processor to 32 bit.
// what else wud cache look like if we only have 1024 addressing
// 8 byte 8 slot = 64 bytes at a time ... this wud work actually.
// we shud do 16 byte 8 slots = 128 bytes
// okay lets do this then

// keep 16 bit processor ! direct mapped because simple as possible!
// can always complicate later.

// right so it goes
/*
 * |tag|slot|offset|
 * slot is the cache location
 * because adjacent memory will always have different cache locations
 * but jumps in code can screw u
 * set associative is almost entirely better
 * but we will stick with direct mapped.
 * because it is simpler
 */
 
 `include "memory_controller.svh"
 
 