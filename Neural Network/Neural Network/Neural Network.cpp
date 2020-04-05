// Neural Network.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <vector>
#include <iostream>
#include <cstdlib>
#include <cassert>
#include <fstream>
#include "Net.h"

using namespace std;


int main()
{
    //  { 3, 2, 1 }
    vector<unsigned> topology;
    topology.push_back(9);
    topology.push_back(2);
    topology.push_back(2);
    topology.push_back(1);
    topology.push_back(9);
    Net myNet(topology);

    vector<double> inputVals = { 0.0, 1.0, .5, 0.0, 1.0, .5, 0.0, 0.0, .5 };
    vector<double> targetVals = { 0.5, 0.0, 0.0, 0.5, 0.0, 0.0, 0.5, 1.0, 0.0 };
    vector<double> resultVals;
    vector<vector<vector<double>>> pathMap;
    double error;
    ofstream Results;
    myNet.feedForward(inputVals);
    myNet.backProp(targetVals);
    myNet.getResults(resultVals);
    Results.open("Neural Network Results.csv");
    Results << "Test Number, Average Error, Result 1, Result 2, Result 3, Result 4, Result 5, Result 6, Result 7, Result 8, Result 9" << endl;

    for (unsigned i = 0; i < 2000; ++i)
    {
        myNet.feedForward(inputVals);
        myNet.backProp(targetVals);
        myNet.getResults(resultVals);
        error = myNet.getRecentAverageError();
        cout << "Test Number " << i << '\n' << '\n';
        cout << "Average Error: " << error << '\n' << '\n';
        cout << "Chosen move values are:" << '\n';
        Results << i << "," << error;
        for (unsigned resultNum = 0; resultNum < resultVals.size(); ++resultNum)
        {
            cout << resultVals[resultNum] << "   ";
            Results << "," << resultVals[resultNum];
        }
        Results << endl;
    }

    myNet.getPathWeights(pathMap);
    Results.close();
    Results.open("Neural Paths.csv");
    unsigned numLayers = pathMap.size();
    cout << '\n' << pathMap.size() << '\n';
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        unsigned numNeurons = pathMap[layerNum].size();
        cout << "    " << pathMap[layerNum].size() << '\n';
        for (unsigned neuronNum = 0; neuronNum < numNeurons; ++neuronNum)
        {
            unsigned numPaths = pathMap[layerNum][neuronNum].size();
            cout << "        " << pathMap[layerNum][neuronNum].size() << '\n';
            for (unsigned pathNum = 0; pathNum < numPaths; ++pathNum)
            {
                Results << pathMap[layerNum][neuronNum][pathNum] << ",";
            }
            Results << endl;
        }
        Results << endl;
    }
    Results.close();
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
