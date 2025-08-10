# Makefile for PeerToPeerCProgram


# Directories
ENCRYPTION_DIR = Encryption
NETWORKING_DIR = Networking
USERACTIVITY_DIR = UserActivity
UTILITY_DIR = UtilityFiles

# Source files
SRC = ChatMain.c \
	$(ENCRYPTION_DIR)/Encryption.c \
	$(NETWORKING_DIR)/Networking.c \
	$(USERACTIVITY_DIR)/UserSettings.c \
	$(UTILITY_DIR)/StringUtils.c \
	$(UTILITY_DIR)/UtilityFunctions.c

# Header include paths
INCLUDES = -I. \
	     -I$(ENCRYPTION_DIR) \
	     -I$(NETWORKING_DIR) \
	     -I$(USERACTIVITY_DIR) \
	     -I$(UTILITY_DIR)

# Output
TARGET = p2pchat.exe

# Compiler and flags
CC = gcc
CFLAGS = -Wall -g $(INCLUDES)
LDFLAGS = -lpthread

all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f $(TARGET)


.PHONY: all clean