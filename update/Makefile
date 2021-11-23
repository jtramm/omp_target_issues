COMPILER = amd

ifeq ($(COMPILER),intel)
  CC = icpx
  CFLAGS = -Wall -fiopenmp -fopenmp-targets=spir64 -D__STRICT_ANSI__
endif

ifeq ($(COMPILER),ibm)
  CC = xlc_r
  CFLAGS = -Wall -qsmp=omp -qoffload
endif

ifeq ($(COMPILER),llvm)
  CC = clang++
  CFLAGS = -Wall -fopenmp -fopenmp-targets=nvptx64 -Xopenmp-target -march=sm_80 -fopenmp-cuda-mode
endif

ifeq ($(COMPILER),amd)
  CC = clang++
  CFLAGS = -Wall -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=gfx908
endif

ifeq ($(COMPILER),nvidia)
  CC = nvc++
  CFLAGS = -Wall -mp=gpu -Minfo=mp -gpu=cc80
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
