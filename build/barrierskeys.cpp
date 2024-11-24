#include "barrierskeys.h"

using namespace std;

// Constructor
BarrierKeyManager::BarrierKeyManager(unsigned int random_seed) {
    srand(random_seed);
}

// Insert barriers and keys into the tree
bool BarrierKeyManager::insertBarriersAndKeys(Tree* tree, int barrierCount, int keyCount, int neededKeys) {
    std::vector<Node*> barrierNodes;
    std::vector<Node*> keyNodes;

    // Insert barriers
    for (int i = 0; i < barrierCount; i++) {
        Node* barrierNode = getRandomNode(tree);
        barrierNodes.push_back(barrierNode);
        barrierNode->barrier_id = i; // Mark node as a barrier
    }

    // Insert keys
    for (int i = 0; i < barrierCount; i++) {
        Node* keyNode = getRandomNode(tree);
        keyNodes.push_back(keyNode);
        keyNode->key_id = i; // Mark node as a key
    }

    for (int i = 0; i < keyCount - barrierCount; i++) {
        Node* keyNode = getRandomNode(tree);
        keyNodes.push_back(keyNode);
        keyNode->key_id = rand() % neededKeys; // Mark node as a key
    }

    if (isSolvable(tree, barrierCount)){
        cout << "solved" << endl;
        return true; 
    }


    // Reset previous barriers and keys
    for (Node* node : barrierNodes) {
        node->barrier_id = -1; // Reset barrier
    }
    for (Node* node : keyNodes) {
        node->key_id = -1; // Reset key
    }
    tree->exitNode = nullptr;
    return false;
}

// Check if the tree is solvable using A* algorithm
bool BarrierKeyManager::isSolvable(Tree* tree, int barrierCount) {
    if (!tree || !tree->root) return false;

    std::queue<Node*> openSet;
    std::unordered_set<Node*> closedSet;
    std::unordered_set<int> keys; // Set of collected keys
    std::vector<Node*> barrierNodes; // Track all nodes with barriers unlocked

    openSet.push(tree->root);

    while (!openSet.empty()) {
        Node* current = openSet.front();
        openSet.pop();
        closedSet.insert(current);

        // Collect key if present
        if (current->key_id != -1) {
            keys.insert(current->key_id);
            if (keys.size() == barrierCount) { // Check if all barriers are unlocked
                if (!barrierNodes.empty()) {
                    Node* lastBarrierNode = barrierNodes.back();
                    for (Node* child : lastBarrierNode->children) {
                        if (!tree->isCollisionOrNull(child)) {
                            tree->exitNode = child; // Set exitNode to any child of the last barrier node
                            return true;
                        }
                    }
                    tree->exitNode = lastBarrierNode; // If no children, set exitNode to the last barrier node itself
                    return true;
                }
            }
        }

        for (Node* neighbor : current->children) {
            if (!tree->isCollisionOrNull(neighbor)) {
                if (neighbor->barrier_id != -1 && keys.find(neighbor->barrier_id) != keys.end()) {
                    barrierNodes.push_back(neighbor); // Add to barrierNodes when a barrier is unlocked
                }
                bool isNeighborValid = closedSet.find(neighbor) == closedSet.end();
                bool isBarrierUnlocked = neighbor->barrier_id == -1 || keys.find(neighbor->barrier_id) != keys.end();
                if (isNeighborValid && isBarrierUnlocked) {
                    openSet.push(neighbor);
                }
            }
        }
    }
    return false;
}

// Get a random node from the tree
Node* BarrierKeyManager::getRandomNode(Tree* tree) {
    size_t randomIndex = std::rand() % tree->allNodes.size();

    // Iterar hasta el elemento aleatorio
    auto it = tree->allNodes.begin();
    std::advance(it, randomIndex); // Avanzar iterador
    return *it;
}