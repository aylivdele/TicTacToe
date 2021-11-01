#include <vector>
#include <map>
#include <random>
#include <ctime>
#include <iostream>

using namespace std;
using PairType = pair<char, int>;
using MovesType = vector<PairType>;

class TicTacToe {
private:
	MovesType _moves;
	vector<char> _field;
	char _winner;
	const char _empty = '-';
	const char _cross = 'x';
	const char _circle = 'o';
	const char _draw = 'd';
	random_device rng;
	uniform_int_distribution<> uid;
	map<char, int> _scores;
	bool _smartIsCross;
	bool _makeFirstMove;
	
public:
	TicTacToe() : _winner(_empty), _smartIsCross(true), _makeFirstMove(false) {};
	TicTacToe(bool makeFirstMove) : _winner(_empty), _smartIsCross(true), _makeFirstMove(makeFirstMove) {};
	void SpeedUp(bool makeFirstMove) {
		_makeFirstMove = makeFirstMove;
	}
	void Play(){
		Clear();
		while (!SetWinner()) {
			bool crossMove = true;
			if (_moves.size() % 2 == 1)
				crossMove = false;
			if (crossMove)
				_smartIsCross ? MakeSmartMove() : MakeRandomMove();
			else 
				!_smartIsCross ? MakeSmartMove() : MakeRandomMove();
			cout << _moves.back().first << _moves.back().second << " ";
		}
		
	}

	MovesType GetMoves() {
		return _moves;
	}

	tuple<vector<char>, char, bool> GetResult() {
		return make_tuple(_field, _winner, _smartIsCross);
	}

private:

	void MakeRandomMove() {

		vector<int> freeSlots;
		int i = 0;
		for (auto it = _field.begin(); it != _field.end(); ++it, i++)
			if (*it == _empty)
				freeSlots.push_back(i);
		bool mark = true;
		size_t index = uid(rng);
		auto it_free = freeSlots.begin() + (index % freeSlots.size());

		_moves.push_back(make_pair(!_smartIsCross ? _cross : _circle, *it_free));
		*(_field.begin() + *it_free) = !_smartIsCross ? _cross : _circle;
	}

	void MakeSmartMove() {
		if (_makeFirstMove && _moves.size() < 3) {// делаем лучший первый ход вручную для ускорения вычисления
			auto it = _field.begin();
			if (*(it + 4) == _empty) { 
				*(it + 4) = _smartIsCross ? _cross : _circle;
				_moves.push_back(make_pair(_smartIsCross ? _cross : _circle, 4));
				return;
			}
			*(it + 0) = _smartIsCross ? _cross : _circle;
			_moves.push_back(make_pair(_smartIsCross ? _cross : _circle, 0));
			return;
		}
		vector<char> tempField;
		for (auto it = _field.begin(); it != _field.end(); ++it)
			tempField.push_back(*it);
		int bestScore = numeric_limits<int>::min();
		int move;
		int i = 0;
		for (auto it = tempField.begin(); it != tempField.end(); ++it, i++) {
			if (*it == _empty) {
				*it = _smartIsCross ? _cross : _circle;
				int score = Minimax(tempField, false);
				*it = _empty;
				if (score > bestScore) {
					bestScore = score;
					move = i;
				}
			}
		}
		*(_field.begin() + move) = _smartIsCross ? _cross : _circle;
		_moves.push_back(make_pair(_smartIsCross ? _cross : _circle, move));
	}

	int Minimax(vector<char> field, bool isMaxing) {
		char winner = CheckIfFinished(field);
		if (winner != _empty)
			return _scores[winner];
		int bestScore = isMaxing ? numeric_limits<int>::min() : numeric_limits<int>::max();
		char player;
		if (isMaxing)
			player = _smartIsCross ? _cross : _circle;
		else
			player = !_smartIsCross ? _cross : _circle;
		for (auto it = field.begin(); it != field.end(); ++it) {
			if (*it == _empty) {
				*it = player;
				int score = Minimax(field, !isMaxing);
				*it = _empty;
				bestScore = isMaxing ? max(score, bestScore) : min(score, bestScore);
			}
		}
		return bestScore;
	}

	void Clear() {
		_moves.clear();
		_field.clear();
		for (int i = 0; i < 9; i++)
			_field.push_back(_empty);
		_winner = _empty;


		_scores.clear();

		uid = uniform_int_distribution<>(0, 9);
		_smartIsCross = !_smartIsCross;

		_scores.insert(make_pair(_draw, 0));
		_scores.insert(make_pair(_circle, _smartIsCross ? -10 : 10));
		_scores.insert(make_pair(_cross, _smartIsCross ? 10 : -10));
	}

	char CheckIfFinished(vector<char> field) {
		auto it = field.begin();
		if (*(it + 0) != _empty && *(it + 0) == *(it + 1) && *(it + 1) == *(it + 2))
			return *(it + 0);
		if (*(it + 3) != _empty && *(it + 3) == *(it + 4) && *(it + 4) == *(it + 5))
			return *(it + 3);
		if (*(it + 6) != _empty && *(it + 6) == *(it + 7) && *(it + 7) == *(it + 8))
			return *(it + 6);
		if (*(it + 0) != _empty && *(it + 0) == *(it + 3) && *(it + 3) == *(it + 6))
			return *(it + 0);
		if (*(it + 1) != _empty && *(it + 1) == *(it + 4) && *(it + 4) == *(it + 7))
			return *(it + 1);
		if (*(it + 2) != _empty && *(it + 2) == *(it + 5) && *(it + 5) == *(it + 8))
			return *(it + 2);
		if (*(it + 0) != _empty && *(it + 0) == *(it + 4) && *(it + 4) == *(it + 8))
			return *(it + 0);
		if (*(it + 2) != _empty && *(it + 2) == *(it + 4) && *(it + 4) == *(it + 6))
			return *(it + 2);
		for (auto it = field.begin(); it != field.end(); ++it)
			if (*it == _empty)
				return _empty;
		return _draw;
	}

	bool SetWinner() {
		char temp = CheckIfFinished(_field);
		if (temp != _empty) {
			_winner = temp;
			return true;
		}
		return false;
	}

};