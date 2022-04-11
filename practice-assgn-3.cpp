#include <bits/stdc++.h>
using namespace std;

class node
{
public:

    vector<node *> children;
    vector<int> elements;

    node(int min_capacity_leaf, int min_capacity_index, bool bool_leaf = false){ 
        d = min_capacity_leaf; 
        t = min_capacity_index; 
        leaf = bool_leaf;
        parent = nullptr;
    };

    void set_leaf(){
        leaf = true;
    };

    void insert_element(int element){
        int i;
        for(i = 0; i < elements.size(); i++){
            if (elements[i] > element) break;
        }
        if (i == elements.size()){
            elements.push_back(element);
        }
        else{
            elements.insert(elements.begin() + i, element);
        }
    };
    
    vector<int> split_leaf(){
        vector<int> x;
        for(int i = d; i < elements.size(); i++){
            x.push_back(elements[i]);
        }
        elements.erase(elements.begin() + d, elements.end());
        return x;
    };

    void set_parent(node *parent_node){
        parent = parent_node; 
    };

    node* return_parent(){
        return parent;
    };

    bool is_full(){
        if (leaf == true){
            if (elements.size() == 2*d) return true;
            else return false;
        }
        else{
            if (elements.size() == 2*t + 1) return true;
            else return false;
        }
    };

    bool is_overflow(){
        if (leaf == true){
            if (elements.size() == 2*d + 1) return true;
            else return false;
        }
        else{
            if (elements.size() == 2*t + 2) return true;
            else return false;
        }
    }

    void print_node(){
        for(auto el: elements){
            cout << el << " ";
        }
        cout << endl;
    };

    bool is_leaf(){
        return leaf;
    };

    // ~node();

private:
    int d;
    int t;
    bool leaf;
    node *parent;

};

class bplus_tree
{
public:

    bplus_tree(int min_capacity_leaf, int min_capacity_index){
        d = min_capacity_leaf;
        t = min_capacity_index;
        root = nullptr;
        no_data_nodes = 0;
        no_index_nodes = 0;
    };

    void split_leaf(node* x, int i){
        node* y = x->children[i];
        node* z = new node(d, t, true);
        no_data_nodes++;

        for(int j = d; j <= 2*d; j++){
            z->elements.push_back(y->elements[j]);
        }

        y->elements.erase(y->elements.begin() + d, y->elements.end());

        int extra = z->elements[0];

        x->elements.insert(x->elements.begin() + i, extra);
        x->children.insert(x->children.begin() + (i + 1), z);

        z->set_parent(x);
    };

    void split_index(node* x, int i){
        node* y = x->children[i];
        node* z = new node(d, t);
        no_index_nodes++;

        for(int j = t+1; j <= 2*t + 1; j++){
            z->elements.push_back(y->elements[j]);
            z->children.push_back(y->children[j]);
        }
        z->children.push_back(y->children[2*t + 2]);

        y->elements.erase(y->elements.begin() + t + 1, y->elements.end());
        y->children.erase(y->children.begin() + t + 1, y->children.end());

        int extra = y->elements[t];
        y->elements.erase(y->elements.begin() + t);

        x->elements.insert(x->elements.begin() + i, extra);
        x->children.insert(x->children.begin() + i + 1, z);

        z->set_parent(x);

        for(auto child: y->children){
            child->set_parent(y);
        }
        for(auto child: z->children){
            child->set_parent(z);
        }
    };

    void split_root(node* x){
        node* new_root = new node(d, t);
        no_index_nodes++;
        node* y = new node(d, t);
        no_index_nodes++;

        for(int j = t+1; j <= 2*t + 1; j++){
            y->elements.push_back(x->elements[j]);
            y->children.push_back(x->children[j]);
        }
        y->children.push_back(x->children[2*t + 2]);

        x->elements.erase(x->elements.begin() + t + 1, x->elements.end());
        x->children.erase(x->children.begin() + t + 1, x->children.end());

        int extra = x->elements[t];
        x->elements.erase(x->elements.begin() + t);

        new_root->elements.push_back(extra);
        new_root->children.push_back(x);
        new_root->children.push_back(y);

        y->set_parent(new_root);
        x->set_parent(new_root);
        root = new_root;

        for(auto child: x->children){
            child->set_parent(x);
        }
        for(auto child: y->children){
            child->set_parent(y);
        }
    };

    void insert_element(int element){
        if (root == nullptr){
            node* first_leaf = new node(d, t, true);
            no_data_nodes++;
            first_leaf->insert_element(element);
            root = first_leaf;
        }
        else if (root->is_leaf() == true){
            if (root->is_full() != true){
                root->insert_element(element);
            }
            else{
                root->insert_element(element);
                node* first_leaf = root;
                node* second_leaf = new node(d, t, true);
                no_data_nodes++;
                vector<int> x = root->split_leaf();
                second_leaf->elements = x;

                node* first_index = new node(d, t);
                no_index_nodes++;
                first_leaf->set_parent(first_index);
                second_leaf->set_parent(first_index);
                root = first_index;
                root->insert_element(x[0]);
                root->children.push_back(first_leaf);
                root->children.push_back(second_leaf);
            }
        }
        else{
            node* curr = root;
            vector<int> number;
            int i;
            while(curr->is_leaf() != true){
                for(i = 0; i < curr->elements.size(); i++){
                    if (curr->elements[i] > element) break;
                }
                curr = curr->children[i];
                number.push_back(i);
            }
            if (curr->is_full() != true){
                curr->insert_element(element);
            }
            else{
                curr->insert_element(element);
                split_leaf(curr->return_parent(), i);
                curr = curr->return_parent();
                number.erase(number.begin() + number.size() - 1);
                while(curr->is_overflow() == true){
                    if (curr == root) break;
                    split_index(curr->return_parent(), number[number.size()-1]);
                    curr = curr->return_parent();
                    number.erase(number.begin() + number.size() - 1);
                }
                if (curr == root){
                    if (curr->is_overflow() == true)
                        split_root(curr);
                }
            }
        }
    };

    void print_tree(){
        root->print_node();
        for(auto child: root->children){
            child->print_node();
        }
        for(auto child: root->children){
            for(auto ch: child->children){
                ch->print_node();
            }
        }
        // for(auto child: root->children){
        //     for(auto ch: child->children){
        //         for(auto c: ch->children){
        //             c->print_node();
        //         }
        //     }
        // }
    };

    void print_ans(){
        cout << no_index_nodes << " " << no_data_nodes << " ";
        root->print_node();
    }

    // ~bplus_tree();

private:
    int d;
    int t;
    node* root;
    int no_index_nodes;
    int no_data_nodes;

};

int main(){
    int d;
    int t;
    cin >> d;
    cin >> t;

    bplus_tree tree(d, t);
    int code;
    while(1){
        cin >> code;
        if (code == 1){
            int x;
            cin >> x;
            tree.insert_element(x);
        }
        else if (code == 2){
            //tree.print_tree();
            tree.print_ans();
        }
        else if (code == 3){
            break;
        }
    }
}