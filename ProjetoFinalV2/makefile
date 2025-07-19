CHAIN=arm-none-eabi
CFLAGS=-std=c99 -Wall -Iinc/
SRC=src/
OBJ=obj/
BIN=bin/

all: app

app: $(OBJ)start.o $(OBJ)main.o $(OBJ)gpio.o $(OBJ)interrupt.o $(OBJ)timer.o $(OBJ)uart.o
	$(CHAIN)-ld $(OBJ)start.o $(OBJ)main.o $(OBJ)gpio.o $(OBJ)interrupt.o $(OBJ)timer.o $(OBJ)uart.o -T $(SRC)memmap.ld -o $(OBJ)main.elf
	$(CHAIN)-objcopy $(OBJ)main.elf $(BIN)spl.boot -O binary
	cp $(BIN)spl.boot /tftpboot/app.bin

$(OBJ)start.o: $(SRC)start.s
	$(CHAIN)-as $(IPATH) $(SRC)start.s -o $(OBJ)start.o

$(OBJ)main.o: $(SRC)main.c
	$(CHAIN)-gcc $(CFLAGS) -c $(SRC)main.c -o $(OBJ)main.o

$(OBJ)gpio.o: $(SRC)gpio.c
	$(CHAIN)-gcc $(CFLAGS) -c $(SRC)gpio.c -o $(OBJ)gpio.o

$(OBJ)interrupt.o: $(SRC)interrupt.c
	$(CHAIN)-gcc $(CFLAGS) -c $(SRC)interrupt.c -o $(OBJ)interrupt.o

$(OBJ)timer.o: $(SRC)timer.c
	$(CHAIN)-gcc $(CFLAGS) -c $(SRC)timer.c -o $(OBJ)timer.o

$(OBJ)uart.o: $(SRC)uart.c
	$(CHAIN)-gcc $(CFLAGS) -c $(SRC)uart.c -o $(OBJ)uart.o

copy:
	cp $(BIN)spl.boot /tftpboot/app.bin

clean:
	rm -rf $(OBJ)*.o
	rm -rf $(OBJ)*.elf
	rm -rf $(BIN)*.boot

dump:
	$(CHAIN)-objdump -D $(OBJ)main.elf
