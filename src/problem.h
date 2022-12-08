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
    problem(vector<node> v){        //initialize problem with a vector  of nodes
        data = std::move(v);
    }

    int dataset_size() const{       //returns number of instances in the dataset
        return data.size();
    }
    //Function that uses leave_one_out_cross_validation to check each feature that is being tested between every row besides itself
    float leave_one_out_cross_validation(vector<int> set){
        float accuracy = 0;
        float curr_dist;

        for(int i=0; i < dataset_size();i++){
            double dist = 99999999.99;          //initalize high number so doesn't error compare
            node min_node;
            for(int j=0; j<dataset_size();j++){
                if(i==j){           //if we are checking the same row and index continue
                    continue;
                }
                curr_dist= eul_distance(data[i].get_features(), data[j].get_features(), set);        //get the features of the current row and index, the set is the features being tested , it has which columns to test
                if(curr_dist < dist){     //if fthe value is less than the lowest distance then assign and assign to min_node
                    dist = curr_dist;
                    min_node=data[j];
                }
            }
            if(min_node.class_type == data[i].class_type){      //check to see if the classtype of the classtype being tested is the same
                accuracy++;     ///if it is the same increase the accuracy
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
        dist +=pow(x[i]-y[i],2);        //grab this info and add to dist += (x1-y1)^2
    }
    return sqrt(dist);                  //sqrt(dist) at the end to get eucledians distance
}
#endif //FEATURE_SELECTION_PROBLEM_H
