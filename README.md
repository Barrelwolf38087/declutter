# declutter
A tool to organize music files based on metadata.

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

For a full list of options, run `declutter --help`.
