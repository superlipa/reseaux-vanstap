LIBS=
all: admin cli ser udplib.o LibSerFJ.o

LibSerFJ.o: LibSerFJ.c LibSerFJ.h data.h
	echo "compilation de LibSerFJ"
	cc -c LibSerFJ.c

admin: data.h admin.c LibSerFJ.o
	echo "Compilation de admin"
	cc -o admin admin.c LibSerFJ.o

udplib.o: ../udplib/udplib.h ../udplib/udplib.c
	echo "Compilation de udplib.o"
	cc -c ../udplib/udplib.c

cli: cli.c requeteFJ.h data.h udplib.o
	echo "Compilation de client"
	cc -o cli cli.c udplib.o $(LIBS)

ser: ser.c requeteFJ.h data.h udplib.o LibSerFJ.o
	echo "Compilation de serveur"
	cc -o ser ser.c udplib.o LibSerFJ.o $(LIBS)

