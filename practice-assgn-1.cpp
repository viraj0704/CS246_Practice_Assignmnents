#include <iostream>
#include <vector>
#include <sstream>
#include <algorithm>
#include <cmath>

using namespace std;

vector<int> create_vec(string s){
    vector<int>arr;
    istringstream is(s);
    int num;
    while(is>>num){
        arr.push_back(num);
    }
    return arr;
}

int cd_func(int n,vector<pair<vector<int>,vector<int>>>fd_data,int total_attributes){
    // x_dep ka int le rha hu usme -1 krke << check krunga ki vo 1 h ya nahi.. agar saare 1 nikle to hee side wale int ko -1 krke << krke or krdunga
    int prev = 0;
    int next = n;
    int iskey  = 0;
    while(prev!=next){
        
        prev = n;
        if(prev== (1<<total_attributes)-1){
            iskey = 1;
            break;
        }

        for(int i=0;i<fd_data.size();i++){
            vector<int>lhs = fd_data[i].first;
            int flag = 1;
            for(int j=0;j<lhs.size();j++){
                int x = lhs[j];
                x = x-1;
                int v1 = n & ( 1<< x);
                if(v1 == 0){
                    flag = 0;
                    // cout << "%" << x << endl;
                }
            }

            if(flag){
                vector<int>rhs = fd_data[i].second;
                for(int j=0;j<rhs.size();j++){
                    int x = rhs[j];
                    x = x - 1;
                    n = n|(1<<x);
                }
            }
        }
        next = n;

    }
    if(prev== (1<<total_attributes)-1){
            iskey = 1;
    }

    return iskey;
}

vector<int> ex_attr(int n){
    int len = log2(n)+1;

    vector<int>vec;

    for(int j=0;j<len;j++){
        if(n&(1<<j)){
            vec.push_back(j+1);
        }
    }
    return vec;
}





int main()
{
    int total_attributes;
    cin >> total_attributes;
    vector<int>candidate_key_set;
    
    int num_func_dep;
    cin >> num_func_dep;
    char c;
    scanf("%c",&c);
    vector<pair<vector<int>,vector<int>>>fd_data;

    for(int i=0;i< 2*num_func_dep;i+=2){
        string x;
        getline(cin,x);
        vector<int>x_dep = create_vec(x);
        string y;
        getline(cin,y);
        vector<int>y_dep = create_vec(y);
        fd_data.push_back(make_pair(x_dep,y_dep));


    }

    // for(int i=0;i<fd_data.size();i++){
    //     for(int j=0;j<fd_data[i].first.size();j++){
    //        cout << fd_data[i].first[j] << " "; 
    //     }
    //     cout << " ## ";
    //     for(int j=0;j<fd_data[i].second.size();j++){
    //        cout << fd_data[i].second[j] << " "; 
    //     }

    //     cout << endl;
        
    // }
    for(int i=1;i<(1<<total_attributes);i++){
        int flag = 0;
        for(int j=0;j<candidate_key_set.size();j++){
                //cout << candidate_key_set[j] << " ";
                if((candidate_key_set[j]&i) == candidate_key_set[j]){
                    flag = 1;
                }
               
        }
        //cout << endl;
        if(flag==0){
            int iscdkey = cd_func(i,fd_data,total_attributes);
            if(iscdkey == 1){
                candidate_key_set.push_back(i);
            }
        }
    }
    vector<pair<int,int>>cd_vecpair;

    for(int k=0;k<candidate_key_set.size();k++){
        vector<int>vec = ex_attr(candidate_key_set[k]);
        cd_vecpair.push_back(make_pair(vec.size(),candidate_key_set[k]));
    }
    sort(cd_vecpair.begin(),cd_vecpair.end());
    cout << candidate_key_set.size() << endl;
    for(int i=0;i<cd_vecpair.size();i++){
        vector<int>vec = ex_attr(cd_vecpair[i].second);
        for(int j=0;j<vec.size();j++){
            cout << vec[j] << " ";
        }
        cout << endl;
    }
    // cout << "#" << cd_func(2,fd_data,total_attributes);
    cout << endl;
}