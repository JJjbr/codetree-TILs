#include <iostream>
#include <queue>

using namespace std;

struct Coord {
	int y, x;
};

int n, m;
int map[22][22];
int dir[4][2] = { {-1,0}, {0,1}, {1,0}, {0,-1} };

int bfs(int y, int x, int num) {
	bool visited[22][22] = { 0, };
	int sum = 0;

	queue<Coord> q;
	q.push({ y, x });
	visited[y][x] = true;
	while (!q.empty()) {
		Coord now = q.front();
		q.pop();
		sum += num;

		for (int i = 0; i < 4; i++) {
			int ny = now.y + dir[i][0];
			int nx = now.x + dir[i][1];
			if (ny < 0 || ny >= n || nx < 0 || nx >= n || visited[ny][nx]) continue;
			visited[ny][nx] = true;
			if (map[ny][nx] != num) continue;
			q.push({ ny, nx });
		}
	}
	return sum;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	
	cin >> n >> m;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cin >> map[i][j];
		}
	}

	int score = 0;
	int dice[6] = { 6, 3, 4, 5, 2, 1 };
	int y = 0, x = 0, dir = 2;
	for (int i = 0; i < m; i++) {
		if (dir == 1) { // 상
			y--;
			if (y < 0) {
				y = 1;
				dir = 3;
				int tmp = dice[0];
				dice[0] = dice[4];
				dice[5] = 7 - dice[0];
				dice[3] = tmp;
				dice[4] = 7 - dice[3];
			}
			else {
				int tmp = dice[0];
				dice[0] = dice[3];
				dice[5] = 7 - dice[0];
				dice[4] = tmp;
				dice[3] = 7 - dice[4];
			}
		}
		else if (dir == 2) { // 우
			x++;
			if (x >= n) {
				x = n - 2;
				dir = 4;
				int tmp = dice[0];
				dice[0] = dice[2];
				dice[5] = 7 - dice[0];
				dice[1] = tmp;
				dice[2] = 7 - dice[1];
			}
			else {
				int tmp = dice[0];
				dice[0] = dice[1];
				dice[5] = 7 - dice[0];
				dice[2] = tmp;
				dice[1] = 7 - dice[2];
			}
		}
		else if (dir == 3) { // 하
			y++;
			if (y >= n) {
				y = n - 2;
				dir = 1;
				int tmp = dice[0];
				dice[0] = dice[3];
				dice[5] = 7 - dice[0];
				dice[4] = tmp;
				dice[3] = 7 - dice[4];
			}
			else {
				int tmp = dice[0];
				dice[0] = dice[4];
				dice[5] = 7 - dice[0];
				dice[3] = tmp;
				dice[4] = 7 - dice[3];
			}	
		}
		else if (dir == 4) { // 좌
			x--;
			if (x < 0) {
				x = 1;
				dir = 2;
				int tmp = dice[0];
				dice[0] = dice[1];
				dice[5] = 7 - dice[0];
				dice[2] = tmp;
				dice[1] = 7 - dice[2];
			}
			else {
				int tmp = dice[0];
				dice[0] = dice[2];
				dice[5] = 7 - dice[0];
				dice[1] = tmp;
				dice[2] = 7 - dice[1];
			}		
		}
		score += bfs(y, x, map[y][x]);

		if (dice[0] > map[y][x]) {
			dir++;
			if (dir > 4) dir = 1;
		}
		else if (dice[0] < map[y][x]) {
			dir--;
			if (dir < 1) dir = 4;
		}
	}

	cout << score << "\n";
	return 0;
}