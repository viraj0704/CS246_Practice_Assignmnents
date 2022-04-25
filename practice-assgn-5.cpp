#include <bits/stdc++.h>

using namespace std;

int main(){
    int b, rpp, n;
    cin>>b>>rpp>>n;
    vector<int> r(n);
    for (int i=0; i<n; i++) cin>>r[i];
    vector<vector<int>> p;
    for (int i=0; i<n; i+=rpp){
        vector<int> te;
        for (int j=i; j<min(i+rpp,n); j++){
            te.push_back(r[j]);
        }
        p.push_back(te);
    }
    for (auto &u: p){
        sort(u.begin(),u.end());
    }
    
    // debug(b-1,p.size());
    while (p.size()>1){
        vector<vector<int>> te;
        for (int i=0; i<p.size(); i+=b-1){
            vector<int> te2;
            for (int j=i; j<min(i+b-1,int(p.size())); j++){
                for (auto u: p[j]){
                    te2.push_back(u);
                }
            }
            sort(te2.begin(),te2.end());
            te.push_back(te2);
        }
        p=te;
        // debug(p.size());
    }
    for (int i=0; i<n; i+=rpp){
        for (int j=i; j<min(i+rpp,n); j++){
            cout<<p[0][j]<<endl;
        }
    }
    // for (int i=25; i>=1; i--) cout<<i<<endl;
    return 0;
}