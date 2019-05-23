cmake_minimum_required(VERSION 2.8)
set(CMAKE_CXX_STANDARD 11) 
project(terrain_generation)

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_UNITY "Build Unity3D plugin" ON)
option (USE_OpenMP "Use OpenMP" ON)

IF(USE_OpenMP)
  FIND_PACKAGE(OpenMP)
  IF(OPENMP_FOUND)
    message("OpenMP Found")
    SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
  ENDIF()
ENDIF()

find_package(OpenCV REQUIRED)

include_directories(${PROJECT_SOURCE_DIR}/include)

if(BUILD_UNITY)
  include_directories(${PROJECT_SOURCE_DIR}/unity)
endif()

set(CORE_LIB_SOURCES
  ${PROJECT_SOURCE_DIR}/src/DiamondSquare.cpp
  ${PROJECT_SOURCE_DIR}/src/Voronoi.cpp
  ${PROJECT_SOURCE_DIR}/src/ThermalErosion.cpp
  ${PROJECT_SOURCE_DIR}/src/HydraulicErosion.cpp
  ${PROJECT_SOURCE_DIR}/src/FastErosion.cpp
  ${PROJECT_SOURCE_DIR}/src/Kernel.cpp
  ${PROJECT_SOURCE_DIR}/src/SimplexNoise.cpp
  ${PROJECT_SOURCE_DIR}/src/Perturb.cpp
  ${PROJECT_SOURCE_DIR}/src/Evaluator.cpp
)

set(TEST_SOURCES
  ${PROJECT_SOURCE_DIR}/src/tests/test_diamondsquare.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_voronoi.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_combine.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_thermal_erosion.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_hydraulic_erosion.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_mask.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_random.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_perturb.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_eval.cpp
  ${PROJECT_SOURCE_DIR}/src/tests/test_fast_erosion.cpp
)

# common.hpp library
# add_library(common INTERFACE)
# target_include_directories(common INTERFACE ${PROJECT_SOURCE_DIR}/include)

add_library(utils INTERFACE)
target_include_directories(utils INTERFACE ${PROJECT_SOURCE_DIR}/include)
add_library(rng INTERFACE)
target_include_directories(rng INTERFACE ${PROJECT_SOURCE_DIR}/include)

# Random library
# add_library(Random STATIC ${PROJECT_SOURCE_DIR}/src/Random.cpp)

foreach(lib_path ${CORE_LIB_SOURCES})
  get_filename_component(lib_file ${lib_path} NAME)
  string( REPLACE ".cpp" "" lib_name ${lib_file} )
  add_library( ${lib_name} STATIC ${lib_path} )
  list(APPEND LIBS ${lib_name})
  target_link_libraries(${lib_name} ${OpenCV_LIBS} utils)
endforeach()

# Unity Plugin
add_library(terrain_plugin SHARED unity/terrain_plugin.cpp)

target_link_libraries(Voronoi rng)
target_link_libraries(DiamondSquare rng)
target_link_libraries(ThermalErosion Kernel)
target_link_libraries(HydraulicErosion Kernel)
target_link_libraries(FastErosion Kernel)
target_link_libraries(Perturb SimplexNoise)
target_link_libraries(Evaluator Kernel)

# Add unit test executables
if(BUILD_TESTS)
  foreach(test_path ${TEST_SOURCES})
    get_filename_component(test_file ${test_path} NAME)
    string( REPLACE ".cpp" "" test_name ${test_file} )
    add_executable( ${test_name} ${test_path} )
    target_link_libraries(${test_name} ${OpenCV_LIBS})
  endforeach()

  target_link_libraries(test_diamondsquare DiamondSquare)
  target_link_libraries(test_voronoi Voronoi)
  target_link_libraries(test_combine Voronoi DiamondSquare)
  target_link_libraries(test_thermal_erosion Voronoi DiamondSquare ThermalErosion Perturb)
  target_link_libraries(test_hydraulic_erosion Voronoi DiamondSquare HydraulicErosion Perturb)
  target_link_libraries(test_mask Voronoi DiamondSquare)
  target_link_libraries(test_random Random)
  target_link_libraries(test_perturb Voronoi DiamondSquare Perturb)
  target_link_libraries(test_eval Voronoi DiamondSquare Perturb Evaluator)
  target_link_libraries(test_fast_erosion Voronoi DiamondSquare FastErosion Perturb)
endif()

if(BUILD_UNITY)
  target_link_libraries(terrain_plugin Voronoi DiamondSquare Perturb ThermalErosion HydraulicErosion FastErosion Evaluator)
endif()