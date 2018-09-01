static const char* const basic_class_names[] = {
  "Object",
  "Proc",
  "False",
  "True",
  "Nil",
  "Array",
  "Fixnum",
  "String",
  "Symbol",
  "Range",
  "Arduino",
};

static const char* const static_procs[] = {
  //class name => sym_id, Ruby method name => sym_id, C func name,
  "Object","puts","c_puts",
  "String","+","c_string_add",
  "String","===","c_string_eql",
  "String","size","c_string_size",
  "Arduino","pin_mode","class_arduino_pin_mode",
  "Arduino","digital_write","class_arduino_digital_write",
  "Arduino","digital_read","class_arduino_digital_read",
  "","","",//don't delete this line
};

