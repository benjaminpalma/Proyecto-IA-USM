#include <set>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "forest.h"
#include "barrierskeys.h"

using namespace std;

struct ScoredTree {
    Tree* tree;
    double score;
    int index; // Posición original del árbol

    ScoredTree(Tree* t, double s, int i) : tree(t), score(s), index(i) {}
};

struct CompareScoredTree {
    bool operator()(const ScoredTree& a, const ScoredTree& b) const {
        return a.score < b.score; // Ordenar de menor a mayor
    }
};

void loadInstance(const string& line, int instance, int steps, int popSize, unsigned int seed, double stdDev, double crossoverProb, double mutationProb, int minMut, int maxMut, int searchTry) {
    int keysCount, roomCount, barrierCount, neededKeysCount;
    double linealC;
    
    size_t start = line.find('(');
    size_t end = line.find(')');
    if (start != string::npos && end != string::npos) {
        string params = line.substr(start + 1, end - start - 1);
        stringstream ss(params);
        char comma;

        ss >> roomCount >> comma >> keysCount >> comma >> barrierCount >> comma >> linealC >> comma >> neededKeysCount;
    }

    // Crear la población
    Forest* population = new Forest(steps, popSize, roomCount, seed, stdDev*instance);

    for (int i = 0; i < steps; ++i) {
        population->next_step(linealC, roomCount, crossoverProb, mutationProb, minMut, maxMut);
    }

    // Insertar barreras y llaves
    BarrierKeyManager manager(seed);

    // Verificar si los árboles son resolvibles
    if (population->bestTree) {
        for (int i = 0; i < searchTry; i++) {
            if (manager.insertBarriersAndKeys(population->bestTree, barrierCount, keysCount, neededKeysCount)) {
                cout << "La evaluación del mejor árbol: " << population->evaluateTree(population->bestTree, linealC, roomCount) << endl;
                cout << "El árbol es resolvible." << endl;
                population->createPlots(population->bestTree, "instance" + to_string(instance));
                delete population;
                return;
            }
        }
    }

    // Crear un set con el comparador personalizado
    std::set<ScoredTree, CompareScoredTree> scoredTrees;

    for (size_t i = 0; i < population->forest.size(); ++i) {
        Tree* tree = population->forest[i];
        double score = population->evaluateTree(tree, linealC, roomCount);
        scoredTrees.insert(ScoredTree(tree, score, i));
    }

    for (const ScoredTree& scoredTree : scoredTrees) {
        for (int i = 0; i < searchTry; i++) {
            if (manager.insertBarriersAndKeys(scoredTree.tree, barrierCount, keysCount, neededKeysCount)) {
                cout << "El árbol es resolvible." << endl;
                population->createPlots(scoredTree.tree, "instance" + to_string(instance));
                delete population;
                return;
            }
        }
    }

    cout << "Ningún árbol es resolvible." << endl;
    delete population;
    return;
}

int main() {

    int steps, popSize, roomCount, barrierCount, keysCount, neededKeysCount, minMut, maxMut, searchTry;
    double linealC;
    float stdDev, crossoverProb, mutationProb;
    unsigned int seed;
    char answer;
    Forest* population;
    
    cout << "Quiere configurar parametros extras? semilla y desviacion standar de iniciacion [y/n]";
    cin >> answer;

    if (answer == 'y' || answer == 'Y') {
        // Solicitar el número de llaves
        cout << "Ingrese una semilla (para la aleatoriedad): ";
        cin >> seed;

        // Solicitar el número de llaves
        cout << "Ingrese una desviacion estandar para la generacion inicial de habitaciones: ";
        cin >> stdDev;

        // Solicitar la probabilidad de crossover
        cout << "Ingrese la probabilidad de crossover: ";
        cin >> crossoverProb;

        // Solicitar la probabilidad de mutacion
        cout << "Ingrese la probabilidad de mutacion: ";
        cin >> mutationProb;

        // Solicitar el minimo de mutaciones
        cout << "Ingrese el minimo de mutaciones: ";
        cin >> minMut;

        // Solicitar el maximo de mutaciones
        cout << "Ingrese el maximo de mutaciones: ";
        cin >> maxMut;

        cout << "Ingrese el numero de pasos: ";
        cin >> steps;

        cout << "Ingrese el tamaño de la poblacion: ";
        cin >> popSize;

    } else {
        seed = static_cast<unsigned int>(time(0));
        steps = 30;
        popSize = 50;
        stdDev = 2.0;
        minMut = 5;
        maxMut = 15;
        crossoverProb = 0.5;
        mutationProb = 1;
        searchTry = 1000;
    }

    ifstream file("instancias.txt");
    if (!file.is_open()) {
        cerr << "Error opening file: instancias.txt" << endl;
        return 1;
    }

    string line;
    int instance_num = 1;

    // Volver al inicio del archivo
    file.clear();
    file.seekg(0, ios::beg);

    // Contar el número de instancias
    while (getline(file, line)) {
        loadInstance(line, instance_num, steps, popSize, seed, stdDev, crossoverProb, mutationProb, minMut, maxMut, searchTry);
        instance_num++;
    }

    // Volver al inicio del archivo
    file.clear();
    file.seekg(0, ios::beg);
}