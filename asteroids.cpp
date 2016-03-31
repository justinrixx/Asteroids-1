/***********************************************************************
* Program:
*    Asteroids Class Source File
*    Brother Helfrich, CS165
* Author:
*    Nicholas Recht
* Summary: 
*    This file contains the class definitions for the Asteroids Class.
************************************************************************/

#include "asteroids.h"
#include "iostream"
#include "gameObjects.h"
#include "uiDraw.h"
using namespace std;

/**********************************************************************
 * ASTEROIDS::++  Increments the game by calling the individual object's
 *          increment functions and by decreasing the various timers
 *          used for the game
 ***********************************************************************/
void Asteroids::operator ++ (int postfix) // NOTE: "postfix" is not used currently
 {


   for (list<GameObject*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
      (*it)->increment(*this);

   for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
      (*it)->increment(*this);

   for (list<GameObject*>::iterator it = debris.begin(); it != debris.end(); ++it)
      (*it)->increment(*this);

//   checkCollision();
   wrap();
//   destroy();
   draw();

   //New Round
   /*
   if (asteroids.size() <= 0) //let's hope its not negative
   {
      difficulty++;
      newWave();
   }

   //MissileCooldown
   if (missileTime > 0)
      missileTime--;

   //SpawnTime
   if (spawnTime > 0)
      spawnTime--;
   */

}

/**********************************************************************
 * ASTEROIDS::DRAW  Draws all the objects in the game by calling their
 *       various individual draw functions
 ***********************************************************************/
void Asteroids::draw()
{
	for (list<GameObject*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
      (*it)->draw();

	for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
      (*it)->draw();

	for (list<GameObject*>::iterator it = debris.begin(); it != debris.end(); ++it)
      (*it)->draw();

   if (lives)
   {
      int i = 0;
      for (list<Ship *>::const_iterator it = players.begin(); it != players.end(); ++it)
      {
         (*it)->draw(i);
         ++i;
      }
   }

   //Draw our Score
   drawNumber(Point((Point().getXMin() + Point().getXMax()) / 2,
                    Point().getYMax() - 10), score);

   if (!missileTime)
      //Draw our Missile Indicator
      drawMissile(Point(Point().getXMax() - 10,
               Point().getYMin() + 10), 90);

     //Draw Lives
   float xStart = Point().getXMin() + 10;
   float yStart = Point().getYMin() + 10; 
   for (int i = lives; i > 0; i--)
      drawShip(Point(xStart + i * 8, yStart), 90); 
}

/**********************************************************************
 * ASTEROIDS::SHIPINPUT  Input: left, right, up, down, space
 *             calls the various ship functions for movement and firing
 *             based on the input parameters
 ***********************************************************************/
void Asteroids::shipInput(int left, int right, int up, int down, bool space)
{
   /*
   ship.advance(left, right, up, down);
   if (space && down && !missileTime && lives)
   {
      bullets.push_back(ship.fireMissile(asteroids));
      missileTime = MISSILECOOLDOWN;
   }
   else if (space && !down && lives)
      bullets.push_back(ship.fire());
   */
}

/**********************************************************************
 * ASTEROIDS::NEWWAVE  Creates a new wave of enemies based on the current
 *           difficulty setting. 
 ***********************************************************************/
void Asteroids::newWave()
{
   /*
   for (int i = difficulty; i > 0; i-=2)
   {
      Saucer * saucer = new Saucer(&ship);
      asteroids.push_back(saucer);
   }

   if (difficulty < 2)
   {
      for (int i = 6; i > 0; i--)
      {
         AsteroidL * asteroidL = new AsteroidL();
         asteroids.push_back(asteroidL);
      }
   }
   else
   {
      for (int i = 6; i > 0; i--)
      {
         AsteroidL * asteroidL = new AsteroidL();
         asteroids.push_back(asteroidL);
      }
   }

   for (int i = difficulty - 2; i > 0; i-=2)
   {
      Destroyer * destroyer = new Destroyer(&ship);
      asteroids.push_back(destroyer);
   }
   */
}

/**********************************************************************
 * ASTEROIDS::CHECKCOLLISION  checks collision between all of the objects
 *            on screen by calling the individual member functions of each
 *            object. 
 ***********************************************************************/
void Asteroids::checkCollision()
{
   /*
   //Check Collision for our ship
   if (!spawnTime && lives)
	   for (list<GameObject*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
         ship.transform.checkCollision((*it)->transform);

   //Check for Bullets and our Ship
   if (!spawnTime && lives)
	   for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
         (*it)->transform.checkCollision(ship.transform);

   //Check for Ship and Bullets
   if (!spawnTime && lives)
	   for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
         ship.transform.checkCollision((*it)->transform);

   //Check for Bullets and the Asteroids
   for (list<GameObject*>::iterator itb = bullets.begin(); itb != bullets.end(); ++itb)
	   for (list<GameObject*>::iterator ita = asteroids.begin(); ita != asteroids.end(); ++ita)
         (*itb)->transform.checkCollision((*ita)->transform);

   //Check for Asteroids and the Bullets
   for (list<GameObject*>::iterator ita = asteroids.begin(); ita != asteroids.end(); ++ita)
	   for (list<GameObject*>::iterator itb = bullets.begin(); itb != bullets.end(); ++itb)
         (*ita)->transform.checkCollision((*itb)->transform);
   
   //Check Bounds
   for (list<GameObject*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
      (*it)->transform.checkBounds();

   ship.transform.checkBounds();

   for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
      (*it)->transform.checkBounds();

   */
}

/**********************************************************************
 * ASTEROIDS::WRAP  applies wrap for all objects that are out of bounds
 ***********************************************************************/
void Asteroids::wrap()
{
   for (list<GameObject*>::iterator it = asteroids.begin(); it != asteroids.end(); ++it)
   {
      if ((*it)->transform.getBounds())
      {
         (*it)->transform.wrap();
      }
   }

   for (list<Ship *>::const_iterator it = players.begin(); it != players.end(); ++it)
   {
      if ((*it)->transform.getBounds())
      {
         (*it)->transform.wrap();           
      }
   }

   for (list<GameObject*>::iterator it = bullets.begin(); it != bullets.end(); ++it)
   {
      if ((*it)->transform.getBounds())
       {
          (*it)->transform.wrap();
       }
   }


}

/**********************************************************************
 * ASTEROIDS::DESTROY  Calls the various destroy functions for all of the
 *             Objects on screen if they are collided and then removes
 *             them from their respective vectors
 ***********************************************************************/
void Asteroids::destroy()
{
   /*
	//use different because we need to remove elements from the middle of the list sometimes
	{
		list<GameObject*>::iterator it = asteroids.begin();
		while (it != asteroids.end())
		{
			if ((*it)->transform.getCollided())
			{
				(*it)->destroy(*this);
				//GameObject* obj = *it;
				it = asteroids.erase(it);
				//delete (obj);
				++score;
			}
			else
				++it;
		}
	}

	{
		list<GameObject*>::iterator it = bullets.begin();
		while (it != bullets.end())
		{
			if ((*it)->transform.getCollided())
			{
				(*it)->destroy(*this);
				//GameObject* obj = *it;
				it = bullets.erase(it);
				//delete (obj);
			}
			else
				++it;
		}
	}

	{
		list<GameObject*>::iterator it = debris.begin();
		while (it != debris.end())
		{
			if ((*it)->transform.getCollided())
			{
				(*it)->destroy(*this);
				//GameObject* obj = *it;
				it = debris.erase(it);
				//delete (obj);
			}
			else
				++it;
		}
	}

   if (ship.transform.getCollided() && lives)
   {
      ship.destroy(*this);
      lives--;
      if (lives)
      {
         spawnTime = SPAWNTIME;
         ship.transform.setCollided(false);
         ship.transform.setX(0);
         ship.transform.setY(0);
         ship.transform.setDX(0);
         ship.transform.setDY(0);
      }
   }
   */
}

