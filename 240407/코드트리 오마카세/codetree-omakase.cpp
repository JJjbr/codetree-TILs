#include <iostream>
#include <unordered_map>
#include <string>
#include <queue>

using namespace std;

struct People {
	int x, n;
	string name;
};

unordered_map<int, unordered_map<string, int>> um;
People people[1502];

int main() {
	int L, Q;
	cin >> L >> Q;

	int opt, t, x, n, pre_t = 0;
	string name;
	int people_idx = 0, people_cnt = 0, dish_cnt = 0, rot = 0;
	for (int q = 0; q < Q; q++) {
		cin >> opt;
		if (opt == 100) {
			cin >> t >> x >> name;

			//rot = (t - 1) % L;
			rot = (t - 1) % L;
			x -= rot;
			if (x < 0) x = L + x;

			// 위치 x에 접시가 없을 때
			if (um.count(x) == 0) {
				// name 접시를 추가한다.
				unordered_map<string, int> tmp;
				tmp.insert({ name, 1 });
				um.insert({ x, tmp });
			}
			// 접시가 있을 때
			else {
				// name 접시가 없을 때
				if (um[x].count(name) == 0) {
					um[x].insert({ name, 1 }); // 접시를 1개 추가한다.
				}
				// 있을 때
				else {
					um[x][name]++; // 접시 수를 1 증가한다.
				}
			}
			dish_cnt++;

			//cout << x << " " << dish_cnt << "\n";

			// 초밥을 먹는다.
			for (int i = pre_t + 1; i <= t; i++) {
				rot = (i - 1) % L;
				for (int j = 0; j < people_idx; j++) {
					if (people[j].n == 0) continue;
					int x = people[j].x - rot;
					string name = people[j].name;
					if (um.count(x) != 0) {
						if (um[x].count(name) != 0) {
							if (um[x][name] >= people[j].n) {
								um[x][name] -= people[j].n;
								dish_cnt -= people[j].n;
								people[j].n = 0;
								people_cnt--;
							}
							else {
								people[j].n -= um[x][name];
								dish_cnt -= um[x][name];
							}
						}
					}
				}
			}
			pre_t = t;

			//cout << x << " " << rot << "\n";
		}
		else if (opt == 200) {
			cin >> t >> x >> name >> n;

			// 손님 입장
			people[people_idx] = { x, n, name };
			people_idx++;
			people_cnt++;

			// 초밥을 먹는다.
			for (int i = pre_t + 1; i <= t; i++) {
				rot = (i - 1) % L;
				for (int j = 0; j < people_idx; j++) {
					if (people[j].n == 0) continue;
					int x = people[j].x - rot;
					string name = people[j].name;
					if (um.count(x) != 0) {
						if (um[x].count(name) != 0) {
							if (um[x][name] >= people[j].n) {
								um[x][name] -= people[j].n;
								dish_cnt -= people[j].n;
								people[j].n = 0;
								people_cnt--;
							}
							else {
								people[j].n -= um[x][name];
								dish_cnt -= um[x][name];
							}
						}
					}
				}
			}
			//rot = (t - 1) % L;
			pre_t = t;

			//cout << people_cnt << " " << dish_cnt << "\n";
			//cout << x << " " << rot << "\n";
		}
		else if (opt == 300) {
			cin >> t;
			// 초밥을 먹는다.
			for (int i = pre_t + 1; i <= t; i++) {
				rot = (i - 1) % L;
				for (int j = 0; j < people_idx; j++) {
					if (people[j].n == 0) continue;
					int x = people[j].x - rot;
					string name = people[j].name;
					if (um.count(x) != 0) {
						if (um[x].count(name) != 0) {
							if (um[x][name] >= people[j].n) {
								um[x][name] -= people[j].n;
								dish_cnt -= people[j].n;
								people[j].n = 0;
								people_cnt--;
							}
							else {
								people[j].n -= um[x][name];
								dish_cnt -= um[x][name];
							}
						}
					}
				}
			}
			//rot = (t - 1) % L;
			pre_t = t;

			cout << people_cnt << " " << dish_cnt << "\n";
		}

		// 초밥을 먹는다.
		//for (int i = 0; i < people_idx; i++) {
		//	if (people[i].n == 0) continue;
		//	int x = people[i].x - rot;
		//	if (um.count(x) != 0) {
		//		if (um[x].count(name) != 0) {
		//			if (um[x][name] >= people[i].n) {
		//				um[x][name] -= people[i].n;
		//				dish_cnt -= people[i].n;
		//				people[i].n = 0;
		//				people_cnt--;
		//			}
		//			else {
		//				people[i].n -= um[x][name];
		//				dish_cnt -= um[x][name];
		//			}
		//		}
		//	}
		//}
	}
	return 0;
}