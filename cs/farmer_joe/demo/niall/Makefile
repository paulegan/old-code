
PVMIncDir = ${PVM_ROOT}/include
PVMLib = -L${PVM_ROOT}/lib/${PVM_ARCH} -lpvm3

IncludeDirs =	-I. -I../../PFM -I${PVMIncDir}

all:
	@echo "make [ dynamic | simpleQueens | bounce ]"

dynamic: dynamic.cc
	g++ -g ${IncludeDirs} dynamic.cc ../../PFM/processor.o  ${PVMLib} -o dynamic
	mv dynamic ${PVM_ROOT}/bin/${PVM_ARCH}

simpleQueens:
	g++ ${IncludeDirs} simpleQueens.cc ../../PFM/processor.o ${PVMLib} -o simpleQueens
	mv simpleQueens ${PVM_ROOT}/bin/${PVM_ARCH}

bounce: numberInc.cc bounce.cc
	g++ ${IncludeDirs} numberInc.cc ../../PFM/processorElement.o ${PVMLib} -o numberInc
	g++ ${IncludeDirs} bounce.cc ../../PFM/processor.o ${PVMLib} -o bounce
	mv numberInc ${PVM_ROOT}/bin/${PVM_ARCH}
	mv bounce ${PVM_ROOT}/bin/${PVM_ARCH}
