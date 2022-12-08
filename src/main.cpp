#include <iostream>
#include <utility>
#include "helperfunctions.h"

using namespace std;

int main() {
    string input("../data/sampletest.txt");
    //string input("../data/CS170_Large_Data__41.txt");
    vector<node> output = read(input);

    int size = output[0].num_features;
    problem prob(output);
    Forward(prob,size);
    //Forward(prob,size);

}
