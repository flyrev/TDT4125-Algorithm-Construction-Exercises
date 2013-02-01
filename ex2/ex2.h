#include "evaluate.h"
#include <vector>
#include <random>
#include <algorithm>
#include <chrono>
#include <cmath>
#include <cassert>
#include <iomanip>

std::default_random_engine generator(std::chrono::system_clock::now().time_since_epoch().count());

std::vector<std::vector<uint32_t>> generateGraph(int nVertices, double edgeDensity, int maxVertexCoverSize){
    std::vector<std::vector<uint32_t>> adjacencyLists(nVertices);
    std::bernoulli_distribution edgeDistribution(edgeDensity);
    std::bernoulli_distribution nodeDistribution((double)maxVertexCoverSize/(double)nVertices);
    int nodesUsed = 0;

    for(int i = 0; i<nVertices; i++){
        if(nodeDistribution(generator) && nodesUsed++ < maxVertexCoverSize){
            for(int j = i+1; j<nVertices; j++){
                if(edgeDistribution(generator)){
                    adjacencyLists[i].push_back(j);
                    adjacencyLists[j].push_back(i);
                }
            }
        }
    }
    return adjacencyLists;
}

auto evaluateEx2 = evaluate<bool>;
