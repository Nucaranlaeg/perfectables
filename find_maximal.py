target = 7

data = []

with open("effectivePerfectables.txt", "r") as file:
	for line in file:
		primes = line.strip().split(" * ")
		value = 1
		iszero = True
		if len(primes) == 2:
			continue
		for prime in primes:
			prime = prime.replace(")", "").replace("(", "").split("^")
			value *= (int(prime[0]) ** int(prime[1])) ** int(prime[2])
			if int(prime[0]) == target:
				iszero = False
		if iszero:
			data.append(value)

print(target, sorted(data)[-1])
