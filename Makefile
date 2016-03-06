# Copyright (c) 2016 Mariano M. Chouza

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to
# deal in the Software without restriction, including without limitation the
# rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
# sell copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:

# The above copyright notice and this permission notice shall be included in
# all copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
# FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
# IN THE SOFTWARE.

CXXFLAGS := -std=c++11 -O3 -Wall -pedantic-errors
LDFLAGS := -lpthread

benchmarks/orders.gen.csv:
	@python -B benchmarks/generate_orders_csv.py $@

benchmarks/locp_benchmark: benchmarks/locp_benchmark.cc locp.h
	@$(CXX) $(CXXFLAGS) -I. benchmarks/locp_benchmark.cc $(LDFLAGS) -o $@

benchmarks/fccp_benchmark: benchmarks/fccp_benchmark.cc benchmarks/third-party/fast-cpp-csv-parser/csv.h
	git submodule update --init benchmarks/third-party/fast-cpp-csv-parser/
	@$(CXX) $(CXXFLAGS) benchmarks/fccp_benchmark.cc $(LDFLAGS) -o $@

benchmark: benchmarks/orders.gen.csv benchmarks/locp_benchmark benchmarks/fccp_benchmark
	@python -B benchmarks/do_benchmarks.py benchmarks/orders.gen.csv benchmarks/*_benchmark

clean:
	@rm -f benchmarks/orders.gen.csv benchmarks/*_benchmark

.PHONY: benchmark clean