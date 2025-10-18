import java.io.*;
import java.util.ArrayList;

class Solution {
    static final int n = 5;
    static final int N = n * n;
    static final int NxN = N * N;
    // bits 1..N à 1 (bit 0 inutilisé) → généralisation (25 → 0x03FFFFFE)
    static final int MaskALL = (1 << (N + 1)) - 2;

    static int[][] g = new int[N][N];
    static int[] row = new int[N], col = new int[N], box = new int[N]; // bits 1..N
    static int[] er = new int[NxN], ec = new int[NxN];                 // positions vides
    static int empties = 0;

    static final int[][] BOX_ID = new int[N][N];
    static {
        for (int r = 0; r < N; r++)
            for (int c = 0; c < N; c++)
                BOX_ID[r][c] = (r / n) * n + (c / n);
    }

    // --- Voisinage pré-calculé par coordonnées (indépendant des swaps) ---
    // NEIGH[r][c] = liste des voisins encodés en (r<<8)|c
    static int[][][] NEIGH = new int[N][N][];

    // Pile pour placements faits pendant la propagation (undo si branche échoue)
    static int[] stR = new int[NxN], stC = new int[NxN], stBit = new int[NxN];
    static int top = 0;

    // --- wdeg léger ---
    static int[] wrow = new int[N], wcol = new int[N], wbox = new int[N];
    static void bump(int r, int c) {
        wrow[r]++; wcol[c]++; wbox[BOX_ID[r][c]]++;
    }

    static int charToInt(char c) {
        if (c == '.') return 0;
        if (c >= 'A' && c <= 'Y') return c - 'A' + 1;
        throw new IllegalArgumentException("Caractère invalide : " + c);
    }
    static char intToChar(int v) {
        if (v == 0) return '.';
        if (v >= 1 && v <= N) return (char) ('A' + v - 1);
        throw new IllegalArgumentException("Valeur invalide : " + v);
    }

    static void swap(int i, int j){
        if (i == j) return;
        int tr = er[i]; er[i] = er[j]; er[j] = tr;
        int tc = ec[i]; ec[i] = ec[j]; ec[j] = tc;
    }

    static void place(int r, int c, int bit) {
        int d = Integer.numberOfTrailingZeros(bit);
        g[r][c] = d; row[r] |= bit; col[c] |= bit; box[BOX_ID[r][c]] |= bit;
        stR[top] = r; stC[top] = c; stBit[top] = bit; top++;
    }
    static void unplace(int n) {
        while (n-- > 0) {
            --top;
            int r = stR[top], c = stC[top], bit = stBit[top];
            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[BOX_ID[r][c]] ^= bit;
        }
    }

    // ---- Build voisins: pour chaque case vide (r,c), lister les autres vides de sa ligne/col/boîte
    static void buildNeighbors() {
        boolean[][] isEmpty = new boolean[N][N];
        for (int i = 0; i < empties; i++) isEmpty[er[i]][ec[i]] = true;

        for (int i = 0; i < empties; i++) {
            int r = er[i], c = ec[i];
            boolean[][] seen = new boolean[N][N];
            ArrayList<Integer> list = new ArrayList<>(3 * N);

            // ligne
            for (int cc = 0; cc < N; cc++) {
                if (cc == c) continue;
                if (!isEmpty[r][cc]) continue;
                if (!seen[r][cc]) { seen[r][cc] = true; list.add((r << 8) | cc); }
            }
            // colonne
            for (int rr = 0; rr < N; rr++) {
                if (rr == r) continue;
                if (!isEmpty[rr][c]) continue;
                if (!seen[rr][c]) { seen[rr][c] = true; list.add((rr << 8) | c); }
            }
            // boîte n×n
            int br = (r / n) * n, bc = (c / n) * n;
            for (int rr = br; rr < br + n; rr++) {
                for (int cc = bc; cc < bc + n; cc++) {
                    if (rr == r && cc == c) continue;
                    if (!isEmpty[rr][cc]) continue;
                    if (!seen[rr][cc]) { seen[rr][cc] = true; list.add((rr << 8) | cc); }
                }
            }
            int[] arr = new int[list.size()];
            for (int t = 0; t < arr.length; t++) arr[t] = list.get(t);
            NEIGH[r][c] = arr; // stocké par coordonnées
        }
    }

    // ---- Propagation (naked singles + hidden singles bitwise) ----
    // Retourne nb de placements effectués (à annuler si la branche échoue), ou -1 si contradiction.
    static int propagateSingles(int k) {
        int pushed = 0;
        boolean changed;

        do {
            changed = false;

            // 1) Naked singles (cascade)
            boolean changedNaked;
            do {
                changedNaked = false;
                for (int i = k; i < empties; i++) {
                    int r = er[i], c = ec[i];
                    if (g[r][c] != 0) continue;
                    int b = BOX_ID[r][c];
                    int mask = (~(row[r] | col[c] | box[b])) & MaskALL;
                    if (mask == 0) { bump(r,c); unplace(pushed); return -1; }
                    if ((mask & -mask) == mask) {
                        place(r, c, mask);
                        pushed++; changed = true; changedNaked = true;
                    }
                }
            } while (changedNaked);

            // 2) Hidden singles — version bitwise (once/multi), collecte -> application

            // LIGNES
            for (int r = 0; r < N; r++) {
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++) {
                    if (er[i] != r) continue;
                    int c = ec[i];
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                    if (m == 0) { bump(r,c); unplace(pushed); return -1; }
                    multi |= (once & m);
                    once  ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0) {
                    for (int i = k; i < empties; i++) {
                        if (er[i] != r) continue;
                        int c = ec[i];
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0) {
                            if ((hit & (hit - 1)) != 0) { bump(r,c); unplace(pushed); return -1; }
                            place(r, c, hit); pushed++; changed = true;
                        }
                    }
                }
            }

            // COLONNES
            for (int c = 0; c < N; c++) {
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++) {
                    if (ec[i] != c) continue;
                    int r = er[i];
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                    if (m == 0) { bump(r,c); unplace(pushed); return -1; }
                    multi |= (once & m);
                    once  ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0) {
                    for (int i = k; i < empties; i++) {
                        if (ec[i] != c) continue;
                        int r = er[i];
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0) {
                            if ((hit & (hit - 1)) != 0) { bump(r,c); unplace(pushed); return -1; }
                            place(r, c, hit); pushed++; changed = true;
                        }
                    }
                }
            }

            // BOÎTES n×n
            for (int b = 0; b < N; b++) {
                int br = (b / n) * n, bc = (b % n) * n;
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++) {
                    int r = er[i], c = ec[i];
                    if (r < br || r >= br+n || c < bc || c >= bc+n) continue;
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[b])) & MaskALL;
                    if (m == 0) { bump(r,c); unplace(pushed); return -1; }
                    multi |= (once & m);
                    once  ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0) {
                    for (int i = k; i < empties; i++) {
                        int r = er[i], c = ec[i];
                        if (r < br || r >= br+n || c < bc || c >= bc+n) continue;
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[b])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0) {
                            if ((hit & (hit - 1)) != 0) { bump(r,c); unplace(pushed); return -1; }
                            place(r, c, hit); pushed++; changed = true;
                        }
                    }
                }
            }

        } while (changed);

        return pushed;
    }

    // ---- Solveur : MRV + wdeg tie-break + degré (via voisins) + LCV (cache used2/m2) + propagation ----
    static boolean solve(int k){
        if (k == empties) return true;

        // 0) Propagation (naked + hidden)
        int pushed = propagateSingles(k);
        if (pushed < 0) return false;
        while (k < empties && g[er[k]][ec[k]] != 0) k++;
        if (k == empties) return true;

        // 1) MRV + tie-break par wdeg (puis degré)
        int best = -1, bestMask = 0, min = N + 1;
        int bestW = -1, bestDeg = -1;

        for (int i = k; i < empties; i++) {
            int r = er[i], c = ec[i];
            if (g[r][c] != 0) continue;
            int b = BOX_ID[r][c];
            int used = row[r] | col[c] | box[b];
            int mask = (~used) & MaskALL;
            int cnt = Integer.bitCount(mask);
            if (cnt == 0) { bump(r,c); unplace(pushed); return false; }

            if (cnt <= min) {
                // wdeg score
                int w = wrow[r] + wcol[c] + wbox[b];

                // degré (voisins encore vides)
                int deg = 0;
                int[] neigh = NEIGH[r][c];
                for (int t = 0; t < neigh.length; t++) {
                    int p = neigh[t];
                    int rr = (p >>> 8) & 0xFF, cc = p & 0xFF;
                    if (g[rr][cc] == 0) deg++;
                }

                if (cnt < min
                    || (cnt == min && w > bestW)
                    || (cnt == min && w == bestW && deg > bestDeg)) {
                    min = cnt; best = i; bestMask = mask;
                    bestW = w; bestDeg = deg;
                    if (cnt == 1) break;
                }
            }
        }

        // 2) Amener la meilleure case en position k
        swap(k, best);
        int r = er[k], c = ec[k], b = BOX_ID[r][c];
        int mask = bestMask;

        // 3) LCV : trier les candidats par impact, avec cache used2/m2 pour chaque voisin
        int[] candBits = new int[min];
        int nb = 0, tmp = mask;
        while (tmp != 0) { int bb = tmp & -tmp; candBits[nb++] = bb; tmp ^= bb; }

        int[] neigh = NEIGH[r][c];
        int L = neigh.length;
        int[] neighAvail = new int[L]; // m2 pour chaque voisin
        for (int u = 0; u < L; u++) {
            int p = neigh[u];
            int rr = (p >>> 8) & 0xFF, cc = p & 0xFF;
            if (g[rr][cc] != 0) { neighAvail[u] = 0; continue; }
            int bb2 = BOX_ID[rr][cc];
            int used2 = row[rr] | col[cc] | box[bb2];
            neighAvail[u] = (~used2) & MaskALL; // m2
        }

        int[] impact = new int[nb];
        for (int t = 0; t < nb; t++) {
            int bit = candBits[t], imp = 0;
            for (int u = 0; u < L; u++) {
                if ((neighAvail[u] & bit) != 0) imp++;
            }
            impact[t] = imp;
        }
        // tri insertion par impact croissant
        for (int i = 1; i < nb; i++) {
            int kb = candBits[i], ki = impact[i], j = i - 1;
            while (j >= 0 && impact[j] > ki) {
                candBits[j+1] = candBits[j]; impact[j+1] = impact[j]; j--;
            }
            candBits[j+1] = kb; impact[j+1] = ki;
        }

        // 4) Brancher selon l’ordre LCV
        boolean anySuccess = false;
        for (int t = 0; t < nb; t++) {
            int bit = candBits[t];
            int d = Integer.numberOfTrailingZeros(bit);

            g[r][c] = d; row[r] |= bit; col[c] |= bit; box[b] |= bit;

            if (solve(k + 1)) { anySuccess = true; break; }

            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[b] ^= bit;
        }

        if (anySuccess) return true;

        // aucune valeur n’a marché pour (r,c) → bump son wdeg (ligne/col/boîte)
        bump(r, c);
        unplace(pushed);
        return false;
    }

    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));

        //System.err.print("MaskALL=0x" + Integer.toHexString(MaskALL).toUpperCase());
        // Lecture + initialisation des masques + liste des vides
        for (int i = 0; i < N; i++) {
            String s = br.readLine();
            if (s == null || s.length() < N)
                throw new IllegalArgumentException("Ligne "+i+" invalide (longueur < N)");
            for (int j = 0; j < N; j++) {
                int d = charToInt(s.charAt(j));
                g[i][j] = d;
                if (d == 0) {
                    er[empties] = i; ec[empties++] = j;
                } else {
                    int bit = 1 << d;
                    int b = BOX_ID[i][j];
                    if ((row[i] & bit)!=0 || (col[j] & bit)!=0 || (box[b] & bit)!=0)
                        throw new IllegalArgumentException("Doublon initial en ("+i+","+j+") valeur "+d);
                    row[i] |= bit; col[j] |= bit; box[b] |= bit;
                }
            }
        }

        // Pré-calcul du voisinage
        buildNeighbors();

        boolean ok = solve(0);
        if (!ok) System.err.println(" — Aucune solution trouvée.");

        StringBuilder out = new StringBuilder();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) out.append(intToChar(g[i][j]));
            out.append('\n');
        }
        System.out.print(out);
    }
}
