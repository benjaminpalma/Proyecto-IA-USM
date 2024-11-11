#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <sys/stat.h>
#include <sys/types.h>
#include "arbol.cpp"
#include "utilidades.cpp"

using namespace std;

class GenForest {
public:
    vector<Tree*> forest;
    unsigned int seed;

    GenForest(int n, int x, float c) {
        seed = static_cast<unsigned int>(time(0));
        srand(seed);
        cout << "Semilla utilizada: " << seed << endl;

        // Crear las carpetas necesarias
        createDir();

        for (int i = 0; i < n; i++) {
            Tree* tree = genRandomTree(x, c, seed);
            forest.push_back(tree);
        }
    }

    // Función para crear las carpetas necesarias
    void createDir() {
        // Crear la carpeta "poblacion/.dot" si no existe
        struct stat info;
        if (stat("poblacion", &info) != 0) {
            mkdir("poblacion", 0777);  // Crear carpeta "poblacion"
        }
        if (stat("poblacion/.dot", &info) != 0) {
            mkdir("poblacion/.dot", 0777);  // Crear carpeta "poblacion/.dot"
        }

	return 0;
    }

    Tree* genRandomTree(int roomCount, float coef) {
        Tree* tree = new Tree();
	for (i = 0; i<roomCount; i++){}
            generarNodosAleatorios(arbol->raiz, habitacionesRestantes, coeficienteLineal, arbol->raiz);
        return tree;
    }

    // Función para generar nodos aleatorios con un valor secuencial.
void generarNodosAleatorios(Nodo* nodo, int& habitacionesRestantes, float coeficienteLineal, Nodo* padre, int& valorNodo) {
    if (habitacionesRestantes <= 0 || !nodo) return;

    // Generamos la cantidad de hijos a crear, con un valor de coeficiente aproximado
    int hijosACrear = static_cast<int>(coeficienteLineal + (rand() % 2));  
    hijosACrear = min(hijosACrear, habitacionesRestantes);

    for (int i = 0; i < hijosACrear && habitacionesRestantes > 0; i++) {
        int valorAleatorio = valorNodo++;  // Incrementamos el valor del nodo

        Nodo* hijoNuevo = new Nodo(valorAleatorio);
        if (hijoNuevo == nullptr) {
            cerr << "Error al crear un nodo hijo. Memoria insuficiente." << endl;
            return;
        }

        nodo->hijos.push_back(hijoNuevo);  // Asignamos el hijo al nodo actual
        habitacionesRestantes--;

        // Recursivamente generar nodos para este hijo
        generarNodosAleatorios(hijoNuevo, habitacionesRestantes, coeficienteLineal, hijoNuevo, valorNodo);
    }
  }
};

int main() {
    int numArboles = 5;
    int habitaciones = 10;
    float coeficiente = 1.5f;

    GeneradorArboles generador(numArboles, habitaciones, coeficiente);

    for (int i = 0; i < numArboles; i++) {
        cout << "Árbol " << i + 1 << " - Total de nodos: "
             << generador.arboles[i]->contarNodos(generador.arboles[i]->raiz)
             << ", Coeficiente: " << generador.arboles[i]->obtenerCoeficiente() << endl;

        // Genera archivo .dot en la carpeta poblacion/.dot
        string nombreArchivoDot = "poblacion/.dot/arbol_" + to_string(i + 1) + ".dot";
        generador.arboles[i]->generarDot(nombreArchivoDot);

        // Convierte archivo .dot a .png en la carpeta poblacion/
        string nombreArchivoPng = "poblacion/arbol_" + to_string(i + 1) + ".png";
        Utilidades::convertirDotAPng(nombreArchivoDot, nombreArchivoPng);
    }

    return 0;
}
