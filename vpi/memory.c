
#include "memory_simulator.h"

static unsigned short memory[MEMORY_SIZE];

static PriorityList* rd_rqst_queue = NULL;
static PriorityList* wr_rqst_queue = NULL;

void mem_init()
{
    rd_rqst_queue = priority_list_constructor(&time_compare);
    wr_rqst_queue = priority_list_constructor(&time_compare);
    int i;
    for(i=0; i<MEMORY_SIZE; i++)
    {
        memory[i] = 0;
    }

    FILE *file;

    file = fopen(MEM_RD_RQSTS_FILE, "w");
    fclose(file);

    file = fopen(MEM_WR_RQSTS_FILE, "w");
    fclose(file);

    file = fopen(MEM_RD_RETS_FILE, "w");
    fclose(file);

    file = fopen(MEM_WR_RETS_FILE, "w");
    fclose(file);
}

void mem_rd_rqst(WORD start_address, TIME current_time)
{
    vpi_printf("mem rd rqst made %d %d\n", current_time, start_address);

    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) malloc(sizeof(mem_rd_rqst_t));
    rqst->start_address = start_address;
    rqst->time = current_time + MEMORY_READ_TIME;

    FILE *file;
    file = fopen(MEM_RD_RQSTS_FILE, "a");
    fprintf(file, "%d, %d: %d\n", current_time, rqst->time, start_address);
    fclose(file);

    priority_list_push(&rqst->time, rqst, rd_rqst_queue);
}

void mem_wr_rqst(WORD* data, WORD start_address, TIME current_time)
{
   vpi_printf("mem wr rqst made %d %d\n", current_time, start_address);

    mem_wr_rqst_t* rqst = (mem_wr_rqst_t*) malloc(sizeof(mem_wr_rqst_t));
    rqst->start_address = start_address;
    rqst->time = current_time + MEMORY_WRITE_TIME;

    memcpy(rqst->data, data, WORDS_PER_CACHE_LINE * sizeof(WORD));

    FILE *file;
    file = fopen(MEM_WR_RQSTS_FILE, "a");
    fprintf(file, "%d, %d: address %d data %d %d %d %d %d %d %d %d\n", 
            current_time, 
            rqst->time, 
            start_address, 
            data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);

    fclose(file);

    priority_list_push(&rqst->time, rqst, wr_rqst_queue);
}

mem_rd_ret_t* mem_rd_ret(TIME current_time)
{
    if(priority_list_empty(rd_rqst_queue))
    {
        return NULL;
    }
    mem_rd_rqst_t* rqst = (mem_rd_rqst_t*) priority_list_front(rd_rqst_queue);
    if(rqst->time <= current_time)
    {
        priority_list_pop(rd_rqst_queue);

        mem_rd_ret_t* ret = (mem_rd_ret_t*) malloc(sizeof(mem_rd_ret_t));

        // memcpy(ret->data, memory + rqst->start_address * sizeof(WORD), WORDS_PER_CACHE_LINE * sizeof(WORD));
        // do it this way instead
        memcpy(ret->data, &(memory[rqst->start_address]), WORDS_PER_CACHE_LINE * sizeof(WORD));

        ret->start_address = rqst->start_address;

        WORD* data = (WORD*) (memory + rqst->start_address * sizeof(WORD));
        FILE *file;
        file = fopen(MEM_RD_RETS_FILE, "a");
        fprintf(file, "%d, %d: address %d data %d %d %d %d %d %d %d %d\n", 
                current_time, 
                rqst->time, 
                ret->start_address, 
                data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        
        fclose(file);

        return ret;
    }
    return NULL;
}

mem_wr_ret_t* mem_wr_ret(TIME current_time)
{
    if(priority_list_empty(wr_rqst_queue))
    {
        return NULL;
    }
    mem_wr_rqst_t* rqst = (mem_wr_rqst_t*) priority_list_front(wr_rqst_queue);
    if(rqst->time <= current_time)
    {
        //vpi_printf("writing to memory %x\n", rqst->data[0]);
        priority_list_pop(wr_rqst_queue);
        
        //memcpy(memory + rqst->start_address * sizeof(WORD), rqst->data, WORDS_PER_CACHE_LINE * sizeof(WORD));
        // do it this way instead
        memcpy(&(memory[rqst->start_address]), rqst->data, WORDS_PER_CACHE_LINE * sizeof(WORD));

        mem_wr_ret_t* ret = (mem_wr_ret_t*) malloc(sizeof(mem_wr_ret_t));
        ret->start_address = rqst->start_address;

        WORD* data = rqst->data;
        FILE *file;
        file = fopen(MEM_WR_RETS_FILE, "a");
        fprintf(file, "%d, %d: address %d data %d %d %d %d %d %d %d %d\n", 
                current_time, 
                rqst->time, 
                ret->start_address, 
                data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7]);
        
        fclose(file);

        return ret;
    }
    return NULL;
}

void dump_memory()
{
    FILE *file;
    file = fopen(MEMORY_FILE, "w");
    
    int i;
    for(i=0; i<MEMORY_SIZE; i++)
    {
        fprintf(file, "%d: %d\n", i, memory[i]);
    }

    fclose(file);
}









