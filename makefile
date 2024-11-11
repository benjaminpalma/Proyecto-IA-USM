# Nombre del ejecutable
EXEC = ejecutable

# Archivos fuente
SOURCES = build/*

ifeq ($(shell which g++),)
  CC = gcc
  CFLAGS = -std=c++11 -xc++ -lstdc++ -shared-libgcc
else
  CC = g++
  CFLAGS = -std=c++11
endif

# Reglas de compilación

all: $(EXEC)

$(EXEC): $(SOURCES)
	$(CC) $(CFLAGS) $(SOURCES) -o $(EXEC)

# Limpia los archivos generados
clean:
	rm -f $(EXEC)