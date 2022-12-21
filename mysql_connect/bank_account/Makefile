SUB=%
OBJ=*.o
EOBJ=%.o

CXX=g++
SRC=*.c*
ESRC=%.c*

NAME=bank
EBIN=%$(NAME)
BIN=$(NAME)

all: $(NAME)

$(EOBJ):$(ESRC)

	$(CXX) -c $(SRC) -lmysqlclient

$(EBIN):$(EOBJ)

	$(CXX) -o $(BIN) $(OBJ) -lmysqlclient

$(SUB):$(EBIN)

	./$(BIN)
	rm -rf $* $(OBJ)