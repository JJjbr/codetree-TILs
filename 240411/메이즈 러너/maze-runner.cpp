#include <iostream>
#include <cstring>
#include <algorithm>
#include <vector>
using namespace std;

struct People {
	int y, x, dist;
};

struct Coord {
	int y, x;
};

struct Minarea {
	int y, x, length;
};

int N, M, K;
int map[12][12];
People people[12];
Coord out;
vector<Minarea> min_area;
int ans;
int out_people;
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

bool cmp(Minarea left, Minarea right) {
	if (left.length == right.length) {
		if(left.y == right.y)
			return left.x < right.x;
		return left.y < right.y;
	}
	return left.length < right.length;
}

void rotateMaze(int sy, int sx, int len) {
	int tmp_map[12][12] = { 0, };
	memcpy(tmp_map, map, sizeof(map));
	bool change_out = false;
	for (int y = 0; y <= len; y++) {
		for (int x = 0; x <= len; x++) {
			int py = sy + y;
			int px = sx + x;

			int ny = sy + x;
			int nx = sx + len - y;

			if (map[py][px] == 0) {
				tmp_map[ny][nx] = 0;
			}
			else if (map[py][px] > 0) {
				tmp_map[ny][nx] = map[py][px] - 1;
			}

			if (!change_out && py == out.y && px == out.x) {
				out.y = ny;
				out.x = nx;
				change_out = true;
			}
		}
	}
	memcpy(map, tmp_map, sizeof(tmp_map));

	for (int i = 1; i <= M; i++) {
		if (people[i].dist == 0) continue;
		if (people[i].y >= sy && people[i].y <= sy + len && people[i].x >= sx && people[i].x <= sx + len) {
			int y = people[i].y - sy;
			int x = people[i].x - sx;

			int ny = sy + x;
			int nx = sx + len - y;

			people[i].y = ny;
			people[i].x = nx;
		}
	}
}

void getMinArea(int y, int x) {
	int length_y = abs(y - out.y);
	int length_x = abs(x - out.x);
	int length = max(length_y, length_x);

	int sy = max(y, out.y) - length;
	if (sy < 1) sy = 1;

	int sx = max(x, out.x) - length;
	if (sx < 1) sx = 1;

	min_area.push_back({ sy, sx, length });
}

void movePerson(int p) {
	People now = people[p];
	for (int i = 0; i < 4; i++) {
		int ny = now.y + dy[i];
		int nx = now.x + dx[i];
		if (ny <= 0 || ny > N || nx <= 0 || nx > N) continue;
		if (map[ny][nx] > 0) continue;
		int dist = abs(ny - out.y) + abs(nx - out.x);
		if (dist >= now.dist) continue;

		ans++;
		people[p].y = ny;
		people[p].x = nx;
		people[p].dist = dist;
		if (dist == 0) out_people++;
		break;
	}
}

void movePeople() {
	for (int i = 1; i <= M; i++) {
		if (people[i].dist == 0) continue;
		movePerson(i);
	}
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= M; i++) {
		cin >> people[i].y >> people[i].x;
		people[i].dist = 1e6;
	}
	cin >> out.y >> out.x;

	for (int i = 1; i <= K; i++) {
		for (int j = 1; j <= M; j++) {
			if (people[j].dist == 0) continue;
			people[j].dist = abs(people[j].y - out.y) + abs(people[j].x - out.x);
		}
		movePeople();

		if (out_people >= M) break;

		for (int j = 1; j <= M; j++) {
			if (people[j].dist == 0) continue;
			getMinArea(people[j].y, people[j].x);
		}

		sort(min_area.begin(), min_area.end(), cmp);
		rotateMaze(min_area[0].y, min_area[0].x, min_area[0].length);
		min_area.clear();
	}
	cout << ans << "\n";
	cout << out.y << " " << out.x << "\n";
	return 0;
}