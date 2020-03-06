###########################################################################
#!make
#
#!Makefile Name: makefile
#
HDF_HOME=/PGSWORK/FY4A/REFLIB
COM_HOME=/PGSWORK/FY4A

# Define internal macros
HDFINC=$(HDF_HOME)/include
HDFLIB=$(HDF_HOME)/lib
COMINCOFHDF=$(COM_HOME)/HEAD/PGSHDFCOMMON
COMINCOFNC=$(COM_HOME)/HEAD/PGSNCCOMMON
COMLIB=$(COM_HOME)/LIB
LOGINC=$(COM_HOME)/HEAD

# FLAGS	   
CC = xlC
FFLAGS = -c -O2 -q64

# HDF4/5 lib
# h4fc/h5fc -show
MYLIB = -L$(HDFLIB) -L$(COMLIB) -L/usr/local/lib -lhdf5 -lhdf5_hl -ljpeg -lz -lnetcdf -lpthread -lm -lfy4NCcomm -lFY4A_PGSCommon -lSYBODBC_NEW -lodbc

# Executable name
TARGET = FHS.e

# Define object files

OBJ =	MainFire.o\
	INIOperation.o \
	ProcessFire.o \


# Target file (executable)
$(TARGET): $(OBJ) 
	$(CC) $(OBJ) $(MYLIB) -q64 -o $@ 

# make clean
clean:
	-rm -f $(OBJ) *.mod
	-rm -f $(OBJ) *.o
	-rm -f $(OBJ) $(TARGET)   

#---------------------------------------------------
#-------------- compiler sequence ------------------
#---------------------------------------------------

#----------------- C code --------------------------
MainFire.o: MainFire.cpp MainFire.h
	$(CC) -I$(HDFINC) -I$(COMINCOFHDF) -I$(COMINCOFNC) -I$(LOGINC) -I/usr/local/include  -c -O2 -q64 MainFire.cpp
	 
INIOperation.o: INIOperation.cpp INIOperation.h
	$(CC) -I$(HDFINC) -I$(COMINCOFHDF) -I$(COMINCOFNC) -I$(LOGINC) -I/usr/local/include  -c -O2 -q64 INIOperation.cpp

ProcessFire.o: ProcessFire.cpp ProcessFire.h
	$(CC) -I$(HDFINC) -I$(COMINCOFHDF) -I$(COMINCOFNC) -I$(LOGINC) -I/usr/local/include  -c -O2 -q64 ProcessFire.cpp
	
	
#********************** END OF MAKEFILE ***************************
