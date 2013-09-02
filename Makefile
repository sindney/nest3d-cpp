# for msys + mingw

INCLUDE = -I"GLEW\include" -I"include"
LIB_PATH = -L"GLEW\lib"
LIBS = -lGLEW32 -lOpenGL32 -m32 -shared
OUTPUT = lib

all: 
	g++ -c source/*.cpp $(INCLUDE)
	ar rvs $(OUTPUT)/nest3d.a *.o
	g++ -o $(OUTPUT)/nest3d.dll *.o $(LIB_PATH) $(LIBS)
	rm ./*.o