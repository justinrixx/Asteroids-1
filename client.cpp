/************************************************************************
* NetworkRoids Client 
*************************************************************************/

#include <ctype.h>
#include <iostream>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>  // printf
#include <stdlib.h> // exit, EXIT_FAILURE
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <pthread.h>

#include "uiInteract.h"
#include "uiDraw.h"
#include "asteroids.h"
//#include "gameEnum.h"

#define NUM_INPUTS 6
#define BUFFER_SIZE 4000
#define NUM_ITEMS_IN_CHUNK 7

using namespace std;


// GLOBAL CONNECTION STUFF
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
bool inputs[NUM_INPUTS];
char tempBuffer[5] = {0, 0, 0, 0, 0};

/**********************************************************************                                                                                * Listen
 * What the thread does forever while the game is running. Get input
 * from the server, serialize it, then reset the game state
 **********************************************************************/
void listen()
{
  int numChunks = 0; // Not used

  // TODO: Make sure this is big enough
  // TODO: is this the right type?
  float buffer[BUFFER_SIZE];
  while (true)
  {
    bzero(tempBuffer, 5);
    read(sockfd, tempBuffer, 4);

    // get the number of chunks
    // FLOAT -> INT?: 
    numChunks = (int)(tempBuffer)[0];

    // read the right number of chunks
    bzero(buffer, BUFFER_SIZE);
    read(sockfd, numChunks * NUM_ITEMS_IN_CHUNK * sizeof(float));

    // TODO inflate to the right types

    // get the score and lives number
    bzero(tempBuffer, 5);
    read(sockfd, tempBuffer, 4);
    int score = (int)(tempBuffer)[0];

    bzero(tempBuffer, 5);
    read(sockfd, tempBuffer, 4);
    int numLives = (int)(tempBuffer)[0];

    // TODO set the score and numLives
  }
}

/**********************************************************************
 * CALLBACK
 * The main interaction loop of the engine. Calls the
 * game ++ operator
 **********************************************************************/
void callBack(const Interface *pUI, void *p)
{
   Asteroids *pAsteroids = (Asteroids *)p;

   // send the user's input to the server
   inputs[0] = pUI->isUp();
   inputs[1] = pUI->isLeft();
   inputs[2] = pUI->isRight();
   inputs[3] = pUI->isDown();
   inputs[4] = pUI->isSpace();
   
   // Advance the Game
   (*pAsteroids)++;

   // Rotate the ship
   pAsteroids->shipInput(pUI->isLeft(), pUI->isRight(), pUI->isUp(),
                    pUI->isDown(), pUI->isSpace());
}

void error(string mess) {
  cout << mess << endl;
  exit(1);
}

/*********************************************************************
 * MAIN
 * initialize the drawing window, initialize
 * the game,and run it. Set up our callBack function.
 *********************************************************************/
int main(int argc, char **argv)
{

  // CONNECTION STUFF
  int sockfd, portno;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[2]; // the message buffer

  if (argc < 3) {
    printf("usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);

  if (server == NULL)
    {
      printf("ERROR, no such host\n");
      exit(0);
    }

  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;

  bcopy((char *)server->h_addr,
	(char *)&serv_addr.sin_addr.s_addr,
	server->h_length);

  serv_addr.sin_port = htons(portno);

  if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
  // END connection stuff

  // wait for the go signal
  do
  {
    bzero(buffer, 2);
    n = read(sockfd, buffer, 2);
  } while (buffer[0] != '1');

  // DONE LOCKSTEP

   // Start the drawing
   Interface ui(argc, argv, "Asteroids");

   // play the game.  Our function callback will get called periodically
   Asteroids asteroids;
   ui.run(callBack, (void *)&asteroids);

   return 0;
}
