#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>
#include <map>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
vector<string> split(const string& s, char sep) {
    vector<string> out;
    string cur;
    for (char c : s) {
        if (c == sep) {
            out.push_back(cur);
            cur.clear();
        } else {
            cur += c;
        }
    }
    out.push_back(cur);
    return out;
}

double to_double(const string& s) {
    string tmp = s;
    replace(tmp.begin(), tmp.end(), ',', '.'); 
    return stod(tmp);
}

double calcDist(double longB, double longA, double latB, double latA)
{
    double x = (longB - longA) * cos((latA+latB)/2);
    double y = latB - latA;

    return (sqrt(x*x+y*y)*6371);
}

 class Defib{
    public:
    // unsigned int numDefib;
    string nameDefib;
    // string adresseDefib;
    // string telDefib;
    double longDefib;
    double latDefib;
    double dist;

    Defib(string& data, double longP, double latP) {
       
        vector<string> fields = split(data, ';');
        //   numDefib = stoi(fields[0]);
        // telDefib = fields[3];
        nameDefib = fields[1];
        // adresseDefib = fields[2];
        longDefib = to_double(fields[4]);
        latDefib = to_double(fields[5]);
        dist = calcDist(longP, longDefib, latP, latDefib);
    };
};

int main()
{
    string lon;
    cin >> lon; cin.ignore();
    string lat;
    cin >> lat; cin.ignore();
    int n;
    cin >> n; cin.ignore();

    double distMin = 1e100;
    string nameMin;
    for (int i = 0; i < n; i++) {
        string defib;
        getline(cin, defib);
        double dist;
        Defib d = Defib(defib,to_double(lon),to_double(lat));
        if(d.dist<distMin){
            distMin=d.dist;
            nameMin=d.nameDefib;
        }
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << nameMin << endl;
}