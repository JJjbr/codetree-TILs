#include <iostream>
#include <algorithm>
#include <cstring>
#include <cmath>

using namespace std;

struct Info {
	//int santa, dist, y, x;
	int dist, y, x;
};

//bool cmp(Info left, Info right) {
//	return left.santa < right.santa;
//}

int map[52][52];
Info info[32];
int santa_state[32]; // 탈락 -1, 정상 0, 기절 1
int santa_score[32];
int dir_santa[4][2] = { {-1, 0}, {0, 1}, {1, 0}, {0, -1} };
int dir_rudolf[8][2] = { {-1, -1}, {-1, 0}, {-1, 1}, {0, -1}, {0, 1}, {1, -1}, {1, 0}, {1, 1} };

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	memset(map, -1, sizeof(map));
	int N, M, P, C, D;
	cin >> N >> M >> P >> C >> D;
	int ry, rx, si, sy, sx;
	cin >> ry >> rx;
	map[ry][rx] = 0;
	int close_santa = 0;
	info[0].dist = 1e6;
	for (int i = 1; i <= P; i++) {
		cin >> si >> sy >> sx;
		map[sy][sx] = si;
		int dist = pow(ry - sy, 2) + pow(rx - sx, 2);
		info[si] = { dist, sy, sx };
		if (dist < info[close_santa].dist) {
			close_santa = si;
			//close_santa.y = sy;
			//close_santa.x = sx;
			//close_santa.dist = dist;
		}
		else if (dist == info[close_santa].dist) {
			if (info[close_santa].y < sy) {
				close_santa = si;
				//close_santa.y = sy;
				//close_santa.x = sx;
			}
			else if (info[close_santa].y == sy) {
				if (info[close_santa].x < sx) {
					close_santa = si;
					//close_santa.y = sy;
					//close_santa.x = sx;
				}
			}
		}
	}
	//sort(info, info + P, cmp);
	//for (int i = 1; i <= P; i++) {
	//	cout << info[i].santa << " " << info[i].dist << " " << info[i].y << " " << info[i].x << "\n";
	//}

	//for (int i = 1; i <= N; i++) {
	//	for (int j = 1; j <= N; j++) {
	//		cout << map[i][j] << " ";
	//	}
	//	cout << "\n";
	//}

	for (int m = 0; m < M; m++) {
		//cout << "Turn " << m + 1 << "\n";
		//memset(close_santa, 0, sizeof(close_santa));
		// 루돌프의 움직임
		map[ry][rx] = -1;
		int dist_r2s = info[close_santa].dist, new_ry = ry, new_rx = rx;
		//cout << close_santa << " " << info[close_santa].dist << "\n";
		for (int i = 0; i < 8; i++) {
			int ny = ry + dir_rudolf[i][0];
			int nx = rx + dir_rudolf[i][1];
			if (ny <= 0 || ny > N || nx <= 0 || nx > N) continue;
			int dist = pow(ny - info[close_santa].y, 2) + pow(nx - info[close_santa].x, 2);
			if (dist == 0) { // 충돌
				map[info[close_santa].y][info[close_santa].x] = -1;
				santa_score[close_santa] += C;
				//cout << "coll before " << info[close_santa].y << " " << info[close_santa].x << "\n";
				info[close_santa].y += (dir_rudolf[i][0] * C);
				info[close_santa].x += (dir_rudolf[i][1] * C);
				//cout << "coll after " << info[close_santa].y << " " << info[close_santa].x << "\n";
				if (info[close_santa].y <= 0 || info[close_santa].y > N || info[close_santa].x <= 0 || info[close_santa].x > N) {
					santa_state[close_santa] = -1; // 탈락
				}
				else {
					int save_santa = map[info[close_santa].y][info[close_santa].x];
					santa_state[close_santa] = 2; // 기절
					map[info[close_santa].y][info[close_santa].x] = close_santa;
					//info[close_santa.santa].y = close_santa.y;
					//info[close_santa.santa].x = close_santa.x;

					while (save_santa != close_santa && save_santa > 0) {
						info[save_santa].y += dir_rudolf[i][0];
						info[save_santa].x += dir_rudolf[i][1];
						if (info[save_santa].y <= 0 || info[save_santa].y > N || info[save_santa].x <= 0 || info[save_santa].x > N) {
							santa_state[save_santa] = -1; // 탈락
							break;
						}
						close_santa = save_santa;
						save_santa = map[info[close_santa].y][info[close_santa].x];
						map[info[close_santa].y][info[close_santa].x] = close_santa;
					}
				}
				new_ry = ny;
				new_rx = nx;
				break;
			}
			if (dist < dist_r2s) {
				dist_r2s = dist;
				new_ry = ny;
				new_rx = nx;
			}
			//cout << i << " " << new_ry << " " << new_rx << "\n";
		}
		ry = new_ry;
		rx = new_rx;
		map[ry][rx] = 0;
		//cout << ry << " " << rx << "\n";

		for (int i = 1; i <= P; i++) {
			if (santa_state[i] < 0) continue;
			int dist = pow(ry - info[i].y, 2) + pow(rx - info[i].x, 2);
			info[i].dist = dist;
			//cout << i << " " << info[i].dist << "\n";
		}

		// 산타의 움직임
		int dist_s2r = 0, new_sy = 0, new_sx = 0;
		for (int p = 1; p <= P; p++) {
			if (santa_state[p] != 0) continue; // 탈락, 기절
			//dist_s2r = info[p].dist;
			map[info[p].y][info[p].x] = -1;
			new_sy = info[p].y;
			new_sx = info[p].x;
			//cout << "ok " << p << " " << new_sy << " " << new_sx << "\n";
			bool coll = false;
			for (int i = 0; i < 4; i++) {
				int ny = info[p].y + dir_santa[i][0];
				int nx = info[p].x + dir_santa[i][1];
				if (ny <= 0 || ny > N || nx <= 0 || nx > N) continue;
				if (map[ny][nx] > 0) continue;
				if (ny == ry && nx == rx) { // 충돌
					coll = true;
					//map[info[p].y][info[p].x] = -1;
					santa_score[p] += D;
					info[p].y = ny - (dir_santa[i][0] * D);
					info[p].x = nx - (dir_santa[i][1] * D);
					if (info[p].y <= 0 || info[p].y > N || info[p].x <= 0 || info[p].x > N) {
						santa_state[p] = -1; // 탈락
						break;
					}

					santa_state[p] = 2; // 기절
					//cout << p << "\n";
					int tmp_p = p;
					int si = map[info[tmp_p].y][info[tmp_p].x];
					map[info[tmp_p].y][info[tmp_p].x] = tmp_p;
					while (si != tmp_p && si > 0) {
						info[si].y -= dir_santa[i][0];
						info[si].x -= dir_santa[i][1];
						if (info[si].y <= 0 || info[si].y > N || info[si].x <= 0 || info[si].x > N) {
							santa_state[si] = -1; // 탈락
							break;
						}
						tmp_p = si;
						si = map[info[tmp_p].y][info[tmp_p].x];
						map[info[tmp_p].y][info[tmp_p].x] = tmp_p;
					}
					break;
				}

				int dist = pow(ry - ny, 2) + pow(rx - nx, 2);
				//cout << p << " " << i << " " << dist << " " << info[p].dist << "\n";
				if (dist < info[p].dist) {
					info[p].dist = dist;
					new_sy = ny;
					new_sx = nx;
					//break;
				}
			}
			if (!coll) {
				info[p].y = new_sy;
				info[p].x = new_sx;
				map[info[p].y][info[p].x] = p;
			}
			//cout << p << " " << info[p].y << " " << info[p].x << "\n";
		}

		int cnt = 0;
		for (int i = 1; i <= P; i++) {
			if (santa_state[i] >= 0) {
				santa_score[i]++;
			}
			else {
				cnt++;
			}
		}

		//cout << "print map\n";
		//for (int i = 1; i <= N; i++) {
		//	for (int j = 1; j <= N; j++) {
		//		cout << map[i][j] << " ";
		//	}
		//	cout << "\n";
		//}
		//cout << "print scores\n";
		//for (int i = 1; i <= P; i++) {
		//	cout << santa_score[i] << " ";
		//}
		//cout << "\n";

		if (cnt >= P) break;

		close_santa = 0;
		//close_santa.dist = 1e6;
		for (int i = 1; i <= P; i++) {
			if (santa_state[i] > 0) santa_state[i]--;
			if (santa_state[i] < 0) continue;
			int sy = info[i].y;
			int sx = info[i].x;
			int dist = pow(ry - sy, 2) + pow(rx - sx, 2);
			info[i].dist = dist;

			//cout << i << " " << sy << " " << sx << " " << dist << "\n";

			if (dist < info[close_santa].dist) {
				close_santa = i;
			}
			else if (dist == info[close_santa].dist) {
				if (info[close_santa].y < sy) {
					close_santa = i;
				}
				else if (info[close_santa].y == sy) {
					if (info[close_santa].x < sx) {
						close_santa = i;
					}
				}
			}
		}
	}

	for (int i = 1; i <= P; i++) {
		cout << santa_score[i] << " ";
	}
	cout << "\n";

	return 0;
}