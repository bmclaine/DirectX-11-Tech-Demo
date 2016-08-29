#include "Light.h"

Light::Light()
{

}

Light::Light(const Light&)
{

}

Light::~Light()
{

}

XMFLOAT4 Light::GetPosition()
{
	return position;
}

XMFLOAT4 Light::GetDirection()
{
	return direction;
}

XMFLOAT4 Light::GetRadius()
{
	return radius;
}

XMFLOAT4 Light::GetInnerConeRatio()
{
	return innerConeRatio;
}

XMFLOAT4 Light::GetOuterConeRatio()
{
	return outerConeRatio;
}

XMFLOAT4 Light::GetColor()
{
	return color;
}

XMFLOAT4 Light::GetAmbience()
{
	return ambience;
}

void Light::SetPosition(XMFLOAT4 pos)
{
	position = pos;

	return;
}

void Light::SetDirection(XMFLOAT4 dir)
{
	direction = dir;

	return;
}

void Light::SetRadius(XMFLOAT4 rad)
{
	radius = rad;

	return;
}

void Light::SetInnerConeRatio(XMFLOAT4 inner)
{
	innerConeRatio = inner;

	return;
}

void Light::SetOuterConeRatio(XMFLOAT4 outer)
{
	outerConeRatio = outer;

	return;
}


void Light::SetColor(XMFLOAT4 col)
{
	color = col;

	return;
}

void Light::SetAmbience(XMFLOAT4 amb)
{
	ambience = amb;

	return;
}
