#*****************************************************************************
#* fichier de creations du projet
#*****************************************************************************

#*****************************************************************************
#* options d'assemblage
#*****************************************************************************
#
OA      =       /c /Zf /Gd /Cp
OADEBUG =       $(OA) /Zi /Zd
OC      =       /w0
OCDEBUG =       $(OC) /d2
#
#*****************************************************************************
#* options de linkage
#*****************************************************************************
#
OL      =       sys dos op c
OLDEBUG =	$(OL) d a op m
#
#*****************************************************************************
#* liste des repertoires
#*****************************************************************************
#
DIR_ASM =       .
DIR_C   =       .
DIR_EXE =       .
DIR_LIB =       C:\DEV\LIB
#
#*****************************************************************************
#* liste des fichiers objet a linker
#*****************************************************************************
#
OBJVERIF        =       tetris.obj genl.obj mcga.obj
OBJ             =       tetris,genl,mcga
#
#*****************************************************************************
#* liste des fichiers cible
#*****************************************************************************
#
tetris:       $(DIR_EXE)\tetris.exe
#
#*****************************************************************************
#* assemblage des fichiers
#*****************************************************************************
#
tetris.obj:    	$(DIR_C)\tetris.c
		wcc $(OCDEBUG) $**

mcga.obj:    	$(DIR_LIB)\mcga.asm
		ml $(OADEBUG) $**

genl.obj:    	$(DIR_LIB)\genl.asm
		ml $(OADEBUG) $**

#*****************************************************************************
#* creation des fichiers executable
#*****************************************************************************
#
$(DIR_EXE)\tetris.exe:  $(OBJVERIF)
			wlink $(OLDEBUG) n tetris f $(OBJ)


#*****************************************************************************
#* fin du fichier de creation du projet
#*****************************************************************************
