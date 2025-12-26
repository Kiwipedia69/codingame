#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int w, h;
    if (!(cin >> w >> h)) return 0;

    vector<string> grid(h);
    for (int y = 0; y < h; ++y) {
        cin >> grid[y];
    }

    int sx = -1, sy = -1;
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            if (grid[y][x] == 'S') {
                sx = x;
                sy = y;
            }
        }
    }

    const int INF = -1;
    vector<vector<int>> dist(h, vector<int>(w, INF));
    queue<pair<int,int>> q;

    dist[sy][sx] = 0;
    q.push({sx, sy});

    int dx[4] = {1, -1, 0, 0};
    int dy[4] = {0, 0, 1, -1};

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        int d = dist[y][x];

        for (int dir = 0; dir < 4; ++dir) {
            int nx = (x + dx[dir] + w) % w; // wrap horizontal
            int ny = (y + dy[dir] + h) % h; // wrap vertical

            if (grid[ny][nx] == '#') continue;      // mur
            if (dist[ny][nx] != INF) continue;      // déjà visité

            dist[ny][nx] = d + 1;
            q.push({nx, ny});
        }
    }

    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            char c = grid[y][x];
            if (c == '#') {
                cout << '#';
            } else {
                int d = dist[y][x];
                if (d == INF) {
                    cout << '.';
                } else {
                    if (d < 10)
                        cout << char('0' + d);
                    else
                        cout << char('A' + (d - 10));
                }
            }
        }
        cout << '\n';
    }

    return 0;
}
