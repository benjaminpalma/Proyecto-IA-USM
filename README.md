# Proyecto de Generación de Mazmorras en C++

## Descripción
El programa crea representaciones de una mazmorra 2D mediante arboles, explciado en Felipe Dumont and María-Cristina Riff. 2024. 2-Step Evolutionary Algorithm for the generation of dungeons with lock door missions using horizontal symmetry. In Proceedings of the Genetic and Evolutionary Computation Conference (GECCO '24). Association for Computing Machinery, New York, NY, USA, 1210–1218. https://doi.org/10.1145/3638529.3654153.

Este programa genera árboles aleatorios (con una visualizacion en la carpeta "poblacion" dentro de el repositorio base) con la finalidad de aplicar a futuro mutaciones en conjunto de cruces, y posteriomente (aqui es donde nosotros nos diferenciamos en relacion al paper) seleccionar al mejor individuo de la poblacion para la funcion de evaluacion unicamente enfocado en la cantidad de habitaciones y coeficiente lineal. A continuacion vendra la parte de generar barreras y llaves, con un unico candidato, si no genera de esta manera una solucion entonces intenta con el segundo individuo mas aceptable.
Ha sido implementado en C++ y está diseñado específicamente para ejecutarse en entornos **UNIX**.

### Falta por implementar:
+ [ ] Evolucion (avanzado pero no funcional para mostrar)
+ [ ] Seleccion de la evolucion
+ [ ] Generacion de barreras y llaves a la solucion con algoritmos A*
+ [ ] Comprobar solucion
+ [ ] Generar una visualizacion*


## Dependencias
Para compilar y ejecutar este programa, es necesario tener instalado el compilador `g++` con soporte para C++11. En sistemas basados en UNIX (como Ubuntu), se puede instalar `g++` con el siguiente comando:


| Distribución | Comando para instalar `g++` |
|:------------|:---------------------------|
| **Ubuntu/Debian** | ```sudo apt-get update\nsudo apt-get install g++``` |
| **Arch Linux** | ```sudo pacman -S g++``` |
| **Red Hat/CentOS** | ```sudo yum install gcc-c++``` |
| **Fedora** | ```sudo dnf install gcc-c++``` |
| **OpenSUSE** | ```sudo zypper install gcc-c++``` |
| **MacOS (con Homebrew)** | ```brew install gcc``` |
| **FreeBSD** | ```sudo pkg install gcc``` |


Para mayor facilidad tambien se necesita el packete make, que se puede conseguir con el siguiente comando:

| Distribución | Comando para instalar `make` |
|:------------|:---------------------------|
| **Ubuntu/Debian** | ```sudo apt-get update\nsudo apt-get install make``` |
| **Arch Linux** | ```sudo pacman -S make``` |
| **Red Hat/CentOS** | ```sudo yum install make``` |
| **Fedora** | ```sudo dnf install make``` |
| **OpenSUSE** | ```sudo zypper install make``` |
| **MacOS (con Homebrew)** | ```brew install make``` |
| **FreeBSD** | ```sudo pkg install make``` |


## instalacion

Se puede obtener el ejecutable simplemente corriendo el comando, clonando el repositorio y accediento a este
```bach
git clone https://github.com/benjaminpalma/Proyecto-IA-USM.git; cd Proyecto-IA-USM; make; chmod +x; ./ejecutable
```

O si se descargo de manera local (como zip) hay que abrir una terminal en la carpeta descomprimida y ejecutar:
```bach
make; ./ejecutable
```

Si lo anterior no funciona, puede ser que no se haya creado con los permisos adecuados, o que se necesite compilar nuevamente para la maquina en ese caso bastara agregarle permisos de ejecucion:
```bach
make; chmod +x ./ejecutable; ./ejecutable
```
### Compilacion manual

Se puede compilar manualmente con el siguiente comando:
```bach
g++ build/* -o ejecutable; ./ejecutable
```
O
```bach
gcc -std=c++11 -xc++ -lstdc++ -shared-libgcc build/* -o ejecutable; ./ejecutable
```
