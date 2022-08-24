#include <fstream>
#include <vector>
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
using namespace std;

int main(){
	string filename;
	ifstream input;
	ofstream output;
	vector <int> currentnum;
	string line;
	char next;
	int nextval, nextpow, process;
	int carry;
	vector <string> obtained;
	string last;
	int entries;
	vector <int> twos;
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	twos.push_back(0);
	int currentbases, maxbases = 0;
	start:
	entries = 0;
	cout << "What file? ";
	cin >> filename;
	input.open(filename);
	while (getline(input, line)){
		entries++;
		stringstream ss(line);
		currentnum.clear();
		currentnum.push_back(1);
		nextval = 0;
		nextpow = 0;
		process = 0;
		currentbases = 1;
		while (ss.get(next)){
			if (next == ' ') continue;
			if (next == '^'){
				process = 1;
				continue;
			}
			if (next == '*'){
				currentbases++;
				//cout << nextval << "^" << nextpow << endl;
				for (int i = 0; i < nextpow; i++){
					for (int j = 0; j < currentnum.size(); j++){
						currentnum[j] *= nextval;
					}
					for (int j = 0; j < currentnum.size(); j++){
						if (j == currentnum.size() - 1 && currentnum[j] >= 1000) currentnum.push_back(0);
						carry = (currentnum[j] - (currentnum[j] % 1000)) / 1000;
						currentnum[j] = currentnum[j] % 1000;
						currentnum[j+1] += carry;
					}
				}
				/*cout << currentnum[currentnum.size()-1];
				for (int i = currentnum.size() - 2; i >= 0; i--){
					if (currentnum[i] < 100) cout << 0;
					if (currentnum[i] < 10) cout << 0;
					cout << currentnum[i];
				}
				cout << endl;*/
				//if (nextval == 2) twos[nextpow-1]++;
				nextval = 0;
				nextpow = 0;
				process = 0;
				continue;
			}
			if (process == 0){
				nextval *= 10;
				nextval += next - 48;
				continue;
			}
			nextpow *= 10;
			nextpow += next - 48;
		}
		//if (nextval == 2) twos[nextpow-1]++;
		//cout << nextval << "^" << nextpow << endl;
		for (int i = 0; i < nextpow; i++){
			for (int j = 0; j < currentnum.size(); j++){
				currentnum[j] *= nextval;
			}
			for (int j = 0; j < currentnum.size(); j++){
				if (j == currentnum.size() - 1 && currentnum[j] >= 1000) currentnum.push_back(0);
				carry = (currentnum[j] - (currentnum[j] % 1000)) / 1000;
				currentnum[j] = currentnum[j] % 1000;
				currentnum[j+1] += carry;
			}
		}
		last = "";
		last += to_string(currentnum[currentnum.size()-1]);
		for (int i = currentnum.size() - 2; i >= 0; i--){
			if (currentnum[i] < 100) last += "0";
			if (currentnum[i] < 10) last += "0";
			last += to_string(currentnum[i]);
		}
		//cout << last << endl;
		obtained.push_back(last);
		if (obtained.back().size() > 100) cout << obtained.size() << " : " << obtained.back().size() << endl;
		//system("Pause");
		if (currentbases >  maxbases){
			maxbases = currentbases;
			cout << "Max prime factors: " << maxbases << endl;
		}
	}
	sort(obtained.begin(), obtained.end());
	obtained.erase(unique(obtained.begin(), obtained.end()), obtained.end());
	cout << endl << "There were " << entries << " entries in this file.\n" << "A total of " << obtained.size() << " perfectable numbers were read.\n";
	/*for (int i = 0; i < twos.size(); i++){
		cout << i+1 << ": " << twos[i] << endl;
	}*/
	input.close();
	output.open("ListOfPerfectables.txt");
	for (int i = 0; i < obtained.size(); i++){
		output << obtained[i] << endl;
	}
	system("SortIntegers.exe");
	goto start;
}
