cmake_minimum_required(VERSION 2.8)
add_compile_options(-std=c++11)
project( terrain_generation)

find_package( OpenCV REQUIRED )

include_directories(${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/unity
)

add_library(DiamondSquare STATIC
  ${PROJECT_SOURCE_DIR}/src/DiamondSquare.cpp
)

add_library(Voronoi STATIC
  ${PROJECT_SOURCE_DIR}/src/Voronoi.cpp
)

add_library(common INTERFACE)
target_include_directories(common INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Unity Plugin
add_library(terrain_plugin SHARED unity/terrain_plugin.cpp)

# set_target_properties(common PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(DiamondSquare ${OpenCV_LIBS} common)
target_link_libraries(Voronoi ${OpenCV_LIBS} common)

add_executable(
  test_diamondsquare ${PROJECT_SOURCE_DIR}/src/test_diamondsquare.cpp
)

add_executable(
  test_voronoi ${PROJECT_SOURCE_DIR}/src/test_voronoi.cpp
)

add_executable(
  test_combine ${PROJECT_SOURCE_DIR}/src/test_combine.cpp
)

target_link_libraries(test_diamondsquare
  ${OpenCV_LIBS}
  DiamondSquare
)

target_link_libraries(test_voronoi
  ${OpenCV_LIBS}
  Voronoi
)

target_link_libraries(test_combine
  ${OpenCV_LIBS}
  Voronoi
  DiamondSquare
)

target_link_libraries(terrain_plugin
  ${OpenCV_LIBS}
  Voronoi
  DiamondSquare
)