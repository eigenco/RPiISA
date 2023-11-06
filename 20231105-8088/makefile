all:
	nasm -O0 -fbin vga86.asm -o vga86.bin
#	cc vga86.c -o vga86 -fsigned-char -lSDL2 -framework OpenGL
#	cc vga86.c -o vga86 -fsigned-char -lm -lSDL2 -lGL
	gcc -O0 vga86.c -o vga86 -fsigned-char -lm -lSDL2
#	gcc -O2 vga86.c -o vga86 -fsigned-char -lm
