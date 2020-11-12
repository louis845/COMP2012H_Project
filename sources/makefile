CC = g++ -std=c++11

HEADER_ROOT_DIRECTORY = -iquote ${CURDIR} -I ../libraries/gmp-6.2.0
OUTPUT_DIRECTORY = ${CURDIR}/../compile/
LIBRARIES = -L ${CURDIR}/../libraries/

SOURCES = main.cpp Ring.cpp R.cpp Field.cpp Double.cpp Long.cpp 

#use -g option whenever debug
DEBUG = -g


link: main.o console.o Ring.o R.o Field.o Double.o DoubleComplex.o Long.o Fraction.o LongComplex.o Polynomial.o Linear.o Linear2.o Steps.o History.o tools.o
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)run $(LIBRARIES) $(OUTPUT_DIRECTORY)/*.o -lgmpxx -lgmp

main.o: main.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)main.o $(HEADER_ROOT_DIRECTORY) -c main.cpp

console.o: console.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)console.o $(HEADER_ROOT_DIRECTORY) -c console.cpp

Ring.o: math/Ring.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Ring.o $(HEADER_ROOT_DIRECTORY) -c math/Ring.cpp

R.o: math/R.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)R.o $(HEADER_ROOT_DIRECTORY) -c math/R.cpp

Field.o: math/Field.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Field.o $(HEADER_ROOT_DIRECTORY) -c math/Field.cpp

Double.o: math/double/Double.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Double.o $(HEADER_ROOT_DIRECTORY) -c math/double/Double.cpp

Long.o: math/long/Long.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Long.o $(HEADER_ROOT_DIRECTORY) -c math/long/Long.cpp

DoubleComplex.o: math/double/DoubleComplex.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)DoubleComplex.o $(HEADER_ROOT_DIRECTORY) -c math/double/DoubleComplex.cpp

LongComplex.o: math/long/LongComplex.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)LongComplex.o $(HEADER_ROOT_DIRECTORY) -c math/long/LongComplex.cpp

Fraction.o: math/fraction/Fraction.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Fraction.o $(HEADER_ROOT_DIRECTORY) -c math/fraction/Fraction.cpp

Polynomial.o: math/polynomial/Polynomial.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Polynomial.o $(HEADER_ROOT_DIRECTORY) -c math/polynomial/Polynomial.cpp

Linear.o: math/linear/LinearOperations.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Linear.o $(HEADER_ROOT_DIRECTORY) -c math/linear/LinearOperations.cpp

Linear2.o: math/linear/LinOpsSteps.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Linear2.o $(HEADER_ROOT_DIRECTORY) -c math/linear/LinOpsSteps.cpp

Steps.o: steps/Step.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Steps.o $(HEADER_ROOT_DIRECTORY) -c steps/Step.cpp

History.o: steps/StepsHistory.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)History.o $(HEADER_ROOT_DIRECTORY) -c steps/StepsHistory.cpp

tools.o: math/tools.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)tools.o $(HEADER_ROOT_DIRECTORY) -c math/tools.cpp