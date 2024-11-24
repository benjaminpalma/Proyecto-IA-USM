# Nombre del ejecutable
EXEC = ejecutable

SRC_DIR = build

# Archivos fuente
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Archivos objeto
OBJECTS = $(patsubst $(SRC_DIR)/%.cpp, $(SRC_DIR)/%.o, $(SOURCES))

ifeq ($(shell which g++),)
	CC = gcc
	CFLAGS = -std=c++11 -xc++ -lstdc++ -shared-libgcc
else
	CC = g++
	CFLAGS = -std=c++11
endif

all: $(EXEC)

# Reglas de compilación
$(EXEC): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

# Regla para compilar archivos .cpp a .o
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Limpia los archivos generados
clean:
	rm -f $(OBJECTS) $(EXEC)