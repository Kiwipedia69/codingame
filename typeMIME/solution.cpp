#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int n; // Number of elements which make up the association table.
    cin >> n; cin.ignore();
    int q; // Number Q of file names to be analyzed.
    cin >> q; cin.ignore();
    cerr<<n<<" "<<q<<"\n";
    unordered_map<string, string> typeMIME;
    for (int i = 0; i < n; i++) {
        string ext; // file extension
        string mt; // MIME type.
        cin >> ext >> mt; cin.ignore();
        transform(ext.begin(), ext.end(), ext.begin(),
        [](unsigned char c){ return std::tolower(c); });
        // cerr<<ext<<" "<<mt<<"\n";
        typeMIME[ext]=mt;
    }
    for (int i = 0; i < q; i++) {
        string fname;
        getline(cin, fname); // One file name per line.
        string extFile = fname.substr(fname.find_last_of('.')+1,fname.size());
        transform(extFile.begin(), extFile.end(), extFile.begin(),
        [](unsigned char c){ return std::tolower(c); });
        auto it = typeMIME.find(extFile);
        auto it2 = fname.find('.');

        // cerr<<"fname ="<<fname<<"\n";
        // cerr<<"extfile ="<<extFile<<"\n";

        if (it != typeMIME.end() && it2!=std::string::npos ) {


            std::cout << it->second << "\n";
        } else {
            std::cout << "UNKNOWN\n";
        }
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;


    // For each of the Q filenames, display on a line the corresponding MIME type. If there is no corresponding type, then display UNKNOWN.
    //cout << "UNKNOWN" << endl;
}