cmake_minimum_required(VERSION 3.29)
project(micromouse)
set(CMAKE_CXX_STANDARD 20)

add_executable(mazemouse_simulator
        src/maze.hpp
        src/game.cpp
        src/maze/Cell.hpp
        src/maze/Edge.hpp
        src/maze/Maze.hpp
        src/maze.hpp
        src/Maze/Direction.hpp
        src/Simulator/SimGame.cpp
        src/Simulator/SimGame.hpp
        src/Simulator/SimMaze.cpp
        src/Simulator/SimMaze.hpp
        src/Simulator/SimMouse.cpp
        src/Simulator/SimMouse.hpp
        src/Simulator/Simulator.hpp
        src/Simulator.hpp
        src/Simulator/SimGameObject.cpp
        src/Simulator/SimGameObject.hpp
        src/Mouse/Mouse.hpp
)
target_link_libraries(mazemouse_simulator sfml-graphics sfml-window sfml-system)