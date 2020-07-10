#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
using namespace std;

struct MARS
{
	string name;
	int W;
	int H;
	int start_x;
	int start_y;
	int elevation;
	int target_num;
	vector<pair<int, int>> goals;
	vector<vector<int>> map;
};

void load(MARS&);
void save(vector<pair<int, int>>&);
void Algorithm_A(MARS&, vector<pair<int, int>>&, int);
void Algorithm_BFS(MARS&, vector<pair<int, int>>&);
void Algorithm_UCS(MARS&, vector<pair<int, int>>&, int);
void UCS(MARS&, vector<pair<int, int>>&, int);
void A_star(MARS &mars, vector<pair<int, int>>& result, int a);

int main()
{
	MARS mars;
	load(mars);
	if (mars.name == "A*")
	{
		//Algorithm_A(mars, result);
		for (int i = 0; i < mars.target_num; i++)
		{
			vector<pair<int, int>> result;
			result.push_back(make_pair(mars.start_x, mars.start_y));
			//UCS(mars, result, i);
			//Algorithm_A(mars, result, i);
			A_star(mars, result, i);
			save(result);
		}
	}
	else if (mars.name == "BFS")
	{
		vector<pair<int, int>> result;
		Algorithm_BFS(mars, result);
	}
	else if (mars.name == "UCS")
	{
		for (int i = 0; i < mars.target_num; i++)
		{
			vector<pair<int, int>> result;
			result.push_back(make_pair(mars.start_x, mars.start_y));
			//UCS(mars, result, i);
			Algorithm_UCS(mars, result, i);
			save(result);
		}
		//Algorithm_UCS(mars, result);
	}

	/*for (int i = 0; i < mars.H; i++)
	{
		for (int j = 0; j < mars.W; j++)
		{
			cout << mars.map[j][i];
		}
	}*/

	//cout << mars.start_x << "," << mars.start_y << " ";
	/*for (int i = 0; i < result.size(); i++)
	{
		cout << result[i].second << "," << result[i].first << " ";
	}*/

	system("pause");
	return 0;
}

void A_star(MARS &mars, vector<pair<int, int>>& result, int a)
{
	vector<vector<int>> open_list(mars.map.size()), close_list(mars.map.size()), F(mars.map.size()), G(mars.map.size());
	vector<vector<pair<int, int>>> parent(mars.map.size());
	for (int i = 0; i < mars.map.size(); i++)
	{
		open_list[i].resize(mars.map[i].size());
		close_list[i].resize(mars.map[i].size());
		F[i].resize(mars.map[i].size());
		G[i].resize(mars.map[i].size());
		parent[i].resize(mars.map[i].size());
	}
	open_list[mars.start_x][mars.start_y] = 1;
	F[mars.start_x][mars.start_y] = 1000;
	vector<pair<int, int>> piority;
	piority.push_back(make_pair(mars.start_x, mars.start_y));
	int x = mars.start_x, y = mars.start_y;
	int right = 0, left = 0;
	if (mars.start_y > mars.goals[a].second)
	{
		right = mars.start_y;
	}
	else if (mars.start_y <= mars.goals[a].second)
	{
		right = mars.goals[a].second;
		left = mars.start_y;
	}

	while (x != mars.goals[a].first || y != mars.goals[a].second)
	{
		int shortGx = 0, shortGy = 0;
		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				if (j <= right && j >= left && i < mars.H && i >= 0)
				{
					if (open_list[i][j] != 1 && close_list[i][j] == 0)
					{
						int cost = 14;
						if (abs(mars.map[x][y] - mars.map[i][j]) <= mars.elevation)
						{
							open_list[i][j] = 1;
							if (i == x || j == y)
								cost = 10;
							piority.push_back(make_pair(i, j));
							F[i][j] = cost + abs(mars.map[x][y] - mars.map[i][j]);
							G[i][j] = cost;
							parent[i][j] = make_pair(x, y);
						}
					}
					else if ((i != x || j != y) && open_list[i][j] == 1)
					{
						int cost = 14;
						if (abs(mars.map[x][y] - mars.map[i][j]) <= mars.elevation)
						{
							if (i == x || j == y)
								cost = 10;
							//piority.push_back(make_pair(i, j));
							if(F[i][j] < cost + abs(mars.map[x][y] - mars.map[i][j]))
								F[i][j] = cost + abs(mars.map[x][y] - mars.map[i][j]);
							if (cost + G[i][j] > G[i][j])
							{
								x = parent[i][j].first;
								y = parent[i][j].second;
								shortGx = i;
								shortGy = j;
								open_list[x][y] = 0;
								close_list[x][y] = 1;
								/*F[i][j] = 1000;
								x = parent[i][j].first;
								y = parent[i][j].second;*/
							}
							//parent[i][j] = make_pair(x, y);
						}
					}
				}
			}
		}
		if (shortGx != 0 || shortGy != 0)
		{
			//open_list[shortGx][shortGy] = 0;
			//close_list[shortGx][shortGy] = 1;
			F[shortGx][shortGy] = 1000;
			x = parent[shortGx][shortGy].first;
			y = parent[shortGx][shortGy].second;
		}
		open_list[x][y] = 0;
		close_list[x][y] = 1;

		int smallF = INT_MAX;
		for (int i = 0; i < piority.size(); i++)
		{
			if ((shortGx != 0 || shortGy != 0) && piority[i].first == shortGx && piority[i].second == shortGy)
			{
				smallF = F[piority[i].first][piority[i].second];
				x = piority[i].first;
				y = piority[i].second;
				break;
			}
			else if (F[piority[i].first][piority[i].second] <= smallF && open_list[piority[i].first][piority[i].second] == 1)
			{
				
				smallF = F[piority[i].first][piority[i].second];
				x = piority[i].first;
				y = piority[i].second;
			}
		}
	}
}

void save(vector<pair<int, int>>& result)
{
	ofstream outFile("output.txt", ios::out | ios::app);

	if (!outFile)                                     
	{
		cerr << "Error" << endl;
		system("pause");
		exit(1);
	}
	else
	{
		for (int i = 0; i < result.size(); i++)  
		{
			if (result[0].first == -1 && result[0].second == -1)
			{
				outFile << "FAIL";
				result.resize(0);
			}
			else
				outFile << result[i].second << "," << result[i].first << " ";
		}
		outFile << endl;
	}
}

void load(MARS &mars)
{
	ifstream infile("input.txt", ios::in);

	if (!infile)                               
	{
		cout << "Open Error" << endl;
		system("pause");
		exit(1);
	}
	else
	{
		infile >> mars.name;
		infile >> mars.W;
		infile >> mars.H;
		infile >> mars.start_y;
		infile >> mars.start_x;
		infile >> mars.elevation;
		infile >> mars.target_num;
		for (int i = 0; i < mars.target_num; i++)
		{
			int x, y;
			infile >> x;
			infile >> y;
			mars.goals.push_back(make_pair(y, x));
		}
		for (int i = 0; i < mars.H; i++)
		{
			vector<int> W;
			for (int j = 0; j < mars.W; j++)
			{
				int mymap;
				infile >> mymap;
				W.push_back(mymap);
			}
			mars.map.push_back(W);
		}
	}
	infile.close();
}

void UCS(MARS &mars, vector<pair<int, int>>& result, int a)
{
	vector<vector<int>> visit(mars.map.size());
	for (int i = 0; i < mars.map.size(); i++)
	{
		visit[i].resize(mars.map[i].size());
	}
	visit[mars.start_x][mars.start_y] = 1;

	int x = mars.start_x, y = mars.start_y;
	int right = 0, left = 0;
	if (mars.start_y > mars.goals[a].second)
	{
		right = mars.start_y;
	}
	else if (mars.start_y <= mars.goals[a].second)
	{
		right = mars.goals[a].second;
		left = mars.start_y;
	}
	while (x != mars.goals[a].first || y != mars.goals[a].second)
	{
		int curX = mars.goals[a].first, curY = mars.goals[a].second, total = 0, max = INT_MIN;

		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				int cost = 14, total = 0;
				if (j <= right && j >= left && i < mars.H && i >= 0)
				{
					if (visit[i][j] != 1)
					{
						visit[i][j] = 1;
						if (i == x || j == y)
							cost = 10;

						if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
							total = cost;
						//if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
						//	total = cost + abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]);
						if (total >= max)
						{
							curX = i;
							curY = j;
							max = total;
						}
					}
				}
			}
		}
		x = curX;
		y = curY;
		/*if (mars.start_y > mars.goals[a].second)
		{
			right = curX;
		}
		else if (mars.start_y <= mars.goals[a].second)
		{
			right = mars.goals[a].second;
			left = curY;
		}*/
		result.push_back(make_pair(x, y));
		if (max == INT_MIN)
		{
			result[0] = make_pair(-1, -1);
		}
	}

	/*if (mars.start_y > mars.goals[a].second)
	{
		while (x >= mars.goals[a].first && y >= mars.goals[a].second)
		{
			int curX = 0, curY = 0, total = INT_MIN, max = INT_MIN;
			if (x == mars.goals[a].first && y == mars.goals[a].second)
				return;

			for (int i = x - 1; i <= x + 1; i++)
			{
				for (int j = y - 1; j <= y + 1; j++)
				{
					int cost = 14;
					if (j <= right && j >= left && i < mars.H && i >= 0)
					{
						if (visit[i][j] != 1)
						{
							visit[i][j] = 1;
							if (i == x || j == y)
								cost = 10;
							if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
								total = cost + abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]);
							if (total > max)
							{
								curX = i;
								curY = j;
								max = total;
							}
						}
					}
				}
			}
			x = curX;
			y = curY;
			result.push_back(make_pair(x, y));
			if (total == INT_MIN)
				result.push_back(make_pair(-1, -1));
		}
	}*/
}

void Algorithm_UCS(MARS &mars, vector<pair<int, int>>& result, int a)
{
	vector<vector<int>> visit(mars.map.size());
	for (int i = 0; i < mars.map.size(); i++)
	{
		visit[i].resize(mars.map[i].size());
	}
	visit[mars.start_x][mars.start_y] = 1;

	int x = mars.start_x, y = mars.start_y;
	int right = 0, left = 0;
	if (mars.start_y > mars.goals[a].second)
	{
		right = mars.start_y;
	}
	else if (mars.start_y <= mars.goals[a].second)
	{
		right = mars.goals[a].second;
		left = mars.start_y;
	}
	while (x != mars.goals[a].first || y != mars.goals[a].second)
	{
		int curX = mars.goals[a].first, curY = mars.goals[a].second, total = 0, max = INT_MIN;

		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				int cost = 14, total = 0;
				if (j <= right && j >= left && i < mars.H && i >= 0)
				{
					if (visit[i][j] != 1)
					{
						visit[i][j] = 1;
						if (i == x || j == y)
							cost = 10;

						if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
							total = cost;
						//if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
						//	total = cost + abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]);
						if (total >= max)
						{
							curX = i;
							curY = j;
							max = total;
						}
					}
				}
			}
		}
		x = curX;
		y = curY;

		result.push_back(make_pair(x, y));
		if (max == INT_MIN || max == 0)
		{
			result[0] = make_pair(-1, -1);
		}
	}
}

void Algorithm_BFS(MARS &mars, vector<pair<int, int>>& result)
{
	queue<int> Q;
	Q.push(mars.map[mars.start_x][mars.start_y]);
	vector<vector<int>> visit(mars.map.size());
	for (int i = 0; i < mars.map.size(); i++)
	{
		visit[i].resize(mars.map[i].size());
	}
	visit[mars.start_x][mars.start_y] = 1;
	while (!Q.empty())
	{
		int front = Q.front();
		cout << front << " ";
		Q.pop();
		for (int i = 0; i < mars.H; i++)
		{
			for (int j = 0; j < mars.W; j++)
			{
				if (!visit[i][j] && abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
				{
					visit[i][j] = 1;
					Q.push(mars.map[i][j]);
				}
			}
		}
	}

}


void Algorithm_A(MARS &mars, vector<pair<int, int>>& result, int a)
{
	vector<vector<int>> visit(mars.map.size());
	for (int i = 0; i < mars.map.size(); i++)
	{
		visit[i].resize(mars.map[i].size());
	}
	visit[mars.start_x][mars.start_y] = 1;

	int x = mars.start_x, y = mars.start_y;
	int right = 0, left = 0;
	if (mars.start_y > mars.goals[a].second)
	{
		right = mars.start_y;
	}
	else if (mars.start_y <= mars.goals[a].second)
	{
		right = mars.goals[a].second;
		left = mars.start_y;
	}
	while (x != mars.goals[a].first || y != mars.goals[a].second)
	{
		int curX = mars.goals[a].first, curY = mars.goals[a].second, min = INT_MAX;

		for (int i = x - 1; i <= x + 1; i++)
		{
			for (int j = y - 1; j <= y + 1; j++)
			{
				int cost = 14, total = 0;
				if (j <= right && j >= left && i < mars.H && i >= 0)
				{
					if (visit[i][j] != 1)
					{
						visit[i][j] = 1;
						if (i == x || j == y)
							cost = 10;

						//if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
						//	total = cost;
						if (abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]) <= mars.elevation)
							total = cost + abs(mars.map[mars.start_x][mars.start_y] - mars.map[i][j]);
						if (total <= min)
						{
							curX = i;
							curY = j;
							min = total;
						}
					}
				}
			}
		}
		x = curX;
		y = curY;

		result.push_back(make_pair(x, y));
		if (min == INT_MAX || min == 0)
		{
			result[0] = make_pair(-1, -1);
		}
	}
}