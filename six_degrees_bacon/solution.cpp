#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// del space before actor name in cast from movie
string trim(const string& s) {
    size_t a = s.find_first_not_of(' ');
    if (a == string::npos) return "";
    size_t b = s.find_last_not_of(' ');
    return s.substr(a, b - a + 1);
}

int main() {
    // codingame hack/fix for issue with time
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    string actor_name;
    actor_name = trim(actor_name);
    getline(cin, actor_name);
    // cerr<<actor_name<<endl;
    
    // nb movies
    string line;
    getline(cin, line);
    int n = stoi(line);
    // cerr<<n<<endl;

    unordered_map<string,int> id;
    vector<vector<int>> adj;

    // fct to associate actor name with unique Id
    auto getId = [&](const string& name) -> int {
        auto it = id.find(name);
        if (it != id.end()) return it->second;
        int idx = (int)id.size();
        id[name] = idx;
        adj.push_back({});
        return idx;
    };

    for (int i = 0; i < n; ++i) {
        string movieLine;
        getline(cin, movieLine);

        size_t pos = movieLine.find(':');

        string cast = movieLine.substr(pos + 1);
        cast = trim(cast);
        // cerr<<cast<<endl;
        
        // actors spliut
        stringstream ss(cast);
        string actor;
        vector<int> castIds;

        while (getline(ss, actor, ',')) {
            actor = trim(actor);
            // cerr<<actor<<endl;
            if (actor.empty()) continue;
            castIds.push_back(getId(actor));
        }

        // adjacence
        int m = (int)castIds.size();
        for (int a = 0; a < m; ++a) {
            for (int b = a + 1; b < m; ++b) {
                int u = castIds[a];
                int v = castIds[b];
                adj[u].push_back(v);
                adj[v].push_back(u);
            }
        }
    }

    // for(auto i=0; i<adj.size();i++){
    //     cerr<<i<<" : ";
    //     for(auto j=0;j<adj[i].size();j++){
    //         cerr<<adj[i][j]<<" ";
    //     }
    //     cerr<<endl;
    // }

    int kevinId  = getId("Kevin Bacon");
    int actorId = getId(actor_name);

    // BFS
    vector<int> dist(adj.size(), -1);
    queue<int> q;

    dist[kevinId] = 0;
    q.push(kevinId);

    while (!q.empty()) {
        int u = q.front(); q.pop();
        // cerr<<u<<endl;
        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                // cerr<<dist[v]<<endl;
                q.push(v);
            }
        }
    }

    cout << dist[actorId] << '\n';
    return 0;
}
