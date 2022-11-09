#pragma once
#include "make_Shader.h"

//width = º¯/2  height = ³ôÀÌ / 2
GLvoid makeCuboid(std::vector<GLfloat>& obj, const GLfloat& width, const GLfloat& hegiht)
{
	obj = {
		//¾Õ¸é
		-width, -hegiht, width,
		width, hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, width,
		width, -hegiht, width,
		width, hegiht, width
			,
		//¿À¸¥¸é
		width, -hegiht, width,
		width, hegiht, -width,
		width, hegiht, width,

		width, -hegiht, width,
		width, -hegiht, -width,
		width, hegiht, -width
		,
		//µÞ¸é
		width, -hegiht, -width,
		-width, -hegiht, -width,
		-width, hegiht, -width,

		width, -hegiht, -width,
		-width, hegiht, -width,
		width, hegiht, -width
		,
		//¿Þ¸é
		-width, -hegiht, -width,
		-width, -hegiht, width,
		-width, hegiht, width,

		-width, -hegiht, -width,
		-width, hegiht, width,
		-width, hegiht, -width
		,
		//À­¸é
		width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, width,

		-width, hegiht, width,
		width, hegiht, -width,
		-width, hegiht, -width
		,
		//µÞ¸é
		width, -hegiht, width,
		-width, -hegiht, width,
		-width, -hegiht, -width,

		width, -hegiht, width,
		-width, -hegiht, -width,
		width, -hegiht, -width
	};
}

GLvoid setCol(std::vector<GLfloat>& col, const GLfloat& red, const GLfloat& green, const GLfloat& blue)
{
	for (int i = 0; i < 36; ++i)
	{
		col.push_back(red);
		col.push_back(green);
		col.push_back(blue);
	}
}

