#ifndef VECTOR2D
#define VECTOR2D

#include <stdio.h>
#include "../constants.hpp"

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
	
	float calculate_vector_length()
	{
		float length = sqrt(pow(x, 2) + pow(y, 2));
		return length;
	}
	float get_vector_rotation()
	{
		float angle = PI / 2;
		if (x == 0) return angle + PI * (int)(y < 0);
		
		angle = atanf(y / x);
		angle += PI * (int)(x < 0);
		return angle;
	}
	Vector2D<T> rotate_vector(float rotation)
	{
		const float rot_cos = cosf(rotation);
		const float rot_sin = sinf(rotation);
		
		Vector2D rotated_vector;
		rotated_vector.x = x * rot_cos - y * rot_sin;
		rotated_vector.y = x * rot_sin + y * rot_cos;
		
		return rotated_vector;
	}
	
	Vector2D<T> normalize_vector_2d()
	{
		Vector2D new_vector = Vector2D(1.0f, 0.0f);
		
		if(length == 0)
			return new_vector;
		
		new_vector.x = x / length;
		new_vector.y = y / length;
		
		return new_vector;
	}
};

#endif
