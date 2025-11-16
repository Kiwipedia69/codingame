#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <cctype>

using namespace std;

int main() {
    int h, w;
    cin >> h; cin.ignore();
    cin >> w; cin.ignore();

    // rayon lumière
    vector<int> ray(h * w, 0);
    // luminosité
    vector<int> bright(h * w, 0);

    // char to int
    for (int y = 0; y < h; ++y) {
        string s;
        getline(cin, s);
        for (int x = 0; x < w; ++x) {
            char c = s[x];
            int r = 0;
            if (c == '.') {
                r = 0;
            } else if (isdigit(c)) {
                r = c - '0';               
            } else if (isupper(c)) {
                r = 10 + (c - 'A');
            }
            ray[y * w + x] = r;
        }
    }

    // propagatrion lumière
    for (int sy = 0; sy < h; ++sy) {
        for (int sx = 0; sx < w; ++sx) {
            int R = ray[sy * w + sx];
            if (R == 0) continue;

            for (int ty = 0; ty < h; ++ty) {
                for (int tx = 0; tx < w; ++tx) {
                    int dx = tx - sx;
                    int dy = ty - sy;
                    double dist = sqrt(dx * dx + dy * dy);
                    int d = (int)std::round(dist);

                    int b = R - d;
                    if (b > 0) {
                        bright[ty * w + tx] += b;
                    }
                }
            }
        }
    }

    // int to char pour tab final
    for (int y = 0; y < h; ++y) {
        for (int x = 0; x < w; ++x) {
            int v = bright[y * w + x];
            if (v < 0) v = 0;
            if (v > 35) v = 35; // max = Z

            char out;
            if (v <= 9)
                out = '0' + v;
            else
                out = 'A' + (v - 10);

            cout << out;
        }
        cout << '\n';
    }
}
