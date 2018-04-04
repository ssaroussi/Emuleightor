# Emuleightor

 An awesome emulator for the classic chip-8, written in pure C++ and SDL2. 
 
## Getting Started

These instructions will get you a copy of the project up and running on your local machine for development and testing purposes.

### Prerequisites

In order to run this sowftware, you need 2 additional programs:

1. CMake - 
[Install CMake] (https://cmake.org/install)
2. SDL2  - 
[Install SDL2] (https://wiki.libsdl.org/Installation)

*Pro-Tip:* if you have a linux machine it's better to use the package manager.

###### Example (Arch Linux):
![alt text](Gifs/prerequisites.gif)

### Installing

```
$ git clone https://github.com/KingGamma/Emuleightor.git
$ cd Emuleightor
$ mkdir build ; cd build
$ cmake ..
$ make
```

###### Example:
![alt text](Gifs/installing.gif)

There you have an executable file called Emuleightor.
Run it and start playing!

### Usage
```
./Emuleightor <Path to rom>
```

## License

This project is licensed under the GNU General Public License V3 License - see the [LICENSE.md](LICENSE.md) file for details

## Acknowledgments

#### Additional Resources:
* http://www.multigesture.net/articles/how-to-write-an-emulator-chip-8-interpreter/
* http://www.geocities.co.jp/Playtown-Yoyo/6130/chip8.htm
* http://devernay.free.fr/hacks/chip8/C8TECH10.HTM

[Roms' source] (https://www.zophar.net/pdroms/chip8.html)

