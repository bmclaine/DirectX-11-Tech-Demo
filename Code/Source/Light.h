#ifndef _LIGHT_H_
#define _LIGHT_H_

#include <DirectXMath.h>
using namespace DirectX;

enum LightType
{

};

class Light
{
private:
	XMFLOAT4 position;
	XMFLOAT4 direction;
	XMFLOAT4 radius;
	XMFLOAT4 innerConeRatio;
	XMFLOAT4 outerConeRatio;
	XMFLOAT4 color;
	XMFLOAT4 ambience;

public:
	Light();
	Light(const Light&);
	~Light();

	XMFLOAT4 GetPosition();
	XMFLOAT4 GetDirection();
	XMFLOAT4 GetRadius();
	XMFLOAT4 GetInnerConeRatio();
	XMFLOAT4 GetOuterConeRatio();
	XMFLOAT4 GetColor();
	XMFLOAT4 GetAmbience();

	void SetPosition(XMFLOAT4 pos);
	void SetDirection(XMFLOAT4 dir);
	void SetRadius(XMFLOAT4 rad);
	void SetInnerConeRatio(XMFLOAT4 inner);
	void SetOuterConeRatio(XMFLOAT4 outer);
	void SetColor(XMFLOAT4 col);
	void SetAmbience(XMFLOAT4 amb);
};

#endif