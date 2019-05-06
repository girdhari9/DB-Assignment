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

using namespace std;

queue<int> inputBuffer, outputBuffer;
ui numBuffers, bufferSize, inputSize, outputSize;

class HashTable{
	int num_records, num_buckets;
    vector<vector<int>> overflow;
	vector<vector<int>> buckets;
	public:

	HashTable(){
        num_buckets = 2;
        overflow = vector< vector<int> >(num_buckets);
        num_records = 0;
		buckets = vector< vector<int> >(num_buckets);
	}

	ui hash(int x){
        ui index = x % num_buckets;
        index = (index + num_buckets) % num_buckets;
        return (ui)index;
    }

	bool isPresent(int x){
		ui k = hash(x);
        int size = buckets.size();
        if(k >= size)
            k = k - (num_buckets/2);

        FOR0(i, buckets[k].size()){
            if(buckets[k][i] == x)
                return true;
        }
        FOR0(i, overflow[k].size()){
   			if(overflow[k][i] == x)
   				return true;
        }
        return false;
	}

    void print() {
        ui size = buckets.size();
        FOR0(i, size) {
            cout<< i;
            cout<< " --> ";

            FOR0(index, buckets[i].size())
                cout<< buckets[i][index] <<' ';
            FOR0(index, overflow[i].size())
                cout<< overflow[i][index] <<' ';
            cout << endl;
        }
        cout << endl;
    }

	void insert(int x){
        int size = buckets.size();
		ui k = hash(x);
        num_records++;
        if(k >= size)
            k = k - (num_buckets/2);

        if(buckets[k].size() < bufferSize)
            buckets[k].push_back(x);
        else
            overflow[k].push_back(x);

        double N = 100.0;
        int TH = 75;
        while(N * double(num_records) / (double(buckets.size()) * double(bufferSize)) >= TH){

            buckets.push_back(vector<int>());
            int size = buckets.size();
            num_buckets = pow(2,ceil(log2(double(size))));
            k = size-1-num_buckets/2;
            overflow.push_back(vector<int>());
            vector<int> v;
            
            int bucketSize = buckets[k].size();
            FOR0(i, bucketSize){
	            v.push_back(buckets[k][i]);
            }

	        buckets[k].clear();
            int overflowSize = overflow[k].size();
	        FOR0(i, overflowSize)
		        v.push_back(overflow[k][i]);

		    overflow[k].clear();
            int vecSize = v.size();
            FOR0(i, vecSize)
                buckets[hash(v[i])].push_back(v[i]);
        }
	}
}hashTable;

void clearOutput(){
    bool flag = outputBuffer.empty();
    while(!flag){
    	cout << outputBuffer.front();
        cout << endl;
        outputBuffer.pop();
        flag = outputBuffer.empty();
    }
}

void clearInput(){
    bool flag = inputBuffer.empty();
    while(!flag){
        int val = inputBuffer.front();
        if(hashTable.isPresent(val) == false){
        	outputBuffer.push(val);
            hashTable.insert(val);
            int bufferSize = outputBuffer.size();
            if(bufferSize == outputSize)
                clearOutput();
        }
        inputBuffer.pop();
        flag = inputBuffer.empty();
    }
}

int main(int argc, char *argv[]){
    if(argc < 4){
        cout<<"Invalid command : ";
        FOR0(i, argc)
            cout<<argv[i]<<" ";
        cout << endl;
    }
    else if(atoi(argv[2]) < 2 || atoi(argv[3]) < 4)
    	cout<<"Invalid buffers, should be M >= 2 and B >= 4\n";

    else{
        numBuffers = atoi(argv[2]);	
    	ifstream file(argv[1]);
        outputSize = bufferSize = atoi(argv[3])/sizeof(int);

        inputSize = (numBuffers - 1) * (bufferSize);
        inputBuffer = queue<int>();
        outputBuffer = queue<int>();
        int x;
        while(file >> x) {
            int size = inputBuffer.size();
            if(size >= inputSize){
                clearInput();
                inputBuffer.push(x);
            }
            else inputBuffer.push(x);                
        }
        clearInput();
        clearOutput();
        return 0;
    }
}