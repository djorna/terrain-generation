cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
project( terrain_generation)

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(DiamondSquare STATIC
  ${PROJECT_SOURCE_DIR}/src/DiamondSquare.cpp
)

target_link_libraries(DiamondSquare ${OpenCV_LIBS})

add_executable(
  main ${PROJECT_SOURCE_DIR}/src/main.cpp
)

target_link_libraries(main
  ${OpenCV_LIBS}
  DiamondSquare
)