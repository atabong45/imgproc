# Nom de l'exécutable final
TARGET = imgproc

# Répertoire de sortie pour l'exécutable
BIN_DIR = bin

# Exécutable final avec son chemin
TARGET_EXEC = $(BIN_DIR)/$(TARGET)

# Compilateur
CC = gcc

# Options de compilation :
# -Wall -Wextra : Active tous les avertissements utiles. Indispensable.
# -g : Inclut les informations de débogage (pour gdb).
# -std=c99 : Spécifie la norme du langage C.
# -Iinclude : Dit au compilateur de chercher les fichiers .h dans le dossier 'include'.
CFLAGS = -Wall -Wextra -g -std=gnu99 -Iinclude 

# Répertoire des sources
SRC_DIR = src

# Répertoire des fichiers objets
BUILD_DIR = build

# Trouve automatiquement tous les fichiers .c dans src et ses sous-dossiers
SRCS = $(shell find $(SRC_DIR) -name '*.c')

# Génère les noms des fichiers objets correspondants dans le dossier 'build'
OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

# Règle principale : ce qui est exécuté quand on tape 'make'
all: $(TARGET_EXEC)

# Règle pour lier les fichiers objets et créer l'exécutable final
$(TARGET_EXEC): $(OBJS)
	@mkdir -p $(BIN_DIR) # Crée le dossier bin s'il n'existe pas
	$(CC) $(OBJS) -o $@ -lm

# Règle pour compiler les fichiers sources .c en fichiers objets .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(@D) # Crée le sous-dossier dans 'build' s'il n'existe pas
	$(CC) $(CFLAGS) -c $< -o $@

# Règle pour nettoyer le projet
clean:
	@echo "Nettoyage du projet..."
	rm -rf $(BUILD_DIR) $(BIN_DIR)

# Règle pour exécuter le programme
run: all
	./$(TARGET_EXEC)

