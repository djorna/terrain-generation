cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11) 
project(terrain_generation)

option(BUILD_TESTS "build unit tests" ON)

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include
  ${PROJECT_SOURCE_DIR}/unity
)

set(CORE_LIB_SOURCES
  ${PROJECT_SOURCE_DIR}/src/DiamondSquare.cpp
  ${PROJECT_SOURCE_DIR}/src/Voronoi.cpp
  ${PROJECT_SOURCE_DIR}/src/ThermalErosion.cpp
  ${PROJECT_SOURCE_DIR}/src/Kernel.cpp
)

set(TEST_SOURCES
  ${PROJECT_SOURCE_DIR}/src/test_diamondsquare.cpp
  ${PROJECT_SOURCE_DIR}/src/test_voronoi.cpp
  ${PROJECT_SOURCE_DIR}/src/test_combine.cpp
  ${PROJECT_SOURCE_DIR}/src/test_thermal_erosion.cpp
)

foreach(lib_path ${CORE_LIB_SOURCES})
  get_filename_component(lib_file ${lib_path} NAME)
  string( REPLACE ".cpp" "" lib_name ${lib_file} )
  add_library( ${lib_name} STATIC ${lib_path} )
  list(APPEND LIBS ${lib_name})
  target_link_libraries(${lib_name} ${OpenCV_LIBS} common)
endforeach()

# common.hpp library
add_library(common INTERFACE)
target_include_directories(common INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Erosion interface
add_library(Erosion INTERFACE)
target_include_directories(Erosion INTERFACE ${PROJECT_SOURCE_DIR}/include)
target_link_libraries(Erosion INTERFACE ${OpenCV_LIBS})

# Unity Plugin
add_library(terrain_plugin SHARED unity/terrain_plugin.cpp)

target_link_libraries(${LIBS} ${OpenCV_LIBS} common)
target_link_libraries(ThermalErosion Kernel)

# Add unit test executables
if(BUILD_TESTS)
  foreach(test_path ${TEST_SOURCES})
    get_filename_component(test_file ${test_path} NAME)
    string( REPLACE ".cpp" "" test_name ${test_file} )
    add_executable( ${test_name} ${test_path} )
    target_link_libraries(${test_name} ${OpenCV_LIBS})
  endforeach()

  target_link_libraries(test_diamondsquare DiamondSquare )
  target_link_libraries(test_voronoi Voronoi)
  target_link_libraries(test_combine Voronoi DiamondSquare)
  target_link_libraries(test_thermal_erosion Voronoi DiamondSquare ThermalErosion)
endif()


target_link_libraries(terrain_plugin Voronoi DiamondSquare)