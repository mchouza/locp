import os
import subprocess
import sys
import time

NUM_ITERS = 5

def avg(l):
	return sum(l) / float(len(l))

if __name__ == '__main__':
	if len(sys.argv) < 3:
		sys.stderr.write('Syntax: %s <csv-file> <bench1> [<bench2> ... [<benchN]]\n' % sys.argv[0])
		sys.exit(1)
	csv_fpath = sys.argv[1]
	bench_fpaths = sys.argv[2:]
	times = {}
	for i in range(NUM_ITERS):
		for bfp in bench_fpaths:
			start = time.time()
			out = subprocess.check_output([bfp, csv_fpath])
			end = time.time()
			assert out == '30000000\n', 'unexpected output for %s' % bfp
			if bfp not in times:
				times[bfp] = []
			times[bfp].append((end - start) * 1000.0)
	for bfp, times_list in times.iteritems():
		print bfp
		print '  min: %.02f ms' % min(times_list)
		print '  avg: %.02f ms' % avg(times_list)
		print '  max: %.02f ms' % max(times_list)
