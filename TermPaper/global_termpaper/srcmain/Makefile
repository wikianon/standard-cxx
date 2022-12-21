#######################################################################################################
#                                                                                                    
# Autor: Jean Zonta                                                                                  
# Pseudonym: wiki.anon                                                                               
# Script Name: Makefile based on java makefile                                                       
# License: GNU/GPL V3                                                                                
# DateTime: sex 16 mar 2018 22:41:10
# echo "}oqo4gtutF_gnuu4ius4hx" | perl -pe \ 's/(.)/chr(ord($1)-2*3)/ge'                             
# echo 'comhttps://slackwarewww' | sed 's@\(com\)\(https://\)\(slackware\)\(www\)@\2\4.\3.\1 ..!!!@' 
#                                                                                                    
#######################################################################################################
#sources headers extensions .h .hh .h++ .hxx .hpp
HDRS = *.h*

#sources code extensions .c .cc .cpp .cxx .c++
SRC = *.c*  

#Objetos de compilação
OBJ = *.o

#restos de execução binária.
OUT = *.out

# compiler gcc-g++
GPP = g++
RM = rm -rf
NAME = termpaper

#Opções do compilador
LDLIBS = $(shell root-config --libs)
LDFLAGS = -g $(shell root-config --ldflags)
CPPFLAGS = -g $(shell root-config --cflags)

JOBS = j6
BIN = -o $(NAME)
LM = -lm
SDL = -lsdl
GL = –lglui –lglut32 –lopengl32
ARCH = arm
ARC32 = -m32
ARCH64 = -m64
MACROS  = -me
DEBUG = -g -ggdb
CFLAGS = -Wall -c $(DEBUG)
LFLAGS = -Wall $(DEBUG)
HOSTCXXFLAGS = -O8

#Encoding
IPTCHST = -finput-charset=UTF-8

all: $(NAME)

$(OBJ): $(SRC)
	@echo "Criando Objeto..."
	@$(GPP) $(CFLAGS) $(SRC) 

$(NAME): $(OBJ)
	 @echo "Criando binário..."
	 @$(GPP) $(BIN) $(OBJ) $(LFLAGS)

clean: 
	@echo "Remove Objetos..."
	@$(RM) $(OBJ) $(OUT)

	@echo "Remove binário..."
	@$(RM) $(NAME)
	
exec:
	@echo "Executando binário..."
	@./$(NAME)

del: 
	@echo "Remove binário..."
	@$(RM) $(NAME)

remove: 
	@echo "Removendo Todos Arquivos..."
	@$(RM) $(OBJ) $(NAME) $(OUT) $(NAME).tgz

tar:
	@echo "Compactando Arquivos..."
	@tar -zcvf $(NAME).tgz $(HDRS) $(NAME) $(SRC) 
	
deltar:
	@echo "Deletando tgz..."
	@$(RM) $(NAME).tgz

install:
	@echo "Instalando Arquivos..."
	sudo cp -r ../include/TermPaper /usr/include

unstall:
	@echo "Desinstalando Arquivos..."
	sudo rm -rf /usr/include/TermPaper
