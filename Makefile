# $@ target
# $^ all prerequisites
# $< first prerequisite

.PHONY: all run

GLFWDIR := glfw-3.2.1.bin.WIN64
LIBS := -lglfw3dll -lglfw3 -lopengl32
LIBDIRS := -L${GLFWDIR}/lib-vc2015 
INCDIRS := -I. -I./include -I${GLFWDIR}/include
CC := clang

all: gltest.exe glbasic.exe gldraw.exe

run: all
	gldraw.exe

gltest.exe: gltest.c
	clang -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glbasic.exe: glbasic.c glad.o
	clang -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gldraw.exe: gldraw.c glad.o
	clang -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glad.o: glad.c
	clang -c -o $@ ${INCDIRS} $^

test.exe: test.c
	clang -o $@ $^

