
PVMIncDir = ${PVM_ROOT}/include
PVMLib = -L${PVM_ROOT}/lib/${PVM_ARCH} -lpvm3

IncludeDirs =	-I. -I../PFM -I${PVMIncDir}

all:	simpleSort

simpleSortCode:
	g++ ${IncludeDirs} -c simpleSortCode.cc

simpleSort: simpleSortCode simpleSortPE
	g++ ${IncludeDirs} simpleSort.cc ../PFM/processor.o simpleSortCode.o ${PVMLib} -o simpleSort
	mv simpleSort ${PVM_ROOT}/bin/${PVM_ARCH}

simpleSortPE:
	g++ ${IncludeDirs} simpleSortPE.cc ../PFM/processorElement.o simpleSortCode.o ${PVMLib} -o simpleSortPE
	mv simpleSortPE ${PVM_ROOT}/bin/${PVM_ARCH}
