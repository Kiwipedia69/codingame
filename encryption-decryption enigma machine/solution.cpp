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
        int shift = (i+x)%26;
        int base = s[i]-'A';

        int res = (base+shift)%26;
        s[i]='A'+res;
    }
    cerr<<s<<endl;
}
// void ceasarShift(string &s, int x){
//     for(auto i=0;i<s.size();i++){
//         int res = (i+x)%26;
//         if(s[i]+res<='Z'){       
//             s[i]+=res;
//         }
//         else{    
//             s[i]+=res-'Z'+'A'-1;
//         }
//     }
//     cerr<<s<<endl;
// }

void invCeasarShift(string &s, int x){     
    for(auto i=0;i<s.size();i++){
        int res = (-i-x)%26;
        cerr<<res<<endl; 
        if(s[i]+res>='A'){       
            s[i]+=res;
        }
        else{   
            s[i]+=res+'Z'-'A'+1;
        }
    }
    cerr<<s<<endl;
}



// void rotor(string& rot, string& code){
//     string res="";

//     for(auto i=0;i<code.size();i++){
//         res+=rot[code[i]-'A'];
//     }
//     code =res;
//     cerr<<res<<endl;
// }
void rotor(const string& rot, string& code) {
    for (char& c : code) {
        c = rot[c - 'A'];
    }
    cerr << code << endl;
}

void invRotor(string& rot, string& code){
    string res="";

    for(auto i=0;i<code.size();i++){
        for(auto j =0;j<rot.size();j++){
            if(code[i]==rot[j])
                res+=j+'A';
        }
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

    cerr<<operation<<endl;
    cerr<<pseudo_random_number<<endl;

    if(operation.compare("ENCODE")==0) {

        cerr<<message<<endl;
        ceasarShift(message, pseudo_random_number);

        for(int i=0;i<listRotors.size();i++){
            rotor(listRotors[i],message);
        }
    }


    if(operation.compare("DECODE")==0) {

        cerr<<message<<endl;

        for(int i=listRotors.size()-1;i>=0;--i){
            invRotor(listRotors[i],message);
        }

        invCeasarShift(message, pseudo_random_number);

    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << message << endl;
}