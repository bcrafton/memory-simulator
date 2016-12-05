
static cache_t cache;

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;
static List* cache_rd_miss_queue = NULL;
static List* cache_wr_miss_queue = NULL;

void cache_rd_rqst(WORD address, TIME time)
{
    if (rd_rqst_queue == NULL)
    {
        rd_rqst_queue = priorityqueue_constructor(&time_compare);
    }

    unsigned char tag = TAG(rqst->address);
    unsigned char cache_line = CACHELINE(rqst->address);
    
    cache_rd_rqst_t* rqst = (cache_rd_ret*) malloc(sizeof(cache_rd_rqst));
    rqst->address = address;
    rqst->time = time;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void cache_wr_rqst(WORD address, WORD data, TIME time)
{
    if (wr_rqst_queue == NULL)
    {
        wr_rqst_queue = priorityqueue_constructor(&time_compare);
    }

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
    if(rd_rqst_queue != NULL && priorityqueueIsEmpty(rd_rqst_queue))
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
            if (cache_rd_miss_queue == NULL)
            {
                cache_rd_miss_queue = list_constructor();
            }
            list_append(rqst, cache_rd_miss_queue);
            // make the read request ... but we need to make sure we are not making double requests
            mem_rd_rqst(tag, cache_line_number, current_time);
        }
    }
    return NULL;
}

cache_wr_ret* cache_wr_ret(TIME current_time)
{
    if(wr_rqst_queue != NULL && priorityqueueIsEmpty(wr_rqst_queue))
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
            if (cache_wr_miss_queue == NULL)
            {
                cache_wr_miss_queue = list_constructor();
            }
            list_append(rqst, cache_wr_miss_queue);
            // make the read request ... but we need to make sure we are not making double requests
            mem_rd_rqst(tag, cache_line_number, current_time);
        }
    }
    return NULL;
    
}

bool inCache(byte cache_line_number, byte tag)
{
    return cache.lines[cache_line_number].tag == tag;
}

void cache_update(TIME current_time)
{
    mem_rd_ret_t* rd_ret = mem_rd_ret(current_time);
    mem_wr_ret_t* wr_ret = mem_wr_ret(current_time);

    // move the cache misses back into valid request queue
    
    // whatever happens for a write.

    // need to make write request if rd ret is valid
    // because we will be flushing somehting
}
























