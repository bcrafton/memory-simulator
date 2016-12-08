
static cache_t cache;

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;
static List* cache_rd_miss_queue = NULL;
static List* cache_wr_miss_queue = NULL;
static TreeSet* rd_rqsts = NULL;

void cache_init()
{
    rd_rqst_queue = priorityqueue_constructor(&time_compare);
    wr_rqst_queue = priorityqueue_constructor(&time_compare);
    cache_rd_miss_queue = list_constructor();
    cache_wr_miss_queue = list_constructor();
    rd_rqsts = tree_set_constructor(something);

    int i;
    for(i=0;i<NUM_CACHE_LINES;i++)
    {
        cache.lines[i].dirty = 0;
        cache.lines[i].lru_next = i+1;
    }
    cache.lru = 0; 
    cache.mru = NUM_CACHE_LINES-1;
}

void cache_rd_rqst(WORD address, TIME time)
{
    unsigned char tag = TAG(rqst->address);
    unsigned char cache_line = CACHELINE(rqst->address);
    
    cache_rd_rqst_t* rqst = (cache_rd_ret*) malloc(sizeof(cache_rd_rqst));
    rqst->address = address;
    rqst->time = time;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void cache_wr_rqst(WORD address, WORD data, TIME time)
{
    unsigned char tag = TAG(rqst->address);
    unsigned char cache_line = CACHELINE(rqst->address);
    
    cache_wr_rqst_t* rqst = (cache_wr_ret*) malloc(sizeof(cache_wr_rqst));
    rqst->address = address;
    rqst->data = data;
    rqst->time = time;

    priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);
}


cache_rd_ret_t* cache_rd_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(rd_rqst_queue))
    {
        return NULL;
    }
    
    byte tag = TAG(rqst->address);
    byte cache_line_number = CACHELINE(rqst->address);
    byte offset = OFFSET(rqst->address);

    cache_rd_rqst_t* rqst = (cache_rd_rqst_t*) priorityqueue_front(cache_rd_rqst_t);
    if(rqst->time <= time)
    {
        priorityqueue_pop(rd_rqst_queue);
        if(inCache(cache_line_number, tag))
        {
            cache_rd_ret_t* ret = (cache_rd_ret_t*) malloc(sizeof(cache_rd_ret_t));
            ret->address = rqst->address;
            ret->data = cache.lines[cache_line_number].line[offset];
            ret->ack = 1;
            return ret;
        }
        else
        {
            list_append(rqst, cache_rd_miss_queue);
            // need to keep track of memory requests
            // if(tree_set_contains())
            mem_rd_rqst(tag, cache_line_number, current_time);
        }
    }
    return NULL;
}

cache_wr_ret* cache_wr_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(wr_rqst_queue))
    {
        return NULL;
    }

    byte tag = TAG(rqst->address);
    byte cache_line_number = CACHELINE(rqst->address);

    cache_wr_rqst_t* rqst = (cache_wr_rqst_t*) priorityqueue_front(cache_wr_rqst_t);
    if(rqst->time <= time)
    {
        priorityqueue_pop(wr_rqst_queue);
        if(inCache(cache_line_number, tag))
        {
            cache_wr_ret_t* ret = (cache_wr_ret_t*) malloc(sizeof(cache_wr_ret_t));
            ret->address = rqst->address;
            ret->ack = 1;
            return ret;
        }
        else
        {
            list_append(rqst, cache_wr_miss_queue);
            // need to keep track of memory requests
            // if(tree_set_contains())
            mem_rd_rqst(tag, cache_line_number, current_time);
        }
    }
    return NULL;
    
}

bool in_cache(byte cache_line_number, byte tag)
{
    return cache.lines[cache_line_number].tag == tag;
}

void cache_update(TIME current_time)
{
    mem_rd_ret_t* rd_ret = mem_rd_ret(current_time);
    mem_wr_ret_t* wr_ret = mem_wr_ret(current_time);

    // put the rd_ret into cache.
    

    // move the cache misses back into valid request queue
    if(rd_ret != NULL)
    {
        int i;
        for(i=0; i<cache_rd_miss_queue.size; i++)
        {
            if(in_cache())
            {
                cache_rd_rqst_t* rqst = list_remove(i, cache_rd_miss_queue);
                rqst.time = current_time + 4;
                priorityqueue_push(rqst, rd_rqst_queue);
            }
        }

        for(i=0; i<cache_wr_miss_queue.size; i++)
        {
            if(in_cache())
            {
                cache_wr_rqst_t* rqst = list_remove(i, cache_wr_miss_queue);
                rqst.time = current_time + 4;
                priorityqueue_push(rqst, wr_rqst_queue);
            }
        }
        
        byte lru = evict_lru();
        short start_address = (cache.lines[lru].tag << (CACHELINE_LOG2 + OFFSET_LOG2)) | (lru << OFFSET_LOG2);
        mem_wr_rqst(cache.lines[lru].data, start_address, lru, NUM_CACHE_LINES);
    }
}




byte evict_lru()
{
    byte evicted = cache.lru;
    cache.lru = cache.lines[cache.lru].next;
    cache.lines[cache.mru].next = evicted;
    cache.mru = evicted;
    return evicted;
}



















