all:
	#clang++ -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa -Xopenmp-target=amdgcn-amd-amdhsa -march=gfx906 main.cpp -o test
	icx -fiopenmp -fopenmp-targets=spir64 -D__STRICT_ANSI__ main.cpp -o test
