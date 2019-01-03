# $@ target
# $^ all prerequisites
# $< first prerequisite

.PHONY: all run

GLFWDIR := glfw-3.2.1.bin.WIN64
LIBS := -lglfw3dll -lglfw3 -lopengl32
LIBDIRS := -L${GLFWDIR}/lib-vc2015 
INCDIRS := -I. -I./include -I${GLFWDIR}/include
CC := clang
FLAGS := -Xclang -flto-visibility-public-std
CURRENT_RUN := getting-started-shaders.exe

all: gltest.exe glbasic.exe gldraw.exe \
	gss-ex1.exe \
	gss-ex2.exe \
	gss-test1.exe \
	gss-exercises.exe \
	gs-textures.exe \
	gs-tex-exercises.exe

run: all
	gs-tex-exercises.exe

gs-tex-exercises.exe: gs-tex-exercises.cpp stb_image.o glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-tex-exercises.cpp: gs-tex-exercises.vs gs-tex-exercises.fs

gs-textures.exe: gs-textures.cpp stb_image.o glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-textures.cpp: gs-textures.vs gs-textures.fs

stb_image.o: stb_image.cpp
	clang ${FLAGS} -c -o $@ ${INCDIRS} $^

gss-exercises.exe: gss-exercises.cpp glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-test1.exe: gss-test1.cpp glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gltest.exe: gltest.c
	clang ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glbasic.exe: glbasic.c glad.o
	clang ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gldraw.exe: gldraw.c glad.o
	clang ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex1.exe: gss-ex1-getting-started-shaders.cpp glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex2.exe: gss-ex2-getting-started-shaders.cpp glad.o
	clang++ ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glad.o: glad.c
	clang ${FLAGS} -c -o $@ ${INCDIRS} $^

test.exe: test.c
	clang ${FLAGS} -o $@ $^

