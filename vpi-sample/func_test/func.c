#include <vpi_user.h>

static int func_compiletf(char*user_data)
{
  return 0;
}

typedef struct memory_t
{
    unsigned short memory[1024];
} memory_t;

static memory_t memory;


static int func_calltf(char*user_data)
{
  vpiHandle vhandle, iterator, arg;
  s_vpi_value inval, outval;
  //int din, dout;
  char* din = malloc(100); 
  char* dout = malloc(100);
  vhandle = vpi_handle(vpiSysTfCall, NULL);
  iterator = vpi_iterate(vpiArgument, vhandle);
  arg = vpi_scan(iterator);

  inval.format = vpiHexStrVal;
  vpi_get_value(arg, &inval);
  din = inval.value.vector;

  vpi_printf("din=%s\n", din);
  dout = din;

  outval.format = vpiIntVal;
  outval.value.integer = dout;
  vpi_put_value(vhandle, &outval, NULL, vpiNoDelay);

  return 0;
}

static int func_retsize()
{
  return 32;
}

void func_register()
{
  s_vpi_systf_data tf_data;
  tf_data.type        = vpiSysFunc;
  tf_data.sysfunctype = vpiIntFunc;
  tf_data.tfname    = "$func_test";
  tf_data.calltf    = func_calltf;
  tf_data.compiletf = func_compiletf;
  tf_data.sizetf    = func_retsize;
  tf_data.user_data = 0;
  vpi_register_systf(&tf_data);
}

void (*vlog_startup_routines[])() = {
  func_register,
  0
};
