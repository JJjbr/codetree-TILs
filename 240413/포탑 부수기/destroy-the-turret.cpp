#include <iostream>
#include <queue>
#include <cstring>
using namespace std;

struct Coord {
	int y, x;
};

struct Strong {
	int power, latest, sum;
	Coord pos;
	bool operator<(Strong right) const {
		if (power == right.power) {
			if (latest == right.latest) {
				if (sum == right.sum) {
					return pos.x > right.pos.x;
				}
				return sum > right.sum;
			}
			return latest > right.latest;
		}
		return power < right.power;
	}
};

struct Weak {
	int power, latest, sum;
	Coord pos;
	bool operator<(Weak right) const {
		if (power == right.power) {
			if (latest == right.latest) {
				if (sum == right.sum) {
					return pos.x < right.pos.x;
				}
				return sum < right.sum;
			}
			return latest < right.latest;
		}
		return power > right.power;
	}
};

struct Move {
	int y, x, dist;
	bool operator<(Move right) const {
		/*if (dir == right.dir) {
			return dir > right.dir;
		}*/
		return dist < right.dist;
	}
};

int N, M, K;
int map[12][12];
priority_queue<Weak> weak;
priority_queue<Strong> strong;
bool visited[12][12];
Coord path[100];
Coord min_path[100];
bool visited_path[12][12];
int max_n;
bool attack;
int attack_time[12][12];
int dy[4] = { 0, 1, 0, -1 };
int dx[4] = { 1, 0, -1, 0 };
int around[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

void input() {
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= M; j++) {
			cin >> map[i][j];
			if (map[i][j] == 0) continue;
			weak.push({ map[i][j], 0, i + j, {i, j} });
			strong.push({ map[i][j], 0, i + j, {i, j} });
		}
	}
}

void attackShell(int sy, int sx, int ey, int ex) {
	map[ey][ex] -= map[sy][sx];

	for (int i = 0; i < 8; i++) {
		int ny = ey + around[i][0];
		if (ny <= 0) ny = N;
		else if (ny > N) ny = 1;

		int nx = ex + around[i][1];
		if (nx <= 0) nx = M;
		else if (nx > M) nx = 1;

		if (map[ny][nx] <= 0) continue;
		map[ny][nx] -= (map[sy][sx] / 2);
		visited[ny][nx] = true;
	}
}

void laserPath(int n, int sy, int sx, int ey, int ex, int y, int x) {
	//cout << y << " " << x << "\n";
	if (n >= max_n) return;
	if (y == ey && x == ex) {
		max_n = n;
		//cout << max_n << "\n";
		memcpy(min_path, path, sizeof(path));
		attack = true;
		return;
	}
	for (int i = 0; i < 4; i++) {
		int ny = y + dy[i];
		if (ny <= 0) ny = N;
		else if (ny > N) ny = 1;

		int nx = x + dx[i];
		if (nx <= 0) nx = M;
		else if (nx > M) nx = 1;

		if (map[ny][nx] <= 0 || visited_path[ny][nx]) continue;
		path[n] = { ny, nx };
		visited_path[ny][nx] = true;
		laserPath(n + 1, sy, sx, ey, ex, ny, nx);
		visited_path[ny][nx] = false;
		//path.pop_back();
	}
}

void attackLaser(int sy, int sx, int ey, int ex) {
	//int dist[12][12] = { 0, };
	//path.clear();
	memset(path, 0, sizeof(path));
	memset(min_path, 0, sizeof(min_path));
	memset(visited_path, 0, sizeof(visited_path));
	map[sy][sx] += (N + M);
	attack_time[sy][sx] += 1;
	max_n = 1e6;
	attack = false;
	visited_path[sy][sx] = true;
	laserPath(0, sy, sx, ey, ex, sy, sx);

	if (attack) {
		//cout << "Laser\n";
		map[ey][ex] -= map[sy][sx];
		for (int i = 0; i < max_n; i++) {
			if ((min_path[i].y == sy && min_path[i].x == sx) || (min_path[i].y == ey && min_path[i].x == ex)) continue;
			map[min_path[i].y][min_path[i].x] -= (map[sy][sx] / 2);
			visited[min_path[i].y][min_path[i].x] = true;
			//cout << i << " " << min_path[i].y << " " << min_path[i].x << "\n";
		}
	}
	else {
		//cout << "Shell\n";
		attackShell(sy, sx, ey, ex);
	}
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);

	input();

	for (int k = 0; k < K; k++) {
		memset(visited, 0, sizeof(visited));

		Weak now_weak = weak.top();
		Strong now_strong = strong.top();
		//cout << now_weak.pos.y << " " << now_weak.pos.x << " " << now_strong.pos.y << " " << now_strong.pos.x << "\n";
		
		attackLaser(now_weak.pos.y, now_weak.pos.x, now_strong.pos.y, now_strong.pos.x);
		visited[now_weak.pos.y][now_weak.pos.x] = true;
		visited[now_strong.pos.y][now_strong.pos.x] = true;
		
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++) {
				if (visited[i][j]) continue;
				if (map[i][j] <= 0) continue;
				map[i][j] += 1;
			}
		}

		weak = priority_queue<Weak>();
		strong = priority_queue<Strong>();
		int total = 0;
		for (int i = 1; i <= N; i++) {
			for (int j = 1; j <= M; j++) {
				//cout << map[i][j] << " ";
				if (map[i][j] <= 0) continue;
				weak.push({ map[i][j], attack_time[i][j], i + j, {i, j} });
				strong.push({ map[i][j], attack_time[i][j], i + j, {i, j} });
				total++;
			}
			//cout << "\n";
		}

		if (total <= 1) break;
	}

	cout << strong.top().power << "\n";
	return 0;
}