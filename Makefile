CFLAGS=-Wall	-Wextra	-pedantic	-Wno-long-long	-o2
OBJS=$(patsubst %.c,%.o,$(wildcard *.c))
EXEC=cartas

$(EXEC):	$(OBJS)
		 $(CC)	$(CFLAGS)	-o	$(EXEC)	$(OBJS)

limpar:
		rm $(EXEC) 	*.o
