CC = g++ -std=c++11

HEADER_ROOT_DIRECTORY = -iquote ${CURDIR}
OUTPUT_DIRECTORY = ${CURDIR}/../compile/

SOURCES = main.cpp Ring.cpp R.cpp Field.cpp Double.cpp

#use -g option whenever debug
DEBUG = -g


link: main.o Ring.o R.o Field.o Double.o
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)run $(OUTPUT_DIRECTORY)*.o

main.o: main.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)main.o $(HEADER_ROOT_DIRECTORY) -c main.cpp

Ring.o: math/Ring.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Ring.o $(HEADER_ROOT_DIRECTORY) -c math/Ring.cpp

R.o: math/R.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)R.o $(HEADER_ROOT_DIRECTORY) -c math/R.cpp

Field.o: math/Field.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Field.o $(HEADER_ROOT_DIRECTORY) -c math/Field.cpp

Double.o: math/double/Double.cpp
	$(CC) $(DEBUG) -o $(OUTPUT_DIRECTORY)Double.o $(HEADER_ROOT_DIRECTORY) -c math/double/Double.cpp
