COMPILER = nvidia

ifeq ($(COMPILER),intel)
  CC = icpx
  CFLAGS = -Wall -fiopenmp -fopenmp-targets=spir64 -D__STRICT_ANSI__ -gline-tables-only
endif

ifeq ($(COMPILER),ibm)
  CC = xlc_r
  CFLAGS = -Wall -qsmp=omp -qoffload
endif

ifeq ($(COMPILER),llvm)
  CC = clang++
  CFLAGS = -Wall -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -Xopenmp-target -march=sm_70 -g
endif

ifeq ($(COMPILER),amd)
  CC = clang++
  CFLAGS = -Wall -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=gfx906
endif

ifeq ($(COMPILER),nvidia)
  CC = nvc++
  CFLAGS = -Wall -fopenmp -mp=gpu -Minfo=mp -gpu=cc70
endif

program = test

source = main.cpp global.cpp

obj = $(source:.cpp=.o)

deps = Makefile global.h shared_array.h

$(program): $(obj) $(deps)
	$(CC) $(CFLAGS) $(obj) -o $@ $(LDFLAGS)

%.o: %.cpp $(deps)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(program) $(obj)

edit:
	vim -p $(source) $(deps)
