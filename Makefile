# Dossiers
SRC_DIR = ./src
LTC_DIR = ./include/libtomcrypt
LTC_HEADERS = $(LTC_DIR)/src/headers
LTC_LIB = $(LTC_DIR)/libtomcrypt.a

# Compiler & flags
CC = gcc
CFLAGS = -I$(LTC_HEADERS) -O2
LDFLAGS = $(LTC_LIB) -fopenmp -lm

# Génère la liste des fichiers .c automatiquement
SRC = $(wildcard $(SRC_DIR)/*.c)
TARGET = HashDiag

# Build
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

rebuild: clean all

# Clean
clean:
	rm -f $(TARGET)
