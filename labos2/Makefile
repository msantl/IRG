PROJECT = labos2
SOURCE = vjezba2.cpp

CC = g++ 
CFLAGS = -Wall -g -c
LDFLAGS =
OBJECTS = ${SOURCE:.cpp=.o}

all: $(SOURCE) $(PROJECT)

$(PROJECT): $(OBJECTS)
		$(CC) $(OBJECTS) -framework GLUT -framework OpenGL -o $(PROJECT)

.cpp.o:
		$(CC) $(CFLAGS) $< -o $@

clean:
		-rm -f $(PROJECT) $(OBJECTS)
