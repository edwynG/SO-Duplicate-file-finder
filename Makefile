
all: prepare bin/program

prepare:
	@echo "Preparando el entorno..."
	mkdir -p bin obj 

bin/program:
	@echo "Build proyect"

execute:
	mkdir -p output 
	gcc src/main.c -o output/program 
	./output/program 
	rm -rf output || true
clean:
	rm -rf bin obj 

