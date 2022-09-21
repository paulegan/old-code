
PVMIncDir = ${PVM_ROOT}/include
PVMLib = -L${PVM_ROOT}/lib/${PVM_ARCH} -lpvm3

IncludeDirs =	-I. -I../../../PFM -I${PVMIncDir}

all: example1
	

example1:
	g++ ${IncludeDirs} -c triple.cc
	g++ ${IncludeDirs} -c inst.cc
	g++ ${IncludeDirs} add.cc ../../../PFM/processorElement.o ${PVMLib} -o add
	g++ ${IncludeDirs} subtract.cc ../../../PFM/processorElement.o ${PVMLib} -o subtract
	g++ ${IncludeDirs} multiply.cc ../../../PFM/processorElement.o ${PVMLib} -o multiply
	g++ ${IncludeDirs} divide.cc ../../../PFM/processorElement.o ${PVMLib} -o divide
	g++ ${IncludeDirs} example1.cc inst.o ../../../PFM/processor.o ${PVMLib} -o TExample1
	mv add ${PVM_ROOT}/bin/${PVM_ARCH}
	mv subtract ${PVM_ROOT}/bin/${PVM_ARCH}
	mv multiply ${PVM_ROOT}/bin/${PVM_ARCH}
	mv divide ${PVM_ROOT}/bin/${PVM_ARCH}
	mv TExample1 ${PVM_ROOT}/bin/${PVM_ARCH}
