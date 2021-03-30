#Compilador
CC = g++

#Arquitetura
OS_BITS = x64

#Definição de Macros
OPTIONS_FLAGS = -D_DEBUG

#Diretório de saída dos arquivos .o
OBJ_DIR = Obj/

#Pasta da PIG
PIG = PigDev/

#Pastas dos headers
INCLUDE_DIR = -I Includes/ -I $(PIG)/include -I $(PIG)/include/SDL -I $(PIG)/include/SDL_image -I $(PIG)/include/video

#Pasta dos sources
SOURCE_DIR = Src/

#Bibliotecas
LIB_FLAGS = -pthread -lm -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net

#Pasta das libs para arquitetura selecionada
CC_LIB = -L$(PIG)/libLinux/$(OS_BITS)
LIB_DEST_DIR = /usr/local/lib/

#Saída do jogo principal
OUTPUT_FILE = Release/Physishot

#Opções de Build
BUILD_LINE = -W -std=c++11 -fpermissive


physics.o: $(SOURCE_DIR)/physics.c
	$(CC) $(BUILD_LINE) $(INCLUDE_DIR) -c $(SOURCE_DIR)physics.c -o $(OBJ_DIR)physics.o $(OPTIONS_FLAGS)

main.o: $(SOURCE_DIR)/main.c
	$(CC) $(BUILD_LINE) $(INCLUDE_DIR) -c $(SOURCE_DIR)main.c -o $(OBJ_DIR)main.o $(OPTIONS_FLAGS)

all: physics.o main.o
	$(CC) $(OBJ_DIR)main.o $(OBJ_DIR)physics.o -o $(OUTPUT_FILE) $(LIB_FLAGS) $(OPTIONS_FLAGS)
