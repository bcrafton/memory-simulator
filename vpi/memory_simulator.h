#include <vpi_user.h>
#include "../Data-Structures/header/priority_queue.h"

#define BITS_IN_INT 32
#define MEMORY_SIZE 1024
#define ADDRESS_WIDTH 16
#define DATA_WIDTH 16
#define WORD_SIZE 16;
#define ACK_WIDTH 1

#define NUM_CACHE_LINES 16
#define WORDS_PER_CACHE_LINE 8;

#define TAG_LOG2 3
#define CACHELINE_LOG2 4
#define OFFSET_LOG2 3

#define TAG(address) (unsigned int)((address >> (CACHELINE_LOG2 + OFFSET_LOG2)) & ((1 << TAG_LOG2) - 1))
#define CACHELINE(address) (unsigned int)((address >> OFFSET_LOG2) & ((1 << CACHELINE_LOG2) - 1))
#define OFFSET(address) (unsigned int)(address & ((1 << OFFSET_LOG2) - 1))

typedef WORD unsigned short;
typedef TIME unsigned long;
typedef byte unsigned char;

void cache_init();
void cache_rd_rqst(WORD address, TIME time);
void cache_wr_rqst(WORD address, WORD data, TIME time);
cache_rd_ret_t* cache_rd_ret(TIME current_time);
cache_wr_ret* cache_wr_ret(TIME current_time);
void cache_update(TIME current_time);

void mem_rd_rqst(byte tag, byte cache_line_number, TIME time);
void mem_wr_rqst(WORD cache_line[], TIME time);
mem_rd_ret_t* mem_rd_ret(TIME current_time);
mem_wr_ret_t* mem_wr_ret(TIME current_time);

// request objects
typedef struct cache_rd_rqst{
    WORD address;
    TIME time;
} cache_rd_rqst_t;

typedef struct mem_rd_rqst{
    byte tag;
    byte cache_line_number;
    TIME time;
} cache_rd_rqst_t;

typedef struct cache_wr_rqst{
    WORD address;
    WORD data;
    TIME time;
} cache_wr_rqst_t;

typedef struct mem_wr_rqst{
    WORD data[WORDS_PER_CACHE_LINE];
    byte tag;
    byte cache_line_number;
    TIME time;
} cache_wr_rqst_t;

// return objects
typedef struct cache_rd_ret{
    WORD address;
    WORD data;
} cache_rd_rqst_t;

typedef struct mem_rd_ret{
    WORD data[WORDS_PER_CACHE_LINE];
    byte tag;
    byte cache_line_number;
} cache_rd_rqst_t;

typedef struct cache_wr_ret{
    WORD address;
} cache_wr_rqst_t;

typedef struct mem_wr_ret{
    WORD start_address;
} cache_wr_rqst_t;


// so defining a cache_line is tricky. we really dont want to pass these things around.
// just pass the data around.
// i dont really want to need like a first bit, so unallocated guys are used first...
// so we need an cache init function.
typedef struct cache_line{
    bool dirty;
    bool valid;
    byte tag;
    byte lru_next;
    WORD data[WORDS_PER_CACHE_LINE];
} cache_line_t;

typedef struct cache{
    cache_line_t lines[NUM_CACHE_LINES];
    byte lru; // lru
    byte mru; // mru
} cache_t;


