#include <vpi_user.h>
#include "../Data-Structures/header/priority_queue.h"

#define BITS_IN_INT 32
#define MEMORY_SIZE 1024
#define ADDRESS_WIDTH 16
#define DATA_WIDTH 16
#define ACK_WIDTH 1

typedef struct rd_rqst_t{
    unsigned short address;
    unsigned long time;
} rd_rqst_t;

typedef struct wr_rqst_t{
    unsigned short address;
    unsigned short data;
    unsigned long time;
} wr_rqst_t;

static unsigned short memory[MEMORY_SIZE];
static PriorityQueue* rd_rqst_queue = NULL;
static PriorityQueue* wr_rqst_queue = NULL;

int time_compare(void *o1, void *o2){
	return *((unsigned long*)o1) - *((unsigned long*)o2);
}

static int func_compiletf(char*user_data)
{
    return 0;
}

static int rd_rqst(char*user_data)
{
    if (rd_rqst_queue == NULL)
    {
        rd_rqst_queue = priorityqueue_constructor(&time_compare);
    }

    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int rd_address;
    unsigned int time_h;
    unsigned int time_l;

    iterator = vpi_iterate(vpiArgument, vhandle);
    
    arg = vpi_scan(iterator);
    inval.format = vpiIntVal;
    vpi_get_value(arg, &inval);
    rd_address = inval.value.integer;

    arg = vpi_scan(iterator);
    inval.format = vpiTimeVal;
    vpi_get_value(arg, &inval);
    time_h = inval.value.time->high;
    time_l = inval.value.time->low;
    
    rd_rqst_t* rqst = (rd_rqst_t*) malloc(sizeof(rd_rqst_t));
    rqst->address = rd_address;
    rqst->time = (time_h << BITS_IN_INT) | time_l;
    rqst->time = rqst->time + 10;

    priorityqueue_push(&rqst->time, rqst, rd_rqst_queue);

    //vpi_printf("%d\n", priorityqueueIsEmpty(rd_rqst_queue));

    //vpi_printf("%d %d\n", rqst->address, rqst->time);
    return 0; 
}

static int wr_rqst(char*user_data)
{
    if (wr_rqst_queue == NULL)
    {
        wr_rqst_queue = priorityqueue_constructor(&time_compare);
    }

    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int wr_address;
    unsigned int wr_data;
    unsigned int time_h;
    unsigned int time_l;

    iterator = vpi_iterate(vpiArgument, vhandle);
    
    arg = vpi_scan(iterator);
    inval.format = vpiIntVal;
    vpi_get_value(arg, &inval);
    wr_address = inval.value.integer;

    arg = vpi_scan(iterator);
    inval.format = vpiIntVal;
    vpi_get_value(arg, &inval);
    wr_data = inval.value.integer;

    arg = vpi_scan(iterator);
    inval.format = vpiTimeVal;
    vpi_get_value(arg, &inval);
    time_h = inval.value.time->high;
    time_l = inval.value.time->low;
    
    wr_rqst_t* rqst = (wr_rqst_t*) malloc(sizeof(wr_rqst_t));
    rqst->address = wr_address;
    rqst->data = wr_data;
    rqst->time = (time_h << BITS_IN_INT) | time_l;
    rqst->time = rqst->time + 10;

    priorityqueue_push(&rqst->time, rqst, wr_rqst_queue);

    //vpi_printf("%d %d %d\n", rqst->address, rqst->data, rqst->time);
    return 0; 
}

static int rd_ret(char*user_data)
{    

    // get the time
    unsigned long current_time;

    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int time_h;
    unsigned int time_l;

    iterator = vpi_iterate(vpiArgument, vhandle);

    arg = vpi_scan(iterator);
    inval.format = vpiTimeVal;
    vpi_get_value(arg, &inval);
    time_h = inval.value.time->high;
    time_l = inval.value.time->low;
    current_time = (time_h << BITS_IN_INT) | time_l;

    // set the output
    unsigned int rd_ret_address;
    unsigned int rd_ret_data;
    unsigned int rd_ret_ack;

    if (rd_rqst_queue == NULL)
    {
        rd_ret_address = 0;
        rd_ret_data = 0;
        rd_ret_ack = 0;
    }
    else if (priorityqueueIsEmpty(rd_rqst_queue))
    {
        rd_ret_address = 0;
        rd_ret_data = 0;
        rd_ret_ack = 0;
    }
    else
    {
        rd_rqst_t* rqst = (rd_rqst_t*) priorityqueue_front(rd_rqst_queue);
        if(rqst->time <= current_time)
        {
            priorityqueue_pop(rd_rqst_queue);
            rd_ret_address = rqst->address;
            rd_ret_data = memory[rqst->address];
            rd_ret_ack = 1;
        }
        else
        {
            rd_ret_address = 0;
            rd_ret_data = 0;
            rd_ret_ack = 0; 
        }
    }

    // this is poorly done
    // we also could have just set address to be 10 bits
    // then only used an integer
    // or cud have just used an integer but increased the width?
    // no actually int cannot be increased width need this
    // ... unless we switched to 10 bits address
    // this is actually better long term tho
    // im prove this logic here tho
    unsigned long bus_out;
    bus_out = rd_ret_address;
    bus_out = (bus_out << DATA_WIDTH) | rd_ret_data;
    bus_out = (bus_out << ACK_WIDTH) | rd_ret_ack;
    vpi_printf("%lx\n", bus_out);

    s_vpi_value out;
    out.format = vpiVectorVal;
    out.value.vector = (s_vpi_vecval*) malloc(sizeof(s_vpi_vecval) * 2);
    out.value.vector[0].aval = bus_out;
    out.value.vector[0].bval = 0;
    out.value.vector[1].aval = bus_out >> 32;
    out.value.vector[1].bval = 0;

    vpi_put_value(vhandle, &out, NULL, vpiNoDelay);

    return 0; 
}

static int wr_ret(char*user_data)
{    
    // get the time
    unsigned long current_time;

    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int time_h;
    unsigned int time_l;

    iterator = vpi_iterate(vpiArgument, vhandle);

    arg = vpi_scan(iterator);
    inval.format = vpiTimeVal;
    vpi_get_value(arg, &inval);
    time_h = inval.value.time->high;
    time_l = inval.value.time->low;
    current_time = (time_h << BITS_IN_INT) | time_l;

    // set the output
    unsigned int wr_ret_address;
    unsigned int wr_ret_ack;

    if (wr_rqst_queue == NULL)
    {
        wr_ret_address = 0;
        wr_ret_ack = 0;
    }
    else if (priorityqueueIsEmpty(wr_rqst_queue))
    {
        wr_ret_address = 0;
        wr_ret_ack = 0;
    }
    else
    {
        wr_rqst_t* rqst = (wr_rqst_t*) priorityqueue_front(wr_rqst_queue);
        if(rqst->time <= current_time)
        {
            priorityqueue_pop(wr_rqst_queue);
            wr_ret_address = rqst->address;
            wr_ret_ack = 1;
            memory[rqst->address] = rqst->data;
        }
        else
        {
            wr_ret_address = 0;
            wr_ret_ack = 0;
        }
    }

    unsigned long bus_out;
    bus_out = wr_ret_address;
    bus_out = (bus_out << ACK_WIDTH) | wr_ret_ack;
    vpi_printf("%lx\n", bus_out);

    s_vpi_value out;
    out.format = vpiVectorVal;
    out.value.vector = (s_vpi_vecval*) malloc(sizeof(s_vpi_vecval));
    out.value.vector[0].aval = bus_out;
    out.value.vector[0].bval = 0;

    vpi_put_value(vhandle, &out, NULL, vpiNoDelay);

    return 0; 
}

static int func_retsize()
{
    return BITS_IN_INT;
}

void rd_rqst_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname    = "$rd_rqst";
    tf_data.calltf    = rd_rqst;
    tf_data.compiletf = func_compiletf;
    tf_data.sizetf    = func_retsize;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void wr_rqst_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname    = "$wr_rqst";
    tf_data.calltf    = wr_rqst;
    tf_data.compiletf = func_compiletf;
    tf_data.sizetf    = func_retsize;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void rd_ret_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSizedFunc;
    tf_data.tfname    = "$rd_ret";
    tf_data.calltf    = rd_ret;
    tf_data.compiletf = func_compiletf;
    tf_data.sizetf    = func_retsize;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void wr_ret_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiSizedFunc;
    tf_data.tfname    = "$wr_ret";
    tf_data.calltf    = wr_ret;
    tf_data.compiletf = func_compiletf;
    tf_data.sizetf    = func_retsize;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
    rd_rqst_register,
    wr_rqst_register,
    rd_ret_register,
    wr_ret_register,
    0
};
