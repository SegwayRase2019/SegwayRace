OBJS1	=	Wii_main.o Wii_calculate.o Wii_command.o
TARGET1	=	board
CFLAGS	=	-c -DNDEBUG

.c.o:
	gcc $(CFLAGS) $<

all: $(TARGET1)

$(TARGET1):	$(OBJS1)
	gcc -o $(TARGET1) $(OBJS1) -lm -lcwiid -lbluetooth


clean:
	rm *.o $(TARGET1)



Wii_main.o: Wii_main.c Wii_common.h
Wii_calculate.o: Wii_calculate.c Wii_common.h
Wii_command.o: Wii_command.c Wii_common.h