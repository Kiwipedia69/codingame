#include <iostream>
#include <string>
#include <vector>
#include <cctype>
using namespace std;

void drop(char c, vector<char>& box, int w, int h, int startX) {
    auto at = [&](int x, int y) -> char& {
        return box[y * w + x];
    };

    int x = startX;
    int y = -1;

    cerr<<c<<" "<<startX<<endl;

    while (true) {
        // drop direct
        if (y + 1 < h && at(x, y + 1) == ' ') {
            ++y;
            continue;
        }

        
        int dir1 = std::islower(c) ? 1 : -1;
        int dir2 = -dir1;

        bool moved = false;
        for (int step = 0; step < 2 && !moved; ++step) {
            int dx = (step == 0) ? dir1 : dir2;
            int nx = x + dx;

            if (nx < 0 || nx >= w) continue;
            if (y + 1 < h && at(nx, y + 1) == ' ') {
                x = nx;
                ++y;
                moved = true;
            }
        }

        if (!moved) break; // pos final
    }

    at(x, y) = c;
}

int main() {
    int w, h;
    cin >> w >> h;
    cin.ignore();

    int n;
    cin >> n;
    cin.ignore();

    vector<char> g(w * h, ' ');

    for (int i = 0; i < n; ++i) {
        string s;
        int p;
        cin >> s >> p;
        cin.ignore();
        char c = s[0];
        drop(c, g, w, h, p);
        
        // test error
        for (int y = 0; y < h; ++y) {
            cerr<<'|';
            for (int x = 0; x < w; ++x)
                cerr << g[y * w + x];
            cerr << "|\n";
        }
        cerr<<'+';
        for(auto i=0;i<w;i++) {cerr<<'-';}
        cerr<<"+\n";
    }

    for (int y = 0; y < h; ++y) {
        cout<<'|';
        for (int x = 0; x < w; ++x)
            cout << g[y * w + x];
        cout << "|\n";
    }
    cout<<'+';
    for(auto i=0;i<w;i++) {cout<<'-';}
    cout<<"+\n";
}
