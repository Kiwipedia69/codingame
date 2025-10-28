#include <bits/stdc++.h>
using namespace std;

// -------- Portabilité popcount/ctz --------
#include <cstdint>
#if __has_include(<bit>) && (__cplusplus >= 202002L)
  #include <bit>
  template<class T> inline int POPCNT(T x){ return std::popcount((make_unsigned_t<T>)x); }
  template<class T> inline int CTZ(T x){ return std::countr_zero((make_unsigned_t<T>)x); }
#else
  template<class T> inline int POPCNT(T x){ return __builtin_popcount((uint32_t)x); }
  template<class T> inline int CTZ(T x){ return __builtin_ctz((uint32_t)x); }
#endif

using u32 = uint32_t;

static constexpr int n   = 5;
static constexpr int N   = n * n;   // 25
static constexpr int NxN = N * N;   // 625
// bits 1..N à 1 (bit 0 inutilisé)
static constexpr u32 MaskALL = (u32(1) << (N + 1)) - 2u; // 0b11...110 (26 bits) - 2

// Etat
static int  g[N][N] = {};
static u32  row[N] = {}, col[N] = {}, box_[N] = {};
static int  er[NxN] = {}, ec[NxN] = {};
static int  empties = 0;

static int  BOX_ID[N][N];

// Voisinage
static vector<int> NEIGH[N][N];

// Pile de propagation (undo)
static int  stR[NxN], stC[NxN];
static u32  stBit[NxN];
static int  top_ = 0;

// Weighted degree
static int wrow[N] = {}, wcol[N] = {}, wbox[N] = {};
static inline void bump(int r, int c){
    wrow[r]++; wcol[c]++; wbox[BOX_ID[r][c]]++;
}

static inline int charToInt(char c){
    if (c == '.') return 0;
    if (c >= 'A' && c <= 'Y') return c - 'A' + 1;
    //throw runtime_error(string("Caractère invalide: ") + c);
}
static inline char intToChar(int v){
    if (v == 0) return '.';
    if (1 <= v && v <= N) return char('A' + v - 1);
    //throw runtime_error("Valeur invalide");
}

static inline void swap_pos(int i, int j){
    if (i == j) return;
    std::swap(er[i], er[j]);
    std::swap(ec[i], ec[j]);
}

static inline void place(int r, int c, u32 bit){
    // bit != 0 garanti par l'appelant
    int d = CTZ(bit);              // 1..N
    g[r][c] = d;
    row[r] |= bit;
    col[c] |= bit;
    box_[BOX_ID[r][c]] |= bit;
    stR[top_] = r; stC[top_] = c; stBit[top_] = bit; ++top_;
}

static inline void unplace(int nundo){
    while (nundo-- > 0){
        --top_;
        int r = stR[top_], c = stC[top_];
        u32 bit = stBit[top_];
        g[r][c] = 0;
        row[r] ^= bit; col[c] ^= bit; box_[BOX_ID[r][c]] ^= bit;
    }
}

static void buildNeighbors(){
    bool isEmpty[N][N] = {};
    for (int i = 0; i < empties; ++i) isEmpty[er[i]][ec[i]] = true;

    for (int i = 0; i < empties; ++i){
        int r = er[i], c = ec[i];
        bool seen[N][N] = {};
        vector<int> list; list.reserve(3 * N);

        // Ligne
        for (int cc = 0; cc < N; ++cc){
            if (cc == c || !isEmpty[r][cc]) continue;
            if (!seen[r][cc]){ seen[r][cc] = true; list.push_back((r << 8) | cc); }
        }
        // Colonne
        for (int rr = 0; rr < N; ++rr){
            if (rr == r || !isEmpty[rr][c]) continue;
            if (!seen[rr][c]){ seen[rr][c] = true; list.push_back((rr << 8) | c); }
        }
        // Boîte
        int br = (r / n) * n, bc = (c / n) * n;
        for (int rr = br; rr < br + n; ++rr){
            for (int cc = bc; cc < bc + n; ++cc){
                if ((rr == r && cc == c) || !isEmpty[rr][cc]) continue;
                if (!seen[rr][cc]){ seen[rr][cc] = true; list.push_back((rr << 8) | cc); }
            }
        }
        NEIGH[r][c] = std::move(list);
    }
}

// Propagation (naked + hidden singles)
// return nb placements poussés (pour undo) ou -1 si contradiction
static int propagateSingles(int k){
    int pushed = 0;
    bool changed;

    do{
        changed = false;

        // Naked singles en cascade
        bool changedNaked;
        do{
            changedNaked = false;
            for (int i = k; i < empties; ++i){
                int r = er[i], c = ec[i];
                if (g[r][c] != 0) continue;
                int b = BOX_ID[r][c];
                u32 mask = (~(row[r] | col[c] | box_[b])) & MaskALL;
                if (mask == 0u){ bump(r,c); unplace(pushed); return -1; }
                // single-bit ?
                if ((mask & (0u - mask)) == mask){
                    place(r, c, mask);
                    ++pushed; changed = true; changedNaked = true;
                }
            }
        } while (changedNaked);

        // Hidden singles — lignes
        for (int r = 0; r < N; ++r){
            u32 once = 0, multi = 0;
            for (int i = k; i < empties; ++i){
                if ( er[i] != r ) continue;
                int c = ec[i];
                if (g[r][c] != 0) continue;
                u32 m = (~(row[r] | col[c] | box_[BOX_ID[r][c]])) & MaskALL;
                if (m == 0u){ bump(r,c); unplace(pushed); return -1; }
                multi |= (once & m);
                once  ^= m;
            }
            u32 unique = once & ~multi;
            if (unique){
                for (int i = k; i < empties; ++i){
                    if ( er[i] != r ) continue;
                    int c = ec[i];
                    if (g[r][c] != 0) continue;
                    u32 m = (~(row[r] | col[c] | box_[BOX_ID[r][c]])) & MaskALL;
                    u32 hit = m & unique;
                    if (hit){
                        if ( (hit & (hit - 1)) != 0u ){ bump(r,c); unplace(pushed); return -1; }
                        place(r, c, hit); ++pushed; changed = true;
                    }
                }
            }
        }

        // Hidden singles — colonnes
        for (int c = 0; c < N; ++c){
            u32 once = 0, multi = 0;
            for (int i = k; i < empties; ++i){
                if ( ec[i] != c ) continue;
                int r = er[i];
                if (g[r][c] != 0) continue;
                u32 m = (~(row[r] | col[c] | box_[BOX_ID[r][c]])) & MaskALL;
                if (m == 0u){ bump(r,c); unplace(pushed); return -1; }
                multi |= (once & m);
                once  ^= m;
            }
            u32 unique = once & ~multi;
            if (unique){
                for (int i = k; i < empties; ++i){
                    if ( ec[i] != c ) continue;
                    int r = er[i];
                    if (g[r][c] != 0) continue;
                    u32 m = (~(row[r] | col[c] | box_[BOX_ID[r][c]])) & MaskALL;
                    u32 hit = m & unique;
                    if (hit){
                        if ( (hit & (hit - 1)) != 0u ){ bump(r,c); unplace(pushed); return -1; }
                        place(r, c, hit); ++pushed; changed = true;
                    }
                }
            }
        }

        // Hidden singles — boîtes
        for (int b = 0; b < N; ++b){
            int br = (b / n) * n, bc = (b % n) * n;
            u32 once = 0, multi = 0;
            for (int i = k; i < empties; ++i){
                int r = er[i], c = ec[i];
                if (r < br || r >= br+n || c < bc || c >= bc+n) continue;
                if (g[r][c] != 0) continue;
                u32 m = (~(row[r] | col[c] | box_[b])) & MaskALL;
                if (m == 0u){ bump(r,c); unplace(pushed); return -1; }
                multi |= (once & m);
                once  ^= m;
            }
            u32 unique = once & ~multi;
            if (unique){
                for (int i = k; i < empties; ++i){
                    int r = er[i], c = ec[i];
                    if (r < br || r >= br+n || c < bc || c >= bc+n) continue;
                    if (g[r][c] != 0) continue;
                    u32 m = (~(row[r] | col[c] | box_[b])) & MaskALL;
                    u32 hit = m & unique;
                    if (hit){
                        if ( (hit & (hit - 1)) != 0u ){ bump(r,c); unplace(pushed); return -1; }
                        place(r, c, hit); ++pushed; changed = true;
                    }
                }
            }
        }

    } while (changed);

    return pushed;
}

static bool solve(int k){
    if (k == empties) return true;

    // Propagation
    int pushed = propagateSingles(k);
    if (pushed < 0) return false;
    while (k < empties && g[er[k]][ec[k]] != 0) ++k;
    if (k == empties) return true;

    // MRV + wdeg + degré
    int best = -1, minC = N + 1, bestW = -1, bestDeg = -1;
    u32 bestMask = 0;

    for (int i = k; i < empties; ++i){
        int r = er[i], c = ec[i];
        if (g[r][c] != 0) continue;
        int b = BOX_ID[r][c];
        u32 used = row[r] | col[c] | box_[b];
        u32 mask = (~used) & MaskALL;
        int cnt = POPCNT(mask);
        if (cnt == 0){ bump(r,c); unplace(pushed); return false; }

        if (cnt <= minC){
            int w = wrow[r] + wcol[c] + wbox[b];

            int deg = 0;
            const auto &neigh = NEIGH[r][c];
            for (int p : neigh){
                int rr = (p >> 8) & 0xFF, cc = p & 0xFF;
                if (g[rr][cc] == 0) ++deg;
            }

            if (cnt < minC || (cnt == minC && (w > bestW || (w == bestW && deg > bestDeg)))){
                minC = cnt; best = i; bestMask = mask; bestW = w; bestDeg = deg;
                if (cnt == 1) break;
            }
        }
    }

    swap_pos(k, best);
    int r = er[k], c = ec[k], b = BOX_ID[r][c];
    u32 mask = bestMask;

    // LCV (impact croissant)
    int nb = POPCNT(mask);
    u32 candBits[32]; int impact[32];
    int tnb = 0; u32 tmp = mask;
    while (tmp){ u32 bb = tmp & (0u - tmp); candBits[tnb++] = bb; tmp ^= bb; }

    const auto &neigh = NEIGH[r][c];
    const int L = (int)neigh.size();
    u32 neighAvail[128]; // L <= ~3N=75
    for (int u = 0; u < L; ++u){
        int p = neigh[u];
        int rr = (p >> 8) & 0xFF, cc = p & 0xFF;
        if (g[rr][cc] != 0){ neighAvail[u] = 0; continue; }
        int bb2 = BOX_ID[rr][cc];
        u32 used2 = row[rr] | col[cc] | box_[bb2];
        neighAvail[u] = (~used2) & MaskALL;
    }

    for (int t = 0; t < tnb; ++t){
        u32 bit = candBits[t]; int imp = 0;
        for (int u = 0; u < L; ++u)
            if (neighAvail[u] & bit) ++imp;
        impact[t] = imp;
    }
    for (int i = 1; i < tnb; ++i){
        u32 kb = candBits[i]; int ki = impact[i], j = i - 1;
        while (j >= 0 && impact[j] > ki){ candBits[j+1] = candBits[j]; impact[j+1] = impact[j]; --j; }
        candBits[j+1] = kb; impact[j+1] = ki;
    }

    // Branch
    for (int t = 0; t < tnb; ++t){
        u32 bit = candBits[t];
        int d = CTZ(bit);

        g[r][c] = d; row[r] |= bit; col[c] |= bit; box_[b] |= bit;
        if (solve(k + 1)) return true;
        g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box_[b] ^= bit;
    }

    bump(r, c);
    unplace(pushed);
    return false;
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    // Précompute BOX_ID
    for (int r = 0; r < N; ++r)
        for (int c = 0; c < N; ++c)
            BOX_ID[r][c] = (r / n) * n + (c / n);

    // Lecture
    string s;
    for (int i = 0; i < N; ++i){
        //if (!getline(cin, s)) throw runtime_error("Entrée insuffisante (ligne manquante)");
        //if ((int)s.size() < N) throw runtime_error("Ligne " + to_string(i) + " trop courte");
        for (int j = 0; j < N; ++j){
            char ch = s[j];
            int d = charToInt(ch);
            g[i][j] = d;
            if (d == 0){
                er[empties] = i; ec[empties] = j; ++empties;
            }else{
                u32 bit = (u32)1 << d;
                int b = BOX_ID[i][j];
                // if ( (row[i] & bit) || (col[j] & bit) || (box_[b] & bit) )
                //     throw runtime_error("Doublon initial en (" + to_string(i) + "," + to_string(j) + ") valeur " + to_string(d));
                row[i] |= bit; col[j] |= bit; box_[b] |= bit;
            }
        }
    }

    buildNeighbors();

    bool ok = solve(0);
    if (!ok) cerr << "— Aucune solution trouvée.\n";

    // Sortie
    for (int i = 0; i < N; ++i){
        for (int j = 0; j < N; ++j) cout << intToChar(g[i][j]);
        cout << '\n';
    }
    return 0;
}
