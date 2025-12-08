#include <bits/stdc++.h>
using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n;
    if (!(cin >> n)) return 0;

    vector<string> grid(n);
    for (int i = 0; i < n; ++i) {
        cin >> grid[i];
    }
    int m = n; // grille n*m 

    // keep position where letter a + 
    vector<pair<int,int>> positions[26];
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            char ch = grid[i][j];
            if ('a' <= ch && ch <= 'z')
                positions[ch - 'a'].push_back({i, j});
        }
    }

    const int di[4] = {-1, 1, 0, 0};
    const int dj[4] = {0, 0, -1, 1};

    // propag a to z
    vector<vector<bool>> prefix(n, vector<bool>(m, false));

    for (auto [i, j] : positions[0]) { // a
        bool ok = false;
        for (int k = 0; k < 4; ++k) {
            int ni = i + di[k], nj = j + dj[k];
            if (0 <= ni && ni < n && 0 <= nj && nj < m
                && grid[ni][nj] == 'b') {
                ok = true;
                break;
            }
        }
        if (ok) prefix[i][j] = true;
    }

    // propag l to l+1
    for (int c = 0; c < 25; ++c) {
        char cur = 'a' + c;
        char nxt = cur + 1;
        for (auto [i, j] : positions[c]) {
            if (!prefix[i][j]) continue;
            for (int k = 0; k < 4; ++k) {
                int ni = i + di[k], nj = j + dj[k];
                if (0 <= ni && ni < n && 0 <= nj && nj < m
                    && grid[ni][nj] == nxt) {
                    prefix[ni][nj] = true;
                }
            }
        }
    }

    // propag z to a
    vector<vector<bool>> sufix(n, vector<bool>(m, false));

    for (auto [i, j] : positions[25]) { // z
        sufix[i][j] = true;
    }

    // propzg l tp l-1
    for (int c = 25; c > 0; --c) {
        char cur = 'a' + c;
        char prev = cur - 1;
        for (auto [i, j] : positions[c]) {
            if (!sufix[i][j]) continue;
            for (int k = 0; k < 4; ++k) {
                int ni = i + di[k], nj = j + dj[k];
                if (0 <= ni && ni < n && 0 <= nj && nj < m
                    && grid[ni][nj] == prev) {
                    sufix[ni][nj] = true;
                }
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < m; ++j) {
            if (prefix[i][j] && sufix[i][j])
                cout << grid[i][j];
            else
                cout << '-';
        }
        cout << '\n';
    }

    return 0;
}
