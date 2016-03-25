/***********************************************************************
* Program:
*    Asteroids Class Header File 
*    Brother Helfrich, CS165
* Author:
*    Nicholas Recht
* Summary: 
*    This file contains the class declarations for the Asteroids Class.
************************************************************************/

#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include <vector>
#include <list>
#include "gameObjects.h"

#define MISSILECOOLDOWN 120
#define SPAWNTIME 90

using namespace std;

class Asteroids
{
  public:
  Asteroids(int numPlayers) : ship(), difficulty(0), score(0), missileTime(0), lives(3), spawnTime(0) 
  { 
	  for (int i = 0; i < numPlayers; ++i)
		  players.push_back(new Ship());
	  newWave(); 
  };

   void operator ++ (int postfix);
   void shipInput(int iPlayer, int left, int right, int up, int down, bool space);
   list<GameObject*> asteroids;
   list<GameObject*> bullets;
   list<GameObject*> debris;
   list<Ship *> players;

   int score;
   int lives;
  private:
   //Ship ship;
   void newWave();
   int difficulty;
   int missileTime;
   int spawnTime;
   void checkCollision();
   void wrap();
   void destroy();
};

#endif //ASTEROIDS_H
