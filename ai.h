/***********************************************************************
* Program:
*    AI Class Header File
*    Brother Helfrich, CS165
* Author:
*    Nicholas Recht
* Summary: 
*    This file contains the class declarations for the AI Class.
*************************************************************************/

#ifndef AI_H
#define AI_H

#include "point.h"
#include <vector>
#include <list>
using namespace std;

class GameObject;

class AI
{
  public:
   AI() { target = NULL; }; // may break stuff
   AI(GameObject * target) : target(target) { };
   AI(GameObject * reference, list<GameObject*> & targets) { findTarget(reference, targets); };
   void findTarget(GameObject * reference, list<GameObject*> & targets);
   float getDistanceBetween(Point thisPoint, Point thatPoint);
   float getAngleBetween(Point thisPoint, Point thatPoint);
   float interpolate(const GameObject * thisObject, const GameObject * thatObject, float speed);
   void setTarget(GameObject * target) { this->target = target; };
   GameObject * getTarget() { return target; };
  private:
   GameObject * target;
   
};

#endif //AI_H
