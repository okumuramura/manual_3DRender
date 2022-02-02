#include "Matrix.h"
#include <cmath>

Matrix Matrix::rotate_matrix(float angle_x, float angle_y, float angle_z, sf::Vector3f origin) {
	Matrix rmat;
	float rad = 0.0174553;
	float alpha = rad * angle_x + rad * angle_y + rad * angle_z;
	if (angle_z != 0.f) {
		rmat.mat[0][0] = std::cosf(alpha);
		rmat.mat[1][0] = std::sinf(alpha);
		rmat.mat[0][1] = -std::sinf(alpha);
		rmat.mat[1][1] = std::cosf(alpha);
	}
	else if (angle_x != 0.f) {
		rmat.mat[1][1] = std::cosf(alpha);
		rmat.mat[1][2] = -std::sinf(alpha);
		rmat.mat[2][1] = std::sinf(alpha);
		rmat.mat[2][2] = std::cosf(alpha);
	}
	else if (angle_y != 0.f) {
		rmat.mat[0][0] = std::cosf(alpha);
		rmat.mat[0][2] = std::sinf(alpha);
		rmat.mat[2][0] = -std::sinf(alpha);
		rmat.mat[2][2] = std::cosf(alpha);
	}

	return rmat;
}

Matrix Matrix::scale_matrix(float x, float y, float z, sf::Vector3f origin) {
	Matrix smat;
	smat.mat[0][0] = x;
	smat.mat[1][1] = y;
	smat.mat[2][2] = z;
	return smat;
}

Matrix Matrix::translate_matrix(float x, float y, float z) {
	Matrix tmat;
	tmat.mat[0][3] = x;
	tmat.mat[1][3] = y;
	tmat.mat[2][3] = z;
	return tmat;
}

Matrix Matrix::operator*(const Matrix& other) {
	Matrix new_matrix;
	for (int i = 0; i < 4; i++) new_matrix.mat[i][i] = 0.f;

	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			for (int k = 0; k < 4; k++) {
				new_matrix.mat[i][j] += this->mat[i][k] * other.mat[k][j];
			}
		}
	}

	return new_matrix;
}

void Matrix::rotate(float angle_x, float angle_y, float angle_z) {
	
}

void Matrix::scale(float x, float y, float z) {
	
}

void Matrix::translate(float x, float y, float z) {
	
}

float Matrix::get(int row, int col) const{
	return this->mat[row][col];
}

void Matrix::set(int row, int col, float volume) {
	this->mat[row][col] = volume;
}