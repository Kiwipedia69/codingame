#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

 char exitChar(vector<char>&m, int posT, int h, int w){
    int currPos = posT+w;//next line

    int testBottom = (h-1)*w;
    cerr<<currPos<<endl;
    while(currPos<testBottom){
        if(currPos%w == 0) { //leftmost
            if(m[currPos+1]!='-') currPos+=w;
            else currPos+=3+w;// 2*'-' + next line
        }
        else if(currPos%w == w-1) {// rightmost
            if(m[currPos-1]!='-') currPos+=w;
            else currPos+=-3+w;// 2*'-' + next line
        }
        else{
            if(m[currPos+1]=='-') currPos+=3+w;// 2*'-' + next line
            else if(m[currPos-1]=='-') currPos+=-3+w;// 2*'-' + next line
            else currPos+=w;
        }
        cerr<<currPos<<endl;
    }
    
    cerr<<"exit"<<currPos+w<<endl;
    return m[currPos];
 }

int main()
{
    int w;
    int h;
    cin >> w >> h; cin.ignore();
    vector<char> m(w * h);

    for (int i = 0; i < h; i++) {
        string line;
        getline(cin, line);
        int j=0;
        for(auto c:line){
            m[i*w+j]=c;
            j++;
        }
    }

    

    for (int i = 0; i < h; i++) {
        for(auto j=0;j<w;j++){
            cerr<<m[i*w+j];
        }
        cerr<<endl;
    }

    for(int i =0; i<w; i++){
        if(m[i] != ' '){
            cout<<m[i];
            cout<< exitChar(m,i,h,w)<<endl;
        }
    }
    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    // cout << "answer" << endl;
}