#ifndef MRBC_SRC_HAL_H_
#define MRBC_SRC_HAL_H_


#ifdef __cplusplus
//extern "C" {
#endif

#define SERIAL_FOR_STDIO_BAUDRATE 9600

void init_hal();
void hal_delay(int msec);
int hal_write(int fd, const void *buf, int nbytes);
inline static int hal_flush(int fd){}
void hal_write_string(char* text);

#ifdef __cplusplus
//}
#endif
#endif // ifndef MRBC_HAL_H_
