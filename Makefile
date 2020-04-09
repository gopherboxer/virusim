src = $(wildcard *.cc)
obj = $(src:.c=.o)

LDFLAGS = -pthread
LDFLAGS = -std=c++11

virusim: $(obj)
	$(CXX) -o $@ $^ $(LDFLAGS)

%.o: %.cc
	$(CXX) -o $@ -c $<

.PHONY: clean
	clean:
		rm -f $(obj) virusim
