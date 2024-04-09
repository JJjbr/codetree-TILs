#include <iostream>
using namespace std;

struct Knight {
	int r, c, h, w, k, dmg, move;
};

int L, N, Q;
int map[42][42];
int knight_map[42][42];
Knight knights[32];
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void updateKnight(int id, int dir) {
	for (int i = 1; i <= N; i++) {
		if (!knights[i].move) continue;
		knights[i].move = 0;

		Knight now = knights[i];
		int ny = now.r + dy[dir];
		int nx = now.c + dx[dir];
		for (int y = now.r; y < now.r + now.h; y++) {
			for (int x = now.c; x < now.c + now.w; x++) {
				if (knight_map[y][x] == i) {
					knight_map[y][x] = 0;
				}
			}
		}
		for (int y = ny; y < ny + now.h; y++) {
			for (int x = nx; x < nx + now.w; x++) {
				knight_map[y][x] = i;
			}
		}

		if (i == id || knights[i].k <= 0) continue;
		knights[i].r = ny;
		knights[i].c = nx;
		now = knights[i];

		int cnt = 0;
		for (int y = now.r; y < now.r + now.h; y++) {
			for (int x = now.c; x < now.c + now.w; x++) {
				if (map[y][x] == 1) cnt++;
			}
		}

		knights[i].k -= cnt;
		knights[i].dmg += cnt;

		if (knights[i].k <= 0) {
			for (int y = now.r; y < now.r + now.h; y++) {
				for (int x = now.c; x < now.c + now.w; x++) {
					knight_map[y][x] = 0;
				}
			}
		}
	}
}

bool moveKnight(int id, int dir) {
	Knight now = knights[id];
	int ny = now.r + dy[dir];
	int nx = now.c + dx[dir];
	if (ny <= 0 || (ny + now.h - 1) > L || nx <= 0 || (nx + now.w - 1) > L) return false;

	for (int y = ny; y < ny + now.h; y++) {
		for (int x = nx; x < nx + now.w; x++) {
			if (map[y][x] == 2) return false;

			if (knight_map[y][x] > 0 && knight_map[y][x] != id) {
				if (!moveKnight(knight_map[y][x], dir)) return false;
			}
		}
	}

	knights[id].move = 1;
	return true;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	cin >> L >> N >> Q;
	for (int y = 1; y <= L; y++) {
		for (int x = 1; x <= L; x++) {
			cin >> map[y][x];
		}
	}
	int r, c, h, w, k;
	for (int i = 1; i <= N; i++) {
		cin >> r >> c >> h >> w >> k;
		knights[i] = { r, c, h, w, k, 0, 0 };

		for (int y = r; y < r + h; y++) {
			for (int x = c; x < c + w; x++) {
				knight_map[y][x] = i;
			}
		}
	}
	int id, dir;
	for (int i = 1; i <= Q; i++) {
		cin >> id >> dir;
		if (knights[id].k <= 0) continue;
		if (!moveKnight(id, dir)) {
			for (int j = 1; j <= N; j++) {
				knights[j].move = 0;
			}
			continue;
		}
		updateKnight(id, dir);
	}

	int ans = 0;
	for (int i = 1; i <= N; i++) {
		if (knights[i].k <= 0) continue;
		ans += knights[i].dmg;
	}
	cout << ans << "\n";
	return 0;
}