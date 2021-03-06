#ifndef FORD_FULKERSON_HPP
#define FORD_FULKERSON_HPP

#include <iostream>
#include <vector>
#include <stack>

#define rep(i, n) for (int i = 0; i < (int)(n); i++)

using namespace std;
const int INF = 999999999;

// 枝の構造体（行先のvertex, みなしcapacity, 逆辺のid, capacity）
struct edge {int to, capacity, rev_id, raw_capacity;};

// Graphの隣接リスト
using Graph = vector<vector<edge> >;

// two arrow Graphに枝を追加
void add_edge_two_arrow(Graph &G, int from, int to, int capacity){

    G[from].push_back( (edge){to, capacity, (int)G[to].size(), capacity} );
    G[to].push_back( (edge){from, 0, (int)G[from].size()-1, 0} );
}

// f-augmenting pathの計算
vector<pair <int, int> > dfs(Graph G, int v, int t, int N, bool &end_frag){

    vector< pair<int, int> > path(N);
    vector<bool> seen(N, false);
    stack<int> S;
    end_frag = true;

    // 探索済みの頂点
    seen[v] = true;
    S.push(v);

    while(!S.empty()){
       v = S.top();
       S.pop();

       rep(i, G[v].size()){
           if (G[v][i].capacity > 0){
               int next_v = G[v][i].to;

               if (!seen[next_v]){
                   seen[next_v] = true;
                   path[next_v] = make_pair(v, i); // 一度seenがtrueになるともうアクセスされないので、これはユニークに定まる
                   S.push(next_v);
               }

               // tに届くと終了する
               if (next_v == t){
                   end_frag = false;
                   break;
               }
           }
       }
       if (!end_frag) break;
    }
    return path;
}

// compute gamma
int compute_gamma(Graph G, vector<pair<int, int> > path, int s, int v){

    int gamma = INF;
    int index;

    while(1){
        index = path[v].second;
        v = path[v].first;

        gamma = (gamma < G[v][index].capacity ? gamma : G[v][index].capacity);

        if (v == s) break;
    }

    return gamma;
}

// augment
void augment(Graph &G, vector<pair<int, int> > path, int gamma, int s, int v){
    int index;

    while(1){

        index = path[v].second;
        v = path[v].first;

        // path上の辺
        G[v][index].capacity = G[v][index].capacity - gamma;
        // path上の辺の逆辺
        edge e = G[v][index];
        G[e.to][e.rev_id].capacity = G[e.to][e.rev_id].capacity + gamma;

        if (v == s) break;
    }
}

void ford_fulkerson(Graph &G, int s, int t, int N){

     // 初期化
    bool end_frag = false;

    // FORD FULKERSON
    while(1) {
        // find augmenting s-t path
        vector<pair<int, int> > path = dfs(G, s, t, N, end_frag);
        if (end_frag) break;

        // compute gamma
        int gamma = compute_gamma(G, path, s, t);

        // augment f along path by gamma
        augment(G, path, gamma, s, t);

    }
}

void output(Graph &G, int N){

    // max flowの出力
    int f;
    cout << "v_from " << "v_to " << "flow" << endl;
    rep(v, N){
        rep(i, G[v].size()){
            if (G[v][i].raw_capacity != 0){
                f = G[v][i].raw_capacity - G[v][i].capacity; // 元のcapacityとみなしcapacityの差がflow
                cout << v << ' ' << G[v][i].to << ' ' << f << endl;
            }
        }
    }
}

#endif // FORD_FULKERSON_HPP
