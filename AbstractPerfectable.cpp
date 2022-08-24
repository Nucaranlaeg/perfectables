#include <iostream>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <tuple>
#include <chrono>
#include <math.h>
using std::cout;
using std::endl;

std::vector <int> prime = {};

const int DEPTH = 40;
FILE * output;

struct prime_factor {
	int power;
	int unusable;
	int base_power;
	double multiplier;
	std::vector <std::tuple<int, int> > factors;
};

std::vector <std::vector <prime_factor> > prime_factors;

std::vector <int> max_count;

int primes[DEPTH][4] = {0};
// First element is the base.
// Second element is the primes required.
// Third element is the power used.
// Fourth element is the base power.
int count = 0;

// Keep track of the current progress toward x2
// It starts above 1 to account for rounding errors.
double total_mult = 1.0001 * log(2);

// For optimization
long long worst_hide_time = 1;

long long ipow(long long base, int exp){
	long long result = 1;
	while(exp){
		if (exp & 1){
			result *= base;
		}
		exp >>= 1;
		base *= base;
	}
	return result;
}

int isprime(int possible){
	for (int i = 2; i < 1000; i++){
		if (possible % i == 0) return i;
	}
	return possible;
}

void display(){
	output = fopen("Perfectables.txt", "a");
	cout << prime[primes[0][0]] << "^" << primes[0][2];
	fprintf(output, "%d^%d", prime[primes[0][0]], primes[0][2]);
	for (int i = 1; i < DEPTH; i++){
		if (primes[i][0] == -1) break;
		fprintf(output, " * %d^%d", prime[primes[i][0]], primes[i][2]);
		cout << " * " << prime[primes[i][0]] << "^" << primes[i][2];
	}
	cout << endl;
	fprintf(output, "\n");
	fclose (output);
}

void displayEffective(){
	output = fopen("EffectivePerfectables.txt", "a");
	// cout << "(" << prime[primes[0][0]] << "^" << primes[0][3] << ")^" << primes[0][2] / primes[0][3];
	fprintf(output, "(%d^%d)^%d", prime[primes[0][0]], primes[0][3], primes[0][2] / primes[0][3]);
	for (int i = 1; i < DEPTH; i++){
		if (primes[i][0] == -1) break;
		fprintf(output, " * (%d^%d)^%d", prime[primes[i][0]], primes[i][3], primes[i][2] / primes[i][3]);
		// cout << " * (" << prime[primes[i][0]] << "^" << primes[i][3] << ")^" << primes[i][2] / primes[i][3];
	}
	// cout << endl;
	fprintf(output, "\n");
	fclose (output);
}

void get_factor_count(bool reset_usable = true, int iteration = 0) {
	std::vector<int> current_count;
	for (int i = max_count.size(); i--;){
		max_count[i] = 0;
	}
	while (max_count.size() < prime.size()){
		max_count.push_back(0);
	}
	while (current_count.size() < prime.size()){
		current_count.push_back(0);
	}
	for (int i = prime.size(); i--;){
		for (int j = prime.size(); j--;){
			current_count[j] = 0;
		}
		int k = 0;
		while (k < DEPTH && primes[k][0] != i) k++;
		for (int j = prime_factors[i].size(); j--;){
			if (reset_usable){
				if (prime_factors[i][j].unusable > iteration){
					prime_factors[i][j].unusable = 0;
				}
			} else if (prime_factors[i][j].unusable) {
				continue;
			} else if (k < DEPTH) {
				if ((primes[k][2] > 0 && primes[k][2] != prime_factors[i][j].power) || prime_factors[i][j].power < primes[k][1]){
					prime_factors[i][j].unusable = iteration;
				}
			} else if (prime_factors[i][j].multiplier > total_mult){
				// If the factor we're looking at would make it impossible to be a perfectable number...
				prime_factors[i][j].unusable = iteration;
				continue;
			}
			if (prime_factors[i][j].unusable) continue;
			for (int m = prime_factors[i][j].factors.size(); m--;){
				int base, multiplicity;
				std::tie(base, multiplicity) = prime_factors[i][j].factors[m];
				current_count[base] = current_count[base] < multiplicity ? multiplicity : current_count[base];
			}
		}
		for (int j = prime.size(); j--;){
			max_count[j] += current_count[j];
		}
	}
}

bool hide_factors(int iteration) {
	auto start = std::chrono::high_resolution_clock::now();
	int filtered = 2;
	while (filtered){
		get_factor_count(filtered == 2, iteration);
		filtered = 0;
		for (int i = prime.size(); i--;){
			// Remove the highest power until it's possible to be found.
			for (int j = prime_factors[i].size(); j--;){
				if (!prime_factors[i][j].unusable && prime_factors[i][j].power > max_count[i]){
					filtered = 1;
					prime_factors[i][j].unusable = iteration;
				} else {
					break;
				}
			}
		}
	}
	auto stop = std::chrono::high_resolution_clock::now();
	long long hide_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	if (hide_time > worst_hide_time) worst_hide_time = hide_time;
	for (int i = 0; i < DEPTH; i++){
		if (primes[i][2] - primes[i][1] > max_count[primes[i][0]]) return true;
	}
	return false;
}

int filter_factors() {
	get_factor_count();
	int filtered = 0;
	for (int i = 0; i < prime.size(); i++){
		// Remove the highest power until it's possible to be found.
		while (prime_factors[i].size() && prime_factors[i][prime_factors[i].size() - 1].power > max_count[i]){
			filtered++;
			prime_factors[i].pop_back();
		}
	}
	return filtered;
}

void build_all_factors() {
	prime_factors.clear();
	int total = 0;
	for (int i = 0; i < prime.size(); i++){
		int base = prime[i];
		std::vector <prime_factor> blank;
		for (int power = 1; power < 32; power++){
			long long sigma, top, bottom;
			double multiplier;
			for (int base_power = 1; base_power <= power; base_power++){
				if (power % base_power != 0) continue;

				// Get the value of sigma((prime^base_power)^power)
				top = ipow(base, double(power + base_power));
				if (top < 0) break;
				bottom = ipow(base, double(base_power));
				sigma = (top - 1)/(bottom - 1);
				if (sigma == 0) continue;

				prime_factor factor;
				factor.power = power;
				factor.base_power = base_power;
				factor.multiplier = log(sigma / pow(base, double(power)));
				// Catch some overflows.
				if (factor.multiplier < 0) continue;
				for (int j = prime.size() - 1; j >= 0; j--){
					int multiplicity = 0;
					while (sigma % prime[j] == 0){
						if (sigma / prime[j] == 0) throw sigma;
						sigma /= prime[j];
						multiplicity++;
					}
					if (multiplicity > 0){
						factor.factors.push_back(std::make_tuple(j, multiplicity));
					}
				}
				if (sigma == 1){
					blank.push_back(factor);
					total++;
				}
			}
			if (top < 0) break;
		}
		prime_factors.push_back(blank);
	}
	cout << "Factorable powers: " << total << endl;
	while (int removed = filter_factors()){
		total -= removed;
	}
	cout << "Factorable powers after filtering: " << total << endl;
}

bool add_factor(std::tuple<int, int> factor) {
	int base, multiplicity;
	std::tie(base, multiplicity) = factor;
	for (int i = 0; i < DEPTH; i++){
		if (primes[i][0] == -1){
			primes[i][0] = base;
			primes[i][1] = multiplicity;
			if (prime[base] == 2){
				primes[i][1] -= 1;
			}
			return false;
		} else if (primes[i][0] == base) {
			primes[i][1] += multiplicity;
			// If we can't possibly make it work...
			if (prime_factors[base][prime_factors[base].size() - 1].power < primes[i][1]) return true;
			// If we've got too many to make it work...
			if (primes[i][2] > 0 && primes[i][1] > primes[i][2]) return true;
			return false;
		}
	}
	return true;
}

void remove_factor(std::tuple<int, int> factor) {
	int base, multiplicity;
	std::tie(base, multiplicity) = factor;
	for (int i = 0; i < DEPTH; i++){
		if (primes[i][0] == base) {
			primes[i][1] -= multiplicity;
			if (primes[i][2] == 0){
				if (prime[base] == 2){
					if (primes[i][1] == -1){
						primes[i][0] = -1;
					}
				} else if (primes[i][1] == 0){
					primes[i][0] = -1;
				}
			}
			return;
		}
	}
}

bool is_success() {
	for (int i = 0; i < DEPTH; i++){
		if (primes[i][0] == -1){
			display();
			displayEffective();
			count++;
			return true;
		}
		if (primes[i][1] != primes[i][2]) return false;
	}
	return false;
}

void factor(int pos = 0) {
	long long last_loop_time = 0;
	for (int i = 0; i < prime_factors[primes[pos][0]].size(); i++){
		auto start = std::chrono::high_resolution_clock::now();
		prime_factor target_factor = prime_factors[primes[pos][0]][i];
		if (target_factor.unusable || target_factor.power < primes[pos][1]) continue;
		primes[pos][2] = target_factor.power;
		primes[pos][3] = target_factor.base_power;
		int j = 0;
		bool failed;
		for (; j < target_factor.factors.size(); j++){
			failed = add_factor(target_factor.factors[j]);
			if (failed){
				j++;
				break;
			}
		}
		j--;
		if (!failed && !is_success()){
			total_mult -= target_factor.multiplier;
			if (total_mult > 0){
				if (last_loop_time > worst_hide_time){
					failed = hide_factors(pos);
				}
				if (!failed){
					factor(pos + 1);
				}
			}
			total_mult += target_factor.multiplier;
		}
		for (; j >= 0; j--){
			remove_factor(target_factor.factors[j]);
		}
		primes[pos][2] -= target_factor.power;
		auto stop = std::chrono::high_resolution_clock::now();
		last_loop_time = std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
	}
}

int main() {
	int starting_prime = 0;
	int next = 1;
	cout << "Input max starting prime: (1109) ";
	std::cin >> starting_prime;
	if (starting_prime){
		while (next < starting_prime){
			next++;
			if (next == isprime(next)) {
				prime.push_back(next);
			}
		}
	}
	for (int i = 0; i < DEPTH; i++){
		primes[i][0] = -1;
	}
	bool automatic = true;
	bool skip = false;
	int pausing_prime = 0;
	long long total_time = 0;
	while (true){
		skip = false;
		if (!automatic){
			cout << "Input new prime to examine: ";
			std::cin >> next;
			if (next == 0){
				automatic = true;
				cout << "Input number to pause on: ";
				std::cin >> pausing_prime;
				continue;
			}
		} else {
			next++;
			if (next == pausing_prime) automatic = false;
		}
		if (next != isprime(next)) continue;
		cout << "Next prime: " << next << endl;
		for (int i = 0; i < prime.size(); i++){
			if (prime[i] == next){
				skip = true;
				cout << "Already in prime list.\n";
				break;
			}
		}
		if (skip || next <= 1) continue;
		prime.push_back(next);
		build_all_factors();
		primes[0][0] = prime.size() - 1;
		primes[0][1] = 0;
		primes[0][2] = 0;
		auto start = std::chrono::high_resolution_clock::now();
		factor(0);
		auto stop = std::chrono::high_resolution_clock::now();
		total_time += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
		cout << "Time: " << std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count() << " : " << total_time << endl;
		cout << "For a total of " << count << " found!\n";
	}
	return 0;
}
