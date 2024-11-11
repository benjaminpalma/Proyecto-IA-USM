#include <iostream>
#include "generador.cpp"

using namespace std;

int main(){
    int treeCount, roomCount, barrierCount, keysCount, neededKeysCount;
    unsigned int seed;
    float linealC, stdDev;
    char answer;

    while(true){
        // Solicitar el número de árboles en el bosque
        cout << "Ingrese el número de árboles en el bosque: ";
        cin >> treeCount;

        // Solicitar el número de habitaciones por árbol
        cout << "Ingrese el número de habitaciones por árbol: ";
        cin >> roomCount;

        // Solicitar el coeficiente lineal
        cout << "Ingrese el coeficiente lineal: ";
        cin >> linealC;

        // Solicitar el número de barreras
        cout << "Ingrese el número de barreras: ";
        cin >> barrierCount;

        // Solicitar el número de llaves
        cout << "Ingrese el número de llaves: ";
        cin >> keysCount;

        // Solicitar el número de llaves necesarias (debe estar entre el número de barreras y el número de llaves)
        do {
            cout << "Ingrese el número de llaves necesarias (entre " << barrierCount << " y " << keysCount << "): ";
            cin >> neededKeysCount;
            if (neededKeysCount < barrierCount || neededKeysCount > keysCount) {
                cout << "Error: El número de llaves necesarias debe estar entre " << barrierCount << " y " << keysCount << ". Intente de nuevo." << endl;
            }
        } while (neededKeysCount < barrierCount || neededKeysCount > keysCount);

        cout << "Quiere configurar parametros extras? semilla y desviacion standar de iniciacion [y/n]";
        cin >> answer;

        if(answer == 'y' || answer == 'Y'){
            // Solicitar el número de llaves
            cout << "Ingrese una semilla (para la aleatoriedad): ";
            cin >> seed;

            // Solicitar el número de llaves
            cout << "Ingrese una desviacion estandar para la generacion inicial de habitaciones: ";
            cin >> stdDev;

            // Crear la instancia de GenForest con los valores ingresados
            Forest generador(treeCount, roomCount, seed, stdDev);
        }else{
            Forest generador(treeCount, roomCount);

        }

        cout << "Bosque generado con éxito." << endl;

        cout << "Desea Reintentar? (y/n): ";
        cin >> answer;

        if (answer == 'n' || answer == 'N') {
            break;
        }
    }
    return 0;
}