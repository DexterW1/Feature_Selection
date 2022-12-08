//
// Created by Dexter on 12/6/2022.
//

#ifndef FEATURE_SELECTION_HELPERFUNCTIONS_H
#define FEATURE_SELECTION_HELPERFUNCTIONS_H
#include <iostream>
#include <vector>
#include <string>
#include <ostream>
#include <set>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <queue>
#include <chrono>
#include "node.h"
#include "problem.h"
#include <iterator>
using namespace std;

struct Set
{
    float accuracy;
    vector<int> features;

    bool operator <(const Set& rhs) const
    {
        return accuracy < rhs.accuracy;
    }
    void best_print(){
        stringstream ss;
        //string output;
        for(int i=0; i < features.size(); i++){
            features[i]+=1;
            //output.push_back((features[i]++) + '0');
            ss << features[i];
            if(i < features.size() - 1){
                ss<<",";
                //output.push_back(',');
            }
        }
        cout<<"Finished search!! The best feature subset is {"<<ss.str()<<"}, which has an accuracy of "<<accuracy*100<<"%"<<endl;
    };
    void print()
    {
        vector<int> incr;
        stringstream ss;
        for(int i=0; i < features.size(); i++){
            incr.push_back(features[i]+1);
            ss << incr[i];
            if(i < features.size() - 1){
                ss<<",";
            }
        }
        cout<<endl;
        cout<<"Features set {"<<ss.str()<<"} was best, accuracy is: "<<accuracy*100<<"%"<<endl<<endl;
    }
};
string vec_to_string(vector<int> input){
    stringstream ss;
    for(int i=0; i< input.size();i++){
        input[i]+=1;
        ss<<input[i];
        if(i<input.size()-1) {
            ss<<",";
        }
    }
    return ss.str();
}
string  back_vec_to_string(vector<int>input,int target){
    stringstream ss;
    for(int i=0; i<input.size();i++){
        if(input[i]==target){
            continue;
        }
        ss <<input[i]+1;
        if(i<input.size()-1){
            ss<<",";
        }
    }
    return ss.str();
}
void backElimFeat(vector<int>&arr, int target){
    for(int i=0; i<arr.size();i++){
        if(arr[i]==target){
            arr.erase(arr.begin()+i);
            break;
        }
    }
}
void Forward(problem prob, int size)                        //function to solve forward selection
{
    cout << "This data has " << size << " features (not including the class attribute), with "<< prob.dataset_size() << " instances." << endl << endl;
    cout<<"Beginning Search."<<endl;

    auto start = chrono::steady_clock::now();

    priority_queue<Set> pq;                    //priority queue for the features set
    Set max;                                        //holds the value of the max accuracy per feature
    Set temp;
    string using_feat;
    vector<int> y;

    temp.features = y;                                       //prints starting state of forward
    temp.accuracy = prob.leave_one_out_cross_validation(y);
    max.accuracy = 0;

    bool warn = true;

    for(int i = 0; i < size; i++)
    {

        for(int j = 0; j < size; j++)
        {
            Set set;
            vector<int> sel_features = y;
            auto it = find(sel_features.begin(),sel_features.end(),j);
            if(it!=sel_features.end()){
                continue;
            }
            sel_features.push_back(j);
            set.features = sel_features;
            set.accuracy = prob.leave_one_out_cross_validation(sel_features);    //the set's accuracy set by NN(takes in features)
            pq.push(set);                              //push feature set onto the queue
            if(i==0) {
                cout << "   Using feature(s) {"<<j+1 <<"} accuracy is "<<set.accuracy*100<<"%"<< endl;
            }
            else{

                cout << "   Using features(s) {"<<using_feat<<","<<j+1<<"} accuracy is "<<set.accuracy*100<<"%"<<endl;
            }
        }
        temp = pq.top();
        (temp.accuracy > max.accuracy) ? max=temp : max;

        if(warn && temp.accuracy < max.accuracy)
        {
            warn = false;
            cout << endl<< "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl << endl;
        }
        cout<<endl;
        temp.print();                                       //print
        y = temp.features;
        using_feat = vec_to_string(y);
        while(!pq.empty())
        {
            pq.pop();
        }
    }
    cout<<"===================================================================================="<<endl;
    auto end = chrono::steady_clock::now();
    max.best_print();
    cout<<endl<<"Elapsed Time in seconds: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<endl;
}
void Backward(problem prob, int size){
    cout<< "This data has " << size << " features with " << prob.dataset_size() << " Instances: "<<endl<<endl;
    cout<<"Beginning Search"<<endl;

    auto start = chrono::steady_clock::now();
    priority_queue<Set> pq;
    Set max;
    Set temp;
    vector<int> tot_feat;
    bool warn = true;
    max.accuracy = 0;

    for(int i=0; i< size;i++){
        tot_feat.push_back(i);
    }

    temp.features = tot_feat;
    temp.accuracy = prob.leave_one_out_cross_validation(tot_feat);
    temp.print();
    for(int i=0; i< size; i++){

        for(int j=0; j<size; j++){
            Set curr_set;
            vector<int> curr_feat = tot_feat;
            auto it = find(curr_feat.begin(), curr_feat.end(), j);
            if(it == curr_feat.end()){
                continue;
            }
            backElimFeat(curr_feat, j);
            curr_set.features=curr_feat;
            curr_set.accuracy= prob.leave_one_out_cross_validation(curr_feat);
            pq.push(curr_set);
            cout << "   Using feature(s) {"<<back_vec_to_string(curr_feat,j) <<"} accuracy is "<<curr_set.accuracy*100<<"%"<< endl;
        }
        temp=pq.top();
        //temp.accuracy>max.accuracy ? max=temp : max;
        if(temp.accuracy>max.accuracy){
            max = temp;
        }
        if(warn && temp.accuracy < max.accuracy){
            warn = false;
            cout << endl<< "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl << endl;
        }
        temp.print();
        tot_feat= temp.features;
        while(!pq.empty()){
            pq.pop();
        }
    }

    cout<<"===================================================================================="<<endl;
    auto end = chrono::steady_clock::now();
    max.best_print();
    cout<<endl<<"Elapsed Time in seconds: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<endl;
}
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
            //had to create this statement because there a weird thing in the files that prevent it from parsing correctly
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
#endif //FEATURE_SELECTION_HELPERFUNCTIONS_H
