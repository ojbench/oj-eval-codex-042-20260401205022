
#include <bits/stdc++.h>
#include "solution.h"
using namespace std;

template<int BP>
int mirror_hash(const string &s){
    long long ans=0; const int s_prime=31;
    for(char ch: s){ ans = (ans * s_prime + ch) % BP; }
    return int((ans + BP) % BP);
}

int main(){
    mt19937 rng(123);
    for(int n: {3,5,7,16,31,64,127}){
        vector<int> bounds; int cur=0; for(int i=0;i<n;i++){cur += rng()%100+1; bounds.push_back(cur);} // increasing
        const int BP = bounds.back();
        SpeedCircularLinkedList<int, 1000000007> *dummy=nullptr; // just to use template
        // Create instance with correct b_prime via macro trick: we can't pass runtime b_prime, so generate for fixed values
        // For test, pick n-specific compile-time BP set: we can't, so instead only test a fixed BP scenario.
    }
    // We'll pick a fixed bounds vector to actually test
    vector<int> bounds = {50, 250, 450, 800, 1000, 1300, 1350, 1600, 1850, 2017};
    const int BP = 2017;
    SpeedCircularLinkedList<int, BP> sys(bounds);
    auto gen_key = [&](){
        string s; int L = rng()%20+1; for(int i=0;i<L;i++){ s.push_back(char('a'+rng()%26)); }
        return s;
    };
    // Insert many keys
    unordered_map<string,int> truth;
    for(int i=0;i<50000;i++){
        string k = gen_key();
        int code = mirror_hash<BP>(k);
        int idx = int(lower_bound(bounds.begin(), bounds.end(), code) - bounds.begin());
        int val = idx;
        sys.put(k, val);
        truth[k]=val;
    }
    // Verify gets
    for(auto &kv: truth){
        int got = sys.get(kv.first);
        if(got != kv.second){
            cerr << "Mismatch for key: " << kv.first << " expected "<<kv.second<<" got "<<got<<"\n";
            return 1;
        }
    }
    cout << "OK" << endl;
    return 0;
}
