#ifndef UI_DRAW_H
#define UI_DRAW_H

#include <string>     // To display text on the screen
#include <cmath>      // for M_PI, sin() and cos()
#include "point.h"    // Where things are drawn
using std::string;

/************************************************************************
 * DRAW DIGIT
 * Draw a single digit in the old school line drawing style.  The
 * size of the glyph is 8x11 or x+(0..7), y+(0..10)
 *************************************************************************/
void drawDigit(const Point &topLeft, char digit);

/*************************************************************************
 * DRAW NUMBER
 * Display an positive integer on the screen using the 7-segment method
 *************************************************************************/
void drawNumber(const Point &topLeft, unsigned int number);

/************************************************************************
 * ROTATE
 * Rotate a given point (point) around a given origin (center) by a given
 * number of degrees (angle).
 *************************************************************************/
void rotate(Point &point, const Point &origin, int rotation);

/************************************************************************
 * DRAW RECTANGLE
 * Draw a rectangle on the screen centered on a given point (center) of
 * a given size (width, height), and at a given orientation (rotation)
 * measured in degrees (0 - 360)
 *************************************************************************/
void drawRect(const Point &center, char width, char height, int rotation);

/************************************************************************
 * DRAW CIRCLE
 * Draw a circle from a given location (center) of a given size (radius).
 *************************************************************************/
void drawCircle(const Point &center, char radius, int points, int rotation);

/************************************************************************
 * DRAW LINE
 * Draw a line on the screen from the beginning to the end.
 *************************************************************************/
void drawLine(const Point &begin, const Point &end);

/************************************************************************
 * DRAW DOT
 * Draw a single point on the screen, 2 pixels by 2 pixels
 *************************************************************************/
void drawDot(const Point &point);

/************************************************************************
 * DRAW Ship
 * Draw the spaceship on the screen
 *************************************************************************/
void drawShip(const Point &point, int rotation, int num = 1);

/************************************************************************
 * DRAW Missile
 * Draw a Missile on the screen
 *************************************************************************/
void drawMissile(const Point &point, int rotation);

/************************************************************************
 * DRAW Destroyer
 * Draw a Destroyer on the screen
 *************************************************************************/
void drawDestroyer(const Point &point, int rotation);

/************************************************************************
 * DRAW Saucer
 * Draw a saucer on the screen
 *************************************************************************/
void drawSaucer(const Point &point);

/******************************************************************
 * RANDOM
 * This function generates a random number.  The user specifies
 * The parameters 
 *    INPUT:   min, max : The number of values (min <= num <= max)
 *    OUTPUT   <return> : Return the integer
 ****************************************************************/
int random(int min, int max);


#endif // UI_DRAW_H
