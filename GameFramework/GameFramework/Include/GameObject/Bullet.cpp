#include "Bullet.h"
#include "../Scene/Scene.h"
#include "../Scene/Camera.h"
#include "../Scene/Camera.h"
#include "../Collision/ColliderBox.h"

CBullet::CBullet()
{
	SetTypeID<CBullet>();
}

CBullet::CBullet(const CBullet& Obj) :
	CGameObject(Obj),
	m_Angle(Obj.m_Angle),
	m_Distance(Obj.m_Distance)
{
}

CBullet::~CBullet()
{
}

bool CBullet::Init()
{
	m_MoveSpeed = 350.f;
	m_Distance = 600.f;
	m_Angle = 180.0f;
	

	SetPos(900.f, 100.f);
	SetSize(50.f, 50.f);
	SetPivot(0.5f, 0.5f);

	CColliderBox* Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(100.f, 100.f);

	Box->SetCollisionBeginFunction<CBullet>(this, &CBullet::CollisionBegin);
	Box->SetCollisionEndFunction<CBullet>(this, &CBullet::CollisionEnd);



	return true;
}

void CBullet::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
	Move(m_Angle);

	m_Pos.x = m_Pos.x + cosf(DegreeToRadian(m_Angle)) * m_MoveSpeed* DeltaTime;
	m_Pos.y = m_Pos.y + sinf(DegreeToRadian(m_Angle)) * m_MoveSpeed * DeltaTime;
	
	m_Distance -= m_MoveSpeed * DeltaTime;

	if (m_Distance <= 0.f)
		SetActive(false);
	
}

void CBullet::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

void CBullet::Render(HDC hDC, float DeltaTime)
{
	Vector2 RenderLT;

	RenderLT = m_Pos - m_Pivot * m_Size - m_Scene->GetCamera()->GetPos();

	Ellipse(hDC, (int)RenderLT.x, (int)RenderLT.y,
		(int)(RenderLT.x + m_Size.x), (int)(RenderLT.y + m_Size.y));


}

void CBullet::CollisionBegin(CCollider* Src, CCollider* Dest)
{
	

	SetActive(false);

}

void CBullet::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}
