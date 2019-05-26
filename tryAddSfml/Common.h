#pragma once

#include <cmath>
#include <iostream>
#define PI 3.14159265

template <typename T> int sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

struct Coordinate {
	Coordinate() :x(0), y(0) {}
	Coordinate(double x, double y) :x(x), y(y) {}
	double x;
	double y;
};

float angleBetweenVectors(const Coordinate& A, const Coordinate& B) {
	double up = (A.x*B.x + A.y*B.y);
	double down = (sqrt(A.x*A.x + A.y*A.y) * sqrt(B.x*B.x + B.y*B.y));
	if(down != 0){
		return 180 * acos(up / down) / PI;
	}
	return 0;
}


struct Vector {
	Vector() {}
	Vector(const Vector& v) : direction(v.direction) {}
	Vector(const Coordinate & coordinate) : direction(coordinate) {}
	Coordinate operator+(const Coordinate&coordinate) {
		return Coordinate(this->direction.x + coordinate.x, this->direction.y + coordinate.y);
	}
	friend Coordinate operator+(const Coordinate&coordinate, const Vector& v) {
		return Coordinate(v.direction.x + coordinate.x, v.direction.y + coordinate.y);
	}
	Vector operator*(double number) {
		Vector v(this->direction);
		v.direction.x *= number;
		v.direction.y *= number;
		return v;
	}
	friend Vector operator*(double number, const Vector&v) {
		Vector newV = v;
		newV.direction.x *= number;
		newV.direction.y *= number;
		return newV;
	}
	Vector toUnitVector() {
		double length = sqrt(this->direction.x*this->direction.x + this->direction.y*this->direction.y);
		Coordinate c = Coordinate(direction.x / length, direction.y / length);
		return Vector(c);
	}
	static Vector calculateVector(const Coordinate& c1, const Coordinate& c2) {
		Coordinate c = Coordinate(c2.x - c1.x, c2.y - c1.y);
		return Vector(c);
	}
	Coordinate direction;
};

sf::Vector2i detectDirection(std::vector<bool>&keyboarding) {
	keyboarding[0] = keyboarding[0]&(keyboarding[0]^keyboarding[1]);
	keyboarding[1] = keyboarding[1]&(keyboarding[0]^keyboarding[1]);
	keyboarding[2] = keyboarding[2]&(keyboarding[2]^keyboarding[3]);
	keyboarding[3] = keyboarding[3]&(keyboarding[2]^keyboarding[3]);
	sf::Vector2i direction;
	if (keyboarding[0] != 0 || keyboarding[1] != 0) {
			direction.x = keyboarding[0] != 0 ? -1: 1;
	}
	else {
		direction.x = 0;
	}

	if (keyboarding[2] != 0 || keyboarding[3] != 0) {
		direction.y = keyboarding[2] != 0 ? -1 : 1;
	}
	else {
		direction.y = 0;
	}
	return direction;
}

bool isBorder(int row, int column, int i, int j) {
	if (i == 0 || j == 0 || i == row - 1 || j == column - 1) {
		return true;
	}
	return false;
}

Coordinate rotateVector(float alpha, Coordinate vector) {
	//https://academo.org/demos/rotation-about-point/
	double x = vector.x*cos(alpha) - vector.y*sin(alpha);
	double y = vector.y*cos(alpha) + vector.x*sin(alpha);
	return Coordinate(x,y);
}