#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <DirectXMath.h>
using namespace DirectX;
#include <Windows.h>

class Camera
{
private:
	XMFLOAT4X4 m_ViewMatrix;

	HWND window;
	POINT cursorPos;
	POINT prevCursorPos;
	POINT currCursorPos;

	float cameraRotX;
	float cameraRotY;

public:
	Camera();
	Camera(const Camera&);
	~Camera();

	void Translate(float x, float y, float z);
	void RotateOnX(float x);
	void RotateOnY(float y);

	void Input(float deltaTime, unsigned int windowHeight, unsigned int windowWidth);
	XMFLOAT4X4 GetInverseViewMatrix();
	XMFLOAT4X4 GetViewMatrix();

	void SetWindow(HWND hwnd);
};

#endif