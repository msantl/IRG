PROJECT = labos1
SOURCE = IVector.cpp AbstractVector.cpp Vector.cpp IMatrix.cpp AbstractMatrix.cpp Matrix.cpp MatrixSubMatrixView.cpp MatrixTransposeView.cpp MatrixVectorView.cpp VectorMatrixView.cpp vjezba1.cpp

CC = g++
CFLAGS = -Wall -g -c
LDFLAGS =
OBJECTS = ${SOURCE:.cpp=.o}

all: $(SOURCE) $(PROJECT)

$(PROJECT): $(OBJECTS)
		$(CC) $(OBJECTS) -o $(PROJECT)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
		-rm -f $(PROJECT) $(OBJECTS)
