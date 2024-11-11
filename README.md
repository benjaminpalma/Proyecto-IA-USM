# Proyecto de Generación de Mazmorras en C++

## Descripción
Este programa genera árboles aleatorios cona la finalidad de aplicar a futuro mutaciones y cruces, posteriomente se buscara obtener el mejor individuo de la poblacion para la funcion de evaluacion unicamente enfocado en la cantidad de habitaciones y coeficiente lineal. Posteriomente vendra la parte de generar barreras y llaves.
Ha sido implementado en C++ y está diseñado específicamente para ejecutarse en entornos UNIX.

## Dependencias
Para compilar y ejecutar este programa, es necesario tener instalado el compilador `g++` con soporte para C++11. En sistemas basados en UNIX (como Ubuntu), se puede instalar `g++` con el siguiente comando:


| Distribución | Comando para instalar `g++` |
|:------------|:---------------------------|
| **Ubuntu/Debian** | ```sudo apt-get update; sudo apt-get install g++``` |
| **Arch Linux** | ```sudo pacman -S gcc``` |
| **Red Hat/CentOS** | ```sudo yum install gcc-c++``` |
| **Fedora** | ```sudo dnf install gcc-c++``` |
| **OpenSUSE** | ```sudo zypper install gcc-c++``` |
| **MacOS (con Homebrew)** | ```brew install gcc``` |
| **FreeBSD** | ```sudo pkg install gcc``` |

Se puede obtener el ejecutable simplemente corriendo el comando
```bach
git clone https://github.com/benjaminpalma/Proyecto-IA-USM.git; cd Proyecto-IA-USM; make
```

O si se descargo de manera local hay que navegar hasta la carpeta base del proyecto y ejecutar:
```bach
make
```

Se puede compilar manualmente con el siguiente comando:
```bach
g++ arbol.cpp generador.cpp utilidades.cpp main.cpp -o ejecutable.exe
```
