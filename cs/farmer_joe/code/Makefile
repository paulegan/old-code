
PVMIncDir = ${PVM_ROOT}/include
PVMLib = -L${PVM_ROOT}/lib/${PVM_ARCH} -lpvm3
IncludeDirs =	-I. -I${PVMIncDir}

all: processor processorElement

processor:
	g++ -gstabs ${IncludeDirs} -c processor.cc

processorElement:
	g++ -gstabs ${IncludeDirs} -c processorElement.cc

clean:
	rm -f *.o
