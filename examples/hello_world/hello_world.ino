#include "mmruby_arduino.h"

void setup()
{
	//inialize variables of the VM
	mmruby_setup();
	//kick the VM
	mmruby_run();
}
void loop()
{
	delay(1000);
}
