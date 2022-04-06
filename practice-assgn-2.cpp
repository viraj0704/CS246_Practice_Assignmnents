#ifndef BUCKET_H
#define BUCKET_H
#include <vector>
#include <bits/stdc++.h>

using namespace std;

// Bucket header
class Bucket
{
    public:
        Bucket();
        Bucket(int,int);
        virtual ~Bucket();
        int get_local_depth();
        bool insert_element(int);
        bool delete_element(int);
        bool incr_local_depth();
        int return_bucket_occupancy();
        bool search_element(int);
        void print_bucket();
        vector<int> empty_bucket();
    protected:

    private:
        int local_depth;
        vector<int>elements;
        int bucket_size;
};

#endif // BUCKET_H

// Bucket CPP

Bucket::Bucket(int bucketsize,int depth){
    bucket_size = bucketsize;
    local_depth = depth;

}

int Bucket::get_local_depth(){
    return local_depth;
}

bool Bucket::insert_element(int val){
    if(elements.size()==bucket_size){
        return false;
    }
    else{
        elements.push_back(val);
        return true;
    }
}

bool Bucket::delete_element(int val){
    vector<int>::iterator itr;

    for(itr = elements.begin();itr<elements.end();itr++){
        if(*itr == val){
            break;
        }
    }
    if(itr == elements.end()){
        return false;
    }
    elements.erase(itr);
    return true;
}

bool Bucket::incr_local_depth(){
    local_depth++;
    return true;
}

int Bucket::return_bucket_occupancy(){
    return elements.size();
}

bool Bucket::search_element(int val){
    vector<int>::iterator itr;

    for(itr = elements.begin();itr<elements.end();itr++){
        if(*itr == val){
            break;
        }
    }
    if(itr == elements.end()){
        return false;
    }

    return true;
}

void Bucket::print_bucket(){
     vector<int>::iterator itr;

    for(itr = elements.begin();itr<elements.end();itr++){
        cout << *itr << " ";
    }
    cout << endl;
}

vector<int> Bucket:: empty_bucket(){
    vector<int> x = elements;
    elements.clear();
    return x;
}

Bucket::~Bucket()
{
    //dtor
}

// Hash Header

#ifndef EXTENDIBLE_HASH_H
#define EXTENDIBLE_HASH_H

class Extendible_hash
{
    public:
        Extendible_hash(int,int);
        virtual ~Extendible_hash();
        bool insert_element(int);
        bool print_hash_table();
        int return_global_depth();
        bool delete_element(int);
        bool search_element(int);
        int return_no_buckets();
        bool print_ans();


    protected:

    private:
        int global_depth;
        int bucket_capacity;
        int no_buckets=0;
        vector<pair<int,Bucket*>> directory;
        vector< Bucket* > order_of_buckets;
};

#endif // EXTENDIBLE_HASH_H


// Hash CPP

Extendible_hash::Extendible_hash(int global,int bucketcapacity)
{
    global_depth = global;
    bucket_capacity = bucketcapacity;
    //cout << "% " << global_depth << endl;
    Bucket* b;
    for(int i=0;i<(1<<global_depth);i++){
        b = new Bucket(bucket_capacity,global_depth);
        directory.push_back(make_pair(i,b));
        no_buckets++;
        order_of_buckets.push_back(b);
    }
    //cout << "% " << directory.size() << endl;
}

bool Extendible_hash::insert_element(int element){
    if(global_depth > 20){
        return false;
    }
    int v1 = (1<<global_depth);
    int index = element%v1;

    Bucket* B1 = directory[index].second;
    bool ins = B1->insert_element(element);

    if(!ins){
        //cout << "FALSE" << endl;
        vector<int>temp = B1->empty_bucket();
        temp.push_back(element);
        int new_index = index + (1<<global_depth);
        if(global_depth == B1->get_local_depth()){
            global_depth++;
            B1->incr_local_depth();
            Bucket* newbuck = new Bucket(bucket_capacity,global_depth);
            no_buckets++;
            order_of_buckets.push_back(newbuck);

            for(int i= (1<<(global_depth-1));i<(1<<global_depth);i++){
                Bucket* bold = directory[i-(1<<(global_depth-1))].second;
                directory.push_back(make_pair(i,bold));
            }
            directory[new_index].second = newbuck;
            v1 = (1<<global_depth);
            for(int i=0;i<temp.size();i++){
                int ele = temp[i];
                /*int store_index = ele%v1;
                Bucket* bucket_to = directory[store_index].second;
                bucket_to->insert_element(ele);*/
                insert_element(ele);

            }
        }
        else{
            B1->incr_local_depth();
            int val_local = B1->get_local_depth();
            Bucket* newbuck = new Bucket(bucket_capacity,val_local);
            no_buckets++;
            order_of_buckets.push_back(newbuck);
            vector<int>indexs_to_b1;
            for(int i=0;i<directory.size();i++){
                if(directory[i].second == B1){
                    indexs_to_b1.push_back(i);
                }
            }

            int temp_value = (1<<(val_local-1));
            for(int i=0;i<indexs_to_b1.size();i++){
                int v3 = (indexs_to_b1[i] & temp_value);
                if(v3 > 0){
                    directory[indexs_to_b1[i]].second = newbuck;
                }
            }
            //directory[new_index].second = newbuck;
            for(int i=0;i<temp.size();i++){
                int ele = temp[i];
                /*int store_index = ele%v1;
                Bucket* bucket_to = directory[store_index].second;
                bucket_to->insert_element(ele);*/
                insert_element(ele);

            }
        }
    }
    return true;

}
bool Extendible_hash::print_hash_table(){
    for(int i=0;i<directory.size();i++){
        cout << i << "  # ";
        Bucket* b = directory[i].second;
        b->print_bucket();
    }
}
int Extendible_hash::return_global_depth(){
    return global_depth;
}
bool Extendible_hash::delete_element(int element){
    for(int i=0;i<directory.size();i++){
        Bucket* b1 = directory[i].second;
        bool present = b1->search_element(element);
        if(present){
            b1->delete_element(element);
        }
    }
}
bool Extendible_hash::search_element(int element){
    for(int i=0;i<directory.size();i++){
        Bucket* b1 = directory[i].second;
        bool present = b1->search_element(element);
        if(present){
            return true;
        }
    }
    return false;
}
int Extendible_hash::return_no_buckets(){
    return no_buckets;
}
bool Extendible_hash::print_ans(){
    cout << global_depth << endl;
    cout << no_buckets << endl;
    for(int i=0;i<order_of_buckets.size();i++){
        Bucket* b = order_of_buckets[i];
        cout << b->return_bucket_occupancy() << " " << b->get_local_depth() << endl;
    }
}

Extendible_hash::~Extendible_hash()
{
    //dtor
}


// Main file

int main()
{
    int global_depth;
    cin >> global_depth;
    int bucket_capacity;
    cin >> bucket_capacity;
    // cout << hashfunc(5,3) << endl;
    // cout << hashfunc(15,3) << endl;
    int command;
    Extendible_hash EH(global_depth,bucket_capacity);

    while(1){
        cin >> command;
        if(command == 2){
            // insert value
            int val;
            cin >> val;
            bool ins = EH.insert_element(val);
            if(!ins){
                cout << "CAN'T INSERT" << endl;
            }
            //EH.print_hash_table();
            //cout << endl;

        }
        else if(command == 3){
            int val;
            cin >> val;
            bool present = EH.search_element(val);
            if(present){
                cout << "PRESENT" << endl;
            }
            else{
                cout << "ABSENT" << endl;
            }
        }
        else if(command == 4){
            int val;
            cin >> val;
            bool present = EH.search_element(val);
            if(present){
                EH.delete_element(val);
                //EH.print_hash_table();
            }
            else{
                cout << "ABSENT" << endl;
            }
        }
        else if(command == 5){
            EH.print_ans();
            //cout << "\n";
            //EH.print_hash_table();
        }
        else if(command == 6){
            break;
        }
    }
}


