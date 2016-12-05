
static unsigned short memory[MEMORY_SIZE];

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;

void mem_rd_rqst(byte tag, byte cache_line_number)
{
    unsigned char tag = TAG(rqst->address);
    unsigned char cache_line = CACHELINE(rqst->address);
    
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) malloc(sizeof(mem_rd_rqst_t));
    rqst->tag = tag;
    rqst->cache_line_number = cache_line_number;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);
}

void mem_wr_rqst(cache_line_t* cache_line)
{
}

mem_rd_ret_t* mem_rd_ret()
{
    if()
    {
        // mem_rd_ret_t wraps this cacheline
        cache_line_t* cache_line = (cache_line_t*) malloc(sizeof(cache_line_t));
        cache_line->tag = tag;
        cache_line->cache_line_number = cache_line_number;
        unsigned short offset = tag | cache_line_number;
        memcpy(cache_line->line, memory + offset * sizeof(WORD), WORDS_PER_CACHE_LINE * sizeof(WORD));
        cache_line->dirty = 0;
        cache_line->valid = 1;
        return cache_line;
    }
    else
    {
        return NULL;
    }
}


mem_wr_ret
