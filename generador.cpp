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

class GenForest {
public:
    std::vector<Tree*> forest;
    unsigned int seed;
    float stdDev;

    // Constructor de GenForest con parámetros ajustables
    GenForest(int n, int x, float c, unsigned int seed = static_cast<unsigned int>(time(0)), int stdDev=0.2)
        : seed(seed), stdDev(stdDev)  // Inicialización de semilla y desviación estándar
    {
        srand(this->seed);
        std::cout << "Semilla utilizada: " << this->seed << std::endl;

        // Crear las carpetas necesarias
        createDir();

        for (int i = 0; i < n; i++) {
            Tree* tree = genRandomTree(x, this->seed, this->stdDev);
            forest.push_back(tree);

            std::cout << "Árbol " << i + 1 << " - Total de nodos: "
                      << forest[i]->nodeNum()
                      << ", Coeficiente: " << forest[i]->Coef() << std::endl;

            // Genera archivo .dot en la carpeta poblacion/.dot
            std::string dotFileName = "poblacion/.dot/arbol" + std::to_string(i + 1) + ".dot";
            forest[i]->dotGen(dotFileName);

            // Convierte archivo .dot a .png en la carpeta poblacion/
            std::string pngFileName = "poblacion/arbol" + std::to_string(i + 1) + ".png";
            Utilidades::convertirDotAPng(dotFileName, pngFileName);
        }
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
    Tree* genRandomTree(int roomCount, unsigned int seed, double standardDev) {
        // Generar el número de nodos usando una distribución normal
        std::mt19937 gen(seed);
        std::normal_distribution<> dist(roomCount, standardDev);
        int nodesCount = std::round(dist(gen));

        // Generar el árbol con la semilla dada
        Tree* tree = new Tree(seed);
        for (int i = 0; i < nodesCount; i++) {
            tree->randomNodeGen();
        }
        return tree;
    }
};

int main() {
    int numArboles = 5;
    int habitaciones = 10;
    float coeficiente = 1.5f;

    GenForest genForest(numArboles, habitaciones, coeficiente);
    return 0;
}