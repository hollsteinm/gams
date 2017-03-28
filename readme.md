# GAMS
### GPU Allocated Memory Streams

## Intro
This is a small library that adapts GPU allocated memory into streams. This helps abstract away the implementation of GPU bound I/O. It implements the following:

- Interfaces for GPU memory readers and writers (in)
- Basic GPU Buffer to inject the readers and writers into (in: opencl, todo: cuda)
- "Factory" for specifying an OpenCL or CUDA memory stream (todo)

## Why?
Mostly, because I can. Also, who knows when you want to read/write formatted locale oriented information to a GPU with a decent level of abstraction so you don't have to think about it.

## Unit Tests
TODO: get them written up and running

## CMake Build
TODO: learn it, make it, build it

## Things to Build/Link
- Some kind of OpenCL SDK ([AMD](http://developer.amd.com/tools-and-sdks/opencl-zone/amd-accelerated-parallel-processing-app-sdk/), [Intel](https://software.intel.com/en-us/intel-opencl?cid=sem43700008897667599&intel_term=intel+opencl+sdk&gclid=CJ3G-4Pp-dICFYNGXgodd9YKSQ&gclsrc=aw.ds), [NVIDIA](https://developer.nvidia.com/cuda-downloads)) (Note: I built it using OpenCL 1.2 with the NVIDIA SDK)
- Link to the libs and dlls
- Create a main() to execute the code!

## Authors
- Martin Hollstein