#ifndef Inferno_Matrix
#define Inferno_Matrix

#include "Inferno.h"

namespace Inferno {
	struct Vector3;

	struct INFERNO_API Matrix {
		//Constants

		static Matrix identity;
		
		//Fields
        
        bool initialised = false;
        
        float m11;
		float m12;
		float m13;
		float m14;
		float m21;
		float m22;
		float m23;
		float m24;
		float m31;
		float m32;
		float m33;
		float m34;
		float m41;
		float m42;
		float m43;
		float m44;

		Matrix();

		Matrix(float m11, float m12, float m13, float m14,
			float m21, float m22, float m23, float m24,
			float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

		//Methods

		static Matrix create_orthographic(float width, float height, float zNear, float zFar);

		static Matrix create_orthographic_off_center(float left, float right, float bottom, float top, float zNear, float zFar);
		
		static Matrix create_rotation_x(float radians);
		
		static Matrix create_rotation_y(float radians);
		
		static Matrix create_rotation_z(float radians);
		
		static Matrix create_scale(float scale);
		
		static Matrix create_scale(Vector3 scale);
		
		static Matrix create_scale(float x_scale, float y_scale, float z_scale);
		
		static Matrix create_translation(Vector3 position);
		
		static Matrix create_translation(float x_pos, float y_pos, float z_pos);

		float* get_array() const;

		Vector3 get_backward_vector() const;

		Vector3 get_down_vector() const;

		Vector3 get_forward_vector() const;

		Vector3 get_left_vector() const;

		Vector3 get_right_vector() const;

		Vector3 get_scale_vector() const;

		Vector3 get_translation_vector() const;

		Vector3 get_up_vector() const;

		static Matrix invert(Matrix matrix);

		//Operators

		Matrix operator+(Matrix b) const;
		
		Matrix operator-(Matrix b) const;
		
		Matrix operator/(Matrix b) const;

		Matrix operator/(int b) const;

		Matrix operator/(float b) const;

		Matrix operator*(Matrix b) const;
		
		Matrix operator*(int b) const;

		Matrix operator*(float b) const;
	};
}

#endif