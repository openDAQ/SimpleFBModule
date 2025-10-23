# Example function block module

Simple example that builds an openDAQ module giving access to an example function block. Said function block scales an input signal with a provided scale, and offsets it by a provided offset.

## Testing the module

To test the module, enable the `OPENDAQ_FB_EXAMPLE_ENABLE_APP` cmake flag. Doing so will add an the openDAQ reference device and function block modules to your project. Those are used to create a simulator device via the "daqref://device0" connection string, as well as a renderer via the "RefFBModuleRenderer" function block ID. The main application connects a reference device signal into both the example scaler and renderer. Additionally, it connects the scaler output into the renderer.

To add additional tests, enable the `EXAMPLE_MODULE_ENABLE_TESTS` cmake flag. Doing so will create a new test target configured for use with the GTest framework.

---

## Prerequisites

- **CMake** (>= 3.25)  
- **Git**  
- **C++ compiler** (Visual Studio on Windows, GCC/Clang on Linux/macOS)  
- Optional: **openDAQ framework** installed locally or available via versioned checkout  

---

## Building the Project

There are two main ways to provide the openDAQ framework:

1. **Using a local openDAQ installation**  
  - Install the openDAQ package on your machine.  
  - Set the environment variable `OPENDAQ_ROOT` to the path containing the binaries.
  - Then CMake will automatically detect the binaries during configuration.

```bash
export OPENDAQ_ROOT=/path/to/opendaq

cmake -S . -B build/output \
  -G "Ninja" \
  -DEXAMPLE_MODULE_ENABLE_TESTS=ON \
  -DSIMPLE_FB_MODULE_OPENDAQ_SDK_VER=3.20.4
```

2. **Using a specific openDAQ version via CMake**  
  - Pass the desired version using the `SIMPLE_FB_MODULE_OPENDAQ_SDK_VER`.
  - CMake will perform a checkout of the openDAQ repository at the specified tag and build the minimal set of binaries needed to build the module and run tests.

```bash
cmake -S . -B build/output \
  -G "Ninja" \
  -DEXAMPLE_MODULE_ENABLE_TESTS=ON \
  -DSIMPLE_FB_MODULE_OPENDAQ_SDK_VER=3.20.4
```

---

### Example: Build Module

```bash
cmake --build build/output --config Release
```

#### Note: 
- The flag `EXAMPLE_MODULE_ENABLE_TESTS=ON` is required if you want to build the module tests for subsequent execution.
- Building the module without `EXAMPLE_MODULE_ENABLE_TESTS=ON` will skip test compilation.
- Providing either `OPENDAQ_ROOT` or `SIMPLE_FB_MODULE_OPENDAQ_SDK_VER` is mandatory for the module to find the required binaries.

### Running Tests
Once the build is complete:
```bash
ctest --test-dir build/output --output-on-failure -C Release -V
```