
#include <vpi_user.h>
#include <stdio.h>
#include <string.h>
#include "../Data-Structures/header/priority_queue.h"
#include "../Data-Structures/header/list.h"

#define BITS_IN_INT 32
#define MEMORY_SIZE 1024
#define ADDRESS_WIDTH 16
#define DATA_WIDTH 16
#define WORD_SIZE 16
#define ACK_WIDTH 1

#define NUM_CACHE_LINES 16
#define WORDS_PER_CACHE_LINE 8

#define TAG_LOG2 3
#define CACHELINE_LOG2 4
#define OFFSET_LOG2 3

#define TAG(address) (unsigned int)((address >> (CACHELINE_LOG2 + OFFSET_LOG2)) & ((1 << TAG_LOG2) - 1))
#define CACHELINE(address) (unsigned int)((address >> OFFSET_LOG2) & ((1 << CACHELINE_LOG2) - 1))
#define OFFSET(address) (unsigned int)(address & ((1 << OFFSET_LOG2) - 1))

#define CACHE_READ_TIME 10
#define CACHE_WRITE_TIME 10
#define MEMORY_READ_TIME 200
#define MEMORY_WRITE_TIME 200

typedef unsigned short WORD;
typedef unsigned long TIME;
typedef unsigned char BYTE;
typedef unsigned char BOOL;


typedef struct cache_rd_rqst{
    WORD address;
    TIME time;
} cache_rd_rqst_t;

typedef struct cache_wr_rqst{
    WORD address;
    WORD data;
    TIME time;
} cache_wr_rqst_t;

typedef struct cache_rd_ret{
    WORD address;
    WORD data;
} cache_rd_ret_t;

typedef struct cache_wr_ret{
    WORD address;
} cache_wr_ret_t;



typedef struct mem_rd_rqst{
    WORD start_address;
    TIME time;
} mem_rd_rqst_t;

typedef struct mem_wr_rqst{    
    WORD start_address;
    WORD data[WORDS_PER_CACHE_LINE];
    TIME time;
} mem_wr_rqst_t;

typedef struct mem_rd_ret{    
    WORD start_address;
    WORD data[WORDS_PER_CACHE_LINE];
} mem_rd_ret_t;

typedef struct mem_wr_ret{
    WORD start_address;
} mem_wr_ret_t;



typedef struct cache_line{
    BOOL dirty;
    BOOL valid;
    BYTE tag;
    BYTE next;
    WORD data[WORDS_PER_CACHE_LINE];
} cache_line_t;

typedef struct cache{
    cache_line_t lines[NUM_CACHE_LINES];
    BYTE lru;
    BYTE mru;
} cache_t;



void cache_init();
void cache_rd_rqst(WORD address, TIME current_time);
void cache_wr_rqst(WORD address, WORD data, TIME current_time);
cache_rd_ret_t* cache_rd_ret(TIME current_time);
cache_wr_ret_t* cache_wr_ret(TIME current_time);
void cache_update(TIME current_time);

void mem_init();
void mem_rd_rqst(WORD start_address, TIME current_time);
void mem_wr_rqst(WORD* data, WORD start_address, TIME current_time);
mem_rd_ret_t* mem_rd_ret(TIME current_time);
mem_wr_ret_t* mem_wr_ret(TIME current_time);

int time_compare(void *o1, void *o2);









