#include <vpi_user.h>

#define BITS_IN_INT 32
#define MEMORY_SIZE 1024

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

    vpi_printf("%d %d\n", rqst->address, rqst->time);
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

    vpi_printf("%d %d %d\n", rqst->address, rqst->data, rqst->time);
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

void (*vlog_startup_routines[])() = {
    rd_rqst_register,
    wr_rqst_register,
    0
};
