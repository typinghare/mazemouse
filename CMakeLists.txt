cmake_minimum_required(VERSION 3.29)
project(micromouse)
set(CMAKE_CXX_STANDARD 20)

add_executable(mazemouse_simulator
        src/Maze/Dir4.hpp
        src/Maze/Maze.hpp
        src/simulator.cpp
        src/Mouse/Mouse.hpp
        src/Mouse/FloodFillMouse.hpp
        src/Mouse/AStarMouse.hpp
        src/Simulator/Game.cpp
        src/Simulator/Game.hpp
        src/Simulator/MazePlugin.cpp
        src/Simulator/MazePlugin.hpp
        src/Simulator.hpp
        src/Mouse/SemiFinishedMouse.hpp
        src/Maze/Vector2.hpp
)
target_link_libraries(mazemouse_simulator sfml-graphics sfml-window sfml-system)