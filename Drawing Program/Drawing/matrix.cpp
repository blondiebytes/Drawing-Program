#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"

using namespace std;

// Initializes the zero vector
Vector::Vector()
{
	double v[3];
	v[0] = 0;
	v[1] = 0;
	v[2] = 1;
} 

// Initializes this vector to be a copy of oldVector
Vector::Vector(const Vector& oldVector) 
{
	double v[3];
	v[0] = oldVector[0];
	v[1] = oldVector[1];
	v[2] = oldVector[2];
}

// Initializes this vector to represent the point (x, y)
Vector::Vector(const double x, const double y)
{
	double v[3];
	v[0] = x;
	v[1] = y;
	v[2] = 1;
}

// ask
// Destructor for vectors
Vector::~Vector()
{
	delete[] this;
}

// ask
// Return a reference to the ith element of this vector
double& Vector::operator[](int index) const
{
	return v[index];
}

// Initialize this to an identity matrix
// 1 0 0
// 0 1 0
// 0 0 1
Matrix::Matrix() 
{
	double m[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// If i = j, put in a 1
			if (i == j) {
				m[i][j] = 1;
			}
			else {
				// otherwise put in a 0
				m[i][j] = 0;
			}
		}
	}

}

// Initialize this matrix to be a copy of oldMatrix
Matrix::Matrix(const Matrix& oldMatrix) 
{
	double m[3][3];
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			// copy it over!
			m[i][j] = oldMatrix[i][j];
			}
	}
}

// Destructor for matrices
Matrix::~Matrix()
{
	for (int i; i < 3; i++) {
		// delete each row
	}
	// then delete the whole thing
}

// Allocate and return a matrix representing the product of this matrix and the other matrix
Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
	double m[2][2];
	for (int i; i < 2; i++) {
		for (int j; j < 2; j++) {
			// COME BACK TO
		}
	}
	return NULL;
}

// Allocate and return a vector representing the produce of this matrix and the Vector
Vector* Matrix::multiply(const Vector* theVector) const
{
	double m[2][2];
	for (int i; i < 2; i++) {
		for (int j; j < 2; j++) {
			m[i][j] = this[i][j] * theVector[j];
		}
	}
	return NULL;
}

double* Matrix::operator[](int index) const
{
	return NULL;
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	return NULL;
}

Matrix* Matrix::rotation(double theta)
{
	return NULL;
}
Matrix* Matrix::shearing(double shearXY, double shearYX)
{
	return NULL;
}

Matrix* Matrix::scaling(double scaleX, double scaleY)
{
	double m[3][3];
	
}

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
