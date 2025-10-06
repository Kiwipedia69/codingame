import java.io.*;

class Solution {
    static int N = 25;
    static int NxN = 625;
    static int MaskALL = 0x03FFFFFE;

    static int[][] g = new int[N][N];
    static int[] row = new int[N], col = new int[N], box = new int[N]; // bits 1..25
    static int[] er = new int[NxN], ec = new int[NxN]; // positions vides
    static int empties = 0;

    static final int[][] BOX_ID = new int[N][N];
    static {
        for (int r=0;r<N;r++) for (int c=0;c<N;c++) BOX_ID[r][c] = (r/5)*5 + (c/5);
    }

    // Pile pour placements en propagation (pour undo)
    static int[] stR = new int[NxN], stC = new int[NxN], stBit = new int[NxN];
    static int top = 0;

    static int boxId(int r, int c){ return (r/5)*5 + (c/5); }

    static int charToInt(char c) {
        if (c == '.') return 0;
        if (c >= 'A' && c <= 'Y') return c - 'A' + 1;
        throw new IllegalArgumentException("Caractère invalide : " + c);
    }
    static char intToChar(int c) {
        if (c == 0) return '.';
        if (c >= 1 && c <= N) return (char) ('A' + c - 1) ;
        throw new IllegalArgumentException("Caractère invalide : " + c);
    }

    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        for (int i = 0; i < N; i++) {
            String s = br.readLine();                 // pas de trim(): on lit exactement 25 chars
            if (s == null || s.length() < N) throw new IllegalArgumentException("Ligne "+i+" invalide");
            for (int j = 0; j < N; j++) {
                int d = charToInt(s.charAt(j));
                g[i][j] = d;
                if (d == 0) { er[empties] = i; ec[empties++] = j; }
                else {
                    int bit = 1 << d;
                    int b = BOX_ID[i][j];
                    if ((row[i] & bit)!=0 || (col[j] & bit)!=0 || (box[b] & bit)!=0)
                        throw new IllegalArgumentException("Doublon initial en ("+i+","+j+") valeur "+d);
                    row[i] |= bit; col[j] |= bit; box[b] |= bit;
                }
            }
        }

        StringBuilder outErr = new StringBuilder();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) outErr.append(intToChar(g[i][j]));
            outErr.append('\n');
        }
        System.err.print(outErr);

        boolean ok = solve(0);

        if (!ok) System.err.println("Aucune solution trouvée.");

        StringBuilder out = new StringBuilder();
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) out.append(intToChar(g[i][j]));
            out.append('\n');
        }
        System.out.print(out);
    }

    // ---- Propagation (naked + hidden singles) ----

    static void place(int r, int c, int bit){
        int d = Integer.numberOfTrailingZeros(bit);
        g[r][c] = d; row[r] |= bit; col[c] |= bit; box[BOX_ID[r][c]] |= bit;
        stR[top] = r; stC[top] = c; stBit[top] = bit; top++;
    }
    static void unplace(int n){
        while (n-- > 0) {
            --top;
            int r = stR[top], c = stC[top], bit = stBit[top];
            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[BOX_ID[r][c]] ^= bit;
        }
    }

    // Retourne nb de placements effectués, ou -1 si contradiction
    static int propagateSingles(int k){
        int pushed = 0;
        boolean changed;
        do {
            changed = false;

            // 1) Naked singles
            for (int i = k; i < empties; i++){
                int r = er[i], c = ec[i];
                if (g[r][c] != 0) continue;
                int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                if (m == 0) { unplace(pushed); return -1; }
                if ((m & -m) == m) { // un seul bit
                    place(r, c, m);
                    changed = true; pushed++;
                }
            }

            // 2) Hidden singles - lignes
            for (int r = 0; r < N; r++){
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++){
                    if (er[i] != r) continue;
                    int c = ec[i];
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                    multi |= once & m; once ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0){
                    for (int i = k; i < empties; i++){
                        if (er[i] != r) continue;
                        int c = ec[i];
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0){
                            if ((hit & (hit - 1)) != 0) { unplace(pushed); return -1; } // 2 uniques => incohérence
                            place(r, c, hit);
                            changed = true; pushed++;
                        }
                    }
                }
            }

            // 3) Hidden singles - colonnes
            for (int c = 0; c < N; c++){
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++){
                    if (ec[i] != c) continue;
                    int r = er[i];
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                    multi |= once & m; once ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0){
                    for (int i = k; i < empties; i++){
                        if (ec[i] != c) continue;
                        int r = er[i];
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[BOX_ID[r][c]])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0){
                            if ((hit & (hit - 1)) != 0) { unplace(pushed); return -1; }
                            place(r, c, hit);
                            changed = true; pushed++;
                        }
                    }
                }
            }

            // 4) Hidden singles - boîtes 5x5
            for (int b = 0; b < N; b++){
                int br = (b / 5) * 5, bc = (b % 5) * 5;
                int once = 0, multi = 0;
                for (int i = k; i < empties; i++){
                    int r = er[i], c = ec[i];
                    if (r < br || r >= br+5 || c < bc || c >= bc+5) continue;
                    if (g[r][c] != 0) continue;
                    int m = (~(row[r] | col[c] | box[b])) & MaskALL;
                    multi |= once & m; once ^= m;
                }
                int unique = once & ~multi;
                if (unique != 0){
                    for (int i = k; i < empties; i++){
                        int r = er[i], c = ec[i];
                        if (r < br || r >= br+5 || c < bc || c >= bc+5) continue;
                        if (g[r][c] != 0) continue;
                        int m = (~(row[r] | col[c] | box[b])) & MaskALL;
                        int hit = m & unique;
                        if (hit != 0){
                            if ((hit & (hit - 1)) != 0) { unplace(pushed); return -1; }
                            place(r, c, hit);
                            changed = true; pushed++;
                        }
                    }
                }
            }
        } while (changed);

        return pushed;
    }

    // ---- Solveur : MRV + tie-break degré + propagation cascade ----

    static boolean solve(int k){
        if (k == empties) return true;

        // Propagation en cascade avant de brancher
        int pushed = propagateSingles(k);
        if (pushed < 0) return false;  // contradiction
        // avancer k au-delà des cases remplies par propagation
        while (k < empties && g[er[k]][ec[k]] != 0) k++;
        if (k == empties) return true;

        // MRV + tie-break par degré
        int best = -1, bestMask = 0, min = N + 1, bestDeg = -1;
        for (int i = k; i < empties; i++) {
            int r = er[i], c = ec[i];
            if (g[r][c] != 0) continue; // déjà fixé par propagation
            int b = BOX_ID[r][c];
            int mask = (~(row[r] | col[c] | box[b])) & MaskALL;
            int cnt = Integer.bitCount(mask);
            if (cnt == 0) { unplace(pushed); return false; }
            if (cnt < min) {
                min = cnt; best = i; bestMask = mask;
                if (cnt == 1) { bestDeg = 0; break; }
                int deg = 0;
                for (int j = k; j < empties; j++) {
                    int rr = er[j], cc = ec[j];
                    if (g[rr][cc] != 0) continue;
                    if (rr == r || cc == c || BOX_ID[rr][cc] == b) deg++;
                }
                bestDeg = deg;
            } else if (cnt == min) {
                int deg = 0;
                for (int j = k; j < empties; j++) {
                    int rr = er[j], cc = ec[j];
                    if (g[rr][cc] != 0) continue;
                    if (rr == r || cc == c || BOX_ID[rr][cc] == b) deg++;
                }
                if (deg > bestDeg) { best = i; bestMask = mask; bestDeg = deg; }
            }
        }

        // amener la meilleure case en k
        swap(k, best);
        int r = er[k], c = ec[k], b = BOX_ID[r][c];
        int mask = bestMask;

        // Brancher (ordre LSB→MSB)
        while (mask != 0) {
            int bit = mask & -mask;
            int d = Integer.numberOfTrailingZeros(bit);
            g[r][c] = d; row[r] |= bit; col[c] |= bit; box[b] |= bit;

            if (solve(k + 1)) { unplace(pushed); return true; }

            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[b] ^= bit;
            mask ^= bit;
        }

        unplace(pushed);
        return false;
    }

    static void swap(int i, int j){
        if (i == j) return;
        int tr = er[i]; er[i] = er[j]; er[j] = tr;
        int tc = ec[i]; ec[i] = ec[j]; ec[j] = tc;
    }
}
