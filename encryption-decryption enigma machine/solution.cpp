#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

void ceasarShift(string &s, int x){
    for(auto i=0;i<s.size();i++){
        if(s[i]+i+x<='Z'){       
            s[i]+=i+x;
        }
        else{    
            s[i]+=i+x-'Z'+'A'-1;
        }
    }
    cerr<<s<<endl;
}

void invCeasarShift(string &s, int x){
    for(auto i=0;i<s.size();i++){
        if(s[i]-i-x>='A'){       
            s[i]+=-i-x;
        }
        else{    
            s[i]+=-i-x+'Z'-'A'+1;
        }
    }
    cerr<<s<<endl;
}

void rotor(string& rot, string& code){
    string res="";

    for(auto i=0;i<code.size();i++){
        res+=rot[code[i]-'A'];
    }
    code =res;
    cerr<<res<<endl;
}


int main()
{
    string operation;
    getline(cin, operation);
    int pseudo_random_number;
    cin >> pseudo_random_number; cin.ignore();

    vector<string> listRotors;
    for (int i = 0; i < 3; i++) {
        string rotor;
        getline(cin, rotor);
        listRotors.push_back(rotor);
    }
    string message;
    getline(cin, message);

    cerr<<message<<endl;
    ceasarShift(message, pseudo_random_number);
    // invCeasarShift(message, pseudo_random_number);

    // message = "ZZZ";
    // cerr<<message<<endl;
    // ceasarShift(message, pseudo_random_number);
    // invCeasarShift(message, pseudo_random_number);

    for(auto i=0;i<listRotors.size();i++){
        rotor(listRotors[i],message);
    }
    


    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << message << endl;
}