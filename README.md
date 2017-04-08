# OPAL
[![Build Status](https://travis-ci.org/DennZo1993/OPAL.svg?branch=master)](https://travis-ci.org/DennZo1993/OPAL) [![Build status](https://ci.appveyor.com/api/projects/status/a3vbuu8mwv80ci0p?svg=true)](https://ci.appveyor.com/project/DennZo1993/opal) [![Coverage Status](https://coveralls.io/repos/github/DennZo1993/OPAL/badge.svg?branch=master)](https://coveralls.io/github/DennZo1993/OPAL?branch=master)

### University project

An algorithm for fast segmentation of medical images.

### Original paper

*Vinh-Thong Ta, R´emi Giraud, D. Louis Collins, Pierrick Coup´e. Optimized PatchMatch for
Near Real Time and Accurate Label Fusion. MICCAI 2014, Sep 2014, United States. 8 p.,
2014.*

### Third-party libraries used
- [LodePNG](https://github.com/lvandeve/lodepng)
- [flow_code](http://sintel.is.tue.mpg.de/downloads)
- [json11](https://github.com/dropbox/json11)

- - -

## Prerequisites

- [CMake](https://cmake.org/download/) 3.0 or higher
- **Linux:** [GCC](https://gcc.gnu.org/) 5.0 or [Clang](http://releases.llvm.org/download.html) 3.9  
  **Windows:** [Visual Studio](https://www.visualstudio.com) 2015
- [Python](https://www.python.org/) 3 (optional)

## How to build

#### Clone the repository  
```bash
git clone https://github.com/DennZo1993/OPAL.git
cd OPAL
```
#### Build
- **Using build script (requires Python)**  
  ```bash
  python opal_build.py
  ```
  Arguments to control build:
  ```
  -d, --debug                         Debug build
  -r, --release                       Release build
  -c COMPILER, --compiler COMPILER    C++ compiler to use
  --clear                             Clear previous build data first
  ```
  To see all supported options, run
  ```
  python opal_build.py --help
  ```
  **Example:** "Release" build on Windows using Visual C++ compiler (cl)
  ```
  python opal_build.py -c cl -r
  ```
  **Example:** "Debug" build on Linux with Clang, clearing previous data
  ```
  python opal_build.py -c clang++ -d --clear
  ```
  The script will create `build` directory containing built libraries and binaries.
  
- **Manual build**  
  * Create `build` directory where build data will be stored and navigate there
    ```bash
    mkdir build
    cd build
    ```
  * Run CMake
    ```bash
    cmake ..
    ```
    Use `-DCMAKE_BUILD_TYPE` and `-DCMAKE_CXX_COMPILER` options to set build type and C++ compiler, respectively.
    
    **Example:** CMake for "Release" build (both on Linux and Windows)
    ```
    cmake -DCMAKE_BUILD_TYPE=Release ..
    ```
    **Example:** CMake for "Debug" build with Clang++ (on Linux)
    ```
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_CXX_COMPILER=clang++ ..
    ```
  * Build OPAL
    ```bash
    cmake --build .
    ```
    Use `--config` option to set build type.
    
    **Example:** Build "Release" version (both on Linux and Windows)
    ```
    cmake --build . --config Release
    ```

## Running tests

- **Using build script (requires Python)**  
  ```bash
  python opal_build.py --test
  ```
  Pass `--verbose` option (short: `-v`) to see verbose output.
  
  **Example:** Build and test "Release" version with verbose output
  ```
  python opal_build.py --release --test --verbose
  ```

- **Manual run**  
  Use `ctest` command to run tests. Set the same build type you used for build, using `-C` option. Use `--verbose` option for verbose output.
  
  **Example:** Run tests for "Debug" version
  ```bash
  ctest -C Debug --verbose
  ```
