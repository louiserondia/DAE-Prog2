#pragma once
#include "structs.h"

struct Vector2i final
{
	// -------------------------
	// Constructors 
	// -------------------------
	Vector2i();
	explicit Vector2i(int x, int y);
	explicit Vector2i(const Vector2i& fromPoint, const Vector2i& tillPoint);

	// -------------------------
	// Member operators
	// -------------------------
	Vector2i operator-() const;
	Vector2i operator+() const;
	Vector2i& operator*=(int rhs);
	Vector2i& operator/=(int rhs);
	Vector2i& operator+=(const Vector2i& rhs);
	Vector2i& operator-=(const Vector2i& rhs);
	bool operator==(const Vector2i& rhs) const;
	explicit operator Vector2i();

	// -------------------------
	// Methods
	// -------------------------

	// Are two vectors equal within a threshold?				
	// u.Equals(v)
	bool Equals(const Vector2i& other, int epsilon = 0.001f) const;

	// Convert to String 
	std::string	ToString() const;

	// DotProduct
	// int d = u.DotProduct(v);
	int DotProduct(const Vector2i& other) const;

	// CrossProduct 
	// int d = u.CrossProduct(v);
	int CrossProduct(const Vector2i& other) const;

	// Norm of a vector 
	// int l = v.Norm();
	float Norm() const;

	// Length of a vector: 
	// int l = v.Length();
	float Length() const;

	// Square Length of a vector.
	// Faster alternative for Length, sqrt is not executed. 
	int SquaredLength() const;

	// AngleWith returns the smallest angle with another vector within the range [-PI/2, PI/2]. 
	// A pos angle is counter clockwise from this to the other
	// int angle = u.AngleWith(v);
	float AngleWith(const Vector2i& other) const;


	// Returns normalized form of a vector
	// Vector2i n = v.Normalized();
	Vector2f Normalized(float epsilon = 0.001f) const;

	// Returns the orthogonal of the Vector2i
	// Vector2i w = v.Orthogonal();
	Vector2i Orthogonal() const;

	// Returns a vector that is the reflection of the Vector2i
	// surfaceNormal: represents the normal of the surface at the reflection point
	Vector2i Reflect(const Vector2i& surfaceNormal) const;

	// Sets the values of x and y
	void Set(int newX, int newY);

	// -------------------------
	// Datamembers 
	// -------------------------
	int x;
	int y;
};
// -------------------------
// Non member operators
// -------------------------
Vector2i operator*(int lhs, Vector2i rhs);
Vector2i operator*(Vector2i lhs, int rhs);
Vector2i operator/(Vector2i lhs, int rhs);

Vector2i operator+(Vector2i lhs, const Vector2i& rhs);
Vector2i operator-(Vector2i lhs, const Vector2i& rhs);

std::ostream& operator<< (std::ostream& lhs, const Vector2i& rhs);

// from stackoverflow
// https://stackoverflow.com/questions/59060054/implement-hash-for-custom-class-c

template<> struct std::hash<Vector2i> {
	std::size_t operator()(Vector2i const& v) const noexcept {
		std::size_t h1 = std::hash<int>{}(v.x);
		std::size_t h2 = std::hash<int>{}(v.y);
		return h1 ^ (h2 << 1);
	}
};

template<> struct std::hash<const Vector2i> {
	std::size_t operator()(Vector2i const& v) const noexcept {
		std::size_t h1 = std::hash<int>{}(v.x);
		std::size_t h2 = std::hash<int>{}(v.y);
		return h1 ^ (h2 << 1);
	}
};

