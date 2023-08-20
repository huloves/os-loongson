####  此脚本应该在command目录下执行

if [[ ! -d "../lib" || ! -d "../build" ]];then
	echo "dependent dir don\`t exist!"
	cwd=$(pwd)
	cwd=${cwd##*/}
	cwd=${cwd%/}
	if [[ $cwd != "command" ]];then
	echo -e "you\`d better in command dir\n"
	fi 
	exit
fi

OBJ_DIR="../build/command"

if [[ ! -d "$OBJ_DIR" ]];then
	echo mkdir -p $OBJ_DIR
	mkdir -p $OBJ_DIR
fi

if [[ -d "$OBJ_DIR" ]];then
	BIN="cat"
	CFLAGS="-m32 -Wall -c -fno-builtin -W -Wstrict-prototypes \
		-Wmissing-prototypes -Wsystem-headers -fno-stack-protector"
	LIBS="-I ../lib/ -I ../lib/kernel/ -I ../lib/user/ -I \
		../kernel/ -I ../device/ -I ../thread/ -I \
		../userprog/ -I ../fs/ -I ../shell/"
	OBJS="../build/string.o ../build/syscall.o \
		../build/stdio.o ../build/assert.o"
	DD_IN=$OBJ_DIR/$BIN
	DD_OUT="/home/huloves/repositories/os-loongson/bochs/hd80M.img" 

	nasm -f elf ./start.S -o $OBJ_DIR/start.o
	ar rcs $OBJ_DIR/simple_crt.a $OBJS $OBJ_DIR/start.o
	gcc $CFLAGS $LIBS -o $OBJ_DIR/$BIN".o" $BIN".c"
	ld -m elf_i386 $OBJ_DIR/$BIN".o" $OBJ_DIR/simple_crt.a -o $OBJ_DIR/$BIN
	SEC_CNT=$(ls -l $OBJ_DIR/$BIN|awk '{printf("%d", ($5+511)/512)}')

	if [[ -f $OBJ_DIR/$BIN ]];then
		dd if=./$DD_IN of=$DD_OUT bs=512 \
		count=$SEC_CNT seek=300 conv=notrunc
	fi
else
	echo "no $OBJ_DIR"
fi
