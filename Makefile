all: clean main

all_objects:
	g++ src/xml_tag.cpp -c -o xml_tag.o
	g++ src/xml_content.cpp -c -o xml_content.o
	g++ src/xml_tree.cpp -c -o xml_tree.o
	g++ src/xml_document.cpp -c -o xml_document.o

main: all_objects
	g++ src/main.cpp -o main *.o
	rm -f *.o

clean:
	rm -f *.o main
