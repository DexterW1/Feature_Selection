#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include "node.h"
using namespace std;
vector<node> read(const string& text){
    ifstream file(text);
    vector<node> output;
    if(file.is_open()) {
        while (file.good()) {
            node n;
            float type = -1.0;
            float temp=0.0;
            float count =0;
            string row;
            getline(file,row);

            istringstream first(row);
            first >> type;
            if(type==-1.0){
                break;
            }
            n.set_type(type);
            while(first >> temp){
                count++;
                n.append_feature(temp);
            }
            n.set_numFeatures(count);
            output.push_back(n);
        }
    }
    file.close();
    return output;
}

void print(vector<node> outs){
    for(int i=0; i<outs.size();i++){
        cout<<i+1<<": ";
        outs[i].print();
    }
    cout<<"Num of Features: "<<outs[0].num_features<<endl;
    cout<<"Num of Instances: "<<outs.size();
}
int main() {
    string input("CS170_Small_Data__6.txt");
    vector<node> output = read(input);
    print(output);
}
