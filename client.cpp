/************************************************************************
* NetworkRoids Client 
*************************************************************************/

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#include "uiInteract.h"
#include "uiDraw.h"
#include "asteroids.h"

#define NUM_INPUTS 6

using namespace std;


// GLOBAL CONNECTION STUFF
int sockfd, portno, n;
struct sockaddr_in serv_addr;
struct hostent *server;
bool[NUM_INPUTS] inputs;

/**********************************************************************
 * CALLBACK
 * The main interaction loop of the engine. Calls the
 * Skeet ++ operator and the gun input functions.
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

/*********************************************************************
 * MAIN
 * initialize the drawing window, initialize
 * the game,and run it. Set up our callBack function.
 *********************************************************************/
int main(int argc, char **argv)
{

  // CONNECTION STUFF
  int sockfd, portno, n;
  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[2]; // the message buffer

  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }

  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    error("ERROR opening socket");
  server = gethostbyname(argv[1]);

  if (server == NULL)
    {
      fprintf(stderr,"ERROR, no such host\n");
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
