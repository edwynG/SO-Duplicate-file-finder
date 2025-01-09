SRCDIR = src
OBJDIR = obj
BINDIR = bin
MD5LIBRARYDIR = resources/md5-lib/libmd5.a
FILE = main
T = 0
D = string
M = e

all: prepare $(BINDIR)/$(FILE)

clean:
	rm -rf $(OBJDIR) $(BINDIR)

prepare:
	mkdir -p $(OBJDIR) $(BINDIR)

$(BINDIR)/$(FILE): $(OBJDIR)/$(FILE).o
	gcc -o $@ $< $(MD5LIBRARYDIR)
	./$(BINDIR)/$(FILE) -t $(T) -d $(D) -m $(M)

$(OBJDIR)/$(FILE).o: $(SRCDIR)/$(FILE).c
	gcc -c -o $@ $<

# Compilar: gcc main.c -o main ../resources/md5-lib/libmd5.a
# Ejecutar: ./main -t int -d string -m char