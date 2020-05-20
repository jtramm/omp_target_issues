CC = icpx
CFLAGS = -Wall -fiopenmp -fopenmp-targets=spir64
program = test

source = \
main.cpp \
globals.cpp

obj = $(source:.cpp=.o)

deps = \
globals.h \
Makefile

$(program): $(obj) $(deps)
	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.cpp $(deps)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) *.h
