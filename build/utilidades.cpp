#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

class Utilidades {
public:
    // Convierte un archivo .dot a .png utilizando el comando Graphviz `dot`
    static void convertirDotAPng(const string& nombreArchivoDot, const string& nombreArchivoPng) {
        // Construimos el comando que ejecutará Graphviz
        string comando = "dot -Tpng " + nombreArchivoDot + " -o " + nombreArchivoPng;
        
        // Ejecutamos el comando
        int resultado = system(comando.c_str());
        
        // Verificamos si la conversión fue exitosa
        if (resultado == 0) {
            cout << "Archivo " << nombreArchivoPng << " generado exitosamente." << endl;
        } else {
            cerr << "Error al generar el archivo " << nombreArchivoPng << ". Asegúrate de tener Graphviz instalado." << endl;
        }
    }
};
