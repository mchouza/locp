# LOCP (Low Overhead CSV Parser)

## Introduction

This is a CSV parser in development, following the principle that the fastest code is the code that is not executed. In its present state is neither very fast nor very featured, only supporting unquoted and unpadded CSV fields and using naive code to split the fields.

## Benchmarks

Some initial benchmark results (obtained using `make benchmark`):

    $ make benchmark
    benchmarks/locp_benchmark
      min: 313.51 ms
      avg: 314.31 ms
      max: 314.94 ms
    benchmarks/fccp_benchmark
      min: 489.99 ms
      avg: 493.06 ms
      max: 497.90 ms

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

