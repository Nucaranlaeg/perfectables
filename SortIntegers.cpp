#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int main(){
	ifstream input;
	ofstream output;
	input.open("ListOfPerfectables.txt");
	output.open("SortedList.txt");
	vector <string> numbers;
	string line;
	int total = 0;
	while (getline(input, line)){
		total++;
		numbers.push_back(line);
	}
	for (int i = 0; total > 0; i++){
		for (int j = 0; j < numbers.size(); j++){
			if (numbers[j].size() == i){
				//cout << numbers[j] << endl;
				output << numbers[j] << endl;
				total--;
			}
		}
	}
	return 0;
}
