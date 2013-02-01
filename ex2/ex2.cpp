#include "ex2.h"
#include <vector>
#include <iostream>
#include <cassert>
using namespace std;

//Example graphs, both with min vertex cover = 2
const vector<vector<uint32_t>> graph1 = {
                                    {1, 2, 3},
                                    {0, 2},
                                    {0, 1},
                                    {0},
                                   };

const vector<vector<uint32_t>> graph2 = {
                                    {1},
                                    {0},
                                    {3},
                                    {2},
                                   };

const vector<vector<uint32_t>> graph3 = {};

bool hasVertexCover(const vector<vector<uint32_t>>& graph, int k){

    //Your code here

    return true;
}

int main(){

    Evaluation eval;

    //Evaluate your code on example graph 1
    for(int k = 0; k <=5; k++){
        bool correct = k >= 2;
        eval = evaluateEx2( bind(hasVertexCover, graph1, k), [correct](bool result){return result == correct ? 1.0 : 0.0;} );
        cout << "graph1 (k="<<k<<"): Your code got the "<<(eval.accuracy == 1.0 ? "right":"wrong")<<" answer in "<<eval.seconds<<" seconds."<<endl;
    }

    //Evaluate your code on example graph 2
    for(int k = 0; k <=5; k++){
        bool correct = k >= 2;
        eval = evaluateEx2( bind(hasVertexCover, graph2, k), [correct](bool result){return result == correct ? 1.0 : 0.0;} );
        cout << "graph2 (k="<<k<<"): Your code got the "<<(eval.accuracy == 1.0 ? "right":"wrong")<<" answer in "<<eval.seconds<<" seconds."<<endl;
    }

    //Evaluate your code on example graph 3
    for(int k = 0; k <=1; k++){
        eval = evaluateEx2( bind(hasVertexCover, graph3, k), [](bool result){return result == true ? 1.0 : 0.0;} );
        cout << "graph3 (k="<<k<<"): Your code got the "<<(eval.accuracy == 1.0 ? "right":"wrong")<<" answer in "<<eval.seconds<<" seconds."<<endl;
    }

    //Set parameters for generated graph
    const int nVertices = 10;
    const int testK = 2;
    const double edgeDensity = .5;
    const int maxVertexCoverSize = 4;

    //generate graph
    auto generatedGraph = generateGraph(nVertices, edgeDensity, maxVertexCoverSize);

    //test speed of your code, no meaningful measurement of accuracy
    eval = evaluateEx2( bind(hasVertexCover, generatedGraph, testK), [](bool result){ return 0.0; } );
    cout << "generated graph (k="<<testK<<"): Your code got an answer in "<<eval.seconds<<" seconds."<<endl;

    return 0;
}
