# LOCP (Low Overhead CSV Parser)

## Introduction

This is a CSV parser in development, following the principle that the fastest code is the code that is not executed. In its present state is neither very fast nor very featured, only supporting unquoted and unpadded CSV fields and using naive code to split the fields.

## Benchmarks

The current benchmark results, comparing against [Ben Strasser's Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser) are:

    $ make benchmark
    benchmarks/locp_benchmark
      min: 146.82 ms
      avg: 147.27 ms
      max: 148.28 ms
    benchmarks/fccp_benchmark
      min: 490.25 ms
      avg: 497.43 ms
      max: 508.17 ms

The system used for the tests is an old quad-core Sandy Bridge:

    $ grep 'processor\|\(model name\)' /proc/cpuinfo 
    processor	: 0
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 1
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 2
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 3
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 4
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 5
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 6
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz
    processor	: 7
    model name	: Intel(R) Core(TM) i7-2600K CPU @ 3.40GHz

The compiler used is the version of GCC included in Ubuntu 14.04:

    $ gcc --version
    gcc (Ubuntu 4.8.4-2ubuntu1~14.04) 4.8.4
    Copyright (C) 2013 Free Software Foundation, Inc.
    This is free software; see the source for copying conditions.  There is NO
    warranty; not even for MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

