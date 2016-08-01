# LOCP (Low Overhead CSV Parser)

## Introduction

This is a CSV parser in development, following the principle that the fastest code is the code that is not executed. In its present state is neither very fast nor very featured, only supporting unquoted and unpadded CSV fields and using code that is not highly optimized to split the fields.

## Benchmarks

The current benchmark results, comparing against [Ben Strasser's Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser) are:

    $ make benchmark
    benchmarks/locp_benchmark
      min: 146.58 ms
      avg: 149.15 ms
      max: 150.65 ms
    benchmarks/fccp_benchmark
      min: 662.00 ms
      avg: 675.33 ms
      max: 686.79 ms
    benchmarks/locp_best_benchmark
      min: 128.81 ms
      avg: 134.67 ms
      max: 137.74 ms
    benchmarks/locp_base_benchmark
      min: 161.36 ms
      avg: 171.61 ms
      max: 187.40 ms

The system used for the tests is a 2015 MacBook Air with a dual-core i5:
    
    $ sysctl -n machdep.cpu.brand_string 
    Intel(R) Core(TM) i5-5250U CPU @ 1.60GHz

The compiler used is the version of Clang included in Mac OS X 10.11.6:

    $ clang++ --version
    Apple LLVM version 7.3.0 (clang-703.0.31)
    Target: x86_64-apple-darwin15.6.0
    Thread model: posix
    InstalledDir: /Library/Developer/CommandLineTools/usr/bin
