CC = gcc
PROGRAM = fireball
CFLAGS  = -g -Wall -I/usr/X11R6/include -I/usr/pkg/include
LDFLAGS = -L/usr/X11R6/lib -L/usr/pkg/lib
LDLIBS  = -lglut -lGLU -lGL -lm

$(PROGRAM): fireball.o ball_functions.o player_functions.o weapon_functions.o background.o 
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^ $(LDLIBS)
	
fireball.o: fireball.c
	$(CC) $(CFLAGS) -c $^
	
ball_functions.o: ball_functions.c
	$(CC) $(CFLAGS) -c $^  

player_functions.o: player_functions.c 
	$(CC) $(CFLAGS) -c $^ 

weapon_functions.o: weapon_functions.c 
	$(CC) $(CFLAGS) -c $^   

background.o: background.c
	$(CC) $(CFLAGS) -c $^  	



clean:
	@rm -f *.o *~ *#