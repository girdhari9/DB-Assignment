#include <fstream>
#include <sstream>
#include<bits/stdc++.h>
#define FOR0(i,n) for(ui i=0;i<n;i++)
#define FOR1(i,n) for(ll i=1;i<=n;i++)
#define FORl(i,l,n) for(ll i=l;i<n;i++)
#define pi pair<ll,ll>
#define pb push_back
#define ll long long
#define ld long double
#define ff first
#define ss second
#define mp make_pair
#define mst0(x) memset(x,0,sizeof(x));
#define vi vector<ll>
#define sync ios_base::sync_with_stdio(0); cin.tie(0); cout.tie(0);
#define endl '\n'
#define sp <<" "<<
#define intmax 1e14
#define ull unsigned long long
#define ui unsigned int
#define MAX 50
using namespace std;
int numberOfPointers;

struct Block{
    int value[MAX];
    int tNodes;
    Block *parent;
    Block *child[MAX];
    Block *brother;
    
    Block(){
        parent = NULL;
        tNodes = 0;
        brother = NULL;
        FOR0(i, numberOfPointers+1){
            child[i] = NULL;
            value[i] = INT_MAX;
        }
    }

}*root;

void insertion(Block *current,int val){
    FOR0(i, current->tNodes+1)
        if(val < current->value[i])
            swap(current->value[i], val);
}

void leftShift(Block *right){
    FOR0(i, numberOfPointers){
        right->child[i] = right->child[i+1];
        right->value[i] = right->value[i+1];
    }
}

int splitPoint(bool isleaf){
    int ptr = numberOfPointers / 2;
    if(isleaf == false){
        //because we want more on right for non leaf because we already gonna delete front of rigth for non leaf
        return ptr;   
    }
    if(numberOfPointers & 1)
        return ++ptr;
    return ptr;
}

void split(Block *current, bool isleaf){
    
    Block *right = new Block();
    int nodes = splitPoint(isleaf);
    current->tNodes = nodes; //no of nodes in left

    int rightNodes = numberOfPointers - nodes;
    if(isleaf)
        right->tNodes = rightNodes;    //no of nodes in right
    else
        right->tNodes = rightNodes - 1;  //no of nodes in right, one less because right first val in not needed in non leaf
    
    right->parent = current->parent;

    int j = 0;
    FORl(i, nodes, numberOfPointers+1){
        right->value[j] = current->value[i];
        right->child[j] = current->child[i];
        j++;
        if(i != nodes)
            current->child[i] = NULL;
        current->value[i] = INT_MAX;
    }
    int val;
    val = right->value[0];
    //left shift right node in case of non leaf
    if(isleaf == false)
        leftShift(right);
        
    // ,Note this only iterate for leaf
    int it = 0;
    while(current->child[it] != NULL){
        current->child[it]->parent = current;
        it++;
    }

    it = 0;
    while(right->child[it] != NULL){
        right->child[it]->parent = right;
        it++;
    }

    if(isleaf == true){   
        Block* temp = NULL;
        temp = current->brother;
        current->brother = right;
        right->brother = temp;
    }

    if(current->parent != NULL){
        current = current->parent;
        insertion(current,val);
        FOR0(i, current->tNodes + 1){
            if(current->child[i]->value[0] > right->value[0])
                swap(current->child[i], right);
        }
        current->tNodes++;
        current->child[current->tNodes] = right;
    }

    else{
        Block *parent = new Block();
        parent->tNodes++;
        parent->value[0] = val;
        parent->child[1] = right;
        parent->child[0] = current;
        root = parent;
        right->parent = parent;
        current->parent = parent;
    }

}

void insert(Block *current, int val){
    if(current->child[0] == NULL){
        current->tNodes++;
        insertion(current,val);
        int curNode = current->tNodes;
        if(curNode == numberOfPointers)
            split(current,true);
       
    }
    else if(current->child[0] != NULL){
        FOR0(i, current->tNodes+1){
            if(val < current->value[i]){   
                insert(current->child[i], val);
                int curNode = current->tNodes;
                if(curNode == numberOfPointers)
                    split(current,false);
                break;
            }
       } 
    }
}

void find(Block *current, int val){
    if(current->child[0] == NULL){
        FOR0(i, current->tNodes+1){
            int currValue = current->value[i];
            if(val == currValue){   
                cout<<"YES\n";
                return;
            }
       } 
    }
    else if(current->child[0] != NULL){
       FOR0(i, current->tNodes+1){
            int currValue = current->value[i];
            if(val < currValue){
                find(current->child[i], val);
                return ;
            }
       } 
    }
    cout<<"NO\n";
}

void range(Block *current, int val1,int val2){
    if(current->child[0] == NULL){
        int count = 0;
        while(true){
            FOR0(i, current->tNodes + 1){
                int currValue = current->value[i];
                if(val2 >= currValue){
                    if(val1 <= currValue)
                        count++;
                }
                else break;
            }
            if(current->brother == NULL)
                break; 
            else 
                current =  current->brother;
        }
       cout<<count<<"\n";
    }
    else if(current->child[0] != NULL){
       FOR0(i, current->tNodes+1){
            if(val1 <= current->value[i]){
                range(current->child[i], val1,val2);
                return;
            }
       } 
    }
}

int main(int argc, char const *argv[]){
    string fileName;
    int n, B, M;
    B = atoi(argv[3]);
    fileName = argv[1];

    numberOfPointers = ((B - 1) / 4) + 1;
    if(numberOfPointers < 3)
        numberOfPointers = 3;
    
    root = new Block();

    ifstream infile(fileName);
    string line;
    int value1, value2;
    while(getline(infile, line)){

        if(line.find("INSERT") != string::npos){
            string str = "INSERT";
            istringstream (line.substr(str.size()+1)) >> value1;
            insert(root, value1);
        }
        else if(line.find("COUNT") != string::npos){
            string str = "COUNT";
            istringstream (line.substr(str.size()+1)) >> value1;
            range(root,value1,value1);
        }
        else if(line.find("RANGE") != string::npos){
            string str = "RANGE";
            istringstream (line.substr(str.size()+1)) >> value1 >> value2;
            range(root, value1, value2);
        }
        else if(line.find("FIND") != string::npos){
            string str = "FIND";
            istringstream (line.substr(str.size()+1)) >> value1;
            find(root,value1);
        }
        else{
            cout << "Invalid Command : ";
            FOR0(i, argc)
                cout<<argv[i]<<" ";
            cout << endl;
        }
    }
    return 0;
}