#include <iostream>
#include <string>
#include <unordered_map>
#include <algorithm>

using namespace std;

static inline void to_lower_inplace(string& s) {
    for (char& c : s) {
        if (c >= 'A' && c <= 'Z') c = char(c - 'A' + 'a');
    }
}


int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n; 
    cin >> n; 
    int q; 
    cin >> q; 
    cin.ignore();

    unordered_map<string, string> typeMIME;
    typeMIME.reserve(n * 2);

    for (int i = 0; i < n; i++) {
        string ext; // file extension
        string mt;  // MIME type
        cin >> ext >> mt;
        cin.ignore();

        // mettre l'extension en minuscule
        // transform(ext.begin(), ext.end(), ext.begin(),
        //           [](unsigned char c){ return std::tolower(c); });
        //opyi pour 100%
        to_lower_inplace(ext);

        typeMIME[ext] = mt;
    }

    for (int i = 0; i < q; i++) {
        string fname;
        getline(cin, fname);

        size_t posDot = fname.find_last_of('.');

        if (posDot == string::npos || posDot == fname.size() - 1) {
            cout << "UNKNOWN\n";
            continue;
        }

        string extFile = fname.substr(posDot + 1);

        // transform(extFile.begin(), extFile.end(), extFile.begin(),
        //           [](unsigned char c){ return std::tolower(c); });
        to_lower_inplace(extFile);

        auto it = typeMIME.find(extFile);
        if (it != typeMIME.end()) {
            cout << it->second << "\n";
        } else {
            cout << "UNKNOWN\n";
        }
    }

    return 0;
}
