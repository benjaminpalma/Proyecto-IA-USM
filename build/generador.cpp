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

    // void mutateRemoveRoom(Tree* tree) {
    //     if (tree->root == nullptr) return;

    //     // Seleccionar un nodo aleatorio en el árbol
    //     Node* current = selectRandomNode(tree);

    //     // Desciende hasta encontrar una hoja
    //     Node* leaf = findLeaf(current);

    //     if (leaf) {
    //         // Eliminar la hoja encontrada del árbol
    //         removeNode(tree, leaf);
    //         cout << "Habitación (nodo) eliminada durante la mutación." << endl;
    //     }
    // }

    // // Función de mutación para agregar una habitación
    // void mutateAddRoom(Tree* tree) {
    //     if (tree->root == nullptr) return;

    //     // Seleccionar un nodo aleatorio
    //     Node* current = selectRandomNode(tree);

    //     // Buscar una posición disponible para un nuevo hijo
    //     Node* available = findAvailablePosition(current);

    //     if (available) {
    //         tree->lastNodeValue++;
    //         Node* newRoom = new Node();
    //         available->child.push_back(newRoom);
    //         cout << "Habitación (nodo) añadida durante la mutación." << endl;
    //     }
    // }

    // // Función para cruzar dos árboles
    // void crossover(Tree* parent1, Tree* parent2) {
    //     Node* crossoverNode1 = selectRandomNode(parent1);
    //     Node* crossoverNode2 = selectRandomNode(parent2);

    //     if (crossoverNode1 && crossoverNode2) {
    //         swapSubtrees(parent1, parent2, crossoverNode1, crossoverNode2);
    //         cout << "Cruce realizado entre los árboles." << endl;
    //     }
    // }

    // private:
    // Node* selectRandomNode(Tree* tree) {
    //     // Selecciona un nodo aleatorio en el árbol
    //     // Aquí podría implementarse un recorrido y selección aleatoria
    //     // ...
    // }

    // Node* findLeaf(Node* node) {
    //     // Encuentra una hoja descendiendo desde el nodo dado
    //     while (!node->child.empty()) {
    //         node = node->child[rand() % node->child.size()];
    //     }
    //     return node;
    // }

    // Node* findAvailablePosition(Node* node) {
    //     // Encuentra un nodo con espacio disponible para agregar un hijo
    //     while (node->child.size() == 3) {
    //         if (node->child.empty()) return nullptr;
    //         node = node->child[rand() % node->child.size()];
    //     }
    //     return node;
    // }

    // void removeNode(Tree* tree, Node* node) {
    //     // Remueve el nodo del árbol
    //     for (auto it = tree->incompleteNodes.begin(); it != tree->incompleteNodes.end(); ++it) {
    //         if (*it == node) {
    //             tree->incompleteNodes.erase(it);
    //             break;
    //         }
    //     }
    //     delete node;
    // }

    // void swapSubtrees(Tree* tree1, Tree* tree2, Node* node1, Node* node2) {
    //     // Realiza el intercambio de subárboles entre node1 y node2
    //     Node* temp = node1;
    //     node1 = node2;
    //     node2 = temp;
    // }
};