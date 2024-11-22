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

using namespace std;

// Node class for the tree
// has a id and an array of 3 children
class Node{
public:
    int id;
    int barrier_id=0;
    tuple<int, int> pos;
    array<Node*, 3> children;

    Node(int id, tuple<int, int> pos) : id(id), pos(pos) {
        children.fill(nullptr);
    }
};

// Adding a reference for colitioned childrens
static Node* collisionMarker = reinterpret_cast<Node*>(0x1);

class Tree{
public:
    //////////////////////////////////////////
    // Initialisation of the tree structure //
    //////////////////////////////////////////

    // initialize parameters
    Node* root;
    int lastNodeId, nodeCount;
    float lastCoef;
    unsigned int seed;
    set<tuple<int, int>> usedCoordinates;
    set<Node*> incompleteNodes;

    // Constructor
    // Tree(random_seed)
    Tree(unsigned int seed = static_cast<unsigned int>(time(0))) : seed(seed), lastNodeId(1),  nodeCount(1), lastCoef(0.0){
        usedCoordinates.insert(make_tuple(0, 0)); // La raíz está en (0, 0)
        root = new Node(lastNodeId, *usedCoordinates.begin()); // Nodo raíz con valor 1 y pos (0,0)
        incompleteNodes.insert(root); // La raíz es el primer nodo incompleto
        srand(seed); // inicializamos la funcion random
    }

    ///////////////////////////////////////////
    //// Destruction of the tree structure ////
    ///////////////////////////////////////////

    // Destructor
    ~Tree() {
        recursiveDeleteNode(this->root);
    }

    // Recursively delete dependant nodes
    void recursiveDeleteNode(Node* node) {
        if (!node || node == collisionMarker) return;

        for (Node* child : node->children) {
            recursiveDeleteNode(child);
        }

        nodeCount--;
        this->usedCoordinates.erase(node->pos);
        
        if (node->children.size() < 3) {
            this->incompleteNodes.erase(node);
        }
        delete node;
    }

    ///////////////////////////////////////////
    ////////// Generation of a Node ///////////
    ///////////////////////////////////////////

    // Check if coordinates are already used
    bool checkCollision(int x, int y) {
        return usedCoordinates.find(make_tuple(x, y)) != usedCoordinates.end();
    }

    // add a node as a child of the parent node
    int addNode(Node* parent, int direction = -1){
        // Handling errors ///
        // Check if the parent node is null
        if (!parent) {
            cerr << "Error: El nodo padre no puede ser nulo." << endl;
            return false;
        // Check if the parent node has already 3 children
        } else if (this->incompleteNodes.find(parent) == this->incompleteNodes.end()) {
            cerr << "Error: El nodo padre ya tiene 3 hijos; no se pueden agregar más." << endl;
            return false;
        }
        
        // initialize variables
        int childX = get<0>(parent->pos);
        int childY = get<1>(parent->pos);

        if (direction == -1) {
            vector<int> validDirections;
            // Check East direction
            if (parent->children[2] == nullptr) {
                if (!checkCollision(childX + 1, childY)) {
                    validDirections.push_back(2); // East
                } else {
                    parent->children[2] = collisionMarker;
                }
            }

            // Check South direction
            if (parent->children[1] == nullptr) {
                if (!checkCollision(childX, childY - 1)) {
                    validDirections.push_back(1); // South
                } else {
                    parent->children[1] = collisionMarker;
                }
            }

            // Check West direction
            if (parent->children[0] == nullptr) {
                if (!checkCollision(childX - 1, childY)) {
                    validDirections.push_back(0); // West
                } else {
                    parent->children[0] = collisionMarker;
                }
            }

            // If no valid directions, return false
            if (validDirections.empty()) {
                cerr << "Error: El nodo colisiona con los espacios libres; no se pueden agregar más." << endl;
                if(parent->children[0] && parent->children[1] && parent->children[2]){
                    this->incompleteNodes.erase(parent);
                }
                return false;
            }

            // Choose a random valid direction
            direction = validDirections[rand() % validDirections.size()];

            switch (direction) {
                case 0: childX -= 1; break; // West
                case 1: childY -= 1; break; // South
                case 2: childX += 1; break; // East
            }
        } else {
            switch (direction) {
                case 0: childX -= 1; break; // West
                case 1: childY -= 1; break; // South
                case 2: childX += 1; break; // East
                default: cerr << "Error: Dirección inválida." << endl; return false; 
            }
            if (parent->children[direction] == nullptr) {
                if (checkCollision(childX, childY)) {
                    parent->children[direction] = collisionMarker;
                    cerr << "Error: El nodo colisiona" << endl;
                    return false;
                }
            } else if (parent->children[direction] == collisionMarker) {
                cerr << "Error: El nodo colisiona" << endl;
                return false;
            } else {
                cerr << "Error: El nodo ya tiene un hijo en la dirección " << direction << "." << endl;
                return false;
            }
        }

        this->lastNodeId++;
        this->nodeCount++;

        Node* child = new Node(this->lastNodeId, make_tuple(childX, childY));
        parent->children[direction] = child; // Add the new node to the parent's children
        usedCoordinates.insert(make_tuple(childX, childY)); // Mark the coordinates as used
        incompleteNodes.insert(child); // Add the new node to the incomplete nodes
        if(parent->children[0] && parent->children[1] && parent->children[2]){
            this->incompleteNodes.erase(parent);
        }
        return true;
    }

    // Function to add a node randomly into the tree
    int randomNodeGen(){
        // choose a random node from the incomplete nodes
        auto it = incompleteNodes.begin();
        advance(it, rand() % incompleteNodes.size());
        Node* selectedNode = *it;

        // return exit code
        return addNode(selectedNode) ? true : false;
    }

    // Function to insert a subtree from a specific node
    bool insertSubtree(Node* parent, Node* subtreeRoot) {
        if (!parent || !subtreeRoot) {
            cerr << "Error: Parent node or subtree root cannot be null." << endl;
            return false;
        }

        // Use a queue to perform a level-order traversal of the subtree
        queue<pair<Node*, Node*>> nodesQueue;
        nodesQueue.push(make_pair(parent, subtreeRoot));

        while (!nodesQueue.empty()) {
            auto currentPair = nodesQueue.front();
            nodesQueue.pop();

            Node* currentParent = currentPair.first;
            Node* currentSubtreeNode = currentPair.second;

            // Try to add the current subtree node to the current parent
            bool added = false;
            for (int direction = 0; direction < 3; ++direction) {
                if (addNode(currentParent, direction)) {
                    added = true;
                    break;
                }
            }

            if (!added) {
                cerr << "Error: Could not add subtree node with id " << currentSubtreeNode->id << endl;
                return false;
            }

            // Add the children of the current subtree node to the queue
            for (int i = 0; i < 3; ++i) {
                if (currentSubtreeNode->children[i] != nullptr) {
                    nodesQueue.push(make_pair(currentParent->children[i], currentSubtreeNode->children[i]));
                }
            }
        }

        return true;
    }

    ///////////////////////////////////////////
    /////// Calculation of linear coef ////////
    ///////////////////////////////////////////

    // Function to average number of children in the tree
    float Coef(){
        float coef = 0.0;
        int totalNodes = 0;

        // Calculamos el coeficiente total basado en todos los nodos (sin contar las hojas)
        recursiveCoefCount(this->root, coef, totalNodes);
        coef /= totalNodes;

        // Validamos el rango del coeficiente y ajustamos si es necesario
        if (coef < 1.0 || coef > 3.0) {
            cerr << "Error: El coeficiente calculado es fuera del rango permitido [1.0, 3.0], ajustando a "
                << (coef < 1.0 ? "1.0" : "3.0") << "." << endl;
            return -1.0;
        }

        this->lastCoef = coef;
        return coef;
    }

    // Function to average number of children recursively
    void recursiveCoefCount(Node* node, float& coef, int& totalNodes){
        if(!node) return;
        int childSize = node->children.size();

        if(childSize > 0){
            coef += childSize;
            totalNodes++;
        }else{ 
            return;
        }

        // Recurre sobre los hijos
        for(Node* child : node->children){
            recursiveCoefCount(child, coef, totalNodes);
        }
    }

    
    ///////////////////////////////////////////
    //////////////// Mutation  ////////////////
    ///////////////////////////////////////////

    // Mutate the tree by adding or removing a leaf
    int mutate() {
        if (rand() % 2 == 0) {
            // Add a leaf
            return randomNodeGen();
        } else {
            // Remove a leaf
            return removeLeaf();
        }
    }

    // Remove a leaf from the tree
    int removeLeaf() {
        if (incompleteNodes.empty()) return 1;

        // Choose a random incomplete node
        auto it = incompleteNodes.begin();
        advance(it, rand() % incompleteNodes.size());
        Node* selectedNode = *it;

        // Remove the node
        recursiveDeleteNode(selectedNode);
        return 0;
    }


    ///////////////////////////////////////////
    //// Auxiliary visualization functions ////
    ///////////////////////////////////////////

    // Función que genera el archivo .dot
    void dotGen(const string& fileName){
        ofstream dotFile(fileName);
        if(!dotFile){
            cerr << "Error al crear el archivo " << fileName << endl;
            return;
        }

        // Inicio del archivo .dot
        dotFile << "digraph G {\n";
        dotFile << "node [shape=circle];\n";  // Forma de los nodos

        // Usamos un set para asegurarnos de que no se repitan conexiones
        set<pair<int, int>> conections;

        // Llamamos a la función recursiva para crear los nodos y sus conexiones
        recursiveDotGen(this->root, dotFile, conections);

        // Final del archivo .dot
        dotFile << "}\n";
        dotFile.close();
    }

    // Función recursiva que escribe los nodos y las conexiones al archivo .dot
    void recursiveDotGen(Node* node, ofstream& dotFile, set<pair<int, int>>& conections){
        if(!node) return;

        // Escribir el nodo en el archivo .dot
        dotFile << "  " << node->id << ";\n";

        // Recorrer a los hijos y agregar las conexiones
        for(Node* child : node->children){
            // Para evitar loops y referencias incorrectas, aseguramos que cada conexión solo se añada una vez
            pair<int, int> conection = make_pair(node->id, child->id);
            if(conections.find(conection) == conections.end()){
                dotFile << "  " << node->id << " -> " << child->id << ";\n";
                conections.insert(conection);  // Guardar la conexión para evitar duplicados
            }

            // Llamar recursivamente para generar los hijos
            recursiveDotGen(child, dotFile, conections);
        }
    }
};
