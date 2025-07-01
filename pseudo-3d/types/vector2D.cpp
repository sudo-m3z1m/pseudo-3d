#include "vector2D.hpp"

template<typename T>
Vector2D<T>::Vector2D()
{
	x = y = 0;
}

template<typename T>
Vector2D<T>::Vector2D(T x, T y)
{
	this->x = x; this->y = y;
}

template<typename T>
float Vector2D<T>::get_vector_length()
{
	float length = sqrt(pow(x, 2) + pow(y, 2));
	return length;
}

template<typename T>
float Vector2D<T>::get_vector_rotation()
{
	float angle = PI / 2;
	if (x == 0) return angle + PI * (int)(y < 0);
	
	angle = atanf(y / x);
	angle += PI * (int)(x < 0);
	return angle;
}

template<typename T>
Vector2D<T> Vector2D<T>::normalize_vector_2d()
{
	Vector2D new_vector = Vector2D();
	float vector_len = get_vector_length(*(this));
	
	if(vector_len == 0)
		return new_vector;
	
	new_vector.x = x / vector_len;
	new_vector.y = y / vector_len;
	
	return new_vector;
}

template<typename T>
Vector2D<T> Vector2D<T>::rotate_vector(float rotation)
{
	const float rot_cos = cosf(rotation);
	const float rot_sin = sinf(rotation);
	
	Vector2D rotated_vector;
	rotated_vector.x = x * rot_cos - y * rot_sin;
	rotated_vector.y = x * rot_sin + y * rot_cos;
	
	return rotated_vector;
}
