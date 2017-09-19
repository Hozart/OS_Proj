# commands/Makefile

#ENTRYPOINT	= 0x1000
HD		= ../80m.img

ASM		= nasm
DASM		= objdump
CC		= gcc -m32
LD		= ld -m elf_i386
ASMFLAGS	= -I ../include/ -I ../include/sys/ -f elf
CFLAGS		= -I ../include/ -I ../include/sys/ -c -fno-builtin -fno-stack-protector -Wall
LDFLAGS		= -Ttext 0x1000
DASMFLAGS	= -D
LIB		= ../lib/orangescrt.a
BIN		= echo pwd help fm su 2048 rand bomb calendar

# All Phony Targets
.PHONY : everything final clean realclean disasm all install

# Default starting position
everything : $(BIN)

install : all clean
	cp ../kernel.bin ./ -v
	tar vcf inst.tar kernel.bin $(BIN)
	dd if=inst.tar of=$(HD) seek=`echo "obase=10;ibase=16;(\`egrep -e '^ROOT_BASE' ../boot/include/load.inc | sed -e 's/.*0x//g'\`+\`egrep -e '#define[[:space:]]*INSTALL_START_SECT' ../include/sys/config.h | sed -e 's/.*0x//g'\`)*200" | bc` bs=1 count=`ls -l inst.tar | awk -F " " '{print $$5}'` conv=notrunc

all : realclean everything

final : all clean

clean :
	rm -f *.o $(OBJS)

realclean :
	rm -f $(BIN) *.o $(OBJS)

kernel.bin :
	cp ../kernel.bin ./

start.o : start.asm
	$(ASM) $(ASMFLAGS) -o $@ $<

echo.o: echo.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

echo : echo.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

pwd.o: pwd.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

pwd : pwd.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

fm.o: fm.c
	$(CC) $(CFLAGS) -o $@ $<

fm : fm.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
	
su.o: su.c
	$(CC) $(CFLAGS) -o $@ $<

su : su.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
	
help.o: help.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

help : help.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

2048.o: 2048.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

2048 : 2048.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

rand.o: rand.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

rand : rand.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

bomb.o: bomb.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

bomb : bomb.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?

calendar.o: calendar.c ../include/type.h ../include/stdio.h
	$(CC) $(CFLAGS) -o $@ $<

calendar : calendar.o start.o $(LIB)
	$(LD) $(LDFLAGS) -o $@ $?
	
