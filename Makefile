# $@ target
# $^ all prerequisites
# $< first prerequisite

.PHONY: all run clean

GLFWDIR := glfw-3.2.1.bin.WIN64
LIBS := -lglfw3dll -lglfw3 -lopengl32
LIBDIRS := -L${GLFWDIR}/lib-vc2015 
INCDIRS := -I. -I./include -I${GLFWDIR}/include
CC := clang++
# -v = verbose, allows you to see invocations of subprocesses, e.g. link.exe
#   default to not included
# FLAGS := -v -Xclang -flto-visibility-public-std
FLAGS := -Xclang -flto-visibility-public-std
CURRENT_RUN := lighting-map.exe

# This line optionally includes a file with local build rules
# (include with out - fails, with - it allows the makefile to continue)
-include Makefile.local

TARGETS := gltest.exe glbasic.exe gldraw.exe \
	gss-ex1.exe \
	gss-ex2.exe \
	gss-test1.exe \
	gss-exercises.exe \
	gs-textures.exe \
	gs-tex-exercises.exe \
	gs-matrices.exe \
	gs-coords.exe \
	gs-camera.exe \
	lighting-colors.exe \
	lighting-basic.exe \
	lighting-material.exe \
	lighting-map.exe

INTERMEDIATES := stb_image.o glad.o

all: ${TARGETS}

clean:
	-rm ${TARGETS}
	-rm ${INTERMEDIATES}

run: all
	${CURRENT_RUN}

lighting-map.exe: lighting-map.cpp stb_image.o glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

lighting-map.cpp: projection-model-view.vs lighting-map.vs lighting-map.fs

lighting-material.exe: lighting-material.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

lighting-material.cpp: projection-model-view.vs lighting-material.vs lighting-material.fs

lighting-basic.exe: lighting-basic.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

lighting-basic.cpp: projection-model-view.vs lighting-basic.vs lighting-basic.fs

lighting-colors.exe: lighting-colors.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

lighting-colors.cpp: projection-model-view.vs lighting-colors.fs lighting-colors-lamp.fs

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

gltest.exe: gltest.cpp
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glbasic.exe: glbasic.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gldraw.exe: gldraw.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex1.exe: gss-ex1-getting-started-shaders.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

gss-ex2.exe: gss-ex2-getting-started-shaders.cpp glad.o
	${CC} ${FLAGS} -o $@ ${LIBS} ${LIBDIRS} ${INCDIRS} $^

glad.o: glad.cpp
	${CC} ${FLAGS} -c -o $@ ${INCDIRS} $^

test.exe: test.cpp
	${CC} ${FLAGS} -o $@ $^

