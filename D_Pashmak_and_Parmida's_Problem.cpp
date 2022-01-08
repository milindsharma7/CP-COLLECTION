/*
    Approach 1 Use Merge Sort tree -> FAILED (MLE)
    Approach 2 Use sqrt Decomposition ->
    Approach 3 Use wavelet tree ->
    Approach 4 Use BIT + Mo's algo -> USE COORDINATE COMPRESSION???
    Approach 5 Use Persistent Seg Tree ->
*/

#include <bits/stdc++.h>
using namespace std;
using ll = long long int;
#define fast ios_base::sync_with_stdio(false);cin.tie(NULL);cout.tie(NULL)

class FenwickTree{
    private:
        vector <ll> Tree;
        vector <ll> Prefix;
    public:
        FenwickTree(vector <ll> &arr){
            ll n = arr.size();
            Tree.resize(n+1,0);
            Prefix.resize(n+1,0);
            for(ll i=1;i<=n;i++){
                Prefix[i] = Prefix[i-1] + arr[i-1]; 
            }
            for(ll i=1;i<=n;i++){
               Tree[i] = Prefix[i] - Prefix[i-(i&-i)];  
            }
        }
        void build(ll idx,ll num){
            ll sum = 0;
            while(idx<Tree.size()){
                Tree[idx] += num;
                idx += (idx&-idx);
            }
        }
        long long int prefix(ll idx){
            ll sum = 0;
            while(idx>0){
                sum += Tree[idx];
                idx -= (idx&-idx);
            }
            return sum;
        }
        long long int query(ll l,ll r){
            return prefix(r) - prefix(l-1);
        }
        void update(ll idx,ll num){
            ll old = query(idx,idx);
            ll delta = num - old;
            build(idx,delta);
        }
};

int main(){
    fast;
    ll n;cin>>n;
    vector <ll> v(n),v1;
    vector<ll> freq(1000001,0);
    unordered_map<ll,ll> compress,back;  // COORDINATE COMPRESSION 
    vector<ll> freq1(1000001,0),freq3(1000001,0);
    FenwickTree F(freq3);
    for(ll i=0;i<n;i++)cin>>v[i];
    v1 = v;sort(v1.begin(),v1.end());
    ll idx = 1;
    for(ll i=0;i<n;i++){
        if(!compress[v1[i]]){
            compress[v1[i]] = idx;
            back[idx++] = v1[i];
        }
    }
    vector <ll> pre(n+2,0);
    vector <ll> suf(n+2,0);
    ll ans = 0,fans = 0;
    for(ll i=1;i<=n;i++){
        freq[compress[v[i-1]]]++;
        pre[i] = freq[compress[v[i-1]]];
    }
    for(ll i=n;i>=2;i--){
        freq1[compress[v[i-1]]]++;
        suf[i] = freq1[compress[v[i-1]]]; 
    }
    for(ll i=n-1;i>=1;i--){
        F.build(suf[i+1],1);
        ans += F.query(1,pre[i]-1);
    }
    cout<<ans;
    return 0;
}