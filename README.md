# micro mruby for arduino uno

This is my experimental project to show that mruby/c can work on Arduino Uno, which has 2KB RAM.
I call it "micro mruby", which is a subset of mruby/c with a lot of limitation.

# Required environment

* Arduino Uno
* Arduino IDE
* Complile environment for C (gcc, make)

# How to run it?

## Install

Copy micro_mruby_for_arduino_uno to the Arduino library folder like other libraries.

## Prepare a transcoder tool

This project needs a transcoder tool. This transcodes a mrb file to C header file for mruby/c.

```
$ cd tool
$ make
```

You will get the TransCoder.

## Compile target mruby code

Please compile your target mruby code. I confirmed it with mruby ver1.3. Latest master version is not supported since the bytecode format is updated.

```
$ mrbc code.rb
```

## Transcode

```
$ Transcode code.mrb
```

It will make to files in /src. They are "code.h" and "symbol_ids.h".

## Make a binary

Open Ardiono IDE. Implement the following sketch.

```C
#include "mmruby_arduino.h"
void setup()
{
	mmruby_setup();
	mmruby_run();
}
void loop(){}
```

Then compile and run it on Arduino Uno.
