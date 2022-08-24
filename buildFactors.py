from sympy.ntheory import factorint, nextprime
import time

MAX_TIME = 0.1

data = {}

prime = 2
while prime < 10:
	p_data = []
	power = 0
	worstTime = 0
	while worstTime < MAX_TIME:
		power += 1
		basePower = 0
		while basePower <= power:
			basePower += 1
			if power % basePower > 0:
				continue
			start = time.process_time()
			value = (prime ** power) // (prime ** basePower - 1)
			print(prime ** basePower, "^", power / basePower, "                              ", end="\r")
			p_data += [{
				"prime": prime,
				"basePower": basePower,
				"power": power,
				"factors": factorint(value)
			}]
			end = time.process_time()
			if worstTime < end - start:
				worstTime = end - start
	print(prime, power, "                                             ")
	data[prime] = p_data
	prime = nextprime(prime)

print(data)