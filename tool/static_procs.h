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
};

static const char* const static_procs[] = {
  //class name => sym_id, Ruby method name => sym_id, C func name,
  "Object","puts","c_puts",
  "Object","test","c_puts",
  "String","+","c_string_add",
  "String","===","c_string_eql",
  "String","size","c_string_size",
  "String","test","c_string_size",
  "","","",//don't delete this line
};

