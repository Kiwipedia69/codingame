import java.io.*;

class Solution {
    static int[][] g = new int[25][25];
    static int[] row = new int[25], col = new int[25], box = new int[25]; // bits 1..25
    static int[] er = new int[625], ec = new int[625]; // positions vides
    static int empties = 0;

    static final int[][] BOX_ID = new int[25][25];
    static {
        for (int r=0;r<25;r++) for (int c=0;c<25;c++) BOX_ID[r][c] = (r/5)*5 + (c/5);
    }

    // static final int[] bitCount = new int[1024];
    // static {
    //     for (int i = 0; i < 1024; i++) bitCount[i] = Integer.bitCount(i);
    // }

    static int boxId(int r, int c){ return (r/5)*5 + (c/5); }

    static int charToInt(char c) {
    if (c == '.') return 0;
    if (c >= 'A' && c <= 'Y') return c - 'A' + 1;
    throw new IllegalArgumentException("Caractère invalide : " + c);
    }

    static char intToChar(int c) {
    if (c == 0) return '.';
    if (c >= 1 && c <= 25) return (char) ('A' + c - 1) ;
    throw new IllegalArgumentException("Caractère invalide : " + c);
    }

    public static void main(String[] args) throws Exception {
        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        for (int i = 0; i < 25; i++) {
            String s = br.readLine().trim();               // <-- remplir une ligne depuis une string
            for (int j = 0; j < 25; j++) {
                int d = charToInt(s.charAt(j) );
                g[i][j] = d;
                if (d == 0) { er[empties] = i; ec[empties++] = j; }
                else {
                    int bit = 1 << d;
                    row[i] |= bit; col[j] |= bit; box[boxId(i,j)] |= bit;
                }
            }
        }
        solve(0);

        StringBuilder out = new StringBuilder();
        for (int i = 0; i < 25; i++) {
            for (int j = 0; j < 25; j++) out.append(intToChar(g[i][j]));
            out.append('\n');
        }
        System.out.print(out);
    }

    // Backtracking avec heuristique "minimum candidats"
    static boolean solve(int k){
        if (k == empties) return true;

        // choisir la case avec le moins de possibilités parmi k..end et l'amener en k
        int best = -1, bestMask = 0, min = 26;
        for (int i = k; i < empties; i++) {
            int r = er[i], c = ec[i], b = BOX_ID[r][c];
            int mask = (~(row[r] | col[c] | box[b])) & 0x03FFFFFE; // bits autorisés 1..25
            int cnt = Integer.bitCount(mask);
            //int cnt = bitCount[mask];
            if (cnt < min) { min = cnt; best = i; bestMask = mask; if (cnt == 1) break; }
        }
        if (min == 0) return false;

        // swap pour fixer l'ordre (évite structures lourdes)
        swap(k, best);
        int r = er[k], c = ec[k], b = BOX_ID[r][c];
        int mask = bestMask;

        // propagation directe : s’il n’y a qu’un candidat
        if (min == 1) {
            int bit = mask & -mask;
            int d = Integer.numberOfTrailingZeros(bit);
            g[r][c] = d; row[r] |= bit; col[c] |= bit; box[b] |= bit;
            if (solve(k + 1)) return true;
            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[b] ^= bit;
            return false;
        }

        while (mask != 0) {
            int bit = mask & -mask;                 // plus faible bit
            int d = Integer.numberOfTrailingZeros(bit);
            g[r][c] = d; row[r] |= bit; col[c] |= bit; box[b] |= bit;

            if (solve(k + 1)) return true;

            // undo
            g[r][c] = 0; row[r] ^= bit; col[c] ^= bit; box[b] ^= bit;
            mask ^= bit;
        }
        // remettre l'ordre si besoin (pas strictement nécessaire pour la sortie)
        // (on ne le refait pas par souci de concision)
        return false;
    }

    static void swap(int i, int j){
        if (i == j) return;
        int tr = er[i]; er[i] = er[j]; er[j] = tr;
        int tc = ec[i]; ec[i] = ec[j]; ec[j] = tc;
    }    
}