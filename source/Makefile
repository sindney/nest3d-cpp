# libpng libjpeg zlib: -ljpeg -g3 -lpng -lz 
# -g3 flags to -O2 to optimize libjpeg)
# -mwindows -static-libstdc++ -static-libgcc

INCLUDE = -I"path_to_glew_include" -I"path_to_glfw_2.7.8_include" -I"path_to_n3dcpp_include"
LIB_PATH = -L"path_to_glew_lib" -L"path_to_glfw_2.7.8_lib"
LIBS = -lGLEW32 -lGLFW -lOpenGL32 -m32
BIN_PATH = bin

demo: main.o
	g++ -o $(BIN_PATH)/demo *.o $(LIB_PATH) $(LIBS)
main.o: main.cpp
	g++ -c *.cpp $(INCLUDE)
clean:
	rm *.o $(BIN_PATH)/demo.exe