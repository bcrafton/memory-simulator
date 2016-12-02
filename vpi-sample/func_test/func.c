#include <vpi_user.h>

// basically if u search types u can fidn exampels online
// api is pretty good
// this is more than doable which is great.
// dont even need strings, just ints

static int func_compiletf(char*user_data)
{
  return 0;
}

static int func_calltf(char*user_data)
{
  vpiHandle vhandle, iterator, arg;
  s_vpi_value inval, outval;
  char* din = malloc(100); 
  char* dout = malloc(100 * sizeof(s_vpi_vecval));
  vhandle = vpi_handle(vpiSysTfCall, NULL);
  iterator = vpi_iterate(vpiArgument, vhandle);
  arg = vpi_scan(iterator);

  inval.format = vpiHexStrVal;
  vpi_get_value(arg, &inval);
  din = inval.value.vector;

  vpi_printf("din=%s\n", din);
  dout = din;

  outval.format = vpiVectorVal;
  outval.value.vector = (p_vpi_vecval) malloc(sizeof(s_vpi_vecval) * 1);
  outval.value.vector[0].aval = 0xffffffff;
  outval.value.vector[1].aval = 0xffffffff;
  outval.value.vector[2].aval = 0xffffffff;
  outval.value.vector[3].aval = 0xffffffff;
  vpi_put_value(vhandle, &outval, NULL, vpiNoDelay);

  return 0;
}

static int func_retsize()
{
  return 64;
}

void func_register()
{
  s_vpi_systf_data tf_data;
  tf_data.type        = vpiSysFunc;
  tf_data.sysfunctype = vpiSizedFunc;
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
