cmake_minimum_required(VERSION 4.0)
project(HEW)

set(CMAKE_CXX_STANDARD 20)

add_executable(HEW
        src/conioex.h
        src/conioex.cpp
        src/Object/Object.cpp
        src/Object/Object.h
        src/Object/Object.h
        src/Object/Entity/Player/PlayerController.cpp
        src/Object/Entity/Player/PlayerController.h
        src/Object/Entity/Player/PlayerController.h
        src/Object/Entity/Entity.cpp
        src/Object/Entity/Entity.h
        src/Object/Entity/Entity.h
        src/Game.cpp
        src/Game.h
        src/World/Tile.h
        src/World/Tileset.cpp
        src/World/Tileset.h
        src/World/Tileset.h
        src/World/Camera.cpp
        src/World/Camera.h
        src/World/Camera.h
        src/World/Scene.cpp
        src/World/Scene.h)
