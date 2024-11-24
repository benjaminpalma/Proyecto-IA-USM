#include "tree.h"
using namespace std;

// Node class for the tree
// has a id, barrier id and an array of 3 children
Node::Node(int id, tuple<int, int> pos, Orientation orientation,  Node* parent)
    : id(id), pos(pos), orientation(orientation), parent(parent) {
    children.fill(nullptr);
}

// Adding a reference for colitioned childrens
Node* COLLISION_MARKER = reinterpret_cast<Node*>(0x1);

//////////////////////////////////////////
// Initialisation of the tree structure //
//////////////////////////////////////////

// Constructor
// Tree(random_seed)
Tree::Tree(unsigned int seed) : seed(seed), lastNodeId(1),  nodeCount(1), lastCoef(1.0){
    usedCoordinates.insert(make_tuple(0, 0)); // La raíz está en (0, 0)
    root = new Node(lastNodeId, *usedCoordinates.begin(), NORTH); // Nodo raíz con valor 1 y pos (0,0)
    incompleteNodes.insert(root); // La raíz es el primer nodo incompleto
    leafNodes.insert(root);
    srand(seed); // inicializamos la funcion random
}

// Copy constructor
Tree::Tree(const Tree& other) 
    : lastNodeId(other.lastNodeId), nodeCount(other.nodeCount), lastCoef(other.lastCoef), 
      coefSum(other.coefSum), seed(other.seed), usedCoordinates(other.usedCoordinates) {
    
    // Copy the root node
    root = new Node(*other.root);
    
    // Copy the nodes recursively
    copyNodes(root, other.root);

    // Copy the sets of nodes
    for (Node* node : other.allNodes) {
        allNodes.insert(findNode(root, node->id));
    }
    // Copy the sets of nodes
    for (Node* node : other.incompleteNodes) {
        incompleteNodes.insert(findNode(root, node->id));
    }
    for (Node* node : other.leafNodes) {
        leafNodes.insert(findNode(root, node->id));
    }
}

// Helper function to copy nodes recursively
void Tree::copyNodes(Node* thisNode, Node* otherNode) {
    for (int i = 0; i < 3; ++i) {
        if (otherNode->children[i] != nullptr && otherNode->children[i] != COLLISION_MARKER) {
            thisNode->children[i] = new Node(*otherNode->children[i]);
            thisNode->children[i]->parent = thisNode;
            copyNodes(thisNode->children[i], otherNode->children[i]);
        } else {
            thisNode->children[i] = otherNode->children[i];
        }
    }
}

// Helper function to find a node by id
Node* Tree::findNode(Node* node, int id) {
    if (node == nullptr || node == COLLISION_MARKER) return nullptr;
    if (node->id == id) return node;
    for (Node* child : node->children) {
        Node* result = findNode(child, id);
        if (result != nullptr) return result;
    }
    return nullptr;
}

///////////////////////////////////////////
//// Destruction of the tree structure ////
///////////////////////////////////////////

// Destructor
Tree::~Tree() {
    recursiveDeleteNode(this->root, false);
}

// Recursively delete dependant nodes
void Tree::recursiveDeleteNode(Node* node, bool father) {
    // Check if the node is null or a collision marker
    if (isCollisionOrNull(node)) return;

    // Recursively delete the children of the node
    for (Node* child : node->children) {
        if (!isCollisionOrNull(child)) {//if the child is not null or collision marker
            recursiveDeleteNode(child, false);
        };
    }

    // update the atributes of the tree
    // dependend on the node
    this->nodeCount--;
    this->coefSum -= childCount(node);
    this->usedCoordinates.erase(node->pos);

    if (node->parent) {
        for (int i = 0; i < 3; ++i) {
            if (node->parent->children[i] == node) {
                node->parent->children[i] = nullptr;
                break;
            }
        }
    }

    // Remove the node from the incomplete nodes
    this->incompleteNodes.erase(node);

    // Remove the node from the leaf nodes
    this->leafNodes.erase(node);

    // remove the node from the set of all nodes
    this->allNodes.erase(node);

    if (father && node->parent) {
        this->incompleteNodes.insert(node->parent);
        if (childCount(node->parent) == 0) {
            this->leafNodes.insert(node->parent);
        }
        coefSum--;
        Coef(); // Calculate the new coefficient
    }

    delete node;
}

///////////////////////////////////////////
/////////// Auxiliary functions ///////////
///////////////////////////////////////////

// Check if a node is null or a collision marker
bool Tree::isCollisionOrNull(Node* node){
    return (!node || node == COLLISION_MARKER);
}

bool Tree::isIncomplete(Node* node){
    return (!node->children[0] || !node->children[1] || !node->children[2]);
}

bool Tree::isInSet(Node* node, set<Node*> set){
    return set.find(node) != set.end();
}

// Check if coordinates are already used
bool Tree::areCollision(tuple<int, int> pos) {
    return usedCoordinates.find(pos) != usedCoordinates.end();
}

int Tree::childCount(Node* node){
    int count = 0;
    for(Node* child : node->children){
        if (!isCollisionOrNull(child)) count++;
    }
    return count;
}

///////////////////////////////////////////
////////// Generation of a Node ///////////
///////////////////////////////////////////

int Tree::randomDirection(Node* node){
    vector<int> validDirections;
    int direction;

    // Check East direction
    if(validDirection(node, 2)) validDirections.push_back(2); //East
    if(validDirection(node, 1)) validDirections.push_back(1); //South
    if(validDirection(node, 0)) validDirections.push_back(0); //West

    // If no valid directions, return error
    if (validDirections.empty()) {
        cerr << "Error: El nodo colisiona con los espacios libres; no se pueden agregar más." << endl;
        return -1;
    }

    // Choose a random valid direction
    return validDirections[rand() % validDirections.size()];
}

// add a node as a child of the parent node
bool Tree::addNode(Node* parent, int direction){
    // Handling errors ///
    // Check if the parent node is null
    if (!parent) {
        cerr << "Error: El nodo padre no puede ser nulo." << endl;
        return EXIT_FAILURE;
    // Check if the parent node has already 3 children
    } else if (!isIncomplete(parent)) {
        cerr << "Error: El nodo padre ya tiene 3 hijos; no se pueden agregar más." << endl;
        return EXIT_FAILURE;
    } else if (direction < -1 || direction > 2) {
        cerr << "Error: Dirección inválida." << endl;
        return EXIT_FAILURE;
    }

    tuple<int, int> new_pos = parent->pos;

    // If direction is not specified, choose a random direction
    if (direction == -1) {
        // Giving a valid Directions to create a new node
        direction = randomDirection(parent);

        // If no valid directions, return false
        if(direction == -1){
            // this is a reparational method***
            // to extract the parent to incomplete nodes
            // due a collision
            if(isInSet(parent, incompleteNodes)){
                this->incompleteNodes.erase(parent);
            }
            return EXIT_FAILURE;
        }
    } else { // else, use the specified direction
        if(!validDirection(parent, direction)){
            return EXIT_FAILURE;
        }
    }
    tuple<tuple<int, int>, Orientation> aux = calculateNewPositionAndOrientation(new_pos, direction, parent->orientation);
    new_pos = get<0>(aux);
    Orientation new_orientation = get<1>(aux);
    
    // Create the new node and add it to the tree
    this->lastNodeId++;
    Node* child = new Node(this->lastNodeId, new_pos, new_orientation, parent);
    parent->children[direction] = child; // Add the new node to the parent's children

    // Update the tree attributes
    this->nodeCount++;
    this->coefSum++;
    this->usedCoordinates.insert(new_pos); // Mark the coordinates as used
    this->incompleteNodes.insert(child); // Add the new node to the incomplete nodes
    this->leafNodes.insert(child);
    this->leafNodes.erase(parent);
    this->allNodes.insert(child);
    if(!isIncomplete(parent)){
        this->incompleteNodes.erase(parent);
    }
    Coef(); // Calculate the new coefficient
    return EXIT_SUCCESS;
}

bool Tree::validDirection(Node* node, int direction){
    tuple<int, int> new_pos = node->pos;

    new_pos = get<0>(calculateNewPositionAndOrientation(new_pos, direction, node->orientation));

    if (node->children[direction] == nullptr) {
        if (!areCollision(new_pos)) {
            return true;
        } else {
            node->children[direction] = COLLISION_MARKER;
        }
    }
    return false;
}

tuple<tuple<int, int>, Orientation> Tree::calculateNewPositionAndOrientation(tuple<int, int> parentPos, int direction, Orientation parentOrientation) {
    int x = get<0>(parentPos);
    int y = get<1>(parentPos);
    Orientation childOrientation = NORTH; // Default orientation

    switch (parentOrientation) {
        case NORTH:
            switch (direction) {
                case 0: x -= 1; childOrientation = EAST; break;  // <- West move
                case 1: y -= 1; childOrientation = NORTH; break; // v South move
                case 2: x += 1; childOrientation = WEST; break;  // -> East move
            }
            break;
        case SOUTH:
            switch (direction) {
                case 0: x += 1; childOrientation = WEST; break;  // -> East move
                case 1: y += 1; childOrientation = SOUTH; break; // ^ North move
                case 2: x -= 1; childOrientation = EAST; break;  // <- West move
            }
            break;
        case EAST:
            switch (direction) {
                case 0: y += 1; childOrientation = SOUTH; break; // ^ North move
                case 1: x -= 1; childOrientation = EAST; break;  // <- West move
                case 2: y -= 1; childOrientation = NORTH; break; // v South move
            }
            break;
        case WEST:
            switch (direction) {
                case 0: y -= 1; childOrientation = NORTH; break; // v South move
                case 1: x += 1; childOrientation = WEST; break;  // -> East move
                case 2: y += 1; childOrientation = SOUTH; break; // ^ North move
            }
            break;
    }

    return make_tuple(make_tuple(x, y), childOrientation);
}

// Function to add a node randomly into the tree
int Tree::randomNodeGen() {
    // choose a random node from the incomplete nodes
    Node* selectedNode = nullptr;
    bool nodeAdded = 1;

    while (nodeAdded && incompleteNodes.size()) {
        auto it = incompleteNodes.begin();
        advance(it, rand() % incompleteNodes.size());
        selectedNode = *it;

        if (!isCollisionOrNull(selectedNode)) {
            nodeAdded = addNode(selectedNode);
        }
    }

    // return exit code
    return nodeAdded ? -1 : 0; // 0 if successful, -1 if failed
}

////////////////////////////////////////////
/////////// Mutation & crossover ///////////
////////////////////////////////////////////

// Mutate the tree by adding or removing a leaf
int Tree::mutate() {
    if (rand() % 2 == 0) {
        // Add a leaf
        return randomNodeGen();
    } else {
        // Remove a leaf
        return removeLeaf();
    }
}

// Function to insert a subtree from a specific node
bool Tree::insertSubtree(Node* parent, Node* subtreeRoot) {
    if (isCollisionOrNull(parent) || isCollisionOrNull(subtreeRoot)) {
        cerr << "Error: Parent node or subtree root cannot be null or a collision marker." << endl;
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

// Remove a leaf from the tree
int Tree::removeLeaf() {
    if (leafNodes.empty()) return 1;
    
    // Choose a random incomplete node
    auto it = leafNodes.begin();
    advance(it, rand() % leafNodes.size());
    Node* selectedNode = *it;

    if (selectedNode == this->root) {
        cerr << "Error: No se puede borrar la raiz" << endl;
        return 0;
    }

    // Remove the node
    recursiveDeleteNode(selectedNode);
    return 0;
}

///////////////////////////////////////////
/////// Calculation of linear coef ////////
///////////////////////////////////////////

// Function to average number of children in the tree
float Tree::Coef() {
    float coef = coefSum / (nodeCount - leafNodes.size());

    // Validate the coefficient range and adjust if necessary
    if (coef < 1.0 || coef > 3.0) {
        cerr << "Error: El coeficiente calculado es fuera del rango permitido [1.0, 3.0], ajustando a "
            << (coef < 1.0 ? "1.0" : "3.0") << "." << endl;
        return (coef < 1.0) ? 1.0 : 3.0;
    }

    this->lastCoef = coef;
    return coef;
}

// Recursive function to count the number of nodes with at least one child
int Tree::countNodesWithChildren(Node* node) {
    if (isCollisionOrNull(node)) return 0;

    int count = 0;
    bool hasChildren = false;
    for (Node* child : node->children) {
        if (!isCollisionOrNull(child)) {
            hasChildren = true;
            count += countNodesWithChildren(child);
        }
    }
    return hasChildren ? count + 1 : count;
}


///////////////////////////////////////////
//// Auxiliary visualization functions ////
///////////////////////////////////////////

// Función que genera el archivo .dot
void Tree::dotGen(const string& fileName){
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
void Tree::recursiveDotGen(Node* node, ofstream& dotFile, set<pair<int, int>>& conections){
    if(!node || node == COLLISION_MARKER) return;

    // Escribir el nodo en el archivo .dot
    if (node == this->root) {
        dotFile << "  " << node->id << " [label=\"In\", style=filled, fillcolor=lightgrey];\n";
    } else if (node == this->exitNode && node->key_id != -1 && node->barrier_id != -1) {
        dotFile << "  " << node->id << " [label=\"Out\\nK-" << node->key_id << "\\nB-" << node->barrier_id << "\", style=filled, fillcolor=grey];\n";
    } else if (node == this->exitNode && node->barrier_id != -1) {
        dotFile << "  " << node->id << " [label=\"Out\\nB-" << node->barrier_id << "\", style=filled, fillcolor=grey];\n";
    } else if (node == this->exitNode) {
        dotFile << "  " << node->id << " [label=\"Out\", style=filled, fillcolor=lightgrey];\n";
    } else if (node->key_id != -1 && node->barrier_id != -1) {
        dotFile << "  " << node->id << " [label=\"K-" << node->key_id << "\\nB-" << node->barrier_id << "\", style=filled, fillcolor=yellow];\n";
    } else if (node->key_id != -1) {
        dotFile << "  " << node->id << " [label=\"K-" << node->key_id << "\", style=filled, fillcolor=green];\n";
    } else if (node->barrier_id != -1) {
        dotFile << "  " << node->id << " [label=\"B-" << node->barrier_id << "\", style=filled, fillcolor=red];\n";
    } else {
        dotFile << "  " << node->id << ";\n";
    }

    // Recorrer a los hijos y agregar las conexiones
    for(Node* child : node->children){
        if (isCollisionOrNull(child)) continue;

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