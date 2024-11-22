#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <random>
#include <cmath>
#include <sys/stat.h>
#include <sys/types.h>
#include "arbol.cpp"
#include "utilidades.cpp"

class Forest {
public:

    ////////////////////////////////////////////
    // Initialisation of the Forest structure //
    ////////////////////////////////////////////

    std::vector<Tree*> forest;
    unsigned int seed;
    float stdDev;

    // Constructor de GenForest con parámetros ajustables
    Forest(int popSize, int roomNumber , unsigned int seed = static_cast<unsigned int>(time(0)), float stdDev=2.0)
        : seed(seed), stdDev(stdDev)  // Inicialización de semilla y desviación estándar
    {
        srand(this->seed);
        std::cout << "Semilla utilizada: " << this->seed << std::endl;

        // Crear las carpetas necesarias
        createDir();

        // Generamos el bosque (poblacion)
        genForest(popSize, roomNumber);
    }

    int genForest(int popSize, int roomNumber){
        for (int i = 0; i < popSize; i++) {
            Tree* tree = genRandomTree(roomNumber, this->seed, this->stdDev);
            this->forest.push_back(tree);

            std::cout << "Árbol " << i + 1 << " - Total de nodos: "
                      << this->forest[i]->nodeCount
                      << ", Coeficiente: " << this->forest[i]->Coef() << std::endl;

            // Genera archivo .dot en la carpeta poblacion/.dot
            std::string dotFileName = "poblacion/.dot/arbol" + std::to_string(i + 1) + ".dot";
            this->forest[i]->dotGen(dotFileName);

            // Convierte archivo .dot a .png en la carpeta poblacion/
            std::string pngFileName = "poblacion/arbol" + std::to_string(i + 1) + ".png";
            Utilidades::convertirDotAPng(dotFileName, pngFileName);
        }
        return 0;
    }

    // Función para crear las carpetas necesarias
    void createDir() {
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
    }

    // Función para generar un árbol aleatorio con un número de nodos basado en una desviación estándar
    Tree* genRandomTree(int roomCount, unsigned int seed, float stdDev) {
        // Generar el número de nodos usando una distribución normal
        // std::mt19937 gen(seed);
        // std::normal_distribution<> dist(roomCount, stdDev);
        // int nodesCount = std::round(dist(gen));
        
        // Hay algo raro con esa distribucion normal, por lo que por ahora no agregamos la opcion.
        int nodesCount = roomCount;

        // Generar el árbol con la semilla dada
        Tree* tree = new Tree(seed);
        for (int i = 1; i < nodesCount; i++) {
            tree->randomNodeGen();
        }
        return tree;
    }

    ///////////////////////////////////////////
    //////////////// Crossover ////////////////
    ///////////////////////////////////////////

    // Crossover two trees by exchanging subtrees
    void crossover(Tree* tree1, Tree* tree2) {
        if (!tree1->root || !tree2->root) return;

        // Choose random nodes from both trees
        Node* subtree1 = getRandomNode(tree1->root);
        Node* subtree2 = getRandomNode(tree2->root);

        // Swap the subtrees
        swapSubtrees(tree1, subtree1, tree2, subtree2);
    }

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

    // Swap subtrees between two trees
    void swapSubtrees(Tree* tree1, Node* subtree1, Tree* tree2, Node* subtree2) {
        if (!subtree1 || !subtree2) return;

        // Swap the subtrees
        Node* temp = subtree1;
        subtree1 = subtree2;
        subtree2 = temp;

        // Update the parent-child relationships
        updateParentChild(tree1, subtree1);
        updateParentChild(tree2, subtree2);
    }

    // Update the parent-child relationships after swapping subtrees
    void updateParentChild(Tree* tree, Node* subtree) {
        if (!subtree) return;

        // Update the parent-child relationships recursively
        for (Node* child : subtree->children) {
            updateParentChild(tree, child);
        }
    }
};