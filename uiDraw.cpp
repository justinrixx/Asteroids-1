/***********************************************************************
 * Source File:
 *    User Interface Draw : put pixels on the screen
 * Author:
 *    Br. Helfrich
 * Summary:
 *    This is the code necessary to draw on the screen. We have a collection
 *    of procedural functions here because each draw function does not
 *    retain state. In other words, they are verbs (functions), not nouns
 *    (variables) or a mixture (objects)
 ************************************************************************/

#include <string>     // need you ask?
#include <sstream>    // convert an integer into text
#include <cassert>    // I feel the need... the need for asserts
#include <time.h>     // for clock

//#define __linux__

#ifdef __APPLE__
#include <openGL/gl.h>    // Main OpenGL library
#include <GLUT/glut.h>    // Second OpenGL library
#endif // __APPLE__

#ifdef __linux__
#include <GL/gl.h>        // Main OpenGL library
#include <GL/glut.h>      // Second OpenGL library
#endif // __linux__

#ifdef _WIN32
#include <stdio.h>
#include <stdlib.h>
#include <GL/glut.h>         // OpenGL library we copied 
#define _USE_MATH_DEFINES
#include <math.h>
#endif // _WIN32

#include "point.h"
#include "uiDraw.h"

using namespace std;

#define deg2rad(value) ((M_PI / 180) * (value))

/*********************************************
 * NUMBER OUTLINES
 * We are drawing the text for score and things
 * like that by hand to make it look "old school."
 * These are how we render each individual charactger.
 * Note how -1 indicates "done".  These are paired
 * coordinates where the even are the x and the odd
 * are the y and every 2 pairs represents a point
 ********************************************/
const char NUMBER_OUTLINES[10][20] =
{
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   0,10,  0, 0,  -1,-1, -1,-1},//0
  {7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//1
  {0, 0,  7, 0,   7, 0,  7, 5,   7, 5,  0, 5,   0, 5,  0,10,   0,10,  7,10},//2
  {0, 0,  7, 0,   7, 0,  7,10,   7,10,  0,10,   4, 5,  7, 5,  -1,-1, -1,-1},//3
  {0, 0,  0, 5,   0, 5,  7, 5,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1},//4
  {7, 0,  0, 0,   0, 0,  0, 5,   0, 5,  7, 5,   7, 5,  7,10,   7,10,  0,10},//5
  {7, 0,  0, 0,   0, 0,  0,10,   0,10,  7,10,   7,10,  7, 5,   7, 5,  0, 5},//6
  {0, 0,  7, 0,   7, 0,  7,10,  -1,-1, -1,-1,  -1,-1, -1,-1,  -1,-1, -1,-1},//7
  {0, 0,  7, 0,   0, 5,  7, 5,   0,10,  7,10,   0, 0,  0,10,   7, 0,  7,10},//8
  {0, 0,  7, 0,   7, 0,  7,10,   0, 0,  0, 5,   0, 5,  7, 5,  -1,-1, -1,-1} //9
};

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawDigit(const Point &topLeft, char digit)
{
   // we better be only drawing digits
   assert(isdigit(digit));
   if (!isdigit(digit))
      return;

   // compute the row as specified by the digit
   int r = digit - '0';
   assert(r >= 0 && r <= 9);

   // go through each segment.
   for (int c = 0; c < 20 && NUMBER_OUTLINES[r][c] != -1; c += 4)
   {
      assert(NUMBER_OUTLINES[r][c    ] != -1 &&
             NUMBER_OUTLINES[r][c + 1] != -1 &&
             NUMBER_OUTLINES[r][c + 2] != -1 &&
             NUMBER_OUTLINES[r][c + 3] != -1);

      //Draw a line based off of the num structure for each number
      Point start;
      start.setX(topLeft.getX() + NUMBER_OUTLINES[r][c]);
      start.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 1]);
      Point end;
      end.setX(topLeft.getX() + NUMBER_OUTLINES[r][c + 2]);
      end.setY(topLeft.getY() - NUMBER_OUTLINES[r][c + 3]);
         
      drawLine(start, end);
   }
}

/*************************************************************************
 * DRAW NUMBER
 * Display an positive integer on the screen using the 7-segment method
 *   INPUT  topLeft   The top left corner of the character
 *          digit     The digit we are rendering: '0' .. '9'
 *************************************************************************/
void drawNumber(const Point &topLeft, unsigned int number)
{
   // render the number as text
   ostringstream sout;
   sout << number;
   string text = sout.str();

   // walk through the text one digit at a time
   Point point = topLeft;
   for (const char *p = text.c_str(); *p; p++)
   {
      assert(isdigit(*p));
      drawDigit(point, *p);
      point.addX(11);
   }
}


/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *    INPUT  point    The point to be moved
 *           center   The center point we will rotate around
 *           rotation Rotation in degrees
 *    OUTPUT point    The new position
 *************************************************************************/
void rotate(Point &point, const Point &origin, int rotation)
{
   // because sine and cosine are expensive, we want to call them only once
   double cosA = cos(deg2rad(rotation));
   double sinA = sin(deg2rad(rotation));

   // remember our original point
   Point tmp(true /*fNoCheck*/);
   tmp.setX(point.getX() - origin.getX());
   tmp.setY(point.getY() - origin.getY());

   // find the new values
   point.setX(static_cast<int> (tmp.getX() * cosA -
                                tmp.getY() * sinA) +
              origin.getX());
   point.setY(static_cast<int> (tmp.getX() * sinA +
                                tmp.getY() * cosA) +
              origin.getY());
}

/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 *  INPUT  center    Center of the rectangle
 *         width     Horizontal size
 *         height    Vertical size
 *         rotation  Orientation
 *************************************************************************/
void drawRect(const Point &center, char width, char height, int rotation)
{
   Point tl(true /*fNoCheck*/); // top left
   Point tr(true /*fNoCheck*/); // top right 
   Point bl(true /*fNoCheck*/); // bottom left
   Point br(true /*fNocheck*/); // bottom right

   //Top Left point
   tl.setX(center.getX() - (width  / 2));
   tl.setY(center.getY() + (height / 2));

   //Top right point
   tr.setX(center.getX() + (width  / 2));
   tr.setY(center.getY() + (height / 2));

   //Bottom left point
   bl.setX(center.getX() - (width  / 2));
   bl.setY(center.getY() - (height / 2));

   //Bottom right point
   br.setX(center.getX() + (width  / 2));
   br.setY(center.getY() - (height / 2));

   //Rotate all points the given degrees
   rotate(tl, center, rotation);
   rotate(tr, center, rotation);
   rotate(bl, center, rotation);
   rotate(br, center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(tl.getX(), tl.getY());
   glVertex2f(tr.getX(), tr.getY());
   glVertex2f(br.getX(), br.getY());
   glVertex2f(bl.getX(), bl.getY());
   glVertex2f(tl.getX(), tl.getY());
   glEnd();
}

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *  INPUT   center   Center of the circle
 *          radius   Size of the circle
 *          points   How many points will we draw it.  Larger the number,
 *                   the more line segments we will use
 *          rotation True circles are rotation independent.  However, if you
 *                   are drawing a 3-sided circle (triangle), this matters!
 *************************************************************************/
void drawCircle(const Point &center, char radius, int points, int rotation)
{
   // begin drawing
   glBegin(GL_LINE_LOOP);

   //loop around a circle the given number of times drawing a line from
   //one point to the next
   for (double i = 0; i < 2 * M_PI; i += (2 * M_PI) / points)
   {
      Point temp(true /*noCheck*/);
      temp.setX(center.getX() + static_cast<int>(radius * cos(i)));
      temp.setY(center.getY() + static_cast<int>(radius * sin(i)));
      rotate(temp, center, rotation);
      glVertex2f(temp.getX(), temp.getY());
   }
   
   // complete drawing
   glEnd();

}

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *   INPUT  begin     The position of the beginning of the line
 *          end       The position of the end of the line
 *************************************************************************/
void drawLine(const Point &begin, const Point &end)
{
   // Get ready...
   glBegin(GL_LINES);

   // Note how we scale the lines by a factor of two
   glVertex2f(begin.getX(), begin.getY());
   glVertex2f(  end.getX(),   end.getY());

   // complete drawing.
   glEnd();
}

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *  INPUT point   The position of the dow
 *************************************************************************/
void drawDot(const Point &point)
{
   // Get ready, get set...
   glBegin(GL_POINTS);

   // Go...
   glVertex2f(point.getX(),     point.getY()    );
   glVertex2f(point.getX() + 1, point.getY()    );
   glVertex2f(point.getX() + 1, point.getY() + 1);
   glVertex2f(point.getX(),     point.getY() + 1);

   // Done!  OK, that was a bit too dramatic
   glEnd();
}

/************************************************************************
 * DRAW Ship
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawShip(const Point &center, int rotation, int num)
{
   num = num % 4;
   switch (num)
   {
      case 0:
         glColor3ub(255, 0, 0);
         break;
      case 1:
         glColor3ub(0, 255, 255);
         break;
      case 2:
         glColor3ub(255, 0, 255);
         break;
      case 3:
         glColor3ub(255, 255, 0);
         break;
   }      
   
   // Get ready, get set...
   glBegin(GL_LINE_STRIP);

   Point bow(      center); // front
   Point stern(    center); // back
   Point starboard(center); // right
   Point port(     center); // left

   bow.addX(6);

   stern.addX(-2);

   starboard.addY(3);
   starboard.addX(-3);

   port.addY(-3);
   port.addX( -3);
   
   //Rotate all points the given degrees
   rotate(bow,       center, rotation);
   rotate(stern,     center, rotation);
   rotate(starboard, center, rotation);
   rotate(port,      center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(      bow.getX(),       bow.getY());
   glVertex2f(starboard.getX(), starboard.getY());
   glVertex2f(    stern.getX(),     stern.getY());
   glVertex2f(     port.getX(),      port.getY());
   glVertex2f(      bow.getX(),       bow.getY());
   
   // Done!  OK, that was a bit too dramatic
   glEnd();   

   glColor3ub(255, 255, 255);
}

/************************************************************************
 * DRAW Missile
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawMissile(const Point &center, int rotation)
{
   glColor3ub(200, 200, 200);
   // Get ready, get set...
   glBegin(GL_LINE_STRIP);

   Point ltail(      center); // front
   Point rtail(    center); // back
   Point lside(center); // right
   Point rside(     center); // left
   Point ltop (     center);
   Point rtop (     center);

   ltail.addX(-4);
   rtail.addX(-4);
   rtail.addY(-3);
   ltail.addY(3);
   
   lside.addX(0);
   rside.addX(0);
   lside.addY(1);
   rside.addY(-1);

   ltop.addX(6);
   rtop.addX(6);
   ltop.addY(1);
   rtop.addY(-1);
   
   //Rotate all points the given degrees
   rotate(ltop,       center, rotation);
   rotate(rtop,     center, rotation);
   rotate(lside, center, rotation);
   rotate(rside,      center, rotation);
   rotate(ltail, center, rotation);
   rotate(rtail,      center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(      ltail.getX(),       ltail.getY());
   glVertex2f(      lside.getX(),       lside.getY());
   glVertex2f(      ltail.getX(),       ltail.getY());
   glVertex2f(      center.getX(),      center.getY());
   glVertex2f(      rtail.getX(),       rtail.getY());
   glVertex2f(      rside.getX(),       rside.getY());
   glVertex2f(      lside.getX(),       lside.getY());
   glVertex2f(      ltop.getX(),        ltop.getY());
   glVertex2f(      rtop.getX(),        rtop.getY());
   glVertex2f(      rside.getX(),       rside.getY());
   // Done!  OK, that was a bit too dramatic
   glEnd();
   glColor3ub(255, 255, 255);
}

/************************************************************************
 * DRAW DESTROYER
 * Draw a destroyer on the screen
 *  INPUT point   The position of the ship
 *        angle   Which direction it is ponted
 *************************************************************************/
void drawDestroyer(const Point &center, int rotation)
{
   glColor3ub(252, 61, 49);
  // Get ready, get set...
   glBegin(GL_LINE_STRIP);

   Point bow(      center); // front
   Point stern(    center); // back
   Point starboard(center); // right
   Point port(     center); // left

   bow.addX(14);

   stern.addX(-14);

   starboard.addY(12);
   starboard.addX(-8);

   port.addY(-12);
   port.addX( -8);
   
   //Rotate all points the given degrees
   rotate(bow,       center, rotation);
   rotate(stern,     center, rotation);
   rotate(starboard, center, rotation);
   rotate(port,      center, rotation);

   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(      bow.getX(),       bow.getY());
   glVertex2f(starboard.getX(), starboard.getY());
   glVertex2f(    stern.getX(),     stern.getY());
   glVertex2f(     port.getX(),      port.getY());
   glVertex2f(      bow.getX(),       bow.getY());
   
   // Done!  OK, that was a bit too dramatic
   glEnd();
   glColor3ub(255, 255, 255);
}

/************************************************************************
 * DRAW Saucer
 * Draw a spaceship on the screen
 *  INPUT point   The position of the ship
 *      
 *************************************************************************/
void drawSaucer(const Point &center)
{
   glColor3ub(0, 255, 0); 
   // Get ready, get set...
   glBegin(GL_LINE_STRIP);

   Point lSide(      center); // front
   Point rSide(    center); // back
   Point lBottom(center); // right
   Point rBottom(     center); // left
   Point lRoof(center);
   Point rRoof(center);
   Point lTop(center);
   Point rTop(center);
   
   lSide.addX(-10);
   rSide.addX(10);

   lBottom.addX(-4);
   rBottom.addX(4);
   lBottom.addY(-3);
   rBottom.addY(-3);

   lRoof.addX(-4);
   rRoof.addX(4);
   lRoof.addY(3);
   rRoof.addY(3);

   lTop.addX(-3);
   rTop.addX(3);
   lTop.addY(7);
   rTop.addY(7);
   
   
   //Finally draw the rectangle
   glBegin(GL_LINE_STRIP);
   glVertex2f(     lBottom.getX(),     lBottom.getY());
   glVertex2f(      lSide.getX(),       lSide.getY());
   glVertex2f(     lBottom.getX(),     lBottom.getY());
   glVertex2f(     rBottom.getX(),     rBottom.getY());
   glVertex2f(      rSide.getX(),       rSide.getY());
   glVertex2f(      lSide.getX(),       lSide.getY());
   glVertex2f(     lRoof.getX(),       lRoof.getY());
   glVertex2f(     rRoof.getX(),       rRoof.getY());
   glVertex2f(      rSide.getX(),       rSide.getY());
   glVertex2f(     rRoof.getX(),       rRoof.getY());
   glVertex2f(     rTop.getX(),      rTop.getY());
   glVertex2f(     lTop.getX(),       lTop.getY());
   glVertex2f(     lRoof.getX(),       lRoof.getY());
   
   // Done!  OK, that was a bit too dramatic
   glEnd();   
   glColor3ub(255, 255, 255);
}

/******************************************************************
 * RANDOM
 * This function generates a random number.  
 *
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max)
{
   assert(min <= max);
   int num = (rand() % (max - min)) + min;
   assert(min <= num && num <= max);

   return num;
}


