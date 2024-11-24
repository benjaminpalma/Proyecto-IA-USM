
#include "forest.h"

////////////////////////////////////////////
// Initialisation of the Forest structure //
////////////////////////////////////////////

// Constructor de GenForest con parámetros ajustables
Forest::Forest(int steps, int popSize, int roomNumber, unsigned int seed, float stdDev, int depthOfSubtree)
    : popSize(popSize), roomNumber(roomNumber), seed(seed), stdDev(stdDev), depthOfSubtree(depthOfSubtree)  // Inicialización de semilla y desviación estándar
{
    srand(this->seed);
    std::cout << "Semilla utilizada: " << this->seed << std::endl;
    genForest(popSize, roomNumber);
    
}


////////////////////////////////////////////
//// Destructor of the Forest structure ////
////////////////////////////////////////////

Forest::~Forest() {
    for (Tree* tree : forest) {
        delete tree;
    }
}

////////////////////////////////////////////
///////////// Forest generation ////////////
////////////////////////////////////////////

// Función para generar el bosque
int Forest::genForest(int popSize, int roomNumber) {
    forest.clear();
    for (int i = 0; i < popSize; ++i) {
        unsigned int treeSeed = seed + i; // Usar una semilla única para cada árbol
        Tree* tree = new Tree(treeSeed);
        if(genRandomTree(roomNumber, treeSeed, stdDev, tree)) {
            cerr << "Error al generar el árbol " << i + 1 << std::endl;
            return EXIT_FAILURE;
        }
        forest.push_back(tree);
    }
    return forest.size();
}

// Función para generar un árbol aleatorio con ruido
int Forest::genRandomTree(int roomCount, unsigned int seed, float stdDev, Tree* tree) {
    // Inicializar el generador de números aleatorios con la semilla dada
    std::default_random_engine generator(seed);
    std::normal_distribution<float> distribution(roomCount, stdDev);

    // Generar la cantidad de nodos con ruido
    int nodesCount = std::round(distribution(generator));
    nodesCount = std::max(1, nodesCount); // Asegurarse de que haya al menos un nodo

    // Generar el árbol con la semilla dada
    for (int i = 1; i < nodesCount; i++) {
        tree->randomNodeGen();
    }
    return EXIT_SUCCESS;
}

////////////////////////////////////////////
//////////// Evolutionary step /////////////
////////////////////////////////////////////

// Función para evaluar un árbol
double Forest::evaluateTree(Tree* tree, float targetCoef, int targetRooms) {
    float coef = tree->Coef();
    int rooms = tree->nodeCount;
    return abs(coef - targetCoef) + abs(rooms - targetRooms);
}

// Función para tomar un paso en el proceso evolutivo del bosque
void Forest::next_step(float targetCoef, int targetRooms, float crossoverProb, float mutationProb, int min_mut, int max_mut) {
    // Evaluar todos los árboles en el bosque
    vector<pair<float, Tree*>> evaluations;
    for (Tree* tree : forest) {
        double score = evaluateTree(tree, targetCoef, targetRooms);
        evaluations.push_back(make_pair(score, tree));
    }

    // Calcular las probabilidades de selección
    float totalScore = 0.0;
    for (const auto& eval : evaluations) {
        totalScore += 1.0 / eval.first; // Invertir la puntuación para que menor sea mejor
    }

    vector<float> probabilities;
    for (const auto& eval : evaluations) {
        probabilities.push_back((1.0 / eval.first) / totalScore);
    }

    // Seleccionar los mejores árboles usando la ruleta
    vector<Tree*> survivors;
    std::default_random_engine generator(seed);
    std::discrete_distribution<int> distribution(probabilities.begin(), probabilities.end());

    int survivorsCount = (forest.size() + 3) / 4; // Redondear hacia arriba
    for (int i = 0; i < survivorsCount; ++i) {
        int selectedIndex = distribution(generator);
        survivors.push_back(evaluations[selectedIndex].second);
    }

    // Encontrar el mejor árbol y guardarlo
    bestTree = min_element(evaluations.begin(), evaluations.end(), [](const pair<float, Tree*>& a, const pair<float, Tree*>& b) {
        return a.first < b.first;
    })->second;

    // Generar nuevos árboles a partir de los sobrevivientes
    vector<Tree*> newForest;
    newForest.push_back(new Tree(*bestTree)); // Guardar el mejor árbol
        
    while (newForest.size() < forest.size()) {
        for (Tree* survivor : survivors) {
            if (newForest.size() >= forest.size()) break;
            Tree* newTree = new Tree(*survivor); // Copiar el árbol

            // // Aplicar crossover con probabilidad crossoverProb
            // if (static_cast<float>(rand()) / RAND_MAX < crossoverProb) {
            //     int partnerIndex = distribution(generator);
            //     Tree* partner = evaluations[partnerIndex].second;
            //     crossover(newTree, partner, depthOfSubtree);
            // }

            // Aplicar mutación con probabilidad mutationProb
            if (static_cast<float>(rand()) / RAND_MAX < mutationProb) {
                std::uniform_int_distribution<int> mutationDist(min_mut, max_mut);
                int mutationCount = mutationDist(generator);
                for (int i = 0; i < mutationCount; ++i) {
                    newTree->mutate();
                }
            }

            newForest.push_back(newTree);
        }
    }

    //Limpiar la memoria de los árboles no sobrevivientes
    for (Tree* tree : forest) {
        if (tree && tree != bestTree) {
            delete tree;
        }
    }

    forest = newForest;
}

///////////////////////////////////////////
//////////////// Crossover ////////////////
///////////////////////////////////////////

// Crossover two trees by exchanging subtrees
void Forest::crossover(Tree* tree1, Tree* tree2, int depthOfSubtree) {
    if (!tree1->root || !tree2->root) return;

    // Choose random nodes from both trees at the specified depth
    Node* subtree1 = getRandomNode(tree1->root, depthOfSubtree);
    Node* subtree2 = getRandomNode(tree2->root, depthOfSubtree);

    // Check for collisions and swap the subtrees if no collision
    if (!checkCollision(subtree1, subtree2)) {
        tree1->insertSubtree(subtree1->parent, subtree2);
        tree2->insertSubtree(subtree2->parent, subtree1);
    } else {
        // If collision, try another subtree
        int attempts = 0;
        while (attempts < 5) {
            subtree1 = getRandomNode(tree1->root, depthOfSubtree);
            subtree2 = getRandomNode(tree2->root, depthOfSubtree);
            if (!checkCollision(subtree1, subtree2)) {
                tree1->insertSubtree(subtree1->parent, subtree2);
                tree2->insertSubtree(subtree2->parent, subtree1);
                break;
            }
            attempts++;
        }
    }
}

// Check for collisions between two subtrees
bool Forest::checkCollision(Node* subtree1, Node* subtree2) {
    // Implement collision checking logic here
    // Return true if there is a collision, false otherwise
    return false;
}

// Get a random node from the tree at a specific depth from a leaf
Node* Forest::getRandomNode(Node* root, int depthOfSubtree) {
    vector<Node*> leaves;
    collectLeaves(root, leaves);

    // Select a random leaf
    Node* leaf = leaves[rand() % leaves.size()];

    // Move up to the specified depth
    Node* current = leaf;
    for (int i = 0; i < depthOfSubtree && current->parent; ++i) {
        current = current->parent;
    }

    return current;
}

// Collect all leaves in the tree
void Forest::collectLeaves(Node* node, vector<Node*>& leaves) {
    if (!node) return;
    bool isLeaf = true;
    for (Node* child : node->children) {
        if (child) {
            isLeaf = false;
            collectLeaves(child, leaves);
        }
    }
    if (isLeaf) {
        leaves.push_back(node);
    }
}

// // Swap subtrees between two trees
// void Forest::swapSubtrees(Tree* tree1, Node* subtree1, Tree* tree2, Node* subtree2) {
//     if (!subtree1 || !subtree2) return;

//     // Swap the subtrees
//     Node* temp = subtree1;
//     subtree1 = subtree2;
//     subtree2 = temp;

//     // Update the parent-child relationships
//     updateParentChild(tree1, subtree1);
//     updateParentChild(tree2, subtree2);
// }

// // Update the parent-child relationships after swapping subtrees
// void Forest::updateParentChild(Tree* tree, Node* subtree) {
//     if (!subtree) return;

//     // Update the parent-child relationships recursively
//     for (Node* child : subtree->children) {
//         if (child) {
//             child->parent = subtree;
//             updateParentChild(tree, child);
//         }
//     }
// }

////////////////////////////////////////////
/////////// Visualization functions ////////
////////////////////////////////////////////

// Función para crear las carpetas necesarias
void Forest::createPlots(Tree* tree, string name) {
    struct stat info;

    // Crear carpeta "poblacion" si no existe
    if (stat("poblacion", &info) != 0) {
        if (mkdir("poblacion", 0777) == -1) {
            std::cerr << "Error al crear la carpeta 'poblacion'" << std::endl;
        }
    }

    // Crear carpeta "poblacion/.dot" si no existe
    if (stat("poblacion/.dot", &info) != 0) {
        if (mkdir("poblacion/.dot", 0777) == -1) {
            std::cerr << "Error al crear la carpeta 'poblacion/.dot'" << std::endl;
        }
    }

    std::cout << "árbol " << name << " - Total de nodos: "
        << tree->nodeCount
        << ", Coeficiente: " << tree->Coef() << std::endl;

    // Genera archivo .dot en la carpeta poblacion/.dot
    std::string dotFileName = "poblacion/.dot/arbol-" + name +".dot";
    tree->dotGen(dotFileName);

    // Convierte archivo .dot a .png en la carpeta poblacion/
    std::string pngFileName = "poblacion/arbol-"+ name +".png";
    Utilidades::convertirDotAPng(dotFileName, pngFileName);

}