#include "mmruby.h"
#include "micro_vm.h"
#include "hal.h"
#include "debug.h"

void mmruby_setup(){
  init_hal();
  init_vm();
}


void mmruby_run(void){
  run_vm();
}


