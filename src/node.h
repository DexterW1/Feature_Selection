//
// Created by Dexter on 11/25/2022.
//

#ifndef FEATURE_SELECTION_NODE_H
#define FEATURE_SELECTION_NODE_H
#include <iostream>
#include <vector>
using namespace std;

class node{
public:
    //-------------- constructor -------------------
    node(){
        class_type = 0.0;
    }
    //-------------- Variables ---------------------
    float class_type;
    vector<float> features;
    float num_features;
    //------------- Functions ----------------------
    vector<float> get_features(){
        return this->features;
    }
    void append_feature(float feat){
        this->features.push_back(feat);
    }
    void set_type(float type){
        this->class_type = type;
    }
    void set_numFeatures(float num){
        this->num_features=num;
    }
    void print(){
        cout<<"Class Type: "<<this->class_type<<" Features: ";
        for(int i=0; i< this->features.size();i++){
            cout<<features[i]<<" ";
        }
        cout<<endl;
    }
};

#endif //FEATURE_SELECTION_NODE_H
