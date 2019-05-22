# Terrain-Generation
Terrain-Generation is an implementation of the [diamond-square algorithm](https://en.wikipedia.org/wiki/Diamond-square_algorithm) for generating mountainous terrain. The program generates a random elevation map, which can be used as an elevation map in Unity3D, Gazebo, or any other simulation environment.

# Paper
**Realtime Procedural Terrain Generation**. *IEEE Transactions on Robotics,* vol. 31, no. 5, pp. 1147-1163, 2015. (**2015 IEEE Transactions on Robotics Best Paper Award**). **[PDF](http://webdiis.unizar.es/~raulmur/MurMontielTardosTRO15.pdf)**.

## Future Improvements
The cited paper describes more complex operations to add realism to the landscape, such as eroding the landscape to simulate water damage. Different techniques could also be used to add mountains and craters.

## Dependencies
* OpenCV
* CMake
* C++11

Refer to the following sources for instructions on installing OpenCV: [Windows](https://www.learnopencv.com/install-opencv3-on-windows/) and [Ubuntu](https://www.learnopencv.com/install-opencv3-on-ubuntu/).

## Building and Running

    makedir build && cd build
    cmake .. && make
    terrain_generation 9 # Create n x n image where n = 2^5 + 1 = 513


![example-image](examples/513x513.png)

