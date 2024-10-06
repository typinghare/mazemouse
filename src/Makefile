COMPILER = c++
OPTS = --std=c++20
#LIBS = -lsfml-graphics -lsfml-window -lsfml-system
DEPS = Maze/Cell.hpp \
	Maze/Direction.hpp \
	Maze/Edge.hpp \
	Maze/Maze.hpp \
	Simulator/SimGame.hpp \
	Simulator/SimGameObject.hpp \
	Simulator/SimMaze.hpp \
	Simulator/SimMouse.hpp \
	Simulator/Simulator.hpp \
	Maze.hpp \
	Simulator.hpp
OBJECTS = Simulator/SimGame.o \
	Simulator/SimGameObject.o \
	Simulator/SimMaze.o \
	Simulator/SimMouse.o \
	game.o

%.o: %.cpp $(DEPS)
	$(COMPILER) $(LIBS) $(OPTS) -c $^

clean:
	rm -f **/*.o **/*.hpp.gch