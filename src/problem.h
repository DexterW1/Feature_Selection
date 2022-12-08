//
// Created by Dexter on 12/5/2022.
//

#ifndef FEATURE_SELECTION_PROBLEM_H
#define FEATURE_SELECTION_PROBLEM_H
#include <iostream>
#include <utility>
#include <vector>
#include <cmath>
#include "node.h"

using namespace std;
float eul_distance(vector<float> x, vector<float> y, vector<int> set);

class problem
{

public:
    problem(vector<node> v){
        data = std::move(v);
    }
    int dataset_size() const{
        return data.size();
    }
    float leave_one_out_cross_validation(vector<int> set){
        float accuracy = 0;
        float cur_dist;

        for(int i=0; i < dataset_size();i++){
            double dist = 99999999.99;
            node n;
            for(int j=0; j<dataset_size();j++){
                if(i==j){
                    continue;
                }
                cur_dist= eul_distance(data[i].get_features(), data[j].get_features(), set);
                if(cur_dist< dist){
                    dist = cur_dist;
                    n=data[j];
                }
            }
            if(n.class_type == data[i].class_type){
                accuracy++;
            }
        }
        return (accuracy/dataset_size());
    }
private:
    vector<node> data;
};

float eul_distance(vector<float> x, vector<float> y, vector<int> set){
    float dist = 0.0;
    for(int i : set){
        dist +=pow(x[i]-y[i],2);
    }
    return sqrt(dist);
}
#endif //FEATURE_SELECTION_PROBLEM_H
