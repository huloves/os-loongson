BUILD_DIR = ./build
CROSS_COMPILE = ../toolchains/cross-tools/bin/loongarch64-unknown-linux-gnu-
AS = $(CROSS_COMPILE)as
CC = $(CROSS_COMPILE)gcc
LD = $(CROSS_COMPILE)ld
CFLAGS = -Wall -g -march=loongarch64 -mabi=lp64s -ffreestanding -fno-common -nostdlib -fno-stack-protector -fno-pie -no-pie -c -I ./include/ -I ./lib/
AFLAGS = -nostdinc -D__ASSEMBLY__ -fno-PIE -mabi=lp64s -G0 -pipe -msoft-float -mexplicit-relocs -ffreestanding -mno-check-zero-division -c -I ./include/
LDFLAGS = -m elf64loongarch --no-warn-rwx-segments -G0 -n -nostdlib 

OBJS = $(BUILD_DIR)/head.o $(BUILD_DIR)/init.o $(BUILD_DIR)/setup.o $(BUILD_DIR)/ns16550a.o

$(BUILD_DIR)/head.o: boot/loongarch/head.S
	$(CC) $(AFLAGS) $< -o $@

$(BUILD_DIR)/setup.o: boot/loongarch/setup.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/init.o: kernel/init.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/ns16550a.o: device/ns16550a.c
	$(CC) $(CFLAGS) $< -o $@

$(BUILD_DIR)/kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -T ./script/kernel.ld $^ -o $@

clean:
	cd $(BUILD_DIR) && rm -rf ./* && if [ -e hd*.img  ];then rm ../*.img;fi

build_dir:
	mkdir -p $(BUILD_DIR)

build: $(BUILD_DIR)/kernel.elf

all: build_dir clean build
