#include "mmruby.h"
#include "micro_vm.h"
#include "class.h"
#include "hal.h"
#include "debug.h"
#include "console.h"

void mmruby_setup(){
  init_hal();
  mrbc_init_class();
  init_vm();
}

void mmruby_run(void){
  run_vm();
}


