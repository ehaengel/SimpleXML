all: clean main

all_objects:
	g++ src/xml_tag.cpp -c -o xml_tag.o

main: all_objects
	g++ src/main.cpp -o main *.o
	rm -f *.o

clean:
	rm -f *.o main
