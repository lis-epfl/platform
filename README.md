# platform
Gazebo client for changing platform heght.

## Installation and Compilation
All dependencies must be installed. Requires CMake 2.8 or later and 
Gazebo 7 or later.
```
git clone --recurse-submodules https://github.com/dronecourse-epfl/platform
cd platform
./build.sh
```
To be able to use the tool everywhere, add `platform` to your path.
For example on Ubuntu16.04 using realpath inside the repository:
```
ln -s $(realpath platform) /home/arthur/.local/bin/platform
```


## Usage
The gazebo server must be running with the platform plugin.

```
Usage: platform [OPTION]...
Interact with the platform plugin of a running gazebo server with the
platform plugin to set the coordinates and the height of the platform.

  -x X                       set the x coordinate to X
  -y Y                       set the y coordinate to Y
  -H h                       set the height to h, if h=0 then height is
                              set at random
  -a                         random position (x, y)
  -d                         used for debuging: dump all and exit
  -h                         display this help and exit
  -v                         output version information and exit

Examples:
Choose coordinates and height
    platform -x 100 -y 0 -h 1
Change position, keep height
    platform -x 100 -y 0
Change to random height, keep position
    platform -h 0
Random position but keep height (order of options is significant)
    platform -a
Random height and position
    platform -a -h 0

Position ranges for -a switch and height range for -h 0 option:
MIN_X=98.5	 MAX_X=101.5
MIN_Y=-1.5	 MAX_Y=1.5
MIN_H=0.8	 MAX_H=1.3
```
