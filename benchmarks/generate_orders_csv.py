import random
import sys

RANDOM_SEED = 43434342
NUM_ORDERS = 5000000
BASE_ID = 70000001
PARTS = ('Mal', 'Wash', 'Simon', 'River', 'Kaylee', 'Shepherd', 'Jayne', 'Zoe', 'Inara')
PRICE_MEAN = 75
PRICE_SD = 15
PRICE_LO = 50
PRICE_HI = 100
Q_ROUND_P = 0.8
Q_ROUND_LAMBDA = 1.0 / 500.0
Q_NON_ROUND_LAMBDA = 1.0 / 400.0
TIMESTAMP_START = 100000
TIMESTAMP_LAMBDA = 1.0 / 5.0
DIRECTIONS = ('BUY', 'SELL')

def normal_bounded(mean, sd, lo, hi):
	while True:
		r = random.normalvariate(mean, sd)
		if lo <= r <= hi:
			return r

if __name__ == '__main__':
	if len(sys.argv) != 2:
		sys.stderr.write('Syntax: %s <output-csv-path>\n' % sys.argv[0])
		sys.exit(1)
	random.seed(RANDOM_SEED)
	with open(sys.argv[1], 'wb') as fp:
		timestamp = TIMESTAMP_START
		for i in xrange(NUM_ORDERS):
			order_id = '%d' % (BASE_ID + i)
			part = random.choice(PARTS)
			price = '%.02f' % normal_bounded(PRICE_MEAN, PRICE_SD, PRICE_LO, PRICE_HI)
			if random.random() < Q_ROUND_P:
				quantity = '%d' % (int(random.expovariate(Q_ROUND_LAMBDA) / 100.0 + 1.0) * 100)
			else:
				quantity = '%d' % (int(random.expovariate(Q_NON_ROUND_LAMBDA)) + 1)
			timestamp += int(random.expovariate(TIMESTAMP_LAMBDA))
			timestamp_str = '%d' % timestamp
			direction = random.choice(DIRECTIONS)
			fp.write('%s,%s,%s,%s,%s,%s\n' % (order_id, part, price, quantity, timestamp, direction))
			if i % (NUM_ORDERS // 100) == 0 or i == NUM_ORDERS - 1:
				percentage = float(i + 1) / NUM_ORDERS * 100.0
				print '\r%d%%' % int(percentage),
				num_blocks = int(percentage / 5)
				print '|' + '=' * num_blocks + '-' * (20 - num_blocks) + '|',
				sys.stdout.flush()
		print