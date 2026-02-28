#include "base.h"
#include <cmath>
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include "Vector2i.h"

//-----------------------------------------------------------------
// Vector2i Constructors
//-----------------------------------------------------------------
Vector2i::Vector2i()
	:Vector2i{ 0, 0 }
{
}

Vector2i::Vector2i(int x, int y)
	: x{ x }
	, y{ y }
{
}

Vector2i::Vector2i(const Vector2i& fromPoint, const Vector2i& tillPoint)
	: Vector2i{ tillPoint.x - fromPoint.x, tillPoint.y - fromPoint.y }
{
}

// -------------------------
// Methods
// -------------------------
bool Vector2i::Equals(const Vector2i& other, int epsilon) const
{
	return (abs(x - other.x) < epsilon) && (abs(y - other.y) < epsilon);
}

int Vector2i::DotProduct(const Vector2i& other) const
{
	return x * other.x + y * other.y;
}

int Vector2i::CrossProduct(const Vector2i& other) const
{
	return x * other.y - y * other.x;
}

std::string Vector2i::ToString() const
{
	std::stringstream buffer;

	buffer << std::fixed;
	buffer << std::setprecision(2);
	buffer << "Vector2i(" << x << ", " << y << ")";
	return buffer.str();
}

float Vector2i::Norm() const
{
	return Length();
}

float Vector2i::Length() const
{
	return sqrt(static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y));
}

int Vector2i::SquaredLength() const
{
	return x * x + y * y;
}

float Vector2i::AngleWith(const Vector2i& other) const
{
	float fx{ static_cast<float>(x) }, fy{ static_cast<float>(y) },
		ofx{ static_cast<float>(other.x) }, ofy{ static_cast<float>(other.y) };

	//https://stackoverflow.com/questions/21483999/using-atan2-to-find-angle-between-two-vectors
	// not calling dot and cross functions to prevent the overhead

	return atan2(fx * ofy - ofx * fy, fx * ofx + fy * ofy);
}

Vector2f Vector2i::Normalized(float epsilon) const
{
	float length{ Length() };
	if (length < epsilon)
	{
		return Vector2f{ 0.f, 0.f };
	}
	else
	{
		return Vector2f{ static_cast<float>(x) / length, static_cast<float>(y) / length };
	}
}

Vector2i Vector2i::Orthogonal() const
{
	return Vector2i{ -y, x };
}

Vector2i Vector2i::Reflect(const Vector2i& surfaceNormal) const
{
	return (*this) - 2 * (this->DotProduct(surfaceNormal) * surfaceNormal);
}

void Vector2i::Set(int newX, int newY)
{
	x = newX;
	y = newY;
}

// -------------------------
// Member operators
// -------------------------
Vector2i Vector2i::operator-() const
{
	return Vector2i{ -x, -y };
}
Vector2i Vector2i::operator+ () const
{
	return Vector2i{ x, y };
}

Vector2i& Vector2i::operator*=(int rhs)
{
	x *= rhs;
	y *= rhs;
	return *this;
}

Vector2i& Vector2i::operator/=(int rhs)
{
	*this *= 1 / rhs;
	return *this;
}

Vector2i& Vector2i::operator+=(const Vector2i& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2i& Vector2i::operator-=(const Vector2i& rhs)
{
	*this += -rhs;
	return *this;
}

bool Vector2i::operator==(const Vector2i& rhs) const
{
	return x == rhs.x && y == rhs.y;
}

Vector2i::operator Vector2i()
{
	return Vector2i{ x,y };
}

// -------------------------
// Non-member operators
// -------------------------
Vector2i operator*(int lhs, Vector2i rhs)
{
	return rhs *= lhs;
}

Vector2i operator*(Vector2i lhs, int rhs)
{
	return lhs *= rhs;
}

Vector2i operator/(Vector2i lhs, int rhs)
{
	return lhs *= (1 / rhs);
}

Vector2i operator+(Vector2i lhs, const Vector2i& rhs)
{
	return lhs += rhs;
}

Vector2i operator-(Vector2i lhs, const Vector2i& rhs)
{
	return lhs += -rhs;
}

std::ostream& operator<< (std::ostream& lhs, const Vector2i& rhs)
{
	lhs << rhs.ToString();
	return lhs;
}