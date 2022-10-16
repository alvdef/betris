#------------------------------------------------------------------------------
# File:   Makefile
# Author: Álvaro de Francisco (838819)
# Date:   mayo 2022
# Coms:   Makefile para el trabajo final de Programación II
#------------------------------------------------------------------------------

#---------------------------------------------------------
# Definición de macros
CC = g++
CPPFLAGS = -I. -std=c++11      #opciones de compilación
LDFLAGS =                      #opciones de linkado
RM = rm -f                     #comando para borrar ficheros

all: mainB
#-----------------------------------------------------------
# Compilacion
mainB.o: mainB.cpp
	${CC} -c mainB.cpp ${CPPFLAGS}
# Linkado
mainB: mainB.o betris.o
	${CC} mainB.o betris.o -o mainB ${LDFLAGS}
betris.o: betris.cpp betris.hpp
	${CC} -c betris.cpp ${CPPFLAGS}
#-----------------------------------------------------------	
# LIMPIEZA de todo lo generado. Ojo: se borrarán sin preguntar
clean:
	${RM} betris.o mainB.o mainB
