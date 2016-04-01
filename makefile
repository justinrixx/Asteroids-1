########################################################################
# Program:
#    Networked Asteroids. What more can we say?
# Authors:
#    Nicholas Recht
#    Gage Peterson
#    Justin Ricks
# Summary:
#    This program is a simple game that simulates the classic Atari game
#    Asteroids. The user controls a ship which is able to fly around
#    the screen using the arrow keys. The object of the game is to
#    destroy asteroids that drift randomly around the screen. The
#    player can fire shots using the space bar and dies if they collide
#    with an Asteroid. As a bonus, you can now play with your friends
#    over the network!
#
########################################################################

deault:
	echo "run make server/client to compile the respective things"

########################################################################
# The game
########################################################################

## SERVER STUFF
server: asteroids-server.h gameObjects.o transform.o gameObjects.o asteroids-server.o uiInteract.o uiDraw.o ai.o server.cpp
	g++ -g -o server server.cpp asteroids-server.h gameObjects.o transform.o asteroids-server.o uiInteract.o uiDraw.o ai.o -lglut -lGLU -lGL -lpthread

asteroids-server.o: gameObjects.o asteroids-server.cpp
	g++ -g -c asteroids-server.cpp gameObjects.o

## CLIENT STUFF
client: transform.o gameObjects.o asteroids.o uiInteract.o uiDraw.o ai.o client.cpp
	g++ -g -o client client.cpp transform.o gameObjects.o asteroids.o uiDraw.o uiInteract.o ai.o -lglut -lGLU -lGL -pthread

#######################################################################
# Unit tests
#######################################################################
bytesTest: bytesTest.cpp transform.o gameObjects.o asteroids.o uiDraw.o uiInteract.o ai.o
	g++ -g -o bytesTest bytesTest.cpp transform.o gameObjects.o asteroids.o uiDraw.o uiInteract.o ai.o -lglut -lGLU -lGL -pthread

#######################################################################
# Seperately compiled files
#######################################################################

uiInteract.o: uiInteract.h uiInteract.cpp
	g++ -g uiInteract.cpp uiInteract.h -c

uiDraw.o: uiDraw.h uiDraw.cpp
	g++ -g uiDraw.cpp uiDraw.h -c

asteroids.o: asteroids.cpp asteroids.h gameObjects.h gameObjects.cpp
	g++ -g asteroids.cpp -c

ai.o: ai.cpp ai.h point.h gameObjects.h gameObjects.cpp
	g++ -g ai.cpp -c

gameObjects.o: gameObjects.cpp gameObjects.h transform.cpp transform.h point.h gameEnum.h
	g++ -g gameObjects.cpp -c

transform.o: transform.cpp transform.h point.h
	g++ -g transform.cpp -c

#######################################################################
# Extra Stuff
#######################################################################
clean:
	rm *.o server client *.gch
