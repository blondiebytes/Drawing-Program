#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"



// Initialize this transform node to have parent p, shape node NULL, and the transform identity
TransformNode::TransformNode(TransformNode* p)
{
	parent = p;
	shapeNode = NULL;
	matrix = new Matrix();
	selected = false;
	identifier = count++;
	idTransformNodeTable[identifier] = this;
}

// Initialize this transform to have parent p, shape node s, and transform t
TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t)
{
	parent = p;
	shapeNode = s;
	matrix = t;
	selected = false;
	identifier = count++;
	idTransformNodeTable[identifier] = this;
}

// Arrange that deleting this transform deletes all its descendants
TransformNode::~TransformNode()
{
	// delete children first
	children.clear();
	// then the parent
	delete[] parent;
	delete[] shapeNode;
	delete(matrix);
}

// Update this transform node nto translate by (dx, dy) in world coordinates
void TransformNode::translate(double deltaX, double deltaY)
{
	// transformationMatrix * originalMatrix = newMatrix
	matrix = Matrix::translation(deltaX, deltaY)->multiply(matrix);
}

// Update this transform node to rotate by theta around the origin in world coordinates
void TransformNode::rotate(double theta)
{
	// move to the origin... then ??
	// rotationMatrix * originalMatrix = newMatrix
	matrix = Matrix::rotation(theta)->multiply(matrix);
}

// Update this transform node to apply shear (sXY, sYX) in world coordinates
void TransformNode::shear(double shearXY, double shearYX)
{
	// shearingMatrix * originalMatrix = newMatrix
	matrix = Matrix::shearing(shearXY, shearYX)->multiply(matrix);
}

// Update this transform node to apply scale(sX, sY) in world coordinates.
void TransformNode::scale(double scaleX, double scaleY)
{
	// scalingMatrix * originalMatrix = newMatrix
	matrix = Matrix::scaling(scaleX, scaleY)->multiply(matrix);
}

// Draw the portion of the scene represented by this transform node
// If displayHelpers is true, display a square indicating the origin
// of the coordinate system of this node !!!!
void TransformNode::draw(bool displayHelpers) const
{
	// Pushing on the matrix and identifier before we draw
	gPush(matrix);
	glPushName(identifier);
	
	// Getting the highlight;
	bool highlight = getHighlight();

	// If selected.. we set the highlight to selected
	if (selected) {
		setHighlight(selected);
	}

	// Draw the shape node first if we have one
	if (shapeNode != NULL) {
		shapeNode->draw();
	}

	// ??  What does :: mean? --> Membership + Templates
	for (list<TransformNode*> :: const_iterator i = children.begin(); i != children.end(); i++) {
		// What does -> mean??
		// Draw all the children until we run out
		(*i)->draw(displayHelpers);
	}

	// where do we do stuff with displayHelpers??

	// Pop it all off because we are done
	setHighlight(highlight);
	glPopName();
	gPop();
		
}

// Return the parent of this transform node
TransformNode* TransformNode::getParent() const 
{ 
	return parent;
}

// Set p to be the parent of this transform node
void TransformNode::setParent(TransformNode* p) 
{ 
	parent = p;
}

// Make nP the new parent of this transform node, taking care that the local coordinate
// system of this node remains unchanged.
void TransformNode::changeParent(TransformNode* newParent)
{
	//?? How do we take care of the coordinate system
	parent = newParent;
}

// Construct a new transform node under this object. Make groupMembers children
// of the new transform node. Assume all members are presently children of this node. !! ??
void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
 // Create a new transform node with this object as the parent
	TransformNode temp = new TransformNode(this);

	// VIA ITERATION:
	// 1. Remove the children from their current parent
	// 2. Make temp their new parent
	for (set<TransformNode*> ::iterator i = groupMembers.begin(); i != groupMembers.end(); i++) {
		// Remove child from current parent
		//(*i).getParent().remove(*i); --> won't allow access
		// Make temp their new parent
		temp.children.push_back(*i);

		// OR
		//(*i).changeParent(temp);
	}


}

// Return the matrix representing the transform associated with this node
Matrix* TransformNode::getTransform() const
{
	return matrix;
}

// Return a copy of this transform node, its shapeNode (if any) and 
// its descendant transform nodes. The copy should have a NULL parent
TransformNode* TransformNode::clone() const
{
	// We have null for the parent because we want to use this clone elsewhere
	// What about childrent/descendant nodes??
	return (TransformNode*)(NULL, shapeNode, matrix);
}

// Add child to the collection of children of this transform node
void TransformNode::addChild(TransformNode* child)
{
	// add the child to list of transform nodes
	children.push_back(child);
}

// Remove child from the collection of children of this transform node
void TransformNode::removeChild(TransformNode* child)
{
	// remove the child from the list of transform nodes
	children.remove(child);
}

// Return the first child in the collection of children of this transform node
TransformNode* TransformNode::firstChild() const
{
	// return the first
	return children.front();
}

// Returns the last child in the collection of children of this transform node
TransformNode* TransformNode::lastChild() const
{
	// return the last
	return children.back();
}

// Returns the child next after param child in the collection of children 
// in this transform node !!
TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	// we must iterate through the children...
	list<TransformNode*> ::const_iterator i = children.begin();

	// Is there a way to simplify this??
	for (; (*i) != child; i++) {
		// Get i to its correct position/index
	}
	
	// Increment i so that we are at the nextChild...
	i++;

	// If the nextChild is at the end, then we reset to the beginning
	if (i == children.end()) {
		i = children.begin();
	}

	// We return the child at the pointer i
	return (*i);
		
}

// Returns the child before the param child in the collection of children
// in this transform node !!
TransformNode* TransformNode::previousChild(TransformNode* child) const
{
	// we must iterate through the children...
	list<TransformNode*> ::const_iterator i = children.begin();

	// Is there a way to simplify this??
	for (; (*i) != child; i++) {
		// Get i to its correct position/index
	}

	// Decrement i so that we are at the previousChild...
	i--;

	// If the previousChild is at the end, then we reset to the beginning
	if (i == children.end()) {
		i = children.begin();
	}

	// We return the child at the pointer i
	return (*i);

}

// Mark this transform node as selected so that its subtree is 
// drawn in a special highlight color
void TransformNode::select() 
{
	selected = true;
}

// Mark this transform node as not selected, so that its subtree 
// is drawn in its normal color
void TransformNode::deSelect() 
{
	selected = false;
}

// Return the transform node associated with identifier
TransformNode* TransformNode::nodeLookup(int identifier)
{
	return idTransformNodeTable[identifier];
}

// Initialize this shape node sub-object with color c and NULL
// transform node
ShapeNode::ShapeNode(colorType c) 
{
	transformNode = NULL;
	color = c;
}

// Set tn to be the transform node of this shape node
void ShapeNode::setTransformNode(TransformNode* tn)
{
	transformNode = tn;
}

// Returns the transform node of this shape node
TransformNode* ShapeNode::getTransformNode()
{
	return transformNode;
}

// Initializes line object of color c connecting (x0, y0) and (x1,y1) in world coordinates
Line::Line(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c)
{
	x_0 = xx0;
	x_1 = xx1;
	y_0 = yy0;
	y_1 = yy1;
	color = c;
}


// Clone a line
ShapeNode* Line::clone() const
{
	// Return a line with the appropriate specifications
	// A line is a ShapeNode so we can return it instead of
	// just a bland ShapeNode
	return (Line*)(x_0, y_0, x_1, y_1, color);
}

// Draw a line
void Line::draw() const
{
	// Set the color to the right one
	setColor(color);
	// And draw out the line
	drawLine(x_0, y_0, x_1, y_1);
}


// Initializes recentage of color c with opposite corners (x0, y0) and (x1, y1)
// in world coordinates
Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c)
{
	color = c;
	x_0 = xx0;
	x_1 = xx1;
	y_0 = yy0;
	y_1 = yy1;
	color = c;
}

// Clone the rectangle
ShapeNode* Rectangle::clone()  const
{
   return (Rectangle*)(x_0, y_0, x_1, y_1, color);
}

// Draw the rectangle
void Rectangle::draw() const
{
	// Set the appropriate color
	setColor(color);
	// And then draw it out
	drawRectangle(x_0, y_0, x_1, y_1);
}

// Initialize a circle of color c with center (cX, cY) and radius r in 
// world coordinates
Circle::Circle(double ccX, double ccY, double r, colorType c) 
	: ShapeNode(c)

{
	cX = ccX;
	cY = ccY;
	radius = r;
}

// Clone the circle
ShapeNode* Circle::clone() const
{
	return (Circle *)(cX, cY, radius, color);
}

// Draw a circle
void Circle::draw() const
{
	// Set the appropriate color
	setColor(color);
	// And then draw it out
	drawCircle(cX, cY, radius);
}

//Why do we need & sign??
// Initialize a polygon of color c with vertices vs in world coordinates
Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	color = c;
	vertices = vs;
}

// Delete a Polygon
Polygon::~Polygon()
{
	// Iterate through the vertices, deleting each one
	for (list<Vector*> ::const_iterator i = vertices.begin(); i != vertices.end(); i++) {
		// Go delete the vertex at i
		delete(*i);
	}
	// Do we need to delete anything else??
}

// Clone a Polygon
ShapeNode* Polygon::clone() const
{
	return (Polygon*)(vertices, color);
}

// Draw a polygon
void Polygon::draw() const
{
	// Set a color
	setColor(color);
	// Draw it out
	drawPolygon(vertices);
}

