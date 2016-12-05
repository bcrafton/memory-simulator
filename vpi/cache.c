
static cache_t;

static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;

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


cache_rd_ret_t* cache_rd_ret(TIME time)
{
    byte tag = TAG(rqst->address);
    byte cache_line = CACHELINE(rqst->address);

    cache_rd_ret* ret = (cache_rd_ret*) malloc(sizeof(cache_rd_ret));
    
    // need to check the time and availability
    

    // then decide whether to make a mem rqst or not
    
}

cache_wr_ret* cache_wr_ret(TIME time)
{
    byte tag = TAG(rqst->address);
    byte cache_line = CACHELINE(rqst->address);

    cache_wr_ret* ret = (cache_wr_ret*) malloc(sizeof(cache_wr_ret));
    
    // need to check the time and availability
    
    // then decide whether to make a mem rqst or not
    
}



























