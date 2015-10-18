#include <iostream>
#include <algorithm>
#include <GL/glut.h>
#include "scene.h"

using namespace std;

// Need to fix grouping
// parent/child relations
// Circle in world coordinates?
// CWT matrix?

int TransformNode::count = 0;
map<int, TransformNode*> TransformNode::idTransformNodeTable = map<int, TransformNode*>();

// Initialize this transform node to have parent p, shape node NULL, and the transform identity
TransformNode::TransformNode(TransformNode* p) : parent(p), shapeNode(NULL), 
matrix(new Matrix()), selected(false), identifier(count++)
{
	idTransformNodeTable[identifier] = this;
}

// Initialize this transform to have parent p, shape node s, and transform t
TransformNode::TransformNode(TransformNode* p, ShapeNode* s, Matrix* t) : parent(p), shapeNode(s), matrix(t), 
selected(false), identifier(count++)
{
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

// Computing the Cumulative World Transform Matrix (CWT)
Matrix* TransformNode::computeCumulativeWorldTransform() const{
	if (getParent() == NULL) {
		return getTransform();
	}
	else {
		// How do we multiply transform nodes?
		return (getParent()->computeCumulativeWorldTransform())->multiply(this->getTransform());
	}
}

// Update this transform node nto translate by (dx, dy) in world coordinates
void TransformNode::translate(double deltaX, double deltaY)
{
	// newMatrix = CWT(transformNode) * translation * CWT(transformNode)-1
	//Matrix* CWT = this->computeCumulativeWorldTransform();
	//Matrix* inverseCWT = CWT->getInverse(); 
	//Matrix* translation = Matrix::translation(deltaX, deltaY);
	//matrix = CWT->multiply(translation)->multiply(inverseCWT);
	matrix = Matrix::translation(deltaX, deltaY)->multiply(matrix);
}

// Update this transform node to rotate by theta around the origin in world coordinates
void TransformNode::rotate(double theta)
{
	// X = CWT(transformNode) * rotation * CWT(transformNode)-1
	//Matrix* CWT = this->computeCumulativeWorldTransform();
	//Matrix* inverseCWT = CWT->getInverse();
	//Matrix* rotation = Matrix::rotation(theta);
	//matrix = CWT->multiply(rotation)->multiply(inverseCWT);
	matrix = Matrix::rotation(theta)->multiply(matrix);
}

// Update this transform node to apply shear (sXY, sYX) in world coordinates
void TransformNode::shear(double shearXY, double shearYX)
{
	// X = CWT(transformNode) * shearing * CWT(transformNode)-1
	//Matrix* CWT = this->computeCumulativeWorldTransform();
	//Matrix* inverseCWT = CWT->getInverse();
	//Matrix* shearing = Matrix::shearing(shearXY, shearYX);
	//matrix = CWT->multiply(shearing)->multiply(inverseCWT);
	matrix = Matrix::shearing(shearXY, shearYX)->multiply(matrix);
}

// Update this transform node to apply scale(sX, sY) in world coordinates.
void TransformNode::scale(double scaleX, double scaleY)
{
	//X = CWT(transformNode) * scaling * CWT(transformNode)-1
	//Matrix* CWT = this->computeCumulativeWorldTransform();
	//Matrix* inverseCWT = CWT->getInverse();
	//Matrix* scaling = Matrix::scaling(scaleX, scaleY);
	//matrix = CWT->multiply(scaling)->multiply(inverseCWT);
	matrix = Matrix::scaling(scaleX, scaleY);
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
	bool oldSelected = getHighlight();

	// If selected.. we set the highlight to selected
	if (selected) {
		setHighlight(true);
	}

	// Draw the shape node first if we have one
	if (shapeNode != NULL) {
		shapeNode->draw();
	}

	for (list<TransformNode*> :: const_iterator i = children.begin(); i != children.end(); i = next(i)) {
		// Draw all the children until we run out
		(*i)->draw(displayHelpers);
	}

	// where do we do stuff with displayHelpers?? LATER

	// Pop it all off because we are done
	setHighlight(oldSelected);
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
	//!! How do we take care of the coordinate system
	// X = new matrix
	// X = CWT(Pnew)-1 * CWT(Pold) * (old matrix)
	parent = newParent;
}

// Construct a new transform node under this object. Make groupMembers children
// of the new transform node. Assume all members are presently children of this node. 
void TransformNode::groupObjects(set<TransformNode*>& groupMembers)
{
 // Create a new transform node with this object as the parent
	TransformNode* temp = new TransformNode(this);

	// VIA ITERATION:
	// 1. Remove the children from their current parent
	// 2. Make temp their new parent
	for (set<TransformNode*> ::iterator i = groupMembers.begin(); i != groupMembers.end(); i = next(i)) {
		// Remove child from current parent
		(*i)->getParent()->removeChild(*i);
		// Make temp their new parent
		temp->addChild(*i);
	}
	// Add temp to be a child of this transformNode
	this->addChild(temp);
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
	Matrix* matrixCopy = new Matrix(*matrix);
	ShapeNode* shapeNodeCopy = shapeNode->clone();
	TransformNode* nodeCopy = new TransformNode(NULL, shapeNodeCopy, matrixCopy);

	for (list<TransformNode*> ::const_iterator i = children.begin(); i != children.end(); i = next(i)) {
		TransformNode* temp = *i;
		nodeCopy->addChild(temp);
	}
	
	return nodeCopy;
	
}

// Add child to the collection of children of this transform node
void TransformNode::addChild(TransformNode* child)
{
	// add the child to list of transform nodes
	children.push_back(child);
	child->parent = this;
}

// Remove child from the collection of children of this transform node
void TransformNode::removeChild(TransformNode* child)
{
	// remove the child from the list of transform nodes
	children.remove(child);
	child->parent = NULL;
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
// in this transform node
TransformNode* TransformNode::nextChild(TransformNode* child) const
{
	// we must iterate through the children...
	list<TransformNode*> ::const_iterator i = children.begin();

	for (; (*i) != child; i=next(i)) {
		// Get i to its correct position/index
	}
	
	// Increment i so that we are at the nextChild...
	i = next(i);

	// If the nextChild is at the end, then we reset to the beginning
	if (i == children.end()) {
		i = children.begin();
	}

	// We return the child at the pointer i
	return (*i);
		
}

// Returns the child before the param child in the collection of children
// in this transform node 
TransformNode* TransformNode::previousChild(TransformNode* child) const
{
	// we must iterate through the children...
	list<TransformNode*> ::const_iterator i = children.begin();

	for (; (*i) != child; i = next(i)) {
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
ShapeNode::ShapeNode(colorType c) : color(c), transformNode(NULL) {}

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
	: ShapeNode(c), x_0(xx0), x_1(xx1), y_0(yy0), y_1(yy1) {}


// Clone a line
ShapeNode* Line::clone() const
{
	// Return a line with the appropriate specifications
	// A line is a ShapeNode so we can return it instead of
	// just a bland ShapeNode
	return new Line(x_0, y_0, x_1, y_1, color);
}

// Draw a line
void Line::draw() const
{

	// Set the color to the right one
	setColor(color);
	// Create vectors
	Vector* p0 = new Vector(x_0, y_0);
	Vector* p1 = new Vector(x_1, y_1);
	// And draw the line
	drawLine(p0, p1);
}

// Initializes recentage of color c with opposite corners (x0, y0) and (x1, y1)
// in world coordinates
Rectangle::Rectangle(double xx0, double yy0, double xx1, double yy1, colorType c)
	: ShapeNode(c), x_0(xx0), x_1(xx1), y_0(yy0), y_1(yy1) {}

// Clone the rectangle
ShapeNode* Rectangle::clone()  const
{
   return new Rectangle(x_0, y_0, x_1, y_1, color);
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
	: ShapeNode(c), cX(ccX), cY(ccY), radius(r) {}


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

// Initialize a polygon of color c with vertices vs in world coordinates
Polygon::Polygon(const list<Vector*>& vs, colorType c)
	: ShapeNode(c){
	list<Vector*> newVertices = list<Vector*>();
	for (list<Vector*> ::const_iterator i = vs.begin(); i != vs.end(); i = next(i)) {
		Vector* temp = *i;
		cout << "Original: " << (*temp)[0] << ":";
		cout << (*temp)[1];
		newVertices.push_back(new Vector(*temp));
	}
	vertices = newVertices;

	for (list<Vector*> ::const_iterator i = vertices.begin(); i != vertices.end(); i = next(i)) {
		Vector* temp = *i;
		cout << "Copied Over:" << (*temp)[0] << ":";
		cout << (*temp)[1];
	}
	

}

// Delete a Polygon
Polygon::~Polygon()
{
	// Iterate through the vertices, deleting each one
	for (list<Vector*> ::const_iterator i = vertices.begin(); i != vertices.end(); i = next(i)) {
		// Go delete the vertex at i
		delete(*i);
	}
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

