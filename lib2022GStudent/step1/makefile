
# cphex\makefile

LIBS=
all:	admin	LibSerFJ.o


LibSerFJ.o:	LibSerFJ.c	LibSerFJ.h	data.h	
	echo "compilation de LibSerFJ"
	gcc -c LibSerFJ.c

admin:	data.h	admin.c	LibSerFJ.o			
	echo "Compilation de admin"
	gcc -o admin	admin.c	LibSerFJ.o 

