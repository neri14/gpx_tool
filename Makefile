
INPUT_DIR=src
INPUT_FILE=${INPUT_DIR}/main.cpp

OUTPUT_DIR=bin
OUTPUT_FILE=${OUTPUT_DIR}/gpx_tool

.PHONY: all
all: ${OUTPUT_DIR}
	g++ ${INPUT_FILE} -o ${OUTPUT_FILE} -lxml2  -I/usr/include/libxml2/ -std=c++14 -g

${OUTPUT_DIR}:
	mkdir -p ${OUTPUT_DIR}

.PHONY: test
test: all
	./test.sh

.PHONY: clean
clean:
	rm -rf ${OUTPUT_DIR}
