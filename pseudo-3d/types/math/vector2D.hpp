#ifndef VECTOR2D
#define VECTOR2D

#include <stdio.h>
#include <math.h>
#include <SDL3/SDL.h>

#include "constants.hpp"

template <typename T>
class Vector2D
{
public:
	T x, y;
	float length;
	
	Vector2D()
	{
		x = y = 0;
		length = 0;
	}
	Vector2D(T x, T y)
	{
		this->x = x; this->y = y;
		length = calculate_vector_length();
	}
	
	Vector2D<T> operator-(Vector2D<T> subtrahend)
	{
		return Vector2D<T>(x - subtrahend.x, y - subtrahend.y);
	}
	Vector2D<T> operator+(Vector2D<T> addend)
	{
		return Vector2D<T>(x + addend.x, y + addend.y);
	}
	Vector2D<T>* operator+=(Vector2D<T> addend)
	{
		x += addend.x; y += addend.y;
		return this;
	}
	Vector2D<T> operator*(float multiplier)
	{
		return Vector2D<T>(x * multiplier, y * multiplier);
	}
	Vector2D<T> operator/(float denominator)
	{
		return Vector2D<T>(x / denominator, y / denominator);
	}
	float operator*(Vector2D<T> multiplier)
	{
		return x * multiplier.x + y * multiplier.y;
	}
	bool operator==(Vector2D<T> point)
	{
		return (x == point.x) && (y == point.y);
	}
	bool operator!=(Vector2D<T> point)
	{
		return !((*this) == point);
	}
	bool operator<(Vector2D<T> point)
	{
		return (x < point.x) || (y < point.y);
	}
	bool operator>(Vector2D<T> point)
	{
		return (x > point.x) || (y > point.y);
	}
	bool operator<=(Vector2D<T> point)
	{
		return (x <= point.x) || (y <= point.y);
	}
	bool operator>=(Vector2D<T> point)
	{
		return (x >= point.x) || (y >= point.y);
	}
	
	float calculate_vector_length()
	{
		float length = sqrt(pow(x, 2) + pow(y, 2));
		return length;
	}
	float get_vector_rotation()
	{
//		float angle = PI / 2;
//		if (x == 0) return angle + PI * (int)(y < 0);
//		
//		angle = atanf(y / x);
//		angle += PI * (int)(x < 0);
//		return angle;
		const float angle = SDL_atan2f(y, x); //FIXME: Need to learn why it need normalizing in fact and how to fix my func.
		return angle;
	}
	void invert_vector()
	{
		x = -x;
		y = -y;
		return;
	}
	float cross_product(Vector2D<T> vector)
	{
		return x * vector.y - y * vector.x;
	}
	Vector2D<T> rotate_vector(float rotation)
	{
		float rot_cos = cosf(rotation), rot_sin = sinf(rotation);
		
		if(abs(rot_cos) < EPS) rot_cos = 0;
		if(abs(rot_sin) < EPS) rot_sin = 0;
		
		Vector2D rotated_vector;
		rotated_vector.x = x * rot_cos - y * rot_sin;
		rotated_vector.y = x * rot_sin + y * rot_cos;
		rotated_vector.length = length;
		
		return rotated_vector;
	}
	
	Vector2D<T> normalize_vector_2d()
	{
		Vector2D new_vector = Vector2D(1.0f, 0.0f);
		
		if(length == 0)
			return Vector2D(0.0f, 0.0f);
		
		new_vector.x = x / length;
		new_vector.y = y / length;
		
		return new_vector;
	}
	Vector2D<T> get_vector_normal()
	{
		Vector2D<T> vector_normal = {-y, x};
		
		return vector_normal.normalize_vector_2d();
	}
};

#endif
