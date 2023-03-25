#pragma once
#include "GameObject.h"
class CTornado :
    public CGameObject
{
	friend class CScene;

protected:
	CTornado();
	CTornado(const CTornado& Obj);
	virtual ~CTornado();

private:
	float		m_MoveSpeed;
	float		m_Angle;
	// 회전~~회오리~~
	float		m_RotationAngle;
	// 총알 사거리
	float		m_Distance;
	Vector2		m_LinePos;
	// 회전 반경
	float		m_Range;

public:

	virtual void SetPos(float x, float y)
	{
		CGameObject::SetPos(x, y);
		m_LinePos = m_Pos;
		
	}

	virtual void SetPos(const Vector2& Pos)
	{
		CGameObject::SetPos(Pos);
		m_LinePos = m_Pos;
	}

	void SetDistance(float Dist)
	{
		m_Distance = Dist;
	}

	void SetAngle(float Angle)
	{
		m_Angle = Angle;

	}
public:
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void Render(HDC hDC, float DeltaTime);



};

