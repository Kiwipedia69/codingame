#include <bits/stdc++.h>
using namespace std;

using u64 = unsigned long long;

int rankId(char r) {
    if (r >= '2' && r <= '9') return r - '2';
    if (r == 'T') return 8;
    if (r == 'J') return 9;
    if (r == 'Q') return 10;
    if (r == 'K') return 11;
    return 12; // 'A'
}

int suitId(char s) {
    if (s == 'C') return 0;
    if (s == 'D') return 1;
    if (s == 'H') return 2;
    return 3; // 'S'
}

// bitmask cause its fun
// cards value in bit is suit*4 + rank
u64 buildMask(const string& s) {
    bool rankSelect[13] = {false};
    bool suitSelect[4]  = {false};
    bool hasRank = false, hasSuit = false;

    for (char c : s) {
        if (c == 'C' || c == 'D' || c == 'H' || c == 'S') {
            suitSelect[suitId(c)] = true;
            hasSuit = true;
        } else {
            rankSelect[rankId(c)] = true;
            hasRank = true;
        }
    }

    u64 mask = 0;
    for (int r = 0; r < 13; ++r) {
        if (hasRank && !rankSelect[r]) continue;
        for (int su = 0; su < 4; ++su) {
            if (hasSuit && !suitSelect[su]) continue;
            int idx = r * 4 + su;          // 0..51
            mask |= (1ULL << idx);
        }
    }
    return mask;
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int R, S;
    if (!(cin >> R >> S)) return 0;

    u64 deck = (52ULL == 64 ? ~0ULL : ((1ULL << 52) - 1)); // 52 bits à 1

    // removed
    for (int i = 0; i < R; ++i) {
        string s;
        cin >> s;
        u64 m = buildMask(s);
        deck &= ~m;
    }

    // sought
    u64 soughtMask = 0;
    for (int i = 0; i < S; ++i) {
        string s;
        cin >> s;
        soughtMask |= buildMask(s);
    }

    u64 remainingSought = deck & soughtMask;

    int remaining = __builtin_popcountll(deck); // count 1
    int hits      = __builtin_popcountll(remainingSought);

    int proba = int( (double)hits * 100.0 / (double)remaining + 0.5 );

    cout << proba << "%\n";
    return 0;
}
