#Compilador
CC = gcc

#Arquitetura
OS_BITS = x64

#Definição de Macros
OPTIONS_FLAGS = -D_DEBUG

#Diretório de saída dos arquivos .o
OBJ_DIR = Obj/

#Pasta da PIG
PIG = PigDev/

#Pastas dos headers
INCLUDE_DIR = -IIncludes/ -I$(PIG)/include -I$(PIG)/include/SDL -I$(PIG)/include/SDL_image -I$(PIG)/include/video

#Pasta dos sources
SOURCE_DIR = Src/

#Bibliotecas
LIB_FLAGS = -pthread -lm -lavcodec -lavdevice -lavfilter -lavformat -lavutil -lswresample -lswscale -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -lSDL2_mixer -lSDL2_net

#Pasta das libs para arquitetura selecionada
CC_LIB = -L$(PIG)/libLinux/$(OS_BITS)
LIB_DEST_DIR = /usr/local/lib/

#Saída do jogo principal
OUTPUT_FILE = Physishot

#Flags da warning
WARNINGS = -Wall

physics.o: $(SOURCE_DIR)/physics.c
	$(CC) $(WARNINGS) -c $(SOURCE_DIR)/physics.c $(OPTIONS_FLAGS) $(INCLUDE_DIR) -o $(OBJ_DIR)/physics.o

all: main.c physics.o
	$(CC) $(WARNINGS) $(OBJ_DIR)*.o $(SOURCE_DIR)/main.c $(OPTIONS_FLAGS) $(INCLUDE_DIR) -o $(OUTPUT_FILE) $(CC_LIB) $(LIB_FLAGS)
