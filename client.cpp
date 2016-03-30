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
#include "gameEnum.h"
#include "gameObjects.h"

#define NUM_INPUTS 6
#define BUFFER_SIZE 7
#define B_S_PLUS_1 BUFFER_SIZE * sizeof(float) + 1
#define NUM_ITEMS_IN_CHUNK 6

using namespace std;


// GLOBAL CONNECTION STUFF
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
bool inputs[NUM_INPUTS];
char tempBuffer[5] = {0, 0, 0, 0, 0};

// SYNCHRONIZATION STUFF
pthread_mutex_t mutex;
Asteroids *pAsteroids;

/**********************************************************************
 * What the thread does forever while the game is running. Get input
 * from the server, serialize it, then reset the game state
 **********************************************************************/
void * listen(void * unused)
{
  int numChunks = 0;

  float buffer[BUFFER_SIZE];
  while (true)
  {
    list<GameObject *> * bullets = new list<GameObject *>;
    list<GameObject *> * asteroids = new list<GameObject *>;
    list<GameObject *> * debris = new list<GameObject *>;
    list<Ship *> * players = new list<Ship *>;

    bzero(tempBuffer, 5);

    read(sockfd, tempBuffer, sizeof(int));

    // get the number of chunks
    numChunks = ((int *)(tempBuffer))[0];

    // inflate the right types
    for (int i = 0; i < numChunks; i++)
    {
      // get the type
      bzero(tempBuffer, 5);
      read(sockfd, tempBuffer, sizeof(TYPE));

      TYPE type = ((TYPE *)(tempBuffer))[0];

      bzero(buffer, B_S_PLUS_1);
      read(sockfd, buffer, (BUFFER_SIZE - 1) * sizeof(float));

      GameObject * obj;

      bool isPlayer = false;

      // Inflate the correct type of object
      switch (type)
      {
        case PLAYER:
        {
          Ship * pship = new Ship();
          pship->fromBytes(buffer);

          players->push_back(pship);

          isPlayer = true;

          break;
        }
        case BULLET:
        {
          obj = new Bullet();
          bullets->push_back(obj);
          break;
        }
        case SMALL_ASTEROID:
        {
          obj = new AsteroidS();
          asteroids->push_back(obj);
          break;
        }
        case MED_ASTEROID:
        {
          obj = new AsteroidM();
          asteroids->push_back(obj);
          break;
        }
        case LARGE_ASTEROID:
        {
          obj = new AsteroidL();
          asteroids->push_back(obj);
          break;
        }
        case MISSILE:
        {
          obj = new Missile();
          bullets->push_back(obj);
          break;
        }
        case DEBRIS:
        {
          obj = new Debris();
          debris->push_back(obj);
          break;
        }
        case DESTROYER:
        {
          obj = new Destroyer();
          asteroids->push_back(obj);
          break;
        }
        case SAUCER:
        {
          obj = new Saucer();
          asteroids->push_back(obj);
          break;
        }
      default:
	cerr << "Type not recognized" << endl;
      }

      // set all the members
      if (!isPlayer) 
      {
        obj->fromBytes(buffer);
      }
    }

    // get the score and lives number
    bzero(tempBuffer, 5);
    read(sockfd, tempBuffer, sizeof(int));
    int score = ((int *)(tempBuffer))[0];

    bzero(tempBuffer, 5);
    read(sockfd, tempBuffer, sizeof(int));
    int numLives = ((int *)(tempBuffer))[0];

    // CRITICAL SECTION
    pthread_mutex_lock(&mutex);
    //pAsteroids->setState(asteroids, bullets, debris, players, score, numLives);
    pAsteroids->asteroids = *asteroids;
    pAsteroids->bullets = *bullets;
    pAsteroids->debris = *debris;
    pAsteroids->players = *players;
    // todo score numlives
    pAsteroids->score = score;
    pAsteroids->lives = numLives;
    pthread_mutex_unlock(&mutex);

  }
}

/**********************************************************************
 * CALLBACK
 * The main interaction loop of the engine. Calls the
 * game ++ operator
 **********************************************************************/
void callBack(const Interface *pUI, void *p)
{
   pAsteroids = (Asteroids *)p;

   // send the user's input to the server
   inputs[0] = pUI->isUp();
   inputs[1] = pUI->isLeft();
   inputs[2] = pUI->isRight();
   inputs[3] = pUI->isDown();
   inputs[4] = pUI->isSpace();

   int n = write(sockfd, inputs, 5);
   if (n < 0)
     cerr << "Error writing" << endl;

   // Advance the Game
   pthread_mutex_lock(&mutex);
   (*pAsteroids)++;
   pthread_mutex_unlock(&mutex);

   /*
   // Rotate the ship
   pAsteroids->shipInput(pUI->isLeft(), pUI->isRight(), pUI->isUp(),
                    pUI->isDown(), pUI->isSpace());
   */
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
    cerr << " Waiting for go signal... " <<  endl;
    bzero(buffer, 2);
    n = read(sockfd, buffer, 1);
  } while (buffer[0] != '1');

  // DONE LOCKSTEP

   // Start the drawing
   Interface ui(argc, argv, "Asteroids");


   // start the thread
   pthread_t listen_thread;
   int id = pthread_create(&listen_thread, NULL, listen, NULL);

   if (id < 0)
     cerr << "Error creating thread" << endl;
   
   // play the game.  Our function callback will get called periodically
   Asteroids asteroids;
   ui.run(callBack, (void *)&asteroids);

   // end the thread

   return 0;
}
