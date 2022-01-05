// https://codeforces.com/contest/609/problem/E

#include <bits/stdc++.h>

using namespace std;
using ll = long long int;
#define MAX 21

vector <ll> DisjointSet;
vector <ll> Height;

class BinaryLiftingTable{
    private:
        vector<vector<ll>> Table;
        vector<vector<ll>> MaxWeight;
        vector<ll> Parent;
        vector<ll> Weight;
        vector<ll> Depth;
        vector<bool> visited;
    public:
        BinaryLiftingTable(vector<vector<pair<ll,ll>>> &tree,ll root){
            Table.resize(MAX,vector<ll>(tree.size(),0));
            MaxWeight.resize(MAX,vector<ll>(tree.size(),0));
            Parent.resize(tree.size());
            Weight.resize(tree.size());
            Depth.resize(tree.size());
            visited.resize(tree.size(),false);
            dfs(tree,root,root,1);
            build();
        }
        void build(){
            Table[0] = Parent;
            for(ll i=1;i<MAX;i++){
                for(ll j=1;j<Table[i].size();j++){
                    Table[i][j] = Table[i-1][Table[i-1][j]];
                }
            }
            MaxWeight[0] = Weight;
            for(ll i=1;i<MAX;i++){
                for(ll j=1;j<Table[i].size();j++){
                    MaxWeight[i][j] = max(MaxWeight[i-1][j],MaxWeight[i-1][Table[i-1][j]]);
                }
            }
        }
        ll query(ll node,ll k){
            ll ans = node;
            for(ll i=0;i<MAX;i++){
                if(k&(1<<i)){
                    ans = Table[i][ans];
                }
            }
            return ans;
        }
        ll getMax(ll node,ll k){
            ll ans = 0;
            ll prev = node;
            for(ll i=0;i<MAX;i++){
                if(k&(1<<i)){
                    ans = max(ans,MaxWeight[i][prev]);
                    prev = Table[i][prev];
                }
            }
            return ans;
        }
        void dfs(vector<vector<pair<ll,ll>>> &tree,ll node,ll parent,ll depth){
            visited[node] = true;
            Parent[node] = parent;
            for(ll i=0;i<tree[node].size();i++){
                if(visited[tree[node][i].first]==false){
                    visited[tree[node][i].first] = true;
                    Depth[tree[node][i].first] = depth;
                    Weight[tree[node][i].first] = tree[node][i].second;
                    dfs(tree,tree[node][i].first,node,depth+1);
                }
            }
        }
        ll LCA(ll u,ll v){
            if(Depth[v]>Depth[u])swap(v,u);
            ll k = Depth[u]-Depth[v];
            for(ll i=20;i>=0;i--){
                if(k&(1<<i)){
                    u = Table[i][u];
                }
            }
            if(v==u)return u;
            for(ll i=20;i>=0;i--){
                if(Table[i][u]!=Table[i][v]){
                    u = Table[i][u];
                    v = Table[i][v];
                }
            }
            return query(u,1);
        }
        ll getDepth(ll u){
            return Depth[u];
        }
};

struct Node{
    ll weight;
    ll u;
    ll v;
};

ll Find(ll node){
    if(DisjointSet[node]==node){
        return node;
    }
    return Find(DisjointSet[node]);
}

bool Union(ll u,ll v){
    ll f1 = Find(u);
    ll f2 = Find(v);
    if(f1==f2){
        return false;
    }
    if(Height[f1]==Height[f2]){
        DisjointSet[f1] = f2;
        Height[f2]++;
    }
    else if(Height[f1]>Height[f2]){
        DisjointSet[f2] = f1;
    }
    else{
        DisjointSet[f1] = f2;
    }
    return true;
}
int main(){
    ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL);
    ll n,m;cin>>n>>m;
    vector <Node> V,V1;
    vector <vector<pair<ll,ll>>> graph(n+1);
    map <pair<ll,ll>,ll> edges;
    DisjointSet.resize(n+1);
    Height.resize(n+1,0);
    ll MST = 0;
    iota(DisjointSet.begin(),DisjointSet.end(),0);
    for(ll i=0;i<m;i++){
        ll u,v,w;
        cin>>u>>v>>w;
        V.push_back({w,u,v});
    }
    V1 = V;
    sort(V.begin(),V.end(),[](Node A,Node B){
        return A.weight<B.weight;
    });
    for(auto i:V){
        if(Union(i.u,i.v)){
            MST += i.weight;
            edges[{i.u,i.v}] = i.weight;
        }
    }
    for(auto i:edges){
        graph[i.first.first].push_back({i.first.second,i.second});
        graph[i.first.second].push_back({i.first.first,i.second});
    }
    BinaryLiftingTable BT(graph,1);
    V = V1;
    for(ll i=0;i<m;i++){
        if(edges[{V[i].u,V[i].v}] || edges[{V[i].v,V[i].u}]){
            cout<<MST<<"\n";
        }
        else{
            ll lca = BT.LCA(V[i].u,V[i].v);
            ll d1 = BT.getDepth(V[i].u);
            ll d2 = BT.getDepth(V[i].v);
            ll d3 = BT.getDepth(lca);
            ll mx = max(BT.getMax(V[i].u,d1-d3),BT.getMax(V[i].v,d2-d3));
            cout<<MST+V[i].weight-mx<<"\n";
        }
    }
    return 0;
}