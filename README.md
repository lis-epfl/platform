# platform
Gazebo client for changing platform heght.

## Usage
The gazebo server must be running with the platform plugin.

```
platformcmd [-x X] [-y Y] [-h H]
```
* `X` is the x position of the platform
* `Y` is the y position of the platform
* `H` is the height of the platform
  * if `H == 0` the platform's height is set to a random positive value;
  * else the platform's height is set as specified.

## Compilation
```
mkdir build
cd build
cmake ..
make
```
