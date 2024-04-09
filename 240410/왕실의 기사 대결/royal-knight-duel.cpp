#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;

struct Knight {
	int r, c, h, w, k, dmg, move;
};

int L, N, Q;
int map[42][42];
int knight_map[42][42];
int tmp_knight_map[42][42];
Knight knights[32];
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void updateKnight(int id, int dir) {
	bool visit[32] = { 0, };
	for (int y = 1; y <= L; y++) {
		for (int x = 1; x <= L; x++) {
			int now = knight_map[y][x];
			if (now == 0 || visit[now]) continue;
			visit[now] = true;

			if (knights[now].k <= 0) continue;
			if (knights[now].r != y || knights[now].c != x) knights[now].move = true;
			knights[now].r = y;
			knights[now].c = x;
		}
	}

	for (int i = 1; i <= N; i++) {
		if (!knights[i].move) continue;
		knights[i].move = 0;

		if (i == id || knights[i].k <= 0) continue;
		Knight now = knights[i];

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

	for (int y = now.r; y < now.r + now.h; y++) {
		for (int x = now.c; x < now.c + now.w; x++) {
			if (tmp_knight_map[y][x] == id) {
				tmp_knight_map[y][x] = 0;
			}
		}
	}
	for (int y = ny; y < ny + now.h; y++) {
		for (int x = nx; x < nx + now.w; x++) {
			if (tmp_knight_map[y][x] == 0) {
				tmp_knight_map[y][x] = id;
			}
		}
	}
	return true;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	//freopen("input.txt", "r", stdin);

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
		memcpy(tmp_knight_map, knight_map, sizeof(knight_map));
		if (!moveKnight(id, dir)) continue;
		memcpy(knight_map, tmp_knight_map, sizeof(tmp_knight_map));
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