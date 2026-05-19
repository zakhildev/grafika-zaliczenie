UNAME := $(shell uname)
COMMON_LIBS = -lglfw -lGLEW -lassimp
COMMON_INCLUDES = -I./dependencies/glm/
CXXFLAGS = -std=c++23 -g

# Linux
ifeq ($(UNAME), Linux)
	CXX = g++
	INCLUDES = $(COMMON_INCLUDES)
	LIBS = $(COMMON_LIBS) -lGL
endif

# MacOS
ifeq ($(UNAME), Darwin)
	CXX = clang++
 	INCLUDES = $(COMMON_INCLUDES) -L/opt/homebrew/lib/
	LIBS = $(COMMON_LIBS) -framework OpenGL 
endif

HEADERS = $(wildcard ./src/include/*.h) $(wildcard ./src/include/*.hpp)
FILES = ./src/main.cpp $(wildcard ./src/lib/*.cpp)

AlcoholGallery: $(FILES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o AlcoholGallery -Wall $(FILES) $(LIBS)
