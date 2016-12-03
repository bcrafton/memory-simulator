/*
Word = 16 bits
1024 deep, 16 bit wide
16 cache lines, 8 words each cache line. 16 * 8 = 128 which is 1/10 the size of memory which is okay.

16 -> 1024
xxxx x000 0000
xx00 0000 0000

8 entires : offset = 3 bits
16 cache lines : slots = 4 bits 
128 entry cache : 7 bits
1024 entires / 128 entries = 3 bits ... tag
only 8 unique tags per cache line
3 bit tag

8 entries will group into 1 cacheline
16 cachelines
128 entires can fit in
1024 entries so 8 unique tags needed.

we also need to handle dirty and valid bits

read: controller -> cache
write: controller -> cache

read ret cache -> controller
write ret cache -> controller

read cache -> memory
write cache -> memory

read ret memory -> cache
write ret memory -> cache

write+allocate: the block is fetched from memory into the cache before being written (read -> write -> store to memory)
write+noallocate: = write around. The block is directly modified in the lower level without being written first (just store to memory) 

keep it simple, write+allocate, flush on a miss

we may need an update function for every cycle...

this whole project gonna need more through
*/

static cache_t;

void rd_rqst_cache(rd_rqst_t* rqst)
{
    unsigned char tag = TAG(rqst->address);
    unsigned char cacheline = CACHELINE(rqst->address);
    
    // cache hit
    if(cache.lines[cacheline].tag == tag)
    {
    }
    // cache miss
    else
    {
        // this needs to define a cacheline to fetch.
        // we probably want to make 2 different structs...
        // and want different queues.
        rd_rqst_memory(rqst);
    }
    
}


















