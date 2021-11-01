#include <iostream>
#include "TicTacToe.cpp"
#include <fstream>

using namespace std;
using PairType = pair<char, int>;
using MovesType = vector<PairType>;
using ResultType = vector<pair<MovesType, tuple<vector<char>, char, bool>>>;

void PrintAndWriteResult(string filename, ResultType result) {
	ofstream File(filename);
	File << "winner;smartIs;moves\n";

	for (auto it_result = result.begin(); it_result != result.end(); ++it_result) {
		//print field, winner and smart player
		int i = 0;
		for (auto it = get<0>((*it_result).second).begin(); it != get<0>((*it_result).second).end(); ++it, i++) {
			if (i % 3 == 0)
				cout << endl;
			cout << *it << " ";
		}
		File << get<1>((*it_result).second) << ";" << (get<2>((*it_result).second) ? "x;" : "o;");
		cout << "\nWinner is " << get<1>((*it_result).second) << ". Smart player is " << (get<2>((*it_result).second) ? "x.\n" : "o.\n");
		//print moves
		cout << "Moves: ";
		for (auto it = (*it_result).first.begin(); it != (*it_result).first.end(); ++it) {
			File << (*it).first << (*it).second << " ";
			cout << (*it).first << (*it).second << " ";
		}
		File << endl;

		cout << endl << endl;
	}
}



int main()
{
	int n = 0;
	cout << "Enter number of games: ";
	cin >> n;
	cout << "Make first move manually to speed up calculation?(y/n): ";
	char speedUp = 'n';
	cin >> speedUp;

	TicTacToe game;
	if (speedUp == 'y')
		game.SpeedUp(true);

	ResultType resultVector;
	for (int i = 0; i < n; i++) {
		game.Play();
		resultVector.push_back(make_pair(game.GetMoves(), game.GetResult()));
		cout << i << endl;
	}
	PrintAndWriteResult("results.csv", resultVector);

	system("pause");

}
