/***********************************************************************
* Program:
*    GameObjects Class Source File
*    Brother Helfrich, CS165
* Author:
*    Nicholas Recht
* Summary: 
*    This file contains all of the class definitions for the Game Objects
*    class and all of its child classes
************************************************************************/

#include "gameObjects.h"
#include "asteroids.h"
#include "transform.h"
#include <cmath>
#include "uiDraw.h"
#include "point.h"
using namespace std;

/**********************************************************************
* GAMEOBJECT::GAMEOBJECT  Creates a new game object based on the given
*             array of integers
***********************************************************************/
void GameObject::fromBytes(float * data)
{
	this->transform.getPos().setX(data[0]);
	this->transform.getPos().setY(data[1]);
	this->transform.setDX(data[2]);
	this->transform.setDY(data[3]);
	this->transform.setAngle(data[4]);
	this->transform.setDAngle(data[5]);
}

/**********************************************************************
* GAMEOBJECT::ToBytes   Converts the given GameObject into an array of
*             integers, which can then be used to construct a new
*             GameObject. 
***********************************************************************/
float * GameObject::toBytes()
{
	float * data = new float[6];

	data[0] = this->transform.getPos().getX();
	data[1] = this->transform.getPos().getY();
	data[2] = this->transform.getDX();
	data[3] = this->transform.getDY();
	data[4] = this->transform.getAngle();
	data[5] = this->transform.getDAngle();

	return data; 
}

/**********************************************************************
 * SHIP::ADVANCE  Input: left, right, up, down
 *       Advances the ship by taking the left, right, up, down user input
 ***********************************************************************/
void Ship::advance(int left, int right, int up, int down)
{

   float angle = transform.getAngle();
   
   if (left)
      transform.setAngle(angle + SHIPTURN);

   if (right)
      transform.setAngle(angle - SHIPTURN);

   angle = transform.getAngle();
   
   //Add our Thrust
   if (up)
   {
      transform.setDX(transform.getDX() +
                      cos(angle * PI / 180) * SHIPTHRUST); //Must convert angle to rad
      transform.setDY(transform.getDY() +
                      sin(angle * PI / 180) * SHIPTHRUST);
   }
   //
   
   transform.move();
}

/**********************************************************************
 * SHIP::DRAW draws the ship
 ***********************************************************************/
void Ship::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawShip(transform.getPos(), transform.getAngle());
   //Apply our wrapping
   drawShip(Point(transform.getPos(), xMax * 2, 0.),
            transform.getAngle());
   drawShip(Point(transform.getPos(), xMin * 2, 0.),
            transform.getAngle());
   drawShip(Point(transform.getPos(), 0., yMax * 2),
            transform.getAngle());
   drawShip(Point(transform.getPos(), 0., yMin * 2),
            transform.getAngle());
}

/**********************************************************************
 * SHIP::DRAW creates a random bunch of debris
 ***********************************************************************/
void Ship::destroy(Asteroids & asteroids)
{
   //Create some Debirs
   int dRand = random(5, 20);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
   
}

/**********************************************************************
 * SHIP::INCREMENT no function - virtual function
 ***********************************************************************/
void Ship::increment(Asteroids & asteroids)
{
}

/**********************************************************************
 * ASTEROIDS::ASTEROIDS Input: pos
 *             Constructor for the small asteroid which takes a point
 *             and creates a small asteroid with random movement
 ***********************************************************************/
AsteroidS::AsteroidS(const Point & pos) : Enemy()
{
   transform.setAngle(random(0, 360));
   transform.setDAngle(random(0, 20));
   transform.setR(10);
   transform.setX(pos.getX());
   transform.setY(pos.getY());
   value = 2.0;
}; 

/**********************************************************************
 * ASTEROIDS::DRAW  draws the asteroid with wrap
 ***********************************************************************/
void AsteroidS::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawCircle(transform.getPos(), transform.getR(), 4, transform.getAngle());

   //Apply our wrapping
   drawCircle(Point(transform.getPos(), xMax * 2, 0.),
              transform.getR(), 4, transform.getAngle());
   drawCircle(Point(transform.getPos(), xMin * 2, 0.),
              transform.getR(), 4, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMax * 2),
              transform.getR(), 4, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMin * 2),
              transform.getR(), 4, transform.getAngle());
}

/**********************************************************************
 * ASTEROIDS::INCREMENT  move the asteroid
 ***********************************************************************/
void AsteroidS::increment(Asteroids & asteroids)
{
   transform.move();
}

/**********************************************************************
 * ASTEROIDS::DESTROY  creates a bunch of random debris
 ***********************************************************************/
void AsteroidS::destroy(Asteroids & asteroids)
{
   //Create some Debirs
   int dRand = random(3, 8);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
   
}

/**********************************************************************
 * ASTEROIDM::ASTEROIDM  Input: pos
 *             Creates a medium asteroid from a given pos, adds random
 *             direction and velocity
 ***********************************************************************/
AsteroidM::AsteroidM(const Point & pos) : Enemy()
{
   transform.setAngle(random(0, 360));
   transform.setDAngle(random(0, 20));
   transform.setR(20);
   transform.setX(pos.getX());
   transform.setY(pos.getY());
   value = 1.75;
}

/**********************************************************************
 * ASTEROIDM::DRAW  draws the asteroid with wrap
 ***********************************************************************/
void AsteroidM::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawCircle(transform.getPos(), transform.getR(), 5, transform.getAngle());

   //Apply our wrapping
   drawCircle(Point(transform.getPos(), xMax * 2, 0.),
              transform.getR(), 5, transform.getAngle());
   drawCircle(Point(transform.getPos(), xMin * 2, 0.),
              transform.getR(), 5, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMax * 2),
              transform.getR(), 5, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMin * 2),
              transform.getR(), 5, transform.getAngle());
}

/**********************************************************************
 * ASTEROIDM::INCREMENT  moves the asteroid
 ***********************************************************************/
void AsteroidM::increment(Asteroids & asteroids)
{
   transform.move();
}

/**********************************************************************
 * ASTEROIDM::DESTROY  creates two new medium asteroids and some random
 *             debris. 
 ***********************************************************************/
void AsteroidM::destroy(Asteroids & asteroids)
{
   AsteroidS * asteroidS = new AsteroidS(transform.getPos());
   asteroidS->transform.setDX(random(-200, 200) / 100.);
   asteroidS->transform.setDY(random(-200, 200) / 100.);
   AsteroidS * asteroidS2 = new AsteroidS(transform.getPos());
   asteroidS2->transform.setDX(random(-200, 200) / 100.);
   asteroidS2->transform.setDY(random(-200, 200) / 100.);
   
   asteroids.asteroids.push_back(asteroidS);
   asteroids.asteroids.push_back(asteroidS2);

   //Create some Debirs
   int dRand = random(5, 10);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
   
   
}

/**********************************************************************
 * ASTEROIDL::ASTEROIDL  creates a large asteroid and gives it random
 *              direction and velocity.
 ***********************************************************************/
AsteroidL::AsteroidL() : Enemy()
{
   int xRand = random(0, 100);
   if (xRand > 50) 
      transform.setX(random(Point().getXMin(), Point().getXMin() + 10));
   else
      transform.setX(random(Point().getXMax() - 10, Point().getXMax()));
   
   transform.setY(random(Point().getYMin(), Point().getYMax()));
   
   transform.setAngle(random(0, 360));
   transform.setDAngle(random(0, 20));
   transform.setR(30);
   transform.setDX(random(-100, 100) / 100.);
   transform.setDY(random(-100, 100) / 100.);
   value = 1.5;
};

/**********************************************************************
 * ASTEROIDL::ASTEROIDL  draws the asteroid
 ***********************************************************************/
void AsteroidL::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawCircle(transform.getPos(), transform.getR(), 6, transform.getAngle());

   //Apply our wrapping
   drawCircle(Point(transform.getPos(), xMax * 2, 0.),
              transform.getR(), 6, transform.getAngle());
   drawCircle(Point(transform.getPos(), xMin * 2, 0.),
              transform.getR(), 6, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMax * 2),
              transform.getR(), 6, transform.getAngle());
   drawCircle(Point(transform.getPos(), 0., yMin * 2),
              transform.getR(), 6, transform.getAngle());
}

/**********************************************************************
 * ASTEROIDL::INCREMENT  moves the asteroid
 ***********************************************************************/
void AsteroidL::increment(Asteroids & asteroids)
{
   transform.move();
}

/**********************************************************************
 * ASTEROIDL::DESTROY  creates two new medium asteroids and a small
 *             asteroid as well as some random debris
 ***********************************************************************/
void AsteroidL::destroy(Asteroids & asteroids)
{
   AsteroidM * asteroidM = new AsteroidM(transform.getPos());
   asteroidM->transform.setDX(random(-150, 150) / 100.);
   asteroidM->transform.setDY(random(-150, 150) / 100.);
   AsteroidM * asteroidM2 = new AsteroidM(transform.getPos());
   asteroidM2->transform.setDX(random(-150, 150) / 100.);
   asteroidM2->transform.setDY(random(-150, 150) / 100.);
   AsteroidS * asteroidS = new AsteroidS(transform.getPos());
   asteroidS->transform.setDX(random(-200, 200) / 100.);
   asteroidS->transform.setDY(random(-200, 200) / 100.);

   asteroids.asteroids.push_back(asteroidM);
   asteroids.asteroids.push_back(asteroidM2);
   asteroids.asteroids.push_back(asteroidS);

   //Create some Debirs
   int dRand = random(5, 20);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
   
}

//THE SAUCER
/**********************************************************************
 * SAUCER::SAUCER  Input: player
 *          Constructor for the Saucer class which takes the player
 *          pointer gameObject, sets the saucer's target to the player
 *          then applies a random velocity and direction.
 ***********************************************************************/
Saucer::Saucer(GameObject * player) : Enemy(), ai(player), fireTime(90)
{
   int xRand = random(0, 100);
   if (xRand > 50) 
      transform.setX(random(Point().getXMin(), Point().getXMin() + 10));
   else
      transform.setX(random(Point().getXMax() - 10, Point().getXMax()));
   
   transform.setY(random(Point().getYMin(), Point().getYMax()));
   
   transform.setAngle(random(0, 360));
   transform.setDAngle(random(0, 20));
   transform.setR(12);
   transform.setDX(random(-200, 200) / 100.);
   value = 1.0;
   // transform.setDY(random(-100, 100) / 100.);
};

/**********************************************************************
 * SAUCER::DRAW  draws the saucer with wrap
 ***********************************************************************/
void Saucer::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawSaucer(transform.getPos());

   //Apply our wrapping
   drawSaucer(Point(transform.getPos(), xMax * 2, 0.));
   drawSaucer(Point(transform.getPos(), xMin * 2, 0.));
   drawSaucer(Point(transform.getPos(), 0., yMax * 2));
   drawSaucer(Point(transform.getPos(), 0., yMin * 2));
}

/**********************************************************************
 * SAUCER::INCREMENT  moves the saucer and calls the ai functions to
 *          interpolate the position of the target and checks to see
 *          if the fire time is ready to fire another shot. 
 ***********************************************************************/
void Saucer::increment(Asteroids & asteroids)
{
   
   
   if (fireTime <= 0)
   {
      transform.setAngle(ai.interpolate(this, ai.getTarget(), BULLETLAUNCH));
      asteroids.bullets.push_back(fire());
      fireTime = 1000000 // SAUCERSHOOTTIME;
   }
   else
      fireTime--;

   
   transform.move();
}

/**********************************************************************
 * SAUCER::DESTROY  creates some random debris
 ***********************************************************************/
void Saucer::destroy(Asteroids & asteroids)
{
   //Create some Debirs
   int dRand = random(5, 20);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
   
}

/**********************************************************************
 * SAUCER::FIRE  creates a bullet using the saucer's transform and
 *          returns a pointer to the bullet
 ***********************************************************************/
GameObject * Saucer::fire()
{
   Bullet * b = new Bullet(transform);
   return b;
}

//THE DESTROYER
/**********************************************************************
 * DESTROYER::DESTROYER  Input: player
 *          Constructor for the Destroyer class which takes the player
 *          pointer gameObject, sets the saucer's target to the player
 *          then applies a random velocity and direction.
 ***********************************************************************/
Destroyer::Destroyer(GameObject * player) : Enemy(), ai(player), fireTime(90)
{
   int xRand = random(0, 100);
   if (xRand > 50)
      transform.setX(random(Point().getXMin(), Point().getXMin() + 10));
   else
      transform.setX(random(Point().getXMax() - 10, Point().getXMax()));
   
   transform.setY(random(Point().getYMin(), Point().getYMax()));
   
   transform.setR(15);
   transform.setDX(random(-200, 200) / 100.);
   if (transform.getDX() < 0)
      transform.setAngle(180);
   
   value = .8;
};

/**********************************************************************
 * DESTROYER::DRAW  draws the destroyer with wrap
 ***********************************************************************/
void Destroyer::draw()
{
   int xMax = transform.getPos().getXMax();
   int xMin = transform.getPos().getXMin();
   int yMax = transform.getPos().getYMax();
   int yMin = transform.getPos().getYMin();
   
   drawDestroyer(transform.getPos(), transform.getAngle());

   //Apply our wrapping
   drawDestroyer(Point(transform.getPos(), xMax * 2, 0.), transform.getAngle());
   drawDestroyer(Point(transform.getPos(), xMin * 2, 0.), transform.getAngle());
   drawDestroyer(Point(transform.getPos(), 0., yMax * 2), transform.getAngle());
   drawDestroyer(Point(transform.getPos(), 0., yMin * 2), transform.getAngle());
}

/**********************************************************************
 * DESTROYER::INCREMENT  moves the destroyer and calls the ai functions to
 *          interpolate the position of the target and checks to see
 *          if the fire time is ready to fire another shot. 
 ***********************************************************************/
void Destroyer::increment(Asteroids & asteroids)
{
   
   
   if (fireTime <= 0)
   {
      float angle = transform.getAngle();
      for (int i = 0; i < DESTROYERGUNS - 1; i++)
      {
         transform.setAngle(ai.interpolate(this, ai.getTarget(), BULLETLAUNCH));
         if (transform.getAngle() < 180 && transform.getAngle() > 20)
            asteroids.bullets.push_back(
               fire(Point(this->transform.getPos(),
                          cos(angle * PI / 180) *
                          DESTROYERGUNSPACE * i,
                          i * 2 + sin(angle * PI / 180) *
                          DESTROYERGUNSPACE * i)));
         else if (transform.getAngle() < 340)
            asteroids.bullets.push_back(
               fire(Point(this->transform.getPos(),
                          cos(angle * PI / 180) *
                          DESTROYERGUNSPACE * i,
                          i * -2 + sin(angle * PI / 180) *
                          DESTROYERGUNSPACE * i)));
      }
      fireTime = DESTROYERSHOOTTIME;
      transform.setAngle(angle);
   }
   
   else
      fireTime--;

   transform.move();
}

/**********************************************************************
 * DESTROYER::DESTROY  creates some random debris
 ***********************************************************************/
void Destroyer::destroy(Asteroids & asteroids)
{
   //Create some Debirs
   int dRand = random(10, 30);
   for (int j = 0; j < dRand; j++)
   {
      Debris * d = new Debris(transform);
      asteroids.debris.push_back(d);
   }
}

/**********************************************************************
 * DESTROYER::FIRE  creates a bullet using the destroyers transform and
 *          returns a pointer to the bullet
 ***********************************************************************/
GameObject * Destroyer::fire(Point p)
{
   Bullet * b = new Bullet(transform, p);
   return b;
}

/**********************************************************************
 * BULLET::BULLET  Input::transfrom
 *          Constructor for the bullet class that takes a transform
 *          to apply to the new bullet plus the bullet's fire velocity
 ***********************************************************************/
Bullet::Bullet(Transform & t) : GameObject(), lifeTime(BULLETLIFETIME)
{
   transform.setAngle(t.getAngle());
   transform.setR(1);
   transform.setX(t.getX() + cos(t.getAngle() * PI / 180) * t.getR());
   transform.setY(t.getY() + sin(t.getAngle() * PI / 180) * t.getR());
   transform.setDX(t.getDX() +
                   (cos(transform.getAngle() * PI / 180) * BULLETLAUNCH));
   transform.setDY(t.getDY() +
                   (sin(transform.getAngle() * PI / 180) * BULLETLAUNCH));
}

/**********************************************************************
 * BULLET::BULLET  Input::transfrom, point
 *          Constructor for the bullet class that takes a transform
 *          to apply to the new bullet plus the bullet's fire velocity
 ***********************************************************************/
Bullet::Bullet(Transform & t, Point p) : GameObject(), lifeTime(BULLETLIFETIME)
{
   transform.setAngle(t.getAngle());
   transform.setR(1);
   transform.setX(p.getX() + cos(t.getAngle() * PI / 180) * t.getR());
   transform.setY(p.getY() + sin(t.getAngle() * PI / 180) * t.getR());
   transform.setDX(t.getDX() +
                   (cos(transform.getAngle() * PI / 180) * BULLETLAUNCH));
   transform.setDY(t.getDY() +
                   (sin(transform.getAngle() * PI / 180) * BULLETLAUNCH));
}

/**********************************************************************
 * BULLET::DRAW  draws the bullet
 ***********************************************************************/
void Bullet::draw()
{
   drawDot(transform.getPos());
}

/**********************************************************************
 * BULLET::INCREMENT  moves the bullet and decreases the lifeTime
 ***********************************************************************/
void Bullet::increment(Asteroids & asteroids)
{
   if (lifeTime > 0)
      lifeTime--;
   else
      transform.setCollided(true);
   
   transform.move();
}

/**********************************************************************
 * BULLET::DESTORY  does nothing
 ***********************************************************************/
void Bullet::destroy(Asteroids & asteroids)
{
   //Intentionally empty for now
}

/**********************************************************************
 * MISSILE::MISSILE  Input: transform, targets
 *           Constructor for the missile class that takes a transform
 *           and a vector of pointers to gameObjects to be used as targets.
 *           Applies the reference transform plus the launch speed. 
 ***********************************************************************/
Missile::Missile(Transform & t, list<GameObject*> & targets) : GameObject(t), ai(this, targets), lifeTime(120)
{
   transform.setAngle(t.getAngle());
   transform.setX(t.getX() + cos(t.getAngle() * PI / 180) * t.getR());
   transform.setY(t.getY() + sin(t.getAngle() * PI / 180) * t.getR());
   transform.setDX(t.getDX() +
                   (cos(transform.getAngle() * PI / 180) * 5));
   transform.setDY(t.getDY() +
                   (sin(transform.getAngle() * PI / 180) * 5));
   transform.setR(3);
};

/**********************************************************************
 * MISSILE::DRAW  draws the missile
 ***********************************************************************/
void Missile::draw()
{
   drawMissile(transform.getPos(), transform.getAngle());
}

/**********************************************************************
 * MISSILE::INCREMENT  moves the missile, decreases the life time, and
 *           calls the appropriate ai functions to adjust the missiles
 *           trajectory based on its targets position.
 ***********************************************************************/
void Missile::increment(Asteroids & asteroids)
{
   if (lifeTime > 0)
      lifeTime--;
   else
      transform.setCollided(true);

   if (lifeTime)
   {
      float angle = transform.getAngle();
      float angleBetween = ai.getAngleBetween(
         Point(transform.getPos(), transform.getDX(), transform.getDY()),
         Point(ai.getTarget()->transform.getPos(), ai.getTarget()->transform.getDX(), ai.getTarget()->transform.getDY()));

      //We want the missile to come to a stop before it goes after the
      //target so that it has a better chance of hitting it
      if (abs(angle - angleBetween) > MISSILETURN)
      {
         
         if (angleBetween > angle)
         {
            if (angleBetween - angle > 180)
               transform.setAngle(angle - MISSILETURN);
            else
               transform.setAngle(angle + MISSILETURN);
         }
         else //(angle > angleBetween)
         {
         if(angle - angleBetween > 180)
            transform.setAngle(angle + MISSILETURN);
         else
            transform.setAngle(angle - MISSILETURN);
         }

         if (transform.getDX() > MISSILESTOP)
            transform.setAX(-MISSILESTOP);
         else if (transform.getDX() < -MISSILESTOP)
            transform.setAX(MISSILESTOP);
         else
         {
            transform.setDX(0.);
            transform.setAX(0.);
         }

         if (transform.getDY() > MISSILESTOP)
            transform.setAY(-MISSILESTOP);
         else if (transform.getDY() < -MISSILESTOP)
            transform.setAY(MISSILESTOP);
         else
         {
            transform.setDY(0.);
            transform.setAY(0.);
         }

      
      }
      
      //Now destroy!
      else
      {
         transform.setAngle(angleBetween);
         if (transform.getSpeed() <= MISSILESPEED)
         {
            transform.setAX(cos(transform.getAngle() * PI / 180) * MISSILETHRUST);
            transform.setAY(sin(transform.getAngle() * PI / 180) * MISSILETHRUST);
         }
         else
         {
            transform.setAX(0);
            transform.setAY(0);
         }
      }
      
   }
   
   transform.move();
}

/**********************************************************************
 * MISSILE::DESTROY  does nothing
 ***********************************************************************/
void Missile::destroy(Asteroids & asteroids)
{
   //Intentionally empty for now
}

/**********************************************************************
 * DEBRIS::DEBRIS  Input::transform
 *          Constructor for the Debris class which creates a debris at
 *          using the reference transfrom and adds random direction and
 *          velocity
 ***********************************************************************/
Debris::Debris(Transform & t) : GameObject(), lifeTime(30)
{
   transform.setR(3);
   transform.setX(t.getX());
   transform.setY(t.getY());
   transform.setAngle(random(0, 360));
   transform.setDX(t.getDX() +
                   random(-500, 500) / 100);
   transform.setDY(t.getDY() +
                   random(-500, 500) / 100);
   transform.setDAngle(random(-1000, 1000) / 100);
   
};

/**********************************************************************
 * DEBRIS::DRAW  draws the debris
 ***********************************************************************/
void Debris::draw()
{
   drawCircle(transform.getPos(), transform.getR(), 3, transform.getAngle());
}

/**********************************************************************
 * DEBRIS::INCREMENT  moves the debris and decreases the lifetime
 ***********************************************************************/
void Debris::increment(Asteroids & asteroids)
{
   if (lifeTime > 0)
      lifeTime--;
   else
      transform.setCollided(true);
   
   transform.move();
}

/**********************************************************************
 * DEBRIS::DESTROY  does nothing
 ***********************************************************************/
void Debris::destroy(Asteroids & asteroids)
{
   //Intentionally empty for now
}

/**********************************************************************
 * SHIP::FIRE  creates and returns a new bullet using the ship's transform
 ***********************************************************************/
GameObject * Ship::fire()
{
   Bullet * b = new Bullet(transform);
   return b;
}

/**********************************************************************
 * SHIP::FIREMISSILE  Input::targets
 *        Creates a missile based off of the ship's transform and the
 *        input targets and returns a pointer to that missile
 ***********************************************************************/
GameObject * Ship::fireMissile(list<GameObject*> & targets)
{
   Missile * m = new Missile(transform, targets);
   return m;
}

