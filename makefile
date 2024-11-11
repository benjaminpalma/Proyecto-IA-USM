# Nombre del ejecutable
EXEC = ejecutable.exe

# Archivos fuente
SOURCES = arbol.cpp generador.cpp utilidades.cpp main.cpp

# Compilador
CC = g++

# Opciones de compilación
CFLAGS = -Wall -std=c++11

# Reglas de compilación

all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)

# Limpia los archivos generados
clean:
	rm -f $(EXEC)