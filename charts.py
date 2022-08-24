import math
import matplotlib.pyplot as plot
import re

perfectables = []

def calculate(number):
	total = 1
	for prime in number:
		total *= (prime[0] ** prime[1]) ** prime[2]
	return total

with open("EffectivePerfectables.txt") as data:
	for line in data:
		number = line.strip().split(" * ")
		number = [[int(y) for y in re.findall("\d+", x)] for x in number]
		number = [calculate(number)] + number
		perfectables += [number]

def drawGraph(requiredPrime, exponent):
	selected = [x for x in perfectables if any(y[0] == requiredPrime and y[1] == exponent for y in x[1:])]
	xlist = [y[2] for x in selected for y in x[1:] if y[0] == requiredPrime and y[1] == exponent]
	ylist = [math.log(x[0], 10) for x in selected]
	plot.plot(xlist, ylist, "o")
	plot.title(f"{requiredPrime}^{exponent} ({requiredPrime ** exponent})")
	plot.xlabel("Log_10 of perfectable")
	plot.ylabel(f"Exponent of {requiredPrime ** exponent}")
	plot.show()

def drawGraphWithZero(requiredPrime, exponent):
	selected = perfectables[:]
	for x in selected:
		if not any(1 for y in x[1:] if y[0] == requiredPrime and y[1] == exponent):
			x += [[requiredPrime, exponent, 0]]
	xlist = [y[2] for x in selected for y in x[1:] if y[0] == requiredPrime and y[1] == exponent]
	ylist = [math.log(x[0], 10) for x in selected]
	plot.plot(xlist, ylist, "o")
	plot.title(f"{requiredPrime}^{exponent} ({requiredPrime ** exponent})")
	plot.xlabel("Log_10 of perfectable")
	plot.ylabel(f"Exponent of {requiredPrime ** exponent}")
	plot.show()

def drawAllPrimeGraph(requiredPrime):
	selected = perfectables[:]
	for x in selected:
		if not any(1 for y in x[1:] if y[0] == requiredPrime):
			x += [[requiredPrime, 0, 0]]
	xlist = [y[1] * y[2] for x in selected for y in x[1:] if y[0] == requiredPrime]
	ylist = [math.log(x[0], 10) for x in selected]
	plot.plot(xlist, ylist, "o")
	plot.title(f"{requiredPrime}")
	plot.xlabel("Log_10 of perfectable")
	plot.ylabel(f"Exponent of {requiredPrime}")
	plot.show()

drawAllPrimeGraph(7)