#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"
#include <iostream>

using namespace std;

// Initializes the zero vector
Vector::Vector()
{
	data = new double[3];
	data[0] = 0;
	data[1] = 0;
	data[2] = 1;
} 

// Initializes this vector to be a copy of oldVector
Vector::Vector(const Vector& oldVector) 
{
	data = new double[3];
	data[0] = oldVector[0];
	data[1] = oldVector[1];
	data[2] = oldVector[2];
}

// Initializes this vector to represent the point (x, y)
Vector::Vector(const double x, const double y)
{
	data = new double[3];
	data[0] = x;
	data[1] = y;
	data[2] = 1;
}

// Destructor for vectors
Vector::~Vector()
{
	delete[] data;
}

// Return a reference to the ith element of this vector
double& Vector::operator[](int index) const
{
	return data[index];
}

// Initialize this to an identity matrix
// 1 0 0
// 0 1 0
// 0 0 1
Matrix::Matrix() 
{
	data = new double*[3];
	for (int i = 0; i < 3; i++) {
		data[i] = new double[3];
		for (int j = 0; j < 3; j++) {
			// If i = j, put in a 1
			if (i == j) {
				data[i][j] = 1;
			}
			else {
				// otherwise put in a 0
				data[i][j] = 0;
			}
		}
	}

}

// Initialize this matrix to be a copy of oldMatrix
Matrix::Matrix(const Matrix& oldMatrix) 
{
	// Creates the container of rows
	data = new double*[3];
	for (int i = 0; i < 3; i++) {
		// Creates a row
		data[i] = new double[3];
		for (int j = 0; j < 3; j++) {
			// copy it over!
			data[i][j] = oldMatrix[i][j];
			}
	}
}

// Destructor for matrices
Matrix::~Matrix()
{
	for (int i = 0; i < 3; i++) {
		// delete each row
		delete [] data[i];
	}
	// then delete the whole thing
	delete [] data;
}

// Allocate and return a matrix representing the product of this matrix and the other matrix
Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{	// Creates a new matrix
	Matrix* c = new Matrix();
	// C_ij = the sum of A_ik B_kj as k goes to n(length of vector)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// Because our init matrix is the identity matrix
			(*c)[i][j] = 0;
			for (int k = 0; k < 3; k++) {
				// Deferencing c to get access to the whole Matrix versus just the pointer
				// We do this because the [] operand is defined for matrices. 
				// HOWEVER, once we get the row at i, we just have a double array so we use
				// the regular indexing operation
				(*c)[i][j] += data[i][k] * (*otherMatrix)[k][j];
			}
		}
	}

	cout << "Data Matrix" << endl;

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << data[i][j] << " ";
		}
		cout << endl;
	}

	cout << "Multiplied Matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << (*otherMatrix)[i][j] << " ";
		}
		cout << endl;
	}

	cout << "New Matrix" << endl;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << (*c)[i][j] << " ";
		}
		cout << endl;
	}

	return c;
}

// Allocate and return a vector representing the produce of this matrix and the Vector
Vector* Matrix::multiply(const Vector* theVector) const
{
	Vector* v = new Vector[3];
	// v[i] = the sum of data_ij times theVector[j] as j goes to n (length of vector)
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// Don't foregt dereferencing to get access to the actual Vector, where the [] operation is defined
			(*v)[i] += data[i][j] * (*theVector)[j];
		}
	}
	return v;
}

// Return a pointer to an array of doubles representing the ith row of this matrix
double* Matrix::operator[](int index) const
{
	// returning a pointer to one of the rows --> i.e. no need to dereferncing
	return data[index];
}

// Allocate and return a matrix implementing a translation (deltaX, deltaY)
Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* m = new Matrix();
	// Creating...
	// ----------
	// 1     0     deltaX
	// 0     1     deltaY
	// 0     0     1
	// ----------
	(*m)[0][2] = deltaX;
	(*m)[1][2] = deltaY;
	return m;
}

// Allocate and return a matrix implementing a rotation by theta (in radians)
Matrix* Matrix::rotation(double theta)
{
	// All we have to do is change [0][0], [0][1], [1,0], and [1,1] 
	// because we are given the identity matrix in init
	// Creating...
	// ----------
	// cos(?)   -sin(?)   0
	// sin(?)   cos(?)    0
	// 0        0         1
	// ----------
	Matrix* m = new Matrix();
	(*m)[0][0] = cos(theta);
	(*m)[0][1] = -sin(theta);
	(*m)[1][0] = sin(theta);
	(*m)[1][1] = cos(theta);
	return m;
}

// Allocate and return a matrix implementing a shearing (shearXY, shearYX)
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	// All we have to do is change [0][1] and [1][0] because we are given
	// the identity matrix in init
	// Creating...
	// ----------
	// 1     Sxy   0
	// Syx   1     0
	// 0     0     1
	// ----------
	Matrix* m = new Matrix();
	(*m)[0][1] = shearXY;
	(*m)[1][0] = shearYX;
	return m;
}

// Allocate and return a matrix implementing a scaling(scaleX, scaleY)
Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	Matrix* m = new Matrix();
	// All we have to do is change [0][0] and [1][1] because we are given
	// the identity matrix in init
	// Creating...
	// ----------
	// Sx  0   0
	// 0   Sy  0
	// 0   0   1
	// ----------
	(*m)[0][0] = scaleX;
	(*m)[1][1] = scaleY;
	return m;
	
}

// Allocate and return the inverse of this matrix
Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->data[0][0] = -data[1][2] * data[2][1] + data[1][1] * data[2][2];
	answer->data[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	answer->data[0][2] = -data[0][2] * data[1][1] + data[0][1] * data[1][2];
	answer->data[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	answer->data[1][1] = -data[0][2] * data[2][0] + data[0][0] * data[2][2];
	answer->data[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	answer->data[2][0] = -data[1][1] * data[2][0] + data[1][0] * data[2][1];
	answer->data[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	answer->data[2][2] = -data[0][1] * data[1][0] + data[0][0] * data[1][1];

	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			answer->data[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][2] * data[2][0] +
		data[0][2] * data[1][0] * data[2][1] - data[0][0] * data[1][2] * data[2][1] -
		data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];
}
