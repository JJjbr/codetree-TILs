#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

struct People {
	int y, x, dist;
};

struct Coord {
	int y, x;
};

struct Check {
	int p, y, x;
	bool operator<(Check right) const {
		if (y > right.y) return true;
		else if(y == right.y) {
			if (x > right.x) return true;
			else if (x == right.x) {
				if (p > right.p) return true;
			}
		}
		return false;
	}
};

struct Minarea {
	int sy, sx, ey, ex;
};

int N, M, K;
int map[12][12];
//int people_map[12][12];
People people[12];
Coord out;
int min_area = 1e6;
Minarea min_area_coord;
int ans;
int dy[4] = { -1, 1, 0, 0 };
int dx[4] = { 0, 0, -1, 1 };

void rotateMaze() {
	int sy = min_area_coord.sy, sx = min_area_coord.sx, ey = min_area_coord.ey, ex = min_area_coord.ex;
	//cout << sy << " " << sx << " " << ey << " " << ex << "\n";
	int tmp_map[12][12] = { 0, };
	memcpy(tmp_map, map, sizeof(map));
	int sum = sy + ex;
	int sub = sy - sx;
	priority_queue<Check> pq;
	for (int i = 1; i <= M; i++) {
		if (people[i].y >= sy && people[i].x >= sx && people[i].y <= ey && people[i].x <= ex) {
			pq.push({ i, people[i].y, people[i].x });
		}
	}
	Check now = pq.top();
	pq.pop();
	bool change_out = false;
	for (int y = sy; y <= ey; y++) {
		for (int x = sx; x <= ex; x++) {
			tmp_map[sub + x][sum - y] = map[y][x];
			if (map[y][x] > 0) {
				tmp_map[sub + x][sum - y] = map[y][x] - 1;
			}
			if (!change_out && y == out.y && x == out.x) {
				out.y = sub + x;
				out.x = sum - y;
				change_out = true;
				//cout << "good " << y << " " << x << " " << out.y << " " << out.x << "\n";
			}
			while (now.y == y && now.x == x) {
				people[now.p].y = sub + x;
				people[now.p].x = sum - y;
				//people[now.p].dist = abs(people[now.p].y - out.y) + abs(people[now.p].x - out.x);
				if (pq.empty()) break;
				now = pq.top();
				pq.pop();
			}
		}
	}
	memcpy(map, tmp_map, sizeof(tmp_map));
}

void getMinArea(int ny, int nx) {
	int y = ny - out.y;
	int x = nx - out.x;
	//cout << ny << " " << nx << " " << out.y << " " << out.x << "\n";
	
	int length = 0, area = 0;
	Minarea tmp = { out.y, out.x, out.y, out.x };
	if (abs(x) < abs(y)) {
		length = y;
		if (length < 0) { // 상
			int start = out.x < nx ? out.x : nx;
			int end = out.x > nx ? out.x : nx;
			for (int c = end + length; c <= start; c++) {
				if (c <= 0 || c > N || c + abs(length) <= 0 || c + abs(length) > N) continue;
				tmp.sy = ny;
				tmp.sx = c;
				tmp.ey = out.y;
				tmp.ex = c + abs(length);
				//cout << "up\n";
				break;
			}
		}
		else if (length > 0) { // 하
			int start = out.x < nx ? out.x : nx;
			int end = out.x > nx ? out.x : nx;
			for (int c = end - length; c <= start; c++) {
				if (c <= 0 || c > N || c + abs(length) <= 0 || c + abs(length) > N) continue;
				tmp.sy = out.y;
				tmp.sx = c;
				tmp.ey = ny;
				tmp.ex = c + abs(length);
				//cout << "down\n";
				break;
			}
		}
	}
	else if (abs(x) > abs(y)) {
		length = x;
		if (length < 0) { // 좌
			int start = out.y < ny ? out.y : ny;
			int end = out.y > ny ? out.y : ny;
			for (int r = end + length; r <= start; r++) {
				if (r <= 0 || r > N || r + abs(length) <= 0 || r + abs(length) > N) continue;
				tmp.sy = r;
				tmp.sx = nx;
				tmp.ey = r + abs(length);
				tmp.ex = out.x;
				//cout << "left\n";
				break;
			}
		}
		else if (length > 0) { // 우
			int start = out.y < ny ? out.y : ny;
			int end = out.y > ny ? out.y : ny;
			for (int r = end - length; r <= start; r++) {
				if (r <= 0 || r > N || r + abs(length) <= 0 || r + abs(length) > N) continue;
				tmp.sy = r;
				tmp.sx = out.x;
				tmp.ey = r + abs(length);
				tmp.ex = nx;
				//cout << "right\n";
				break;
			}
		}
	}
	//cout << tmp.sy << " " << tmp.sx << " " << tmp.ey << " " << tmp.ex << "\n";

	area = (abs(length) + 1) * (abs(length) + 1);
	if (area < min_area) {
		min_area = area;
		min_area_coord = tmp;
	}
}

bool movePerson(int p) {
	People now = people[p];
	for (int i = 0; i < 4; i++) {
		int ny = now.y + dy[i];
		int nx = now.x + dx[i];
		if (ny <= 0 || ny > N || nx <= 0 || nx > N) continue;
		if (map[ny][nx] > 0) continue;
		int dist = abs(ny - out.y) + abs(nx - out.x);
		if (dist < now.dist) {
			people[p].y = ny;
			people[p].x = nx;
			people[p].dist = dist;
			return true;
		}
	}
	return false;
}

void movePeople() {
	for (int i = 1; i <= M; i++) {
		if (people[i].dist == 0) continue;
		if (movePerson(i)) {
			ans++;
		}
	}
}

int main() {
	// 입력
	cin >> N >> M >> K;
	for (int i = 1; i <= N; i++) {
		for (int j = 1; j <= N; j++) {
			cin >> map[i][j];
		}
	}
	for (int i = 1; i <= M; i++) {
		cin >> people[i].y >> people[i].x;
		people[i].dist = -1;
	}
	cin >> out.y >> out.x;

	// 초기 거리 구하기
	//for (int i = 1; i <= M; i++) {
	//	people[i].dist = abs(people[i].y - out.y) + abs(people[i].x - out.x);
	//}

	//cout << ans << "\n";
	//for (int i = 1; i <= N; i++) {
	//	for (int j = 1; j <= N; j++) {
	//		cout << map[i][j] << " ";
	//	}
	//	cout << "\n";
	//}

	for (int i = 1; i <= K; i++) {
		for (int i = 1; i <= M; i++) {
			if (people[i].dist == 0) continue;
			people[i].dist = abs(people[i].y - out.y) + abs(people[i].x - out.x);
		}

		//cout << "Time " << i << "\n";
		movePeople();
		//for (int j = 1; j <= M; j++) {
		//	if (people[j].dist == 0) continue;
		//	cout << people[j].y << " " << people[j].x << "\n";
		//}

		min_area = 1e6;
		for (int j = 1; j <= M; j++) {
			if (people[j].dist == 0) continue;
			//cout << people[j].y << " " << people[j].x << "\n";
			getMinArea(people[j].y, people[j].x);
		}

		rotateMaze();

		//cout << "ok " << min_area << "\n";
		//for (int i = 1; i <= N; i++) {
		//	for (int j = 1; j <= N; j++) {
		//		cout << map[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		//for (int j = 1; j <= M; j++) {
		//	if (people[j].dist == 0) continue;
		//	cout << people[j].y << " " << people[j].x << "\n";
		//}
		//cout << ans << "\n";
		//cout << out.y << " " << out.x << "\n";
	}
	cout << ans << "\n";
	cout << out.y << " " << out.x << "\n";
	return 0;
}