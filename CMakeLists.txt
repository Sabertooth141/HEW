cmake_minimum_required(VERSION 4.0)
project(HEW)

set(CMAKE_CXX_STANDARD 20)

add_executable(HEW
        src/lib/conioex.h
        src/lib/conioex.cpp
        src/Object/Entity/Player/PlayerController.cpp
        src/Object/Entity/Player/PlayerController.h
        src/Object/Entity/Player/PlayerController.h
        src/Object/Entity/Entity.cpp
        src/Object/Entity/Entity.h
        src/Object/Entity/Entity.h
        src/core/Game.cpp
        src/core/Game.h
        src/World/Tile.h
        src/World/Camera.cpp
        src/World/Camera.h
        src/World/Camera.h
        src/World/Tilemap.cpp
        src/World/Tilemap.h
        src/Object/Object.cpp
        src/Object/Object.h)
