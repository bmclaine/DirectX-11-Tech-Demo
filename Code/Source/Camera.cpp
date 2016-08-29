#include "Camera.h"

Camera::Camera()
{
	cursorPos.x = 0;
	cursorPos.y = 0;

	prevCursorPos.x = 0;
	prevCursorPos.y = 0;

	currCursorPos.x = 0;
	currCursorPos.y = 0;

	cameraRotX = 0.0f;
	cameraRotY = 0.0f;

	XMStoreFloat4x4(&m_ViewMatrix, XMMatrixIdentity());
}

Camera::Camera(const Camera&)
{

}

Camera::~Camera()
{

}

void Camera::Translate(float x, float y, float z)
{
	XMMATRIX viewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	viewMatrix = XMMatrixMultiply(XMMatrixTranslation(x, y, z), viewMatrix);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	return;
}

void Camera::RotateOnX(float angle)
{
	XMFLOAT3 viewPosVector = { m_ViewMatrix._41, m_ViewMatrix._42, m_ViewMatrix._43 };

	m_ViewMatrix._41 = 0.0f;
	m_ViewMatrix._42 = 0.0f;
	m_ViewMatrix._43 = 0.0f;

	XMMATRIX viewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	viewMatrix = XMMatrixMultiply(XMMatrixRotationX(XMConvertToRadians(angle)), viewMatrix);
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	m_ViewMatrix._41 = viewPosVector.x;
	m_ViewMatrix._42 = viewPosVector.y;
	m_ViewMatrix._43 = viewPosVector.z;

	return;
}

void Camera::RotateOnY(float angle)
{
	XMFLOAT3 viewPosVector = { m_ViewMatrix._41, m_ViewMatrix._42, m_ViewMatrix._43 };

	m_ViewMatrix._41 = 0.0f;
	m_ViewMatrix._42 = 0.0f;
	m_ViewMatrix._43 = 0.0f;

	XMMATRIX viewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	viewMatrix = XMMatrixMultiply(viewMatrix, XMMatrixRotationY(XMConvertToRadians(angle)));
	XMStoreFloat4x4(&m_ViewMatrix, viewMatrix);

	m_ViewMatrix._41 = viewPosVector.x;
	m_ViewMatrix._42 = viewPosVector.y;
	m_ViewMatrix._43 = viewPosVector.z;

	return;
}

void Camera::Input(float deltaTime, unsigned int windowHeight, unsigned int windowWidth)
{
	if (GetAsyncKeyState('A'))
		Translate(-deltaTime * 5.0f, 0.0f, 0.0f);

	if (GetAsyncKeyState('D'))
		Translate(deltaTime * 5.0f, 0.0f, 0.0f);

	if (GetAsyncKeyState('Q'))
		Translate(0.0f, deltaTime * 5.0f, 0.0f);

	if (GetAsyncKeyState('E'))
		Translate(0.0f, -deltaTime * 5.0f, 0.0f);

	if (GetAsyncKeyState('W'))
		Translate(0.0f, 0.0f, deltaTime * 5.0f);

	if (GetAsyncKeyState('S'))
		Translate(0.0f, 0.0f, -deltaTime * 5.0f);

	cameraRotX = 0.0f;
	cameraRotY = 0.0f;

	if (GetCursorPos(&cursorPos) && (GetAsyncKeyState(VK_LBUTTON) & 0x8000))
	{
		POINT tempPos = cursorPos;
		ScreenToClient(window, &tempPos);

		if (tempPos.x < (long)windowWidth && tempPos.x > 0 && tempPos.y < (long)windowHeight && tempPos.y > 0)
		{
			currCursorPos.x = cursorPos.x - prevCursorPos.x;
			currCursorPos.y = cursorPos.y - prevCursorPos.y;

			cameraRotX += currCursorPos.y * 0.5f;// * deltaTime * 20.0f;
			cameraRotY += currCursorPos.x * 0.5f;// * deltaTime * 20.0f;
		}
	}

	prevCursorPos.x = cursorPos.x;
	prevCursorPos.y = cursorPos.y;

	RotateOnX(cameraRotX);
	RotateOnY(cameraRotY);
}

XMFLOAT4X4 Camera::GetInverseViewMatrix()
{
	XMFLOAT4X4 tempInverseMat;
	XMMATRIX tempViewMatrix = XMLoadFloat4x4(&m_ViewMatrix);
	tempViewMatrix = XMMatrixInverse(nullptr, tempViewMatrix);
	XMStoreFloat4x4(&tempInverseMat, tempViewMatrix);

	return tempInverseMat;
}

XMFLOAT4X4 Camera::GetViewMatrix()
{
	return m_ViewMatrix;
}

void Camera::SetWindow(HWND hwnd)
{
	window = hwnd;
}
