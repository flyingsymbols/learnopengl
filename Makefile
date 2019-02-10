# $@ target
# $^ all prerequisites
# $< first prerequisite

.PHONY: all run

GLFWDIR := glfw-3.2.1.bin.WIN64
LIBS := -lglfw3dll -lglfw3 -lopengl32
LIBDIRS := -L${GLFWDIR}/lib-vc2015 
INCDIRS := -I. -I./include -I${GLFWDIR}/include
CC := clang -v
# FLAGS := -X${CC} -flto-visibility-public-std
CURRENT_RUN := gs-camera.exe
CURRENT_RUN := getting-started-shaders.exe
include Makefile.local

all: gltest.exe glbasic.exe gldraw.exe \
	gss-ex1.exe \
	gss-ex2.exe \
	gss-test1.exe \
	gss-exercises.exe \
	gs-textures.exe \
	gs-tex-exercises.exe \
	gs-matrices.exe \
	gs-coords.exe \
	gs-camera.exe

run: all
	${CURRENT_RUN}

gs-camera.exe: gs-camera.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-camera.cpp: gs-camera.vs gs-camera.fs

gs-coords.exe: gs-coords.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-coords.cpp: gs-coords.vs gs-coords.fs

gs-matrices.exe: gs-matrices.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-matrices.cpp: gs-matrices.vs gs-matrices.fs

gs-tex-exercises.exe: gs-tex-exercises.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-tex-exercises.cpp: gs-tex-exercises.vs gs-tex-exercises.fs

gs-textures.exe: gs-textures.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gs-textures.cpp: gs-textures.vs gs-textures.fs

stb_image.o: stb_image.cpp
	${CC} ${FLAGS} -c -o $@ ${INCDIRS} $^

gss-exercises.exe: gss-exercises.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-test1.exe: gss-test1.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gltest.exe: gltest.c
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glbasic.exe: glbasic.c glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gldraw.exe: gldraw.c glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex1.exe: gss-ex1-getting-started-shaders.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex2.exe: gss-ex2-getting-started-shaders.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glad.o: glad.c
	${CC} ${FLAGS} -c -o $@ ${INCDIRS} $^

test.exe: test.c
	${CC} ${FLAGS} -o $@ $^

