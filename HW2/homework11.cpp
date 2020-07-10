#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <vector>

using namespace std;

int whiteScore[16][16] =
{
	{20,19,18,17,16,6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0},
	{19,19,18,17,16,6, 5, 4, 3, 2, 1, 0, 0, 0, 0, 0},
	{18,18,18,17,7, 6, 5, 4, 3, 2, 1, 1, 0, 0, 0, 0},
	{17,17,17,8, 7, 6, 5, 4, 3, 2, 2, 1, 1, 0, 0, 0},
	{16,16,7, 7, 7, 6, 5, 4, 3, 3, 2, 2, 1, 1, 0, 0},
	{6, 6, 6, 6, 6, 6, 5, 4, 4, 3, 3, 2, 2, 1, 1, 1},
	{5, 5, 5, 5, 5, 5, 5, 5, 4, 4, 3, 3, 2, 2, 2, 2},
	{4, 4, 4, 4, 4, 4, 5, 5, 5, 5, 4, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 4, 4, 5, 5, 4, 4, 3, 3, 3, 3, 3},
	{2, 2, 2, 2, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2},
	{1, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 1},
	{0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1, 0, 0},
	{0, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 2, 3, 3, 2, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 0, 0, 0}
};

int blackScore[16][16] =
{
	{0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 0, 1, 2, 3, 3, 2, 1, 0, 0, 0, 0, 0},
	{0, 0, 0, 0, 1, 1, 2, 3, 3, 2, 1, 1, 0, 0, 0, 0},
	{0, 0, 0, 1, 1, 2, 2, 3, 3, 2, 2, 1, 1, 0, 0, 0},
	{0, 0, 1, 1, 2, 2, 3, 3, 3, 3, 2, 2, 1, 1, 0, 0},
	{1, 1, 1, 2, 2, 3, 3, 4, 4, 3, 3, 2, 2, 1, 1, 1},
	{3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 3, 3, 2, 2, 2, 2},
	{4, 4, 4, 4, 4, 4, 4, 5, 5, 4, 4, 3, 3, 3, 3, 3},
	{3, 3, 3, 3, 3, 4, 4, 5, 5, 4, 4, 4, 4, 4, 4, 4},
	{2, 2, 2, 2, 3, 3, 4, 4, 4, 5, 5, 5, 5, 5, 5, 5},
	{1, 1, 1, 2, 2, 3, 3, 4, 4, 5, 6, 6, 6, 6, 6, 6},
	{0, 0, 1, 1, 2, 2, 3, 3, 4, 5, 6, 7, 7, 7,16,16},
	{0, 0, 0, 1, 1, 2, 2, 3, 4, 5, 6, 7, 8,17,17,17},
	{0, 0, 0, 0, 1, 1, 2, 3, 4, 5, 6, 7,17,18,18,18},
	{0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,16,17,18,19,19},
	{0, 0, 0, 0, 0, 1, 2, 3, 4, 5, 6,16,17,18,19,20}
};

class Game
{
public:
	bool isSingle;
	bool isBlack;
	double playTime;
	char board[16][16];

	void readInput()
	{
		ifstream file("input2.txt");
		string line;
		getline(file, line);
		line == "SINGLE" ? isSingle = true : isSingle = false;
		getline(file, line);
		line == "BLACK" ? isBlack = true : isBlack = false;
		getline(file, line);
		istringstream iss(line);
		iss >> playTime;
		for (int i = 0; i < 16; i++)
		{
			getline(file, line);
			for (int j = 0; j < 16; j++)
			{
				board[i][j] = line[j];
			}
		}
		file.close();
	}
};

class Move
{
public:
	bool isSingle;
	bool isBlack;
	double playTime;
	char board[16][16];
	double score;
	int depth;
	vector<string> path;

	Move()
	{
		isSingle = true;
		isBlack = true;
		playTime = 0;
		score = 0;
		depth = 0;
	}

	Move(bool isSingle, bool isBlack, double playTime, char board[16][16])
	{
		this->isSingle = isSingle;
		this->isBlack = isBlack;
		this->playTime = playTime;
		memcpy(this->board, board, sizeof(char) * 16 * 16);
		score = 0;
		depth = 0;
	}

	Move(const Move &m)
	{
		isSingle = m.isSingle;
		isBlack = m.isBlack;
		playTime = m.playTime;
		memcpy(board, m.board, sizeof(char) * 16 * 16);
		score = m.score;
		depth = m.depth;
		path = vector<string>(m.path);
	}

	Move& operator=(Move &m)
	{
		isSingle = m.isSingle;
		isBlack = m.isBlack;
		playTime = m.playTime;
		memcpy(board, m.board, sizeof(char) * 16 * 16);
		score = m.score;
		depth = m.depth;
		path = vector<string>(m.path);
		return *this;
	}

	void calculateScore()
	{
		score = 0;
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (isBlack)
				{
					if (board[i][j] == 'B')
					{
						score += blackScore[i][j];
					}
					else if (board[i][j] == 'W')
					{
						score -= whiteScore[i][j];
					}
				}
				else
				{
					if (board[i][j] == 'B')
					{
						score -= blackScore[i][j];
					}
					else if (board[i][j] == 'W')
					{
						score += whiteScore[i][j];
					}
				}
			}
		}
	}

	bool win()
	{
		int score = 0;
		for (int i = 0; i < 16; i++)
		{
			for (int j = 0; j < 16; j++)
			{
				if (isBlack && board[i][j] == 'B')
				{
					score += blackScore[i][j];
				}
				else if (!isBlack && board[i][j] == 'W')
				{
					score += whiteScore[i][j];
				}
			}
		}
		return score == 333;
	}

	bool isValid(int i, int j)
	{
		if (i >= 0 && i < 16 && j >= 0 && j < 16 && board[j][i] == '.')
		{
			return true;
		}
		return false;
	}

	void move(int fromX, int fromY, int toX, int toY)
	{
		board[toY][toX] = board[fromY][fromX];
		board[fromY][fromX] = '.';
		calculateScore();
	}
};

Move MaxStart(Move &move);
Move Min(Move &move);
Move Max(Move &move);

Move MaxStart(Move &move)
{
	int depth = 3;
	if (move.playTime < 30)
	{
		depth = 2;
	}
	if (move.win() || move.depth > depth)
	{
		return move;
	}
	move.depth++;
	Move max;
	max.score = -1024;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if ((move.isBlack && move.board[y][x] == 'B') || (!move.isBlack && move.board[y][x] == 'W'))
			{
				if (move.isValid(x - 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x - 1) << "," << y;
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x + 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x + 1) << "," << y;
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x - 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x - 1) << "," << (y - 1);
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x + 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x + 1) << "," << (y + 1);
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << x << "," << (y - 1);
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << x << "," << (y + 1);
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x - 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x - 1) << "," << (y + 1);
						max.path.push_back(oss.str());
					}
				}
				if (move.isValid(x + 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "E " << x << "," << y << " " << (x + 1) << "," << (y - 1);
						max.path.push_back(oss.str());
					}
				}
				// jump
				if (move.isValid(x - 2, y) && move.board[y][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x - 2) << "," << y;
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x + 2, y) && move.board[y][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x + 2) << "," << y;
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x - 2, y - 2) && move.board[y - 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x - 2) << "," << (y - 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x + 2, y + 2) && move.board[y + 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x + 2) << "," << (y + 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x, y - 2) && move.board[y - 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << x << "," << (y - 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x, y + 2) && move.board[y + 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << x << "," << (y + 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x - 2, y + 2) && move.board[y + 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x - 2) << "," << (y + 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
				if (move.isValid(x + 2, y - 2) && move.board[y - 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
						ostringstream oss;
						oss << "J " << x << "," << y << " " << (x + 2) << "," << (y - 2);
						max.path.push_back(oss.str());
					}

					if (nextMove.isValid(x + 2, y + 2) && nextMove.board[y + 1][x + 2] != '.')
					{
						Move next2Move(nextMove);
						next2Move.move(x + 2, y, x + 2, y + 2);
						Move minMove = Min(next2Move);
						if (minMove.score > max.score)
						{
							max = minMove;
							ostringstream oss;
							oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
							oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2);
							max.path.push_back(oss.str());
						}

						if (nextMove.isValid(x + 4, y + 4) && nextMove.board[y + 3][x + 3] != '.')
						{
							Move next3Move(next2Move);
							next3Move.move(x + 2, y + 2, x + 4, y + 4);
							Move minMove = Min(next3Move);
							if (minMove.score > max.score)
							{
								max = minMove;
								ostringstream oss;
								oss << "J " << x << "," << y << " " << (x + 2) << "," << y << endl;
								oss << "J " << (x + 2) << "," << y << " " << (x + 2) << "," << (y + 2) << endl;
								oss << "J " << (x + 2) << "," << (y + 2) << " " << (x + 4) << "," << (y + 4);
								max.path.push_back(oss.str());
							}
						}
					}
				}
			}
		}
	}
	return max;
}

Move Max(Move &move)
{
	int depth = 3;
	if (move.playTime < 30)
	{
		depth = 2;
	}
	if (move.win() || move.depth > depth)
	{
		return move;
	}
	move.depth++;
	Move max;
	max.score = -1024;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if ((move.isBlack && move.board[y][x] == 'B') || (!move.isBlack && move.board[y][x] == 'W'))
			{
				if (move.isValid(x - 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x - 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x - 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y + 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y - 1);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				// jump
				if (move.isValid(x - 2, y) && move.board[y][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 2, y) && move.board[y][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x - 2, y - 2) && move.board[y - 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 2, y + 2) && move.board[y + 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x, y - 2) && move.board[y - 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x, y + 2) && move.board[y + 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x - 2, y + 2) && move.board[y + 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y + 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
				if (move.isValid(x + 2, y - 2) && move.board[y - 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y - 2);
					Move minMove = Min(nextMove);
					if (minMove.score > max.score)
					{
						max = minMove;
					}
				}
			}
		}
	}
	return max;
}

Move Min(Move &move)
{
	int depth = 3;
	if (move.playTime < 30)
	{
		depth = 2;
	}
	if (move.win() || move.depth > depth)
	{
		return move;
	}
	move.depth++;
	Move min;
	min.score = 1024;
	for (int y = 0; y < 16; y++)
	{
		for (int x = 0; x < 16; x++)
		{
			if ((move.isBlack && move.board[y][x] == 'W') || (!move.isBlack && move.board[y][x] == 'B'))
			{
				// move
				if (move.isValid(x - 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 1, y))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x - 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y - 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y + 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y - 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x - 1, y + 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 1, y + 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 1, y - 1))
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 1, y - 1);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				// jump
				if (move.isValid(x - 2, y) && move.board[y][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 2, y) && move.board[y][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x - 2, y - 2) && move.board[y - 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y - 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 2, y + 2) && move.board[y + 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y + 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x, y - 2) && move.board[y - 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y - 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x, y + 2) && move.board[y + 1][x] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x, y + 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x - 2, y + 2) && move.board[y + 1][x - 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x - 2, y + 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
				if (move.isValid(x + 2, y - 2) && move.board[y - 1][x + 1] != '.')
				{
					Move nextMove(move);
					nextMove.move(x, y, x + 2, y - 2);
					Move maxMove = Max(nextMove);
					if (maxMove.score < min.score)
					{
						min = maxMove;
					}
				}
			}
		}
	}
	return min;
}

int main(int argc, char *argv[])
{
	Game game;
	game.readInput();

	Move start(game.isSingle, game.isBlack, game.playTime, game.board);
	Move result = MaxStart(start);

	ofstream outputFile;
	outputFile.open("output.txt");

	for (unsigned int i = 0; i < result.path.size(); i++)
	{
		outputFile << result.path[i] << endl;
	}
	outputFile.close();
	return 0;
}
