#ifndef FOREST_H
#define FOREST_H

#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>
#include "tree.h"
#include "utilities.h"

class Forest {
public:
    std::vector<Tree*> forest;
    Tree* bestTree = nullptr;
    unsigned int seed;
    float stdDev;
    int popSize;
    int roomNumber;
    int depthOfSubtree;

    Forest(int steps, int popSize, int roomNumber,  unsigned int seed = static_cast<unsigned int>(time(0)), float stdDev=2.0, int depthOfSubtree=3);
    ~Forest();

    int genForest(int popSize, int roomNumber);
    int genRandomTree(int roomCount, unsigned int seed, float stdDev, Tree* tree);
    void createPlots(Tree* tree, string name);
    double evaluateTree(Tree* tree, float targetCoef, int targetRooms);
    void next_step(float targetCoef, int targetRooms, float crossoverProb, float mutationProb, int min_mut, int max_mut);
    void crossover(Tree* tree1, Tree* tree2, int depthOfSubtree);
    bool checkCollision(Node* subtree1, Node* subtree2);
    Node* getRandomNode(Node* root, int depthOfSubtree);
    void collectLeaves(Node* node, vector<Node*>& leaves);
    //void swapSubtrees(Tree* tree1, Node* subtree1, Tree* tree2, Node* subtree2);
    //void updateParentChild(Tree* tree, Node* subtree);
};

#endif // FOREST_H