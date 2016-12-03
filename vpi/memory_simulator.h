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

typedef struct cache_rd_rqst{
    WORD address;
    TIME time;
} cache_rd_rqst_t;

typedef struct cache_wr_rqst{
    WORD address;
    WORD data;
    TIME time;
} cache_wr_rqst_t;

typedef struct cache_line{
    // unsigned char flags
    bool dirty; // 1 bit
    bool valid; // 1 bit
    unsigned char tag; // 3 bits
    WORD line[WORDS_PER_CACHE_LINE];
} cache_line_t;

typedef struct cache{
    cache_line_t lines[NUM_CACHE_LINES];
} cache_t;

