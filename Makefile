SRCDIR = src
OBJDIR = obj
BINDIR = bin

CFLAGS = -Wall -lpthread -I ./include  # Agrega el directorio de las cabeceras
SOURCE = $(wildcard $(SRCDIR)/*.c) # Lista de achivos fuentes
OBJ = $(SOURCE:$(SRCDIR)/%.c=$(OBJDIR)/%.o) # Aplica un map en SOURCE y crea archivos objetos

# $(info SRC = $(SOURCE)) ##Debug para visualizar las varibales
# $(info OBJ = $(OBJ))

MD5LIBRARYDIR = resources/md5-lib/libmd5.a
FILE = main
T = 0
D = string
M = e

all: prepare $(BINDIR)/$(FILE)

clean:
	rm -rf $(OBJDIR) $(BINDIR) || true

prepare:
	mkdir -p $(OBJDIR) $(BINDIR)

$(BINDIR)/$(FILE): $(OBJ)
	gcc $(CFLAGS) -o $@ $^ $(MD5LIBRARYDIR)
	
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

execute: $(BINDIR)/$(FILE)
	./$(BINDIR)/$(FILE) -t $(T) -d $(D) -m $(M)

##! CONTRUCCION Y PRUEBAS DESDE LA CONSOLA
#* Compilar: gcc main.c -o main ../resources/md5-lib/libmd5.a
#* Ejecutar: ./main -t int -d string -m char