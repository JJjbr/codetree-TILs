#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <cstring>

using namespace std;

struct Info {
	int power, latest, sum, y, x;
};

struct Move {
	int y, x;
};

int N, M, K;
vector<Info> tower;
int map[12][12];
int attack_time[12][12];
bool path[12][12];
int total;
int dy[4] = { 0, 1, 0, -1 };
int dx[4] = { 1, 0, -1, 0 };
int around[8][2] = { -1, -1, -1, 0, -1, 1, 0, -1, 0, 1, 1, -1, 1, 0, 1, 1 };

bool cmp(Info left, Info right) {
	if (left.power == right.power) {
		if (left.latest == right.latest) {
			if (left.sum == right.sum) {
				return left.x > right.x;
			}
			return left.sum > right.sum;
		}
		return left.latest > right.latest;
	}
	return left.power < right.power;
}

void input() {
	cin >> N >> M >> K;
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++) {
			cin >> map[i][j];
			if (map[i][j] == 0) continue;
			tower.push_back({ map[i][j], 0, i + j, i, j });
			total++;
		}
	}
	sort(tower.begin(), tower.end(), cmp);
}

void attackCannon() {
	int tower_size = tower.size();
	Info weak = tower[0];
	Info strong = tower[tower_size - 1];

	map[strong.y][strong.x] -= map[weak.y][weak.x];
	if (map[strong.y][strong.x] <= 0) total--;

	for (int i = 0; i < 8; i++) {
		int ny = strong.y + around[i][0];
		if (ny < 0) ny = N - 1;
		else if (ny >= N) ny = 0;

		int nx = strong.x + around[i][1];
		if (nx < 0) nx = M - 1;
		else if (nx >= M) nx = 0;

		if (map[ny][nx] <= 0) continue;
		if (ny == weak.y && nx == weak.x) continue;
		map[ny][nx] -= (map[weak.y][weak.x] / 2);
		path[ny][nx] = true;
		if (map[ny][nx] <= 0) total--;
	}
}

bool attackLaser() {
	int tower_size = tower.size();
	Info weak = tower[0];
	Info strong = tower[tower_size - 1];

	vector<vector<pair<int, int>>> visited(N, vector<pair<int, int>>(M, { -1, -1 }));
	pair<int, int> init = { -1, -1 };
	visited[weak.y][weak.x] = { weak.y, weak.x };

	queue<Move> q;
	q.push({ weak.y, weak.x });

	map[weak.y][weak.x] += (N + M);
	path[weak.y][weak.x] = true;
	path[strong.y][strong.x] = true;

	while (!q.empty()) {
		Move now = q.front();
		q.pop();

		if (now.y == strong.y && now.x == strong.x) {
			map[strong.y][strong.x] -= map[weak.y][weak.x];
			if (map[strong.y][strong.x] <= 0) total--;

			int py = visited[strong.y][strong.x].first;
			int px = visited[strong.y][strong.x].second;
			while (py != weak.y || px != weak.x) {
				map[py][px] -= (map[weak.y][weak.x] / 2);
				path[py][px] = true;
				if (map[py][px] <= 0) total--;
				
				int tmp = visited[py][px].first;
				px = visited[py][px].second;
				py = tmp;
			}
			return true;
		}

		for (int i = 0; i < 4; i++) {
			int ny = now.y + dy[i];
			if (ny < 0) ny = N - 1;
			else if (ny >= N) ny = 0;

			int nx = now.x + dx[i];
			if (nx < 0) nx = M - 1;
			else if (nx >= M) nx = 0;

			if (map[ny][nx] <= 0) continue;
			if (visited[ny][nx] != init) continue;
			q.push({ ny, nx });
			visited[ny][nx] = { now.y, now.x };
		}
	}
	return false;
}

int main() {

	input();

	int answer = 0;
	for (int k = 1; k <= K; k++) {
		memset(path, 0, sizeof(path));

		Info weak = tower[0];
		attack_time[weak.y][weak.x] = k;
		if (!attackLaser()) {
			attackCannon();
		}
		if (total <= 1) {
			answer = map[weak.y][weak.x];
			break;
		}

		total = 0;
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (map[i][j] <= 0) continue;
				total++;
				if (path[i][j]) continue;
				map[i][j]++;
			}
		}
		tower.clear();
		for (int i = 0; i < N; i++) {
			for (int j = 0; j < M; j++) {
				if (map[i][j] <= 0) continue;
				tower.push_back({ map[i][j], attack_time[i][j], i + j, i, j });
			}
		}
		sort(tower.begin(), tower.end(), cmp);

		//cout << "\n";
		//for (int i = 0; i < N; i++) {
		//	for (int j = 0; j < M; j++) {
		//		cout << map[i][j] << " ";
		//	}
		//	cout << "\n";
		//}

		answer = tower.back().power;
	}
	cout << answer << "\n";

	return 0;
}