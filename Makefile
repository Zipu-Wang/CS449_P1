all: cbm2bmp

cbm2bmp: main.c infoHandler.c conversion.c
	gcc -o cbm2bmp main.c infoHandler.c conversion.c

clean:
	rm -f cbm2bmp
