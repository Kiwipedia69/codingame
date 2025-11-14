#include <iostream>
#include <string>
#include <vector>

using namespace std;

char decodeChar(const string& code) {
    if (code == "sp") return ' ';     // space
    if (code == "bS") return '\\'; // backSlash
    if (code == "sQ") return '\'';    // single quote
    return code[0]; // char basique
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string t;
    getline(cin, t); 

    // split (space)
    vector<string> tokens;
    size_t start = 0, pos;
    while ((pos = t.find(' ', start)) != string::npos) {
        tokens.push_back(t.substr(start, pos - start));
        start = pos + 1;
    }
    tokens.push_back(t.substr(start));

    for (const string& tok : tokens) {
        if (tok == "nl") {
            cout << '\n';
            continue;
        }

        // first non numerical char
        size_t i = 0;
        while (i < tok.size() && isdigit((unsigned char)tok[i])) ++i;

        if (i == tok.size()) {
            // full numerical characters, last one is the symbol to print
            int count = stoi(tok.substr(0, tok.size() - 1));
            char ch = tok.back();
            for (int k = 0; k < count; ++k) cout << ch;
        } else {
            // numeric + code char 
            int count = stoi(tok.substr(0, i));
            string codeChar = tok.substr(i); 
            char ch = decodeChar(codeChar);
            for (int k = 0; k < count; ++k) cout << ch;
        }
    }

    cout << '\n';
    return 0;
}
