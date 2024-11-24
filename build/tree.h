#ifndef ARBOL_H
#define ARBOL_H

#include <iostream>
#include <ctime>
#include <array>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>
#include <tuple>
#include <queue>

enum Orientation {
    NORTH,
    SOUTH,
    EAST,
    WEST
};

class Node {
public:
    int id;
    int barrier_id = -1;
    int key_id = -1;
    std::tuple<int, int> pos;
    std::array<Node*, 3> children;
    Node* parent;
    Orientation orientation;

    Node(int id, std::tuple<int, int> pos, Orientation orientation, Node* parent = nullptr);
};

// Adding a reference for colitioned childrens
extern Node* COLLISION_MARKER;

class Tree {
public:
    Node* root;
    Node* exitNode = nullptr;
    int lastNodeId, nodeCount;
    float lastCoef, coefSum;
    unsigned int seed;
    std::set<std::tuple<int, int>> usedCoordinates;
    std::set<Node*> incompleteNodes;
    std::set<Node*> allNodes;
    std::set<Node*> leafNodes;

    Tree(unsigned int seed = static_cast<unsigned int>(time(0)));
    Tree(const Tree& other);
    ~Tree();

    void copyNodes(Node* thisNode, Node* otherNode);
    Node* findNode(Node* node, int id);
    void recursiveDeleteNode(Node* node, bool father = true);
    bool isCollisionOrNull(Node* node);
    bool isIncomplete(Node* node);
    bool isInSet(Node* node, std::set<Node*> set);
    bool areCollision(std::tuple<int, int> pos);
    int childCount(Node* node);
    int randomDirection(Node* node);
    bool addNode(Node* parent, int direction = -1);
    bool validDirection(Node* node, int direction);
    std::tuple<std::tuple<int, int>, Orientation> calculateNewPositionAndOrientation(std::tuple<int, int> parentPos, int direction, Orientation parentOrientation);
    int randomNodeGen();
    int mutate();
    bool insertSubtree(Node* parent, Node* subtreeRoot);
    int removeLeaf();
    float Coef();
    int countNodesWithChildren(Node* node);
    void dotGen(const std::string& fileName);
    void recursiveDotGen(Node* node, std::ofstream& dotFile, std::set<std::pair<int, int>>& conections);
};

#endif // ARBOL_H