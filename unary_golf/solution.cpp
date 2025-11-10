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

string to7bitFromStringChar(string & val)
{
    string res ="";
    for(char c : val){
        res+=bitset<7>(c).to_string();
    }
    return res;
}

int main()
{
    string message;
    getline(cin, message);

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;
    cerr<<to7bitFromStringChar(message)<<endl;

    string resT = to7bitFromStringChar(message);

    string resF ="";
    char last_bit = -1;

    for(char bit : resT)
    {
        resF += bit != last_bit ? bit == '1' ? " 0 0" : " 00 0" : "0";
        last_bit=bit;
    }
    resF.erase(0, 1);
    // cerr<<resF<<endl;
    cout << resF << endl;
}