[![Travis Build Status](https://travis-ci.org/rwols/yaml-archive.svg?branch=master)](https://travis-ci.org/rwols/yaml-archive "Travis Build Status")
[![Appveyor Build Status](https://ci.appveyor.com/api/projects/status/github/rwols/yaml-archive?branch=master&svg=true)](https://ci.appveyor.com/project/rwols/yaml-archive "Appveyor Build Status")

# yaml-archive
A drop-in replacement for `boost::archive::xml_oarchive` and 
`boost::archive::xml_iarchive`.

# Installation
This project uses CMake. As such, there are a couple of options of integrating
yaml-archive into your project.

## Method 1: Install with CMake
Download the source to your local machine.

For Linux and OSX:
```bash
$ cd /path/to/yaml-archive
$ mkdir build
$ cd build
$ cmake .. -DBUILD_SHARED_LIBS=OFF -DCMAKE_BUILD_TYPE=Release
$ cmake --build . --target install
```

For Windows:
```bash
$ cd /path/to/yaml-archive
$ mkdir build
$ cd build
$ cmake .. -DBUILD_SHARED_LIBS=OFF -DBOOST_ROOT=/path/to/boost
$ cmake --build . --target ALL_BUILD --config Release
```

## Method 2: As a Git Submodule
Add yaml-archive as a git-submodule to your git project. Then, in your
CMakeLists.txt file, add the line

```cmake
add_subdirectory(relative/path/to/yaml-archive)
```

To link with yaml-archive and bring in the correct include paths, add the line:

```cmake
target_link_libraries(your-target PUBLIC yaml-archive)
```

This will make your-target link with yaml-archive. yaml-archive links with
Boost.Serialization in turn, so you don't have to link with Boost.Serialization
anymore.

# Usage
Search for `xml_oarchive` in your project. Replace with `yaml_oarchive`.
Same for `xml_iarchive`, `xml_woarchive` and `xml_wiarchive`. That's it.

# Example
[Here is what an XML archive looks like](http://www.boost.org/doc/libs/1_63_0/libs/serialization/example/demo_save.xml). And here is the same information from the "bus stops"
tutorial but realized as a YAML archive:

```yaml
%YAML 1.2
%TAG ! boost/archive/v14/
---
s: !!c0v2
  schedule: !!c1v0
    count: 6
    item_version: 0
    item: !!c2v0
      first: !!c3v3
        driver: "bob"
        hour: 6
        minute: 24
      second: !!c4tv0 &0
        stops: !!c7v0
          count: 3
          item_version: 0
          item: !!c5tv0 &1
            bus_stop: !!c8v0
              latitude: !!c9v0
                degrees: 34
                minutes: 135
                seconds: 5.256000137e+01
              longitude: 
                degrees: 134
                minutes: 22
                seconds: 7.830000305e+01
            street1: "24th Street"
            street2: "10th Avenue"
          item: !!r5 &2
            bus_stop: 
              latitude: 
                degrees: 35
                minutes: 137
                seconds: 2.345599937e+01
              longitude: 
                degrees: 133
                minutes: 35
                seconds: 5.411999893e+01
            street1: "State street"
            street2: "Cathedral Vista Lane"
          item: !!c6tv0 &3
            bus_stop: 
              latitude: 
                degrees: 35
                minutes: 136
                seconds: 1.545600033e+01
              longitude: 
                degrees: 133
                minutes: 32
                seconds: 1.530000019e+01
            name: "White House"
    item: 
      first: 
        driver: "bob"
        hour: 9
        minute: 57
      second: !!r4 *0
    item: 
      first: 
        driver: "alice"
        hour: 11
        minute: 2
      second: !!r4 *0
    item: 
      first: 
        driver: "ted"
        hour: 7
        minute: 17
      second: !!r4 &4
        stops: 
          count: 3
          item_version: 0
          item: !!r6 &5
            bus_stop: 
              latitude: 
                degrees: 35
                minutes: 134
                seconds: 4.878900146e+01
              longitude: 
                degrees: 133
                minutes: 32
                seconds: 1.622999954e+01
            name: "Lincoln Memorial"
          item: !!r6 *3
          item: !!r5 *2
    item: 
      first: 
        driver: "ted"
        hour: 9
        minute: 38
      second: !!r4 *4
    item: 
      first: 
        driver: "alice"
        hour: 11
        minute: 47
      second: !!r4 *4
...
```



