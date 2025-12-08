#include <iostream>
#include <string>
#include <vector>
#include <algorithm>


using namespace std;

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/

int main()
{
    int n;
    int res = 0x7FFFFFFF;
    cin >> n; cin.ignore();
    vector<int> ordered_list;
    for (int i = 0; i < n; i++) {
        int pi;
        cin >> pi; cin.ignore();
        ordered_list.push_back(pi);
    }
    sort(ordered_list.begin(), ordered_list.end());


    for(int i=1;i<n;i++){
        if(ordered_list[i]-ordered_list[i-1]<res)
            res=ordered_list[i]-ordered_list[i-1];
    }

    // Write an answer using cout. DON'T FORGET THE "<< endl"
    // To debug: cerr << "Debug messages..." << endl;

    cout << res << endl;
}