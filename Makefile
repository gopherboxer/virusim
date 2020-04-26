src = $(wildcard *.c)
obj = $(src:.c=.o)

virusim: $(obj)
	$(CC) -o $@ $^ 

%.o: %.cc
	$(CC) -o $@ -c $<
