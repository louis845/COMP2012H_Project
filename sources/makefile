CC = g++ -std=c++11

HEADER_ROOT_DIRECTORY = -iquote ${CURDIR} -I ../libraries/gmp-6.2.0
OUTPUT_DIRECTORY = ${CURDIR}/../compile/
LIBRARIES = -L ${CURDIR}/../libraries/

INCLUDE_DIRS = -I. -Imath -Imath/double -Imath/fraction -Imath/linear -Imath/long \
	-Imath/polynomial -Iparser -Iutils -I../libraries/Armadillo/include \
	-I../libraries/gmp-6.2.0

CFLAGS = -g -O3 -Wall -o $(OUTPUT_DIRECTORY)$@ $(INCLUDE_DIRS) -c $<

OBJS = $(wildcard $(CURDIR)/../compile/*.o)

SOURCES = main.cpp Ring.cpp R.cpp Field.cpp Double.cpp Long.cpp 

#use -g option whenever debug
DEBUG = -g


link: main.o console.o Ring.o R.o Field.o Double.o DoubleComplex.o Long.o Fraction.o LongComplex.o Polynomial.o Linear.o Linear2.o Linear3.o Steps.o History.o tools.o
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

Linear3.o: math/linear/CoupledOperations.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Linear3.o $(HEADER_ROOT_DIRECTORY) -c math/linear/CoupledOperations.cpp

Steps.o: steps/Step.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Steps.o $(HEADER_ROOT_DIRECTORY) -c steps/Step.cpp

History.o: steps/StepsHistory.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)History.o $(HEADER_ROOT_DIRECTORY) -c steps/StepsHistory.cpp

tools.o: math/tools.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)tools.o $(HEADER_ROOT_DIRECTORY) -c math/tools.cpp

# --------------------------------------------------------------------------------------
# O3 optimization is enabled, compilation could be slow
test_parser: parser/test_parser.cpp expr_ast.o lexer.o parser.o math_wrapper.o
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)$@ $(OBJS) $(LIBRARIES) -lgmpxx -lgmp

expr_ast.o: parser/expr_ast.cpp parser/expr_ast.h
	$(CC) $(CFLAGS)

lexer.o: parser/lexer.cpp parser/lexer.h
	$(CC) $(CFLAGS)

parser.o: parser/parser.cpp parser/parser.h
	$(CC) $(CFLAGS)

math_wrapper.o: utils/math_wrapper.cpp utils/math_wrapper.h
	$(CC) $(CFLAGS)

.PHONY: clean 
clean: 
	-rm $(OBJS)