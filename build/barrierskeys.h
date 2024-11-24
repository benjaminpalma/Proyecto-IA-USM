#ifndef BARRIERS_KEYS_H
#define BARRIERS_KEYS_H

#include <iostream>
#include <vector>
#include <queue>
#include <cstdlib>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include "tree.h"

class BarrierKeyManager {
public:
    BarrierKeyManager(unsigned int random_seed = static_cast<unsigned int>(time(0)));
    bool insertBarriersAndKeys(Tree* tree, int barrierCount, int keyCount, int neededKeys);
    bool isSolvable(Tree* tree, int barrierNodes);

private:
    int neededKeys;
    int barrierCount;
    Node* getRandomNode(Tree* tree);
};

#endif // BARRIERS_KEYS_H