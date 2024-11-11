#include <iostream>
#include <ctime>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <set>

using namespace std;

class Node{
public:
    int value;
    vector<Node*> child;

    Node(int value) : valor(value) {}
};

class Tree{
public:
    Node* root;
    int lastNodeValue;
    float lastCoef;
    unsigned int seed;
    set<Node*> incompleteNodes;

    Tree(unsigned int seed = static_cast<unsigned int>(time(0))) : seed(seed), lastNodeValue(1), lastCoef(0.0) {
        root = new Node(lastNodeValue); // Nodo raíz con valor 1
        incompleteNodes.insert(root); // La raíz es el primer nodo incompleto
        srand(seed);
    }

    ~Tree() {
        recuriveDeleteNode(root);
    }

    int nodeNum(){
        return lastNodeValue;
    }

    void recursiveCoefCount(Node* node, float& coef, int& totalNodes){
        if(!node) return;
        int childSize = node->child.size();

        if(childSize > 0){
            coef += childSize;
            totalNodes++;
        }else{ 
            return;
        }

        // Recurre sobre los hijos
        for(Node* child : node->child){
            recursiveCoefCount(child, coef, totalNodes);
        }
    }

    float Coef(){
        float coef = 0.0;
        int totalNodes = 0;

        // Calculamos el coeficiente total basado en todos los nodos (sin contar las hojas)
        recursiveCoefCount(this->root, coef, totalNodes);

        if(totalNodes == 0){ // Cuidado de dividir en 0
            cerr << "Error: Division por 0, en calculo de coeficiente lineal." << endl;
            return -1.0;
        }

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

    int randomNodeGen(){
        // Elegir un nodo aleatorio de la lista de nodos incompletos
        auto it = incompleteNodes.begin();
        advance(it, rand() % incompleteNodes.size());
        Node* selectedNode = *it;

        // Crear el nuevo nodo con el siguiente valor secuencial
        lastNodeValue++;
        Node* newNode = new Node(lastNodeValue);
        selectedNode->child.push_back(newNode); // Agregar el nuevo nodo como hijo

        // Agregar el nuevo nodo a la lista de nodos incompletos
        incompleteNodes.insert(newNode);

        // Si el nodo seleccionado ahora tiene 3 hijos, se elimina de la lista de incompletos
        if (selectedNode->child.size() == 3) {
            incompleteNodes.erase(selectedNode);
        }

        return 0;
    }

    int addNode(Node* node){
        if(node->child.size() == 3){
            cout << "El nodo ya tiene 3 hijos; no se pueden agregar más." << endl;
            return 1;
        }

        this->lastNodeValue++;

        // Crear el nuevo nodo con el siguiente valor secuencial
        Node* newNode = new Node(this->lastNodeValue);  // Incrementar el contador para que el nodo tenga un valor único
        node->child.push_back(newNode);// Agregar el nuevo nodo a los hijos del nodo actual
        if(node->child.size() == 3){
            this->incompleteNodes.erase(node);
        }
        return 0;
    }

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
        dotFile << "  " << node->value << ";\n";

        // Recorrer a los hijos y agregar las conexiones
        for(Node* child : node->child){
            // Para evitar loops y referencias incorrectas, aseguramos que cada conexión solo se añada una vez
            pair<int, int> conection = make_pair(node->value, child->value);
            if(conections.find(conection) == conection.end()){
                dotFile << "  " << node->value << " -> " << child->value << ";\n";
                conections.insert(conection);  // Guardar la conexión para evitar duplicados
            }

            // Llamar recursivamente para generar los hijos
            recursiveDotGen(child, dotFile, conections);
        }
    }

    void recuriveDeleteNode(Node* node) {
        if (!node) return;
        for (Node* child : node->child) {
            recuriveDeleteNode(child);
        }
        delete node;
    }
};
