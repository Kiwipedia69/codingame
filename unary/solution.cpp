#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/



std::string to7bitBinaryString(const std::string& text) {
    std::string result;
    result.reserve(text.size() * 7); // optimisation pour éviter les reallocations

    for (unsigned char c : text) {
        for (int i = 6; i >= 0; --i) {
            result += ((c >> i) & 1) ? '1' : '0';
        }
    }
    return result;
}

std::string encodeBitRuns(const std::string& bits) {
    if (bits.empty()) return "";

    // test validité chaine
    // for (char ch : bits)
    //     if (ch != '0' && ch != '1')
    //         throw std::invalid_argument("La chaine doit contenir uniquement '0' ou '1'.");

    std::string out;
    out.reserve(bits.size() * 3); // marge

    char curr = bits[0];
    int run = 1;

    auto flush_run = [&](char bit, int len) {
        if (!out.empty()) out += ' '; // si pas le premier bit
        if (bit == '1') { out += '0'; out += ' '; }
        else  { out += '0'; out += '0'; out += ' '; }
        out.append(len, '0');
    };

    for (size_t i = 1; i < bits.size(); ++i) {
        if (bits[i] == curr) {
            ++run;
        } else {
            flush_run(curr, run);
            curr = bits[i];
            run = 1;
        }
    }
    flush_run(curr, run);
    return out;
}


int main()
{
    unsigned int xMax = 127; // 0b1111111

    string message;
    getline(cin, message);


    // int nbChar = message.size();
    // int messChar[nbChar];

    // for(int i = 0; i<nbChar; i++) {
    //     messChar[i] = message[i];
    //     cerr << messChar[i] <<endl;
    //     cerr << bitset<7>(messChar[i]) <<endl;
    //     cerr << bitset<7>(xMax) <<endl;
    //     compareBits(messChar[i], xMax);
    // }

    // string res = to7bitBinaryString(message);
    // cerr<<res<<endl;

    // string resF = encodeBitRuns(res);
    // cerr<<resF<<endl;
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    
    // cerr << message << endl;
    cout << encodeBitRuns(to7bitBinaryString(message)) << endl;
}