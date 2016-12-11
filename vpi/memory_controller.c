
#include "memory_simulator.h"

static int func_compiletf(char*user_data)
{
    return 0;
}

static int rd_rqst(char*user_data)
{
    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int rd_address;
    unsigned int time_h;
    unsigned int time_l;
    unsigned long current_time;

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
    current_time = (time_h << BITS_IN_INT) | time_l;

    cache_rd_rqst(rd_address, current_time);

    return 0; 
}

static int wr_rqst(char*user_data)
{
    vpiHandle vhandle, iterator, arg;
    vhandle = vpi_handle(vpiSysTfCall, NULL);

    s_vpi_value inval;
    
    unsigned int wr_address;
    unsigned int wr_data;
    unsigned int time_h;
    unsigned int time_l;
    unsigned long current_time;

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
    
    current_time = (time_h << BITS_IN_INT) | time_l;
    
    cache_wr_rqst(wr_address, wr_data, current_time);

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

    cache_rd_ret_t* ret = (cache_rd_ret_t*) cache_rd_ret(current_time);

    if(ret != NULL)
    {
        rd_ret_address = ret->address;
        rd_ret_data = ret->data;
        rd_ret_ack = 1;
    }
    else
    {
        rd_ret_address = 0;
        rd_ret_data = 0;
        rd_ret_ack = 0;
    }

    unsigned long bus_out;
    bus_out = rd_ret_address;
    bus_out = (bus_out << DATA_WIDTH) | rd_ret_data;
    bus_out = (bus_out << ACK_WIDTH) | rd_ret_ack;

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

    cache_wr_ret_t* ret = (cache_wr_ret_t*) cache_wr_ret(current_time);

    if(ret != NULL)
    {
        wr_ret_address = ret->address;
        wr_ret_ack = 1;
    }
    else
    {
        wr_ret_address = 0;
        wr_ret_ack = 0;
    }

    unsigned long bus_out;
    bus_out = wr_ret_address;
    bus_out = (bus_out << ACK_WIDTH) | wr_ret_ack;

    s_vpi_value out;
    out.format = vpiVectorVal;
    out.value.vector = (s_vpi_vecval*) malloc(sizeof(s_vpi_vecval));
    out.value.vector[0].aval = bus_out;
    out.value.vector[0].bval = 0;

    vpi_put_value(vhandle, &out, NULL, vpiNoDelay);

    return 0; 
}

static int update(char*user_data)
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

    cache_update(current_time);
    //mem_update(current_time);
}

static int init(char*user_data)
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

    cache_init();
    mem_init();
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

void update_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname    = "$update";
    tf_data.calltf    = update;
    tf_data.compiletf = func_compiletf;
    tf_data.sizetf    = func_retsize;
    tf_data.user_data = 0;
    vpi_register_systf(&tf_data);
}

void init_register()
{
    s_vpi_systf_data tf_data;
    tf_data.type        = vpiSysFunc;
    tf_data.sysfunctype = vpiIntFunc;
    tf_data.tfname    = "$init";
    tf_data.calltf    = init;
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
    update_register,
    init_register,
    0
};
