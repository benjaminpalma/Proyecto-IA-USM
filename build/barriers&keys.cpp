#include <iostream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <unordered_set>
#include "arbol.cpp"

using namespace std;

class BarrierKeyManager {
public:
    // Insert barriers and keys into the tree
    void insertBarriersAndKeys(Tree* tree, int barrierCount, int keyCount, int neededKeys) {
        if (!tree || !tree->root) return;

        // Insert barriers
        for (int i = 0; i < barrierCount; ++i) {
            Node* barrierNode = getRandomNode(tree->root);
            barrierNode->id = -1; // Mark node as a barrier
        }

        // Insert keys
        for (int i = 0; i < keyCount; ++i) {
            Node* keyNode = getRandomNode(tree->root);
            keyNode->id = -2; // Mark node as a key
        }

        // Ensure the tree is solvable by adjusting the needed keys
        this->neededKeys = neededKeys;
    }

    // Check if the tree is solvable using A* algorithm
    bool isSolvable(Tree* tree) {
        if (!tree || !tree->root) return false;

        // A* algorithm to check if all barriers can be opened
        priority_queue<pair<int, Node*>, vector<pair<int, Node*>>, greater<pair<int, Node*>>> openSet;
        unordered_set<Node*> closedSet;
        unordered_map<Node*, int> gScore;
        unordered_map<Node*, int> fScore;

        openSet.push({0, tree->root});
        gScore[tree->root] = 0;
        fScore[tree->root] = heuristic(tree->root);

        while (!openSet.empty()) {
            Node* current = openSet.top().second;
            openSet.pop();

            if (isGoal(current)) {
                return true;
            }

            closedSet.insert(current);

            for (Node* neighbor : current->children) {
                if (!neighbor || closedSet.find(neighbor) != closedSet.end()) continue;

                int tentative_gScore = gScore[current] + 1;

                if (gScore.find(neighbor) == gScore.end() || tentative_gScore < gScore[neighbor]) {
                    gScore[neighbor] = tentative_gScore;
                    fScore[neighbor] = gScore[neighbor] + heuristic(neighbor);
                    openSet.push({fScore[neighbor], neighbor});
                }
            }
        }

        return false;
    }

private:
    int neededKeys;

    // Get a random node from the tree
    Node* getRandomNode(Node* root) {
        vector<Node*> nodes;
        collectNodes(root, nodes);
        return nodes[rand() % nodes.size()];
    }

    // Collect all nodes in the tree
    void collectNodes(Node* node, vector<Node*>& nodes) {
        if (!node) return;
        nodes.push_back(node);
        for (Node* child : node->children) {
            collectNodes(child, nodes);
        }
    }

    // Heuristic function for A* algorithm
    int heuristic(Node* node) {
        return 0; // Simple heuristic: distance to goal (can be improved)
    }

    // Check if the current node is the goal
    bool isGoal(Node* node) {
        // Check if all barriers are opened
        return node->id == -1 && neededKeys <= 0;
    }
};