cmake_minimum_required(VERSION 3.29)
project(micromouse)
set(CMAKE_CXX_STANDARD 20)

add_executable(mazemouse
        src/simulator.cpp
        src/common/Size.hpp
        src/common/Direction.hpp
        src/common/Coordinate.hpp
        src/maze/Maze.hpp
        src/maze/Cell.hpp
        src/maze/Edge.hpp
        src/sim/SimMaze.cpp
        src/sim/SimMaze.hpp
        src/sim/SimGame.cpp
        src/sim/SimGame.hpp
        src/sim/SimMouse.cpp
        src/sim/SimMouse.hpp
)
target_link_libraries(mazemouse sfml-graphics sfml-window sfml-system)
