#include <bits/stdc++.h>

using namespace std;

class page
{
public:
    vector<pair<int,int>>data;
    int count_records=0;
    int available_space;
    page* next_page;
    page* prev_page;

    page(int size_page){
        page_size = size_page;
        available_space = size_page - 16;
        next_page = nullptr;
        prev_page = nullptr;
    }

    bool insert_element(int record_size, int primary_key){
        pair<int,int> record;
        record.first = record_size;
        record.second = primary_key;
        if(available_space >= record_size + 4){
            data.push_back(record);
            count_records++;
            available_space = available_space - (record_size + 4);
            return true;
        }
        else{
            return false;
        }

    }

    int search_element(int primary_key){
        for(int i=0;i<data.size();i++){
            if(data[i].second == primary_key){
                return i;
            }
        }
        return -1;
    }




private:
    int page_size;


};

class heapfile
{
public:
    page* firstptr;
    int numofpages=0;
    int page_size;
    heapfile(int size_1){
        firstptr = nullptr;
        page_size = size_1;
    }
    bool insert_element(int record_size,int primary_key){
        page* curr = firstptr;
        page* prevcurr = nullptr;
        while(curr != nullptr){
            bool yes = curr->insert_element(record_size,primary_key);
            if(yes == true){
                break;
            }
            else{
                prevcurr = curr;
                curr = curr->next_page;
            }
        }

        if(curr == nullptr){

            page* new_page = new page(page_size);

            new_page->insert_element(record_size,primary_key);

            new_page->next_page = nullptr;
            if(prevcurr != nullptr){
                new_page->prev_page = prevcurr;
                prevcurr->next_page = new_page;
            }

            numofpages++;
            if(numofpages == 1){
                firstptr = new_page;
            }
        }

        return true;
    }

    pair<int,int> search_element(int primary_key){
        int v = numofpages;
        page* curr = firstptr;

        int page_slot=0;

        while(curr != nullptr){
            int yes = curr->search_element(primary_key);

            if(yes != -1){
                return {page_slot,yes};
            }
            else{
                curr = curr->next_page;
            }
             page_slot++;
        }

        if(curr == nullptr){
            return {-1,-1};
        }
    }

    void status(){
        cout << numofpages << " ";

        page* curr = firstptr;

        while(curr != nullptr){
            int v1 = curr->count_records;
            cout << v1 << " ";
            curr = curr->next_page;
        }
        cout << endl;
    }
};


int main()
{
    int page_size;
    cin >> page_size;

    heapfile h1(page_size);
    int command;
    while(1){
        cin >> command;

        if(command == 1){
            int record_size,primary_key;
            cin >> record_size >> primary_key;
            h1.insert_element(record_size,primary_key);
        }
        else if(command == 2){
            h1.status();
        }
        else if(command == 3){
            int primary_key;
            cin >> primary_key;
            pair<int,int>ans;
            ans = h1.search_element(primary_key);
            cout << ans.first << " " << ans.second << endl;
        }
        else if(command == 4){
            break;
        }
    }
}
