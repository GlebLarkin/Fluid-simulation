cmake_minimum_required (VERSION 3.6)

#set(CMAKE_CXX_STANDART 11)

set(CMAKE_CXX_FLAGS "-std=c++17 -O2 -g")

#-fsanitize=address,undefined

project (GunsFolder)

#find_library(PTHREAD_LIBRARY pthread)
find_package(SFML COMPONENTS graphics window system audio REQUIRED)

add_library(Lib SHARED 
    src/Func.cpp
    src/Particle.cpp
    src/Pressure_map.cpp
    src/Pressure_map_cell.cpp
)

add_executable(MyProj src/main.cpp)

target_include_directories(MyProj PUBLIC include/)
target_include_directories(Lib PUBLIC include/)
target_include_directories(Lib PUBLIC /opt/homebrew/include/)
target_include_directories(Lib PUBLIC thirdparty)

#target_link_libraries(src/main.cpp ${PTHREAD_LIBRARY})
target_link_libraries(Lib sfml-graphics sfml-window sfml-system)
target_link_libraries(MyProj sfml-graphics sfml-window sfml-system sfml-audio Lib)
