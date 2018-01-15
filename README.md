# Kidnapped Vehicle Project
[![Udacity - Self-Driving Car NanoDegree](https://s3.amazonaws.com/udacity-sdc/github/shield-carnd.svg)](http://www.udacity.com/drive)
[![CircleCI](https://circleci.com/gh/sgalkin/CarND-T2P3.svg?style=svg&circle-token=ec4a862f1ebe95c2e269fff5c9ad26c579558837)](https://circleci.com/gh/sgalkin/CarND-T2P3)

## Overview
This project implements  2 dimensional particle filter in C++ to estimate
the position of a moving object of interest using map of the location,
noisy GPS estimate of its initial location, and noisy sensor and control data.

This is the final project for the Localization course in Udacity's Self-Driving Car Nanodegree.

Implementation requires to match accuracy and performance values outlined in
the specification.

---

## Usage
```sh
Usage:
  particle_filter [options]
Available options:
  -m, --map   map file
  -?, --help  print this help screen
```

In order to use provided map, one may use the following commands
```sh
cd build
./particle_filter -m ../data/map_data.txt
```
or
```sh
./run.sh
```

## Dependencies
### Runtime
* [Term 2 Simulator](https://github.com/udacity/self-driving-car-sim/releases)

### Tools
* `cmake` >= 3.5
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* `make` >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* `gcc/g++` >= 5.4, clang
  * Linux: gcc/g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

### Libraries not included into the project
* [`uWebSocketIO`](https://github.com/uWebSockets/uWebSockets) == v0.13.0
  * Ubuntu/Debian: the repository includes `install-ubuntu.sh` that can be used to set
    up and install `uWebSocketIO`
  * Mac: the repository includes `install-mac.sh` that can be used to set
    up and install `uWebSocketIO`
  * Windows: use either Docker, VMware, or even [Windows 10 Bash on     Ubuntu](https://www.howtogeek.com/249966/how-to-install-and-use-the-linux-bash-shell-on-windows-10/)

### Libraries included into the project
* [`JSON for Modern C++`](https://github.com/nlohmann/json) - JSON parser
* [`Catch2`](https://github.com/catchorg/Catch2) - Unit-testing framework
* [`ProgramOptions.hxx`](https://github.com/Fytch/ProgramOptions.hxx) - Single-header program options parsing library for C++11

## Build
0. Clone this repo.
1. `mkdir build`
2. `cd build`
3. `cmake .. -G "Unix Makefiles"`
4. `make`
5. `make test # -- optional`

or use provided build script

0. Clone this repo.
1. `./build.sh`

## Protocol
The project uses `uWebSocketIO` request-response protocol in communicating with the simulator.

_INPUT_: values provided by the simulator to the c++ program
```json
// sense noisy position data from the simulator
["sense_x"]
["sense_y"]
["sense_theta"]

// get the previous velocity and yaw rate to predict the particle's transitioned state
["previous_velocity"]
["previous_yawrate"]

// receive noisy observation data from the simulator, in a respective list of x/y values
["sense_observations_x"]
["sense_observations_y"]
```

_OUTPUT_: values provided by the c++ program to the simulator
```json
// best particle values used for calculating the error evaluation
["best_particle_x"]
["best_particle_y"]
["best_particle_theta"]

//Optional message data used for debugging particle's sensing and associations
["best_particle_associations"] <= list of associated ID labels
["best_particle_sense_x"] <= list of sensed x positions
["best_particle_sense_y"] <= list of sensed y positions
```

## The Map
`data/map_data.txt` includes the position of landmarks (in meters) on an arbitrary Cartesian coordinate system. Each row has three columns
1. x position
2. y position
3. landmark id
