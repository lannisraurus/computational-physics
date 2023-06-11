EIGEN := $(shell pkg-config eigen3 --cflags)
ROOT := $(shell root-config --cflags)
ROOT_LIBS := $(shell root-config --libs)

SRC := $(wildcard src/*.cpp)

C_FLAGS := -std=c++20
C_FLAGS_ADD := -Wall -Werror -Wextra -O2

SDL_FLAGS := -lSDL2 -lSDL2main -lSDL2image

#hw3: proj3.exe proj2.exe proj4.exe
    

%.exe: main/%.cpp
	@echo "COMPILING WITH SDL2 $< --> $@"
	g++ -g $(C_FLAGS) $< -lSDL2 -lSDL2main -lSDL2image -o $@ -I headers $(SRC)

dump:
	@echo $(EIGEN)
	@echo $(ROOT)
	@echo $(ROOT_LIBS)
	@echo $(SRC)
