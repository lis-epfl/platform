# platform
Gazebo client for changing platform heght.

## Usage
The gazebo server must be running with the platform plugin.

```
platformcmd HEIGHT
```
`HEIGHT` is the desired height for the platform:

* if `HEIGHT >= 0` the platform's height is set as specified;
* if `HEIGHT < 0` the platform's height is set randomly.

## Compilation
```
mkdir build
cd build
cmake ..
make
```
