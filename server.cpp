/*
	SERVER SIDE
*/

#include <iostream>
#include <cstdlib>
#include <cerrno>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>

#include <pthread.h>

#include <vector>

#include <asteroids-server.h>

using namespace std;

#define MY_SOCK_PATH "."
#define LISTEN_BACKLOG 50
#define PLAYER_INPUT_BUFFER_SIZE 5

/* PlayerInput: holds the file desciptor and input bools for each player vector */
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

void writeGameState(const Asteroids & asteroids)
{

}

/*******************************************************************
* Respond to player input
*******************************************************************/
void *playerInputHandler(void *param)
{
	bufferItem consumedItem;

	PlayerInput * player = (PlayerInput *)param; 

	bool[PLAYER_BUFFER_SIZE + 1] buffer; 

	while (true)
	{
		//Get the input from the players fd
		bzero(buffer, PLAYER_BUFFER_SIZE + 1);
		n = read(player->fd, buffer, PLAYER_BUFFER_SIZE);

		pthread_mutex_lock(&(player->mutex));
			
		//CRITICAL SECTION
		player->up = buffer[0];
		player->left = buffer[1];
		player->right = buffer[2];
		player->down = buffer[3];
		player->space = buffer[4];
		//
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

	if (argc > 1)
	{
		port = atoi(argv[1]);
	}
	else
		error("no port number specified");

	int sockfd, clifd1, clifd2, portno;
	socklen_t clilen1, clilen2;
	char buffer[2];
	struct sockaddr_in serv_addr, cli_addr1, cli_addr2;
	int n;

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
	vector<PlayerInput *> players(2);
	players[0] = new PlayerInput();
	players[1] = new PlayerInput();

	// Accept Client 1
	clilen1 = sizeof(cli_addr1);
	clifd1 = accept(sockfd, (struct sockaddr *) &cli_addr1, &clilen1);
	if (clifd1 < 0)
		error(strerror(errno));
	players[0]->fd = clifd1;
	
	// Accept Client 2
	clilen2 = sizeof(cli_addr2);
	clifd2 = accept(sockfd, (struct sockaddr *) &cli_addr2, &clilen2);
	if (clifd2 < 0)
		error(strerror(errno));
	players[1]->fd = clifd2; 

	// Write back to the players that the game has started
	n = write(clifd2, "1", 1);
	if (n < 0)
		error("writing to the socket");

	n = write(clifd1, "1", 1);
	if (n < 0)
		error("writing to the socket");
	
	//
	// START THE GAME
	//

	//kick off the player input threads
	for (int i = 0; i < 2; ++i)
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
	Asteroids asteroids;
	while (true)
	{
		// Advance the Game
		asteroids++;

		// Rotate the Gun
		asteroids.shipInput(pUI->isLeft(), pUI->isRight(), pUI->isUp(),
			pUI->isDown(), pUI->isSpace());
	}
	
	//
	// END THE GAME
	//
	n = write(endingPlayer, "e", 1);
	if (n < 0)
		error("writing to the socket");

	n = write(otherPlayer, "e", 1);
	if (n < 0)
		error("writing to the socket");

	// close the sockets
	close(clifd1);
	close(clifd2);
	close(sockfd);

	return 0;
}
