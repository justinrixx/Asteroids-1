/*
	SERVER SIDE
*/

#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <pthread.h>

#include <vector>
#include <list>

#include "asteroids-server.h"

using namespace std;

#define MY_SOCK_PATH "."
#define LISTEN_BACKLOG 50
#define PLAYER_BUFFER_SIZE 5

/* 
	PlayerInput: holds the file desciptor and input bools for each player vector 
*/
struct PlayerInput
{
	bool down;
	bool up;
	bool left;
	bool right;
	bool space;
	int fd; 
	pthread_mutex_t mutex;
};


/*********************************************
 * Writes to all the players
 *********************************************/
void writeAll(const void * info, int size, const vector<PlayerInput *> & players)
{
   for (int i = 0; i < players.size(); ++i) {
      write(players[i]->fd, info, size);
   }
}

void writeAllGameObject(const list<GameObject *> & goList, const vector<PlayerInput *> & players)
{
   // for every player
   for (int i = 0; i < players.size(); ++i) {

      // for every game object...
      for (list<GameObject*>::const_iterator it = goList.begin(); it != goList.end(); ++it)
      {
         TYPE type = (*it)->getType();
         float * dat = (*it)->toBytes();

	 // NOTE: Hard coded value, 7! It happened to be the same everywhere
	 write(players[i]->fd, &type, sizeof(TYPE));
	 write(players[i]->fd, dat, 6 * sizeof(float));

         delete [] dat;
      }
   }
}

/* 
   Writes the current game state to each of the player's file descriptors
*/
void writeGameState(const Asteroids & asteroids, const vector<PlayerInput *> & players)
{
   int total = asteroids.asteroids.size() 
               + asteroids.bullets.size() 
               + asteroids.debris.size() 
               + players.size();
   
   writeAll(&total, sizeof(int), players);

   writeAllGameObject(asteroids.asteroids, players);

   writeAllGameObject(asteroids.bullets, players);

   writeAllGameObject(asteroids.debris, players);

   for (vector<Ship*>::const_iterator it = asteroids.players.begin(); it != asteroids.players.end(); ++it)
   {
      TYPE type = (*it)->getType();
      float * dat = (*it)->toBytes();
      
      for (int i = 0; i < players.size(); ++i)
      {
         write(players[i]->fd, &type, sizeof(int));
         write(players[i]->fd, dat, 6 * sizeof(float));
      }
      delete [] dat;
   }
   
   // score
   writeAll(&asteroids.score, sizeof(int), players);
   writeAll(&asteroids.lives, sizeof(int), players);

   // write end input
   char c = END_INPUT_CHAR;
   writeAll(&c, sizeof(char), players);
}

/*******************************************************************
* Respond to player input
*******************************************************************/
void *playerInputHandler(void *param)
{
   PlayerInput * player = (PlayerInput *)param;
   
   bool buffer [PLAYER_BUFFER_SIZE + 1];
   
   while (true)
   {  
      //Get the input from the players fd
      bzero(buffer, PLAYER_BUFFER_SIZE + 1);
      int n = read(player->fd, buffer, PLAYER_BUFFER_SIZE);
      
      pthread_mutex_lock(&(player->mutex));
      
      //CRITICAL SECTION
      player->up = buffer[0];
      player->left = buffer[1];
      player->right = buffer[2];
      player->down = buffer[3];
      player->space = buffer[4];
      
      //release the locks     
      pthread_mutex_unlock(&(player->mutex));
   }
}

/*
* handleError
* Outputs the given error message to cerr and exits the program
*/
void error(const char* msg)
{
	cerr << "Error: " << msg << endl;
	exit(0);
}

/*
* main
*/
int main(int argc, char **argv)
{
	int port = 0;
        int numPlayers = 1;
        
	if (argc > 1)
	{
           port = atoi(argv[1]);
	}
        if (argc > 2)
        {
           numPlayers = atoi(argv[2]);
        }
	else
           error("no port number specified");

	int sockfd, clifd1, clifd2, portno;
	socklen_t clilen1, clilen2;
	char buffer[2];
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	int n;
        socklen_t clilen;
        int clifd;
        struct sockaddr_in cli_addr;

	// create the socket
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sockfd < 0)
		error(strerror(errno));

	bzero((char *)&serv_addr, sizeof(serv_addr));
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind the socket
	if (bind(sockfd, (struct sockaddr *) &serv_addr,
		sizeof(serv_addr)) < 0)
		error(strerror(errno));

	// listen to the socket
	listen(sockfd, 5);

	//Set up Players
	vector<PlayerInput *> players(numPlayers);
        for (int i = 0; i < numPlayers; ++i)
           players[i] = new PlayerInput();

	// Accept Client 1
        for (int i = 0; i < numPlayers; ++i)
        {
           clilen = sizeof(cli_addr);
           clifd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
           if (clifd < 0)
              error(strerror(errno));
           players[i]->fd = clifd;
        }
	//clilen1 = sizeof(cli_addr1);
	//clifd1 = accept(sockfd, (struct sockaddr *) &cli_addr1, &clilen1);
	//if (clifd1 < 0)
	//	error(strerror(errno));
	//players[0]->fd = clifd1;
	
	// Accept Client 2
	//clilen2 = sizeof(cli_addr2);
	//clifd2 = accept(sockfd, (struct sockaddr *) &cli_addr2, &clilen2);
	//if (clifd2 < 0)
	//	error(strerror(errno));
	//players[1]->fd = clifd2;
        

	// Write back to the players that the game has started
        for (int i = 0; i < numPlayers; ++i)
        {
           n = write(players[i]->fd, "1", 1);
           if (n < 0)
              error("writing to the socket");
        }
	
	//
	// START THE GAME
	//
        vector<pthread_t> threads;
	//kick off the player input threads
	for (int i = 0; i < numPlayers; ++i)
	{
           threads.push_back(pthread_t());
           if (pthread_create(&threads[threads.size() - 1], NULL,
                              playerInputHandler, (void*)players[i]))
           {
              cout << "Error: unable to create the thread\n";
              exit(-1);
           }
	}

	//
	Asteroids asteroids(numPlayers);
        while (true)
	{
           // Advance the Game
           asteroids++;
           
           // Handle Player Input
           for (int i = 0; i < numPlayers; ++i)
           {
              asteroids.shipInput(
		    i,
		    players[i]->left,
		    players[i]->right, 
		    players[i]->up,
		    players[i]->down,
		    players[i]->space);
           }
           
           // Write the game state
           writeGameState(asteroids, players);
           
           //wait for refresh time here...
           usleep((1.0 / 30.0) * 1000000.);
        }
	
	//
	// END THE GAME
	//

	// close the sockets
        for (int i = 0; i < players.size(); ++i)
           close(players[i]->fd);
	close(sockfd);

	return 0;
}
