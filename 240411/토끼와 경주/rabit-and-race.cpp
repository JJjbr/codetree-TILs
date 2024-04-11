#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cstring>
using namespace std;

struct Rabbit {
	int y, x, pid, d;
};

struct Race {
	int jump, sum, row, col, pid, i;
	bool operator<(Race right) const {
		if (jump == right.jump) {
			if (sum == right.sum) {
				if (row == right.row) {
					if (col == right.col) {
						return pid > right.pid;
					}
					return col > right.col;
				}
				return row > right.row;
			}
			return sum > right.sum;
		}
		return jump > right.jump;
	}
};

struct Move {
	int sum, row, col, pid;
	bool operator<(Move right) const {
		if (sum == right.sum) {
			if (row == right.row) {
				if (col == right.col) {
					return pid < right.pid;
				}
				return col < right.col;
			}
			return row < right.row;
		}
		return sum < right.sum;
	}
};

int Q, N, M, P;
Rabbit rabbit[2002];
priority_queue<Race> pq_rabbit;
Move s_rabbit[2002];
long long rabbit_score[2002];
int dy[4] = { -1, 0, 1, 0 };
int dx[4] = { 0, 1, 0, -1 };

void raceRabbit() {
	Race now = pq_rabbit.top();
	pq_rabbit.pop();

	priority_queue<Move> pq;
	for (int i = 0; i < 4; i++) {
		int ny = now.row + dy[i] * rabbit[now.i].d;
		int nx = now.col + dx[i] * rabbit[now.i].d;
		while (ny <= 0 || ny > N || nx <= 0 || nx > M) {
			if (ny <= 0) {
				ny = 1 - ny;
				ny = 1 + ny;
			}
			if (ny > N) {
				ny = N - ny;
				ny = N + ny;
			}
			if (nx <= 0) {
				nx = 1 - nx;
				nx = 1 + nx;
			}
			if (nx > M) {
				nx = M - nx;
				nx = M + nx;
			}
		}
		pq.push({ ny + nx, ny, nx, 0 });
	}
	Move move = pq.top();
	pq_rabbit.push({ now.jump + 1, move.sum, move.row, move.col, now.pid, now.i });
	rabbit[now.i].y = move.row;
	rabbit[now.i].x = move.col;

	for (int i = 1; i <= P; i++) {
		if (i == now.i) continue;
		rabbit_score[i] += move.sum;
	}

	s_rabbit[now.i] = { move.sum, move.row, move.col, now.pid };
	//cout << "ok " << now.i << " " << s_rabbit[now.i].pid << "\n";
}

int main() {
	ios::sync_with_stdio(false); cin.tie(NULL); cout.tie(NULL);
	cin >> Q;

	int cmd, pid, d;
	cin >> cmd >> N >> M >> P;
	for (int i = 1; i <= P; i++) {
		cin >> pid >> d;
		rabbit[i] = { 1, 1, pid, d };
		pq_rabbit.push({ 0, 2, 1, 1, pid, i });
	}
	
	for (int q = 1; q < Q; q++) {
		cin >> cmd;
		if (cmd == 200) {
			int K, S;
			cin >> K >> S;
			memset(s_rabbit, 0, sizeof(s_rabbit));
			for (int i = 0; i < K; i++) {
				//cout << "TEST " << i << "\n";
				raceRabbit();
				//for (int i = 1; i <= P; i++) {
				//	cout << rabbit[i].y << " " << rabbit[i].x << " " << rabbit[i].pid << " " << rabbit[i].d << "\n";
				//	cout << rabbit_score[i] << "\n";
				//}
			}
			sort(s_rabbit, s_rabbit + P + 1);
			for (int i = 1; i <= P; i++) {
				if (rabbit[i].pid == s_rabbit[P].pid) {
					rabbit_score[i] += S;
					break;
				}
			}
		}
		else if (cmd == 300) {
			int pid_t, L;
			cin >> pid_t >> L;
			for (int i = 1; i <= P; i++) {
				if (rabbit[i].pid == pid_t) {
					rabbit[i].d *= L;
					break;
				}
			}
		}
		//for (int i = 1; i <= P; i++) {
		//	cout << rabbit[i].y << " " << rabbit[i].x << " " << rabbit[i].pid << " " << rabbit[i].d << "\n";
		//	cout << rabbit_score[i] << "\n";
		//}
	}
	sort(rabbit_score, rabbit_score + P + 1, greater<long long>());
	cout << rabbit_score[0] << "\n";
	return 0;
}