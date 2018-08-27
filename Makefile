#CC="arm-poky-linux-gnueabi-gcc  -march=armv7ve -mfpu=neon  -mfloat-abi=hard -mcpu=cortex-a7 --sysroot=$SDKTARGETSYSROOT"
all:
	$(CC) fx-serial.c main.c -lpthread -o example

shared:
	$(CC) fx-serial.c -fPIC -shared -o libfx-serial.so -lpthread
clean:
	rm -rf example *.so
