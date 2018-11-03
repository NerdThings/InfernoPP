#ifndef Inferno_Vector2
#define Inferno_Vector2

#include "Inferno.h"

namespace Inferno
{
	struct INFERNO_API Vector2
	{
		//Constants

		static const Vector2* zero;
		static const Vector2* unit_x;
		static const Vector2* unit_y;

		//Fields

		float x;
		float y;

		Vector2(float x, float y);

		Vector2(float value);

		//Methods

		float get_magnitude() const;

		float get_magnitude_squared() const;

		float dot(Vector2* b) const;

		float distance(Vector2* b) const;

		//Operators

		Vector2* operator+(Vector2* b) const;

		Vector2* operator+(int b) const;

		Vector2* operator+(float b) const;

		Vector2* operator-(Vector2* b) const;

		Vector2* operator-(int b) const;

		Vector2* operator-(float b) const;

		Vector2* operator*(Vector2* b) const;

		Vector2* operator*(int b) const;

		Vector2* operator*(float b) const;

		Vector2* operator/(Vector2* b) const;

		Vector2* operator/(int b) const;

		Vector2* operator/(float b) const;

		bool operator==(Vector2* b) const;

		//TODO: Matrix Transform
	};
}

#endif