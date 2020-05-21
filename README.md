# declutter
A tool to organize music files based on metadata. Probably won't work on Windows without some tinkering or Cygwin.

## Installation
Dependencies:
  - CMake >= 3.17
  - TagLib

### Create the build directory
`$ mkdir build && cd build`

### Configure
`$ cmake -DCMAKE_BUILD_TYPE=Release -G "Unix Makefiles" ../`

Optionally, you can specify the installation prefix with `-DCMAKE_INSTALL_PREFIX`.
The default is `/usr/local`.

### Build
`$ cmake --build . --target declutter`

### Install
`# make install`

## Usage
`$ declutter --source /path/to/my/unorganized/library --dest /put/the/organized/files/here/`

This will organize the files first by artist, then by album. I might make the order configurable if I get bored,
but I figured this would cover a lot of use cases (including my own).

For a full list of options, run `declutter --help`.

## Known Issues
- Segfaults when there are no input files. I haven't bothered to look into this yet.
- Creates a false vacuum and deletes the universe when any input file lacks any of the used tags (probably).
