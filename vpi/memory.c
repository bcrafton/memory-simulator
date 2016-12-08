
static unsigned short memory[MEMORY_SIZE];

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;

void mem_init()
{
    rd_rqst_queue = priorityqueue_constructor(&time_compare);
    wr_rqst_queue = priorityqueue_constructor(&time_compare);
}

void mem_rd_rqst(WORD start_address, byte size, TIME time)
{
    unsigned char tag = TAG(rqst->address);
    unsigned char cache_line = CACHELINE(rqst->address);
    
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) malloc(sizeof(mem_rd_rqst_t));
    rqst->start_address = start_address;
    rqst->size = size;
    rqst->time = time;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void mem_wr_rqst(WORD data[], WORD start_address, byte size, TIME time)
{
    mem_wr_rqst* rqst = (mem_wr_rqst*) malloc(sizeof(mem_wr_rqst));
    rqst->data = data;
    rqst->start_address = start_address;
    rqst->size = size;
    rqst->time = time;

    priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);
}

mem_rd_ret_t* mem_rd_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(rd_rqst_queue))
    {
        return NULL;
    }
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) priorityqueue_front(mem_rd_rqst_t);
    if(rqst->time <= time)
    {
        // this is doing to much, it shud not be returning all this stuff to the cache, just the data.
        priorityqueue_pop(rd_rqst_queue);
        mem_rd_ret_t* ret = (mem_rd_ret_t*) malloc(sizeof(mem_rd_ret_t));
        cache_line_t* cache_line = (cache_line_t*) malloc(sizeof(cache_line_t));
        cache_line->tag = tag;
        cache_line->cache_line_number = cache_line_number;
        unsigned short offset = tag | cache_line_number;
        memcpy(cache_line->line, memory + offset * sizeof(WORD), WORDS_PER_CACHE_LINE * sizeof(WORD));
        cache_line->dirty = 0;
        cache_line->valid = 1;
        ret->cache_line = cache_line;
        ret->ack = 1;
        return ret;
    }
    return NULL;
}


mem_wr_ret_t* mem_wr_ret(TIME current_time)
{
    if(priorityqueueIsEmpty(wr_rqst_queue))
    {
        return NULL;
    }
    mem_wr_rqst_t* rqst = (mem_wr_rqst_t*) priorityqueue_front(mem_wr_rqst_t);
    if(rqst->time <= time)
    {
        priorityqueue_pop(wr_rqst_queue);
        unsigned short offset = rqst->cache_line->tag | rqst->cache_line->cache_line_number;
        memcpy(memory + offset * sizeof(WORD), rqst->cache_line->line, WORDS_PER_CACHE_LINE * sizeof(WORD));

        mem_wr_ret_t* ret = (mem_wr_ret_t*) malloc(sizeof(mem_wr_ret_t));
        ret->tag = rqst->cache_line->tag;
        ret->cache_line_number = rqst->cache_line->cache_line_number;
        ret->ack = 1;
        return ret;
    }
    return NULL;
}
