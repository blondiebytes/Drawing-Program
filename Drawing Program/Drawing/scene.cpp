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
	children.clear;
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

void TransformNode::draw(bool displayHelpers) const
{

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
	//??
	parent - newParent;
}

void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
 
}

// Return the matrix representing the transform associated with this node
Matrix* TransformNode::getTransform() const
{
	return matrix;
}

TransformNode* TransformNode::clone() const
{
   return NULL;
}

void TransformNode::addChild(TransformNode* child)
{

}

void TransformNode::removeChild(TransformNode* child)
{
	
}

// Return the first child in the collection of children of this transform node
TransformNode* TransformNode::firstChild() const
{
	return children._Getpfirst;
}

TransformNode* TransformNode::lastChild() const
{
	return NULL;
}

TransformNode* TransformNode::nextChild(TransformNode* child) const
{
   return NULL;
}

TransformNode* TransformNode::previousChild(TransformNode* child) const
{
   return NULL;
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


ShapeNode* Line::clone() const
{
   return NULL;
}

void Line::draw() const
{

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


ShapeNode* Rectangle::clone()  const
{
   return NULL;
}

void Rectangle::draw() const
{

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

ShapeNode* Circle::clone() const
{
   return NULL;
}

void Circle::draw() const
{

}

//Why do we need & sign??
// Initialize a polygon of color c with vertices vs in world coordinates
Polygon::Polygon(const list<Vector*>& vs, colorType c) 
	: ShapeNode(c)
{
	color = c;
	vertices = vs;
}

Polygon::~Polygon()
{

}

ShapeNode* Polygon::clone() const
{
   return NULL;
}

void Polygon::draw() const
{

}

