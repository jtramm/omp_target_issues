COMPILER = intel

ifeq ($(COMPILER),intel)
  CC = icpx
  CFLAGS = -Wall -fiopenmp -fopenmp-targets=spir64 -D__STRICT_ANSI__
endif

ifeq ($(COMPILER),ibm)
  CC = xlc_r
  CFLAGS = -Wall -qsmp=omp -qoffload
endif

program = test

source = main.cpp

obj = $(source:.cpp=.o)

deps = Makefile

$(program): $(obj) $(deps)
	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.cpp $(deps)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) $(deps)
