#include <iostream>
#include <utility>
#include "helperfunctions.h"

using namespace std;

int main() {
    int choice;
    string user_input;
    string input("../data/");

    print_menu();
    cin>> user_input;
    input.append(user_input);
    vector<node> output = read(input);


    int size = output[0].num_features;
    problem prob(output);

    cout<<"Type the number of the algorithm you want to run."<<endl;
    cout<<"1) Forward Selection "<<endl;
    cout<<"2) Backward Elimination "<<endl;
    cin>>choice;

    if(choice == 1) {
        Forward(prob, size);
    }
    else if(choice ==2){
        Backward(prob,size);
    }
    else{
        cout<<"Incorrect selection please try again!"<<endl;
    }

}
