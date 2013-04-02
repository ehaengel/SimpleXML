all: clean all_objects main

all_objects:
	g++ src/xml_tag.cpp -c -o xml_tag.o
	g++ src/xml_content.cpp -c -o xml_content.o
	g++ src/xml_tree.cpp -c -o xml_tree.o
	g++ src/xml_document.cpp -c -o xml_document.o
	ar -cr libsimplexml.a *.o
	rm -f *.o

main:
	g++ src/main.cpp -o main -L. -lsimplexml

clean:
	rm -f libsimplexml.a *.o main
