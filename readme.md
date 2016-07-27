# LOCP (Low Overhead CSV Parser)

## Introduction

This is a CSV parser in development, following the principle that the fastest code is the code that is not executed. In its present state is neither very fast nor very featured, only supporting unquoted and unpadded CSV fields and using code that is not highly optimized to split the fields.

## Benchmarks

The current benchmark results, comparing against [Ben Strasser's Fast C++ CSV Parser](https://github.com/ben-strasser/fast-cpp-csv-parser) are:

    $ make benchmark
    benchmarks/locp_benchmark
      min: 166.05 ms
      avg: 172.22 ms
      max: 179.23 ms
    benchmarks/fccp_benchmark
      min: 584.32 ms
      avg: 589.50 ms
      max: 597.63 ms
    benchmarks/locp_best_benchmark
      min: 196.35 ms
      avg: 198.62 ms
      max: 205.09 ms
    benchmarks/locp_base_benchmark
      min: 299.48 ms
      avg: 306.10 ms
      max: 312.83 ms

The system used for the tests is a 2015 MacBook Air with a dual-core i5:
    
    $ sysctl -n machdep.cpu.brand_string 
    Intel(R) Core(TM) i5-5250U CPU @ 1.60GHz

The compiler used is the version of Clang included in Mac OS X 10.11.6:

    $ clang++ --version
    Apple LLVM version 7.3.0 (clang-703.0.31)
    Target: x86_64-apple-darwin15.6.0
    Thread model: posix
    InstalledDir: /Library/Developer/CommandLineTools/usr/bin
