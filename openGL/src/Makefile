#OBJS specifies which files to compile as part of the project
OBJS = shader.o model.o mesh.o texturearray.o camera.o quaternion.o terrain.o quadtree.o

#CC specifies which compiler we're using
CC = g++ -g

INCLUDE_PATHS =
LIBRARY_PATHS =

#COMPILER_FLAGS specifies the additional compilation options we're using
# -w suppresses all warnings
# -Wl,-subsystem,windows gets rid of the console window
COMPILER_FLAGS = 
#-w -Wl,-subsystem,windows
#LINKER_FLAGS specifies the libraries we're linking against
LINKER_FLAGS =  -lsfml-graphics -lsfml-window -lsfml-system -lGLEW -lGLU -lGL 

#OBJ_NAME specifies the name of our exectuable
OBJ_NAME = ../build/GLex

#This is the target that compiles our executable
all: main.cpp model.o shader.o camera.o core.o terrain.o misc.cpp
	$(CC) main.cpp misc.cpp $(OBJS) $(INCLUDE_PATHS) $(LIBRARY_PATHS) $(LINKER_FLAGS) $(COMPILER_FLAGS) -o $(OBJ_NAME)

model.o: model/mesh.cpp model/model.cpp model/texturearray.cpp model/mesh.h model/model.h model/texturearray.h
	$(CC) -c model/model.cpp model/mesh.cpp model/texturearray.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS)
shader.o: shaders/shader.cpp shaders/shader.h
	$(CC) -c shaders/shader.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS)
camera.o: camera/camera.cpp camera/camera.h
	$(CC) -c camera/camera.cpp camera/camera.h $(INCLUDE_PATHS) $(LIBRARY_PATHS)
terrain.o: terrain/terrain.cpp terrain/terrain.h terrain/quadtree.h terrain/quadtree.cpp
	$(CC) -c terrain/terrain.cpp terrain/quadtree.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS)
core.o: core/quaternion.h core/quaternion.cpp
	$(CC) -c core/quaternion.cpp $(INCLUDE_PATHS) $(LIBRARY_PATHS)
clean:
	rm $(OBJS)

