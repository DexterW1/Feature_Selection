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
//This structure will a current sets features and its accuracy
struct Set
{
    float accuracy;
    vector<int> features;

    bool operator <(const Set& rhs) const
    {
        return accuracy < rhs.accuracy;
    }
    //used to print the the best feature if this feature is selected
    void best_print(){
        stringstream ss;
        for(int i=0; i < features.size(); i++){
            features[i]+=1;
            ss << features[i];
            if(i < features.size() - 1){
                ss<<",";
            }
        }
        cout<<"Finished search!! The best feature subset is {"<<ss.str()<<"}, which has an accuracy of "<<accuracy*100<<"%"<<endl;
    };
    //used to print the best accuracy when having to choose to add or remove 1 element
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
//function to help print the "Using" statement
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
//function to help print the "Using" statement
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
//Function to help erase/remove the element in the feature set in backward elimination
void backElimFeat(vector<int>&arr, int target){
    for(int i=0; i<arr.size();i++){
        if(arr[i]==target){
            arr.erase(arr.begin()+i);
            break;
        }
    }
}
//Function to search using forward selection, basically start from {} -> continue searching and greedly search for the highest accuracy
void Forward(problem prob, int size)                        //function to solve forward selection
{
    cout << "This data has " << size << " features (not including the class attribute), with "<< prob.dataset_size() << " instances." << endl << endl;
    cout<<"Beginning Search."<<endl;

    auto start = chrono::steady_clock::now();

    priority_queue<Set> pq;                    //priority queue for the features set
    Set max;                                        //holds the value of the max accuracy per feature
    Set temp;                                   //temp Set
    string using_feat;                          //Used to print the "Using" statements
    vector<int> tot_feat;                       // the current total features being checked

    temp.features = tot_feat;                                       //prints starting state of forward
    temp.accuracy = prob.leave_one_out_cross_validation(tot_feat);
    max.accuracy = 0;
    temp.print();
    bool warning = true;

    for(int i = 0; i < size; i++)
    {

        for(int j = 0; j < size; j++)
        {
            Set set;
            vector<int> curr_feat = tot_feat;
            auto it = find(curr_feat.begin(), curr_feat.end(), j);      //check to see if we are comparing to itself
            if(it != curr_feat.end()){
                continue;
            }
            curr_feat.push_back(j);         //push back the feature we want to currently search for into our feature set
            set.features = curr_feat;       //asssign the vector<int> to the set.features
            set.accuracy = prob.leave_one_out_cross_validation(curr_feat);    //calculate the accuracy by using leave_one_out_cross_validation based on the current features in the Set
            pq.push(set);                              //push feature set into priority queue
            if(i==0) {
                cout << "   Using feature(s) {"<<j+1 <<"} accuracy is "<<set.accuracy*100<<"%"<< endl;
            }
            else{
                cout << "   Using features(s) {"<<using_feat<<","<<j+1<<"} accuracy is "<<set.accuracy*100<<"%"<<endl;
            }
        }
        temp = pq.top();            //grab the highest accuracy feature
        (temp.accuracy > max.accuracy) ? max=temp : max;    //compare if temp > max accuracy them assign

        if(warning && temp.accuracy < max.accuracy)     //check if the warning flag has been raised before if not print warning when accuracy is decreases
        {
            warning = false;
            cout << endl<< "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl << endl;
        }
        cout<<endl;
        temp.print();                                       //print
        tot_feat = temp.features;                           // assign the total features, with the highest accuracy subset feature in use
        using_feat = vec_to_string(tot_feat);
        while(!pq.empty())      //while the pq, is not empty continue removing the element in it
        {
            pq.pop();
        }
    }
    cout<<"===================================================================================="<<endl;
    auto end = chrono::steady_clock::now();     //stop time
    max.best_print();
    cout<<endl<<"Elapsed Time in seconds: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<endl; //print out the time in seconds
}
void Backward(problem prob, int size){
    cout<< "This data has " << size << " features with " << prob.dataset_size() << " Instances: "<<endl<<endl;
    cout<<"Beginning Search"<<endl;

    auto start = chrono::steady_clock::now();
    priority_queue<Set> pq;     //pq to hold Sets
    Set max;                    // Max or highest Set accuracy currently
    Set temp;                   // Temp to hold and assign later
    vector<int> tot_feat;
    bool warn = true;
    max.accuracy = 0;

    for(int i=0; i< size;i++){          //push all the features into the total features list to start off since this is backward elimination
        tot_feat.push_back(i);
    }

    temp.features = tot_feat;
    temp.accuracy = prob.leave_one_out_cross_validation(tot_feat);  //calculate its accuracy
    temp.print();                                                       //print features and the best accuracy currently
    for(int i=0; i< size; i++){

        for(int j=0; j<size; j++){
            Set curr_set;
            vector<int> curr_feat = tot_feat;
            auto it = find(curr_feat.begin(), curr_feat.end(), j);
            if(it == curr_feat.end()){
                continue;
            }
            backElimFeat(curr_feat, j);         //remove element from current feature set
            curr_set.features=curr_feat;
            curr_set.accuracy= prob.leave_one_out_cross_validation(curr_feat);  //run cross_validation on current set of features and assign it to the Set accuracy
            pq.push(curr_set);
            cout << "   Using feature(s) {"<<back_vec_to_string(curr_feat,j) <<"} accuracy is "<<curr_set.accuracy*100<<"%"<< endl; //print "Using" feat string
        }
        temp=pq.top();
        temp.accuracy>max.accuracy ? max=temp : max;        //check to see if the current highest accuracy is bigger than the max accuracy
        if(warn && temp.accuracy < max.accuracy){
            warn = false;
            cout << endl<< "(Warning, Accuracy has decreased! Continuing search in case of local maxima)" << endl << endl;
        }
        temp.print();
        tot_feat= temp.features;
        while(!pq.empty()){     //clear the pq, since we have temp variable holding onto the highest accuracy already
            pq.pop();
        }
    }

    cout<<"===================================================================================="<<endl;
    auto end = chrono::steady_clock::now();         //end the clock time
    max.best_print();                   //print the best subset based on the highest accuracy feature
    cout<<endl<<"Elapsed Time in seconds: "<<chrono::duration_cast<chrono::seconds>(end-start).count()<<endl;       //print the elapsed time in seconds
}
//Function to parse in the .txt file and return a vector of nodes
vector<node> read(const string& text){
    ifstream file(text);    //grab the file
    vector<node> output;
    if(file.is_open()) {        //check if file is open
        while (file.good()) {   //while file does not run into errors
            node n;                 //create new node
            float type = -1.0;      //initialize variables
            float temp=0.0;
            float count =0;
            string row;
            getline(file,row);  //grab first row

            istringstream first(row);   //parse that row so each element has their own index basically
            first >> type;
            //had to create this statement because there a weird thing in the files that prevent it from parsing correctly
            if(type==-1.0){
                break;
            }
            n.set_type(type);               //set the class type using the first column
            while(first >> temp){           //while there are stuff to pass in, attach them to the node .append feature which is a vector of <float> that will hold the features
                count++;
                n.append_feature(temp);
            }
            n.set_numFeatures(count);
            output.push_back(n);               //push into output vector of nodes
        }
    }
    file.close();
    return output;
}
//Function to test if the .txt was parsed in correctly ONLY FOR TESTING
void print(vector<node> outs){
    for(int i=0; i<outs.size();i++){
        cout<<i+1<<": ";
        outs[i].print();
    }
    cout<<"Num of Features: "<<outs[0].num_features<<endl;
    cout<<"Num of Instances: "<<outs.size();
}
// Function that will print the intro message
void print_menu(){
    cout<<"Welcome to Dexter (Yiu Ming) Wong Feature Selection Algorithm."<<endl;
    cout<<"Type in the name of the file you wish to test: ";
}
#endif //FEATURE_SELECTION_HELPERFUNCTIONS_H
