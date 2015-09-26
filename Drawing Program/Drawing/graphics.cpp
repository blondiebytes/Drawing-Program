#include "graphics.h"
#include "drawing.h"

#include <GL/glut.h>

#include <cfloat>
#include <cmath>

using namespace std;

const colorType HIGHLIGHT_COLOR = WHITE;

const double TRANSFORM_GISMO_SCALE = 25.0;


bool highlight = false;

bool getHighlight()
{
  return highlight;
}

void setHighlight(bool h)
{
  if (h) setColor(HIGHLIGHT_COLOR);
  highlight = h;
}


colorType intToColor(int item)
{
  switch ( item ) {
    case 0 : return BLACK   ;
    case 1 : return BLUE    ;
    case 2 : return GREEN   ;
    case 3 : return CYAN    ;
    case 4 : return RED     ;
    case 5 : return MAGENTA ;
    case 6 : return YELLOW  ;
    case 7 : return WHITE   ;
    case 8 : return GRAY    ;
   default : return BLACK   ;
  }
}


void setColor( colorType color )
{
  if (!highlight)
    switch ( color )
    {
      case BLACK   : glColor3f( 0.0, 0.0, 0.0 ); break;
      case BLUE    : glColor3f( 0.0, 0.0, 1.0 ); break;
      case GREEN   : glColor3f( 0.0, 1.0, 0.0 ); break;
      case CYAN    : glColor3f( 0.0, 1.0, 1.0 ); break;
      case RED     : glColor3f( 1.0, 0.0, 0.0 ); break;
      case MAGENTA : glColor3f( 1.0, 0.0, 1.0 ); break;
      case YELLOW  : glColor3f( 1.0, 1.0, 0.0 ); break;
      case WHITE   : glColor3f( 1.0, 1.0, 1.0 ); break;
      case GRAY    : glColor3f( 0.5, 0.5, 0.5 ); break;
    }
}



TransformStack transformStack;

// Initializes this transform stack to hold only an identity matrix
TransformStack::TransformStack()
{
	mStack = stack<Matrix*>();
	mStack.push(new Matrix());
}

// Form the matrix product top * transform and push it onto the stack
void TransformStack::push(Matrix* transform)
{
	// If the transform stack is empty, then we can 
	// just push on the transform
	if (mStack.empty()) {
		mStack.push(transform);
	}
	// Otherwise we have to do math
	else {
		Matrix* m = top()->multiply(transform);
		mStack.push(m);
	}
}

//Pop this transform stack
void TransformStack::pop()
{
	if (!mStack.empty()) {
		mStack.pop();
	}
}

// Return the matrix at the top of the stack
Matrix* TransformStack::top()
{
	if (!mStack.empty()) {
		return mStack.top();
	}
}

// Push on transformMatrix
void gPush(Matrix* transform)
{
	transformStack.push(transform);
}

// Pop on transformMatrix
void gPop()
{
	transformStack.pop();
	
}

// Consider the line that connects the point (x0, y0) and (x1, y1) given
// in local coordinates. Draw the line in world coordinates. 
void drawLine(double x0, double y0, double x1, double y1)
{
	glBegin(GL_LINE);
	glVertex2i(x0, y0);
	glVertex2i(x0, x1);
	glEnd();
}

// Consider the line that connects the vectors p0 and p1 given
// in local coordinates. Draw the line in world coordinates.
void drawLine(Vector* p0, Vector* p1)
{
	// Getting p0 and p1 in world coordinates
	p0 = transformStack.top()->multiply(p0);
	p1 = transformStack.top()->multiply(p1);

	// de-reference the vector pointer to get access to the actual vector
	// then go get the actual points
	double x0 = (*p0)[0];
	double y0 = (*p0)[1];
	double x1 = (*p1)[0];
	double y1 = (*p1)[1];

	// Plot the points
	drawLine(x0, y0, x1, y1);
}

// Consider the rectangle with opposite corners (x0, x0) and (x1, y1) given
// in local coordinates, whose sides are parallel to the local coordinate axies
// Draw the rectangle in world coordinates
void drawRectangle(double x0, double y0, double x1, double y1)
{
	// Imagine:
	// (x0,y1) --------- (x1, y1)
	//  |                   |
	//  |                   |
	// (x0,y0) --------- (x1, y0)

	// Now let's draw it!
	glBegin(GL_LINE_LOOP);
	glVertex2i(x0, y1);
	glVertex2i(x1, y1);
	glVertex2i(x1, y0);
	glVertex2i(x0, y0);
	glEnd();
}

// Consider the circle with radius running from (x0, y0) and (x1, y1)
// local coordinates. Draw the circle in world coordinates
void drawCircle(double x0, double y0, double x1, double y1)
{
	// Find the radius --> use formula
	double r = sqrt(((x1 - x0) * (x1 - x0)) + ((y1 - y0) * (y1 - y0)));

	// Find cX --> center X
	double cX = (x1 + x0) / 2;

	// Find cY --> center Y
	double cY = (y1 + y0) / 2;

	// Draw the circle
	drawCircle(cX, cY, r);
}

// Consider the circle with radius r, center of X at cX,
// and center of Y at cY. Draw the circle in world coordinates
void drawCircle(double cX, double cY, double radius)
{
	// drawing a series of lines to make a circle ??
	for (double theta = 0; theta < 2 * PI; theta += DELTA) {
		// Here, DELTA is 2PI / 360
		drawLine(cX + radius * cos(theta),
			cY + radius * sin(theta),
			cX + radius * cos(theta + DELTA),
			cY + radius * sin(theta + DELTA));
	}

}

// Consider the polygon with sides connecting sucessive points 
// in vertices, given in local coordinates. Draw the polygon in world
// coordinates. Close the polygon if close == true. Otherwise,
// omit the side from the last point in vertices back to the first
// point in vertices
void drawPolygon(const list<Vector*>& vertices, bool close)
{
	// Setting up iterators...

	// We have access to the current Vector and the next Vector
	list<Vector*> ::const_iterator current = vertices.begin();
	list<Vector*> ::const_iterator next = vertices.begin();
	// Increment the next so it actually is the next
	next++;
	
	// While the next vector isn't at the end
	while (next != vertices.end()) {
		// have * to dereference
		drawLine(*current, *next);
		current++;
		next++;
	}

	// If we are supposed to close the polygon...
	if (close) {
		// We draw a line from the last (current) vector
		// to the first vector
		drawLine(*current, *vertices.begin());
	}

}

void drawTransformGismo()
{
  double halfSize = TRANSFORM_GISMO_SCALE/2.0;
  drawRectangle(-halfSize,-halfSize,halfSize,halfSize);
}
