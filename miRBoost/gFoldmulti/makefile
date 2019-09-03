
#########################################
# Directory where to put the object files
OBJDIR=.

#########################################
# Directory where to locate source files
SRCDIR=.

##########
# Compiler
CC=g++

###############
# All the flags
STFLAGS= -DIBISC -DLENGTHTYPE=Uint
DEBUGFLAGS= -O5 -g -DDEBUG -Wall
CFLAGS =  $(STFLAGS) $(DEBUGFLAGS)

######################
# All the object files
OBJFILES=$(OBJDIR)/gFold.o $(OBJDIR)/Matrice.o $(OBJDIR)/Thermodynamic.o $(OBJDIR)/Palindrome.o $(OBJDIR)/Selection.o $(OBJDIR)/StructureRNA.o $(OBJDIR)/AllongeGraine.o $(OBJDIR)/Potentiel.o $(OBJDIR)/Hairpin.o $(OBJDIR)/Affichage.o $(OBJDIR)/Fichier.o 

#####################
# Commands
all: $(OBJFILES) gFold
objets: $(OBJFILES)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) -c $? -o $@

gFold: $(OBJFILES)
	      $(CC) $(CFLAGS) $(OBJFILES) -o $(OBJDIR)/$@ -lm

clean:
	/bin/rm -f $(OBJFILES) $(OBJDIR)/gFold *~
