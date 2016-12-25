
#include "memory_simulator.h"

static cache_t cache;

static PriorityList* rd_rqst_queue = NULL;
static PriorityList* wr_rqst_queue = NULL;
static RBTree* cache_rd_miss_table = NULL;
static RBTree* cache_wr_miss_table = NULL;

static BOOL in_cache(BYTE cache_line_number, BYTE tag);
static BOOL mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number);
static void set_mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number);
static void clear_mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number);
static BYTE evict_lru(void);

static print_pending();

static BYTE pending_mem_rd_rqsts[(MEMORY_SIZE >> OFFSET_LOG2)];

void cache_init()
{
    rd_rqst_queue = priority_list_constructor(&time_compare);
    wr_rqst_queue = priority_list_constructor(&time_compare);
    cache_rd_miss_table = rbtree_constructor(&address_compare);
    cache_wr_miss_table = rbtree_constructor(&address_compare);

    int i, j;
    for(i=0;i<NUM_CACHE_LINES;i++)
    {
        for(j=0; j<WORDS_PER_CACHE_LINE; j++)
        {
            cache.lines[i].data[j] = 0;
        }
        cache.lines[i].dirty = 0;
        cache.lines[i].valid = 0;
        cache.lines[i].next = i+1;
    }
    cache.lru = 0; 
    cache.mru = NUM_CACHE_LINES-1;

    FILE *file;

    file = fopen(CACHE_RD_RQSTS_FILE, "w");
    fclose(file);

    file = fopen(CACHE_WR_RQSTS_FILE, "w");
    fclose(file);

    file = fopen(CACHE_WR_RETS_FILE, "w");
    fclose(file);
}

void cache_rd_rqst(WORD address, TIME current_time)
{
    assert(0);
    cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) malloc(sizeof(cache_rd_rqst_t));
    rqst->address = address;
    rqst->time = current_time + CACHE_READ_TIME;

    FILE *file;
    file = fopen(CACHE_RD_RQSTS_FILE, "a");
    fprintf(file, "%d, %d: %d\n", current_time, rqst->time, address);
    fclose(file);

    priority_list_push(&rqst->time, rqst, rd_rqst_queue);
}

void cache_wr_rqst(WORD address, WORD data, TIME current_time)
{
    //vpi_printf("wr rqst made\n");
    if(cache.lines[0].dirty==1)
    {
        //vpi_printf("dirty bit is set %d\n", current_time);
    }
    cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) malloc(sizeof(cache_wr_rqst_t));
    rqst->address = address;
    rqst->data = data;
    rqst->time = current_time + CACHE_WRITE_TIME;


    BYTE tag = TAG(rqst->address);
    BYTE cache_line_number = CACHELINE(rqst->address);
    BYTE offset = OFFSET(rqst->address);
    FILE *file;
    file = fopen(CACHE_WR_RQSTS_FILE, "a");
    fprintf(file, "%d, %d: address %d data %d tag %d cache line # %d offset %d\n", current_time, rqst->time, address, data, tag, cache_line_number, offset);
    fclose(file);

    priority_list_push(&rqst->time, rqst, wr_rqst_queue);
}

cache_rd_ret_t* cache_rd_ret(TIME current_time)
{
    assert(0);
    if(priority_list_empty(rd_rqst_queue))
    {
        return NULL;
    }

    cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) priority_list_front(rd_rqst_queue);
    
    BYTE tag = TAG(rqst->address);
    BYTE cache_line_number = CACHELINE(rqst->address);
    BYTE offset = OFFSET(rqst->address);
    WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);

    if(rqst->time <= current_time)
    {
        //vpi_printf("%d %d\n", rqst->address, rqst->time);
        priority_list_pop(rd_rqst_queue);

        if(in_cache(cache_line_number, tag))
        {
            cache_rd_ret_t* ret = (cache_rd_ret_t*) malloc(sizeof(cache_rd_ret_t));
            ret->address = rqst->address;
            ret->data = cache.lines[cache_line_number].data[offset];
            return ret;
        }
        else
        {
            rbtree_add(&rqst->address, rqst, cache_rd_miss_table);
            if(!mem_rd_rqst_pending(tag, cache_line_number))
            {
                mem_rd_rqst(start_address, current_time);
                set_mem_rd_rqst_pending(tag, cache_line_number);
            }
        }
    }
    return NULL;
}

cache_wr_ret_t* cache_wr_ret(TIME current_time)
{
    if(priority_list_empty(wr_rqst_queue))
    {
        return NULL;
    }

    cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) priority_list_front(wr_rqst_queue);

    BYTE tag = TAG(rqst->address);
    BYTE cache_line_number = CACHELINE(rqst->address);
    BYTE offset = OFFSET(rqst->address);
    WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);

    if(rqst->time <= current_time)
    {
        //vpi_printf("%d: address: %d %d %d %d %d\n", current_time, start_address, tag, cache_line_number, cache.lines[cache_line_number].tag, cache.lines[cache_line_number].valid);
        priority_list_pop(wr_rqst_queue);
        if(in_cache(cache_line_number, tag))
        {
            FILE *file;
            file = fopen(CACHE_WR_RETS_FILE, "a");
            fprintf(file, "%d, %d: address %d data %d\n", current_time, rqst->time, rqst->address, rqst->data);
            fclose(file);

            vpi_printf("%d: writing %d to %d\n", current_time, rqst->data, rqst->address);
            cache.lines[cache_line_number].data[offset] = rqst->data;
            cache.lines[cache_line_number].dirty = 1;

            cache_wr_ret_t* ret = (cache_wr_ret_t*) malloc(sizeof(cache_wr_ret_t));
            ret->address = rqst->address;
            return ret;
        }
        else
        {
            //vpi_printf("time: %d address: %d tag: %d cl: %d pending: %u pending: %u index: %u\n", current_time, start_address, tag, cache_line_number, pending_mem_rd_rqsts[(start_address>>OFFSET_LOG2)], mem_rd_rqst_pending(tag, cache_line_number), (start_address>>OFFSET_LOG2) );

            rbtree_add(&rqst->address, rqst, cache_wr_miss_table);
            if(!mem_rd_rqst_pending(tag, cache_line_number))
            {
                //print_pending();
                vpi_printf("mem rd rqst made\n");
                mem_rd_rqst(start_address, current_time);
                set_mem_rd_rqst_pending(tag, cache_line_number);
            }
        }
    }
    return NULL;
}

void cache_update(TIME current_time)
{
    mem_rd_ret_t* rd_ret = mem_rd_ret(current_time);
    mem_wr_ret_t* wr_ret = mem_wr_ret(current_time);

    // move the cache misses back into valid request queue
    if(rd_ret != NULL)
    {
        vpi_printf("rd_ret not null %d %d\n", current_time, rd_ret->start_address);
        BYTE cache_line_number = CACHELINE(rd_ret->start_address);
        BYTE tag = TAG(rd_ret->start_address);

        // need to use the start address
        clear_mem_rd_rqst_pending(tag, cache_line_number);
        
        // find where to evict
        BYTE lru = evict_lru();
        //vpi_printf("LRU: %d %d\n", lru, rd_ret->start_address);

        // flush lru to open up cache line for new memory
        if (cache.lines[lru].dirty==1)
        {
            //vpi_printf("%d\n", cache.lines[lru].dirty);
            WORD start_address = (cache.lines[lru].tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (lru << OFFSET_LOG2);
            mem_wr_rqst(cache.lines[lru].data, start_address, current_time);
        }

        // update this. 
        // we need to make sure we are thinking of this correctly
        cache.lines[lru].tag = tag;
        cache.lines[lru].valid = 1;
        cache.lines[lru].dirty = 0;
        
        // put the rd_ret into cache.
        memcpy(cache.lines[cache_line_number].data, rd_ret->data, WORDS_PER_CACHE_LINE * sizeof(WORD));
        // we were doing this like a complete moron and it was overwriting valid and next
        // memcpy(cache.lines[cache_line_number].data, rd_ret->data, NUM_CACHE_LINES * sizeof(WORD));

        WORD address = rd_ret->start_address;
        while(address < rd_ret->start_address+WORDS_PER_CACHE_LINE)
        {
            if(rbtree_contains(&address, cache_rd_miss_table))
            {
                cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) rbtree_search(&address, cache_rd_miss_table);
                rqst->time = current_time + CACHE_READ_TIME;
                priority_list_push(&rqst->time, rqst, rd_rqst_queue);
                rbtree_remove(&address, cache_rd_miss_table);
            }
            else if(rbtree_contains(&address, cache_wr_miss_table))
            {
                cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) rbtree_search(&address, cache_wr_miss_table);
                rqst->time = current_time + CACHE_READ_TIME;
                priority_list_push(&rqst->time, rqst, wr_rqst_queue);
                rbtree_remove(&address, cache_wr_miss_table);
            }
            else
            {
                address++;
            }
        }

    }
    if(wr_ret != NULL)
    {
        vpi_printf("wr_ret not null %d\n", current_time);
    }
}

void dump_cache()
{
    FILE *file;
    file = fopen(CACHE_FILE, "w");
    
    int i, j;
    for(i=0; i<NUM_CACHE_LINES; i++)
    {
        fprintf(file, "cache line: %d dirty %d valid %d next %d\n", i, cache.lines[i].dirty, cache.lines[i].valid, cache.lines[i].next);
        for(j=0; j<WORDS_PER_CACHE_LINE; j++)
        {
            fprintf(file, "%d: %d\n", j, cache.lines[i].data[j]);
        }
        fprintf(file, "\n");
    }

    fclose(file);
}

static BOOL in_cache(BYTE cache_line_number, BYTE tag)
{
    return ( (cache.lines[cache_line_number].tag == tag) && (cache.lines[cache_line_number].valid==1) );
}

static BOOL mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number)
{
    WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);
    unsigned int index = start_address >> OFFSET_LOG2;
    return pending_mem_rd_rqsts[index];
}

static void set_mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number)
{
    WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);
    unsigned int index = start_address >> OFFSET_LOG2;
    //printf("setting address: %u\n", index);
    pending_mem_rd_rqsts[index] = 1;
}

static void clear_mem_rd_rqst_pending(BYTE tag, BYTE cache_line_number)
{
    WORD start_address = (tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (cache_line_number << OFFSET_LOG2);
    unsigned int index = start_address >> OFFSET_LOG2;
    pending_mem_rd_rqsts[index] = 0;
}

static BYTE evict_lru()
{
    int i;
    for(i=0; i<NUM_CACHE_LINES;i++)
    {
        //vpi_printf("%d NEXT: %d %d %d\n", i, cache.lines[i].next, cache.lru, cache.mru);
    }
    BYTE evicted = cache.lru;
    cache.lru = cache.lines[cache.lru].next;
    cache.lines[cache.mru].next = evicted;
    cache.mru = evicted;
    return evicted;
    // this actually works
    // return cache.lru++;
}

static void set_mru()
{
}

static print_pending()
{
    int i;
    for(i=0; i<10; i++)
    {
        printf("%u | ", pending_mem_rd_rqsts[i]);
    }
    printf("\n");
}






