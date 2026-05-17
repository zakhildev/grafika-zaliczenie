UNAME := $(shell uname)

# Linux
ifeq ($(UNAME), Linux)
	CXX = g++
	INCLUDES = -I./dependencies/glm/
	LIBS = -lglfw -lGLEW -lassimp
endif

# MacOS
ifeq ($(UNAME), Darwin)
	CXX = clang++
	CXXFLAGS = -std=c++23 -g
 	INCLUDES = -I./dependencies/glm/ -L/opt/homebrew/lib/ -I/opt/homebrew/opt/freetype/include/freetype2 -I/opt/homebrew/opt/libpng/include/libpng16
	LIBS = -lglfw -lGLEW -lassimp -lfreetype -framework OpenGL 
endif

HEADERS = $(wildcard ./src/include/*.h) $(wildcard ./src/include/*.hpp)
FILES = ./src/main.cpp $(wildcard ./src/lib/*.cpp)

AlcoholGallery: $(FILES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o AlcoholGallery -Wall $(FILES) $(LIBS)
