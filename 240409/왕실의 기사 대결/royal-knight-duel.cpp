#include <iostream>
#include <queue>

using namespace std;

struct Info {
	int r, c, h, w, k, dmg, move;
};

int L, N, Q;
int map[42][42];
int knight_map[42][42];
Info knight[32];
//queue<int> move_list;
int dir[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };

void getDamages(int cmd_id, int d) {
	for (int id = 1; id <= N; id++) {
		if (!knight[id].move) continue;
		knight[id].move = 0;
		
		//int id = move_list.front();
		//move_list.pop();

		int y = knight[id].r;
		int x = knight[id].c;
		
		//cout << id << " " << y << " " << x << "\n";

		int nr = y + dir[d][0];
		int nc = x + dir[d][1];

		bool dead = 0;
		if (cmd_id != id) {
			for (int i = nr; i < nr + knight[id].h; i++) {
				for (int j = nc; j < nc + knight[id].w; j++) {
					// 맵을 벗어난다.
					if (i <= 0 || i > L || j <= 0 || j > L) {
						dead = 1;
						break;
					}
					if (map[i][j] == 1) {
						knight[id].k--;
						knight[id].dmg++;
					}
					if (knight[id].k <= 0) {
						dead = 1;
						break;
					}
				}
				if (dead) break;
			}
		}
		//cout << knight[id].k << "\n";

		if (dead) {
			knight[id].r = 0;
			knight[id].c = 0;
			for (int i = y; i < y + knight[id].h; i++) {
				for (int j = x; j < x + knight[id].w; j++) {
					// 맵을 벗어난다.
					if (i <= 0 || i > L || j <= 0 || j > L) break;
					knight_map[i][j] = 0;
				}
			}
		}
		else if (d == 0) {
			int ny = y + dir[d][0];
			int py = y + knight[id].h - 1;
			knight[id].r += dir[d][0];
			for (int nx = x; nx < x + knight[id].w; nx++) {
				// 맵을 벗어난다.
				if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
				// 벽이 있다.
				if (map[ny][nx] == 2) continue;
				knight_map[ny][nx] = id;
				knight_map[py][nx] = 0;
			}
		}
		else if (d == 1) {
			int nx = x + knight[id].w - 1 + dir[d][1];
			int px = x;
			knight[id].c += dir[d][1];
			for (int ny = y; ny < y + knight[id].h; ny++) {
				// 맵을 벗어난다.
				if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
				// 벽이 있다.
				if (map[ny][nx] == 2) continue;
				knight_map[ny][nx] = id;
				knight_map[ny][px] = 0;
			}
		}
		else if (d == 2) {
			int ny = y + knight[id].h - 1 + dir[d][0];
			int py = y;
			knight[id].r += dir[d][0];
			for (int nx = x; nx < x + knight[id].w; nx++) {
				// 맵을 벗어난다.
				if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
				// 벽이 있다.
				if (map[ny][nx] == 2) continue;
				knight_map[ny][nx] = id;
				knight_map[py][nx] = 0;
			}
		}
		else if (d == 3) {
			int nx = x + dir[d][1];
			int px = x + knight[id].w - 1;
			knight[id].c += dir[d][1];
			for (int ny = y; ny < y + knight[id].h; ny++) {
				// 맵을 벗어난다.
				if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
				// 벽이 있다.
				if (map[ny][nx] == 2) continue;
				knight_map[ny][nx] = id;
				knight_map[ny][px] = 0;
			}
		}
	}
}

bool pushKnight(int id, int d) {
	int y = knight[id].r;
	int x = knight[id].c;
	if (d == 0) {
		int ny = y + dir[d][0];
		for (int nx = x; nx < x + knight[id].w; nx++) {
			// 맵을 벗어난다.
			if (ny <= 0 || ny > L || nx <= 0 || nx > L) return true;
			// 벽이 있다.
			if (map[ny][nx] == 2) return false;
			// 기사가 있다.
			if (knight_map[ny][nx] > 0) {
				if (pushKnight(knight_map[ny][nx], d)) {
					knight[knight_map[ny][nx]].move = 1;
					//move_list.push(knight_map[ny][nx]);
				}
				else return false;
			}
		}
	}
	else if (d == 1) {
		int nx = x + knight[id].w - 1 + dir[d][1];
		for (int ny = y; ny < y + knight[id].h; ny++) {
			// 맵을 벗어난다.
			if (ny <= 0 || ny > L || nx <= 0 || nx > L) return true;
			// 벽이 있다.
			if (map[ny][nx] == 2) return false;
			// 기사가 있다.
			if (knight_map[ny][nx] > 0) {
				if (pushKnight(knight_map[ny][nx], d)) {
					knight[knight_map[ny][nx]].move = 1;
					//move_list.push(knight_map[ny][nx]);
				}
				else return false;
			}
		}
	}
	else if (d == 2) {
		int ny = y + knight[id].h - 1 + dir[d][0];
		for (int nx = x; nx < x + knight[id].w; nx++) {
			// 맵을 벗어난다.
			if (ny <= 0 || ny > L || nx <= 0 || nx > L) return true;
			// 벽이 있다.
			if (map[ny][nx] == 2) return false;
			// 기사가 있다.
			if (knight_map[ny][nx] > 0) {
				if (pushKnight(knight_map[ny][nx], d)) {
					knight[knight_map[ny][nx]].move = 1;
					//move_list.push(knight_map[ny][nx]);
				}
				else return false;
			}
		}
	}
	else if (d == 3) {
		int nx = x + dir[d][1];
		for (int ny = y; ny < y + knight[id].h; ny++) {
			// 맵을 벗어난다.
			if (ny <= 0 || ny > L || nx <= 0 || nx > L) return true;
			// 벽이 있다.
			if (map[ny][nx] == 2) return false;
			// 기사가 있다.
			if (knight_map[ny][nx] > 0) {
				if (pushKnight(knight_map[ny][nx], d)) {
					knight[knight_map[ny][nx]].move = 1;
					//move_list.push(knight_map[ny][nx]);
				}
				else return false;
			}
		}
	}
	return true;
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	cin >> L >> N >> Q;
	for (int i = 1; i <= L; i++) {
		for (int j = 1; j <= L; j++) {
			cin >> map[i][j];
		}
	}
	int r, c, h, w, k;
	for (int i = 1; i <= N; i++) {
		cin >> r >> c >> h >> w >> k;
		knight[i] = { r, c, h, w, k, 0 };
		for (int y = r; y < r + h; y++) {
			for (int x = c; x < c + w; x++) {
				knight_map[y][x] = i;
			}
		}
	}
	int cmd_id, d;
	for (int q = 1; q <= Q; q++) {
		cin >> cmd_id >> d;
		if (pushKnight(cmd_id, d)) {
			//cout << q << " ok\n";
			knight[cmd_id].move = 1;
			//move_list.push(id);
			//getDamages(id, d);

			for (int id = 1; id <= N; id++) {
				if (!knight[id].move) continue;
				knight[id].move = 0;

				//int id = move_list.front();
				//move_list.pop();

				int y = knight[id].r;
				int x = knight[id].c;

				//cout << id << " " << y << " " << x << "\n";

				int nr = y + dir[d][0];
				int nc = x + dir[d][1];

				bool dead = 0;
				if (cmd_id != id) {
					for (int i = nr; i < nr + knight[id].h; i++) {
						for (int j = nc; j < nc + knight[id].w; j++) {
							// 맵을 벗어난다.
							if (i <= 0 || i > L || j <= 0 || j > L) {
								dead = 1;
								break;
							}
							if (map[i][j] == 1) {
								knight[id].k--;
								knight[id].dmg++;
							}
							if (knight[id].k <= 0) {
								dead = 1;
								break;
							}
						}
						if (dead) break;
					}
				}
				//cout << knight[id].k << "\n";

				if (dead) {
					knight[id].r = 0;
					knight[id].c = 0;
					for (int i = y; i < y + knight[id].h; i++) {
						for (int j = x; j < x + knight[id].w; j++) {
							// 맵을 벗어난다.
							if (i <= 0 || i > L || j <= 0 || j > L) break;
							knight_map[i][j] = 0;
						}
					}
				}
				else if (d == 0) {
					int ny = y + dir[d][0];
					int py = y + knight[id].h - 1;
					knight[id].r += dir[d][0];
					for (int nx = x; nx < x + knight[id].w; nx++) {
						// 맵을 벗어난다.
						if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
						// 벽이 있다.
						if (map[ny][nx] == 2) continue;
						knight_map[ny][nx] = id;
						knight_map[py][nx] = 0;
					}
				}
				else if (d == 1) {
					int nx = x + knight[id].w - 1 + dir[d][1];
					int px = x;
					knight[id].c += dir[d][1];
					for (int ny = y; ny < y + knight[id].h; ny++) {
						// 맵을 벗어난다.
						if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
						// 벽이 있다.
						if (map[ny][nx] == 2) continue;
						knight_map[ny][nx] = id;
						knight_map[ny][px] = 0;
					}
				}
				else if (d == 2) {
					int ny = y + knight[id].h - 1 + dir[d][0];
					int py = y;
					knight[id].r += dir[d][0];
					for (int nx = x; nx < x + knight[id].w; nx++) {
						// 맵을 벗어난다.
						if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
						// 벽이 있다.
						if (map[ny][nx] == 2) continue;
						knight_map[ny][nx] = id;
						knight_map[py][nx] = 0;
					}
				}
				else if (d == 3) {
					int nx = x + dir[d][1];
					int px = x + knight[id].w - 1;
					knight[id].c += dir[d][1];
					for (int ny = y; ny < y + knight[id].h; ny++) {
						// 맵을 벗어난다.
						if (ny <= 0 || ny > L || nx <= 0 || nx > L) break;
						// 벽이 있다.
						if (map[ny][nx] == 2) continue;
						knight_map[ny][nx] = id;
						knight_map[ny][px] = 0;
					}
				}
			}

		}
		//for (int i = 1; i <= L; i++) {
		//	for (int j = 1; j <= L; j++) {
		//		cout << knight_map[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
	}

	int ans = 0;
	for (int i = 1; i <= N; i++) {
		if (knight[i].k <= 0) continue;
		ans += knight[i].dmg;
		//cout << knight[i].dmg << " ";
	}
	cout << ans << "\n";
	return 0;
}