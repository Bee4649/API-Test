#include "Player.h"
#include "..//Scene/Scene.h"
#include "Bullet.h"
#include "Tornado.h"
#include "AurelionSol.h"
#include "../Input.h"
#include "../GameManager.h"
#include "../Scene/Camera.h"
#include "../Collision/ColliderBox.h"

CPlayer::CPlayer()
{
	SetTypeID<CPlayer>();
}

CPlayer::CPlayer(const CPlayer& Obj)	:
	CCharacter(Obj) 
{
}

CPlayer::~CPlayer()
{
}

bool CPlayer::Init()
{
	CGameObject::Init();

	m_MoveSpeed = 400.f;
	m_GunAngle = 0.f;
	m_GunLength = 70.f;


	SetPos(100.f, 100.f);
	SetSize(85.f, 75.f);
	SetPivot(0.5f, 1.f);

	// SetTexture("Player", TEXT("Player/Right/alert.bmp"));
	// SetColorKey(255, 0, 255);

	m_SolAngle[0] = 0.f;
	m_SolAngle[1] = 120.f;
	m_SolAngle[2] = 240.f;

	m_SolLength = 100.f;
	m_SolRotationSpeed = 60.f;
	m_SolSkillOn = false;

	m_SolLengthMin = 100.f;
	m_SolLengthMax = 200.f;

	m_SolSkillTime = 0.f;
	m_SolSkillDir = 1.f;

	SkillCoolDownInfo Info = {};

	Info.CoolDown = 5.f;
	m_vecCoolDown.push_back(Info);
	
	Info.CoolDown = 10.f;
	m_vecCoolDown.push_back(Info);

	
	// 위성 객체 3개를 생성한다.
	for (int i = 0; i < 3; ++i)
	{
		m_Sol[i] = m_Scene->CreateObject<CAurelionSol>("Sol");
		
		Vector2 Pos;
		Pos.x = m_Pos.x + cosf(DegreeToRadian(m_SolAngle[i])) * m_SolLength;
		Pos.y = m_Pos.y + sinf(DegreeToRadian(m_SolAngle[i])) * m_SolLength;
	
		m_Sol[i]->SetPos(Pos);
	}

	CreateAnimation();

	AddAnimation("PlayerRightIdle");
	AddAnimation("PlayerRightWalk");
	AddAnimation("PlayerRightAttack", false, 0.3f);
	AddAnimation("PlayerLeftIdle");
	AddAnimation("PlayerLeftWalk");
	AddAnimation("PlayerLefAttack",false, 0.3f);

	SetEndFunction<CPlayer>("PlayerRightAttack", this, &CPlayer::AttackEnd);
	SetEndFunction<CPlayer>("PlayerLefAttack", this, &CPlayer::AttackEnd);

	AddNotify<CPlayer>("PlayerRightAttack", 2, this, &CPlayer::Attack);
	AddNotify<CPlayer>("PlayerLefAttack", 2, this, &CPlayer::Attack);

	m_vecSequenceKey[0].push_back("PlayerRightIdle"); 
	m_vecSequenceKey[0].push_back("PlayerRightWalk");
	m_vecSequenceKey[0].push_back("PlayerRightAttack");

	m_vecSequenceKey[1].push_back("PlayerLeftIdle");
	m_vecSequenceKey[1].push_back("PlayerLeftWalk");
	m_vecSequenceKey[1].push_back("PlayerLeftAttack");

	// 오른쪽 보고 있음.
	m_PlayerDir = 1;

	// 공격중이 아닐 때.
	m_Attack = false;

	// 충돌체 추가
	CColliderBox* Box = AddCollider<CColliderBox>("Head");

	Box->SetExtent(56.f, 42.f);
	Box->SetOffset(7.f, -52.f);
	Box->SetCollisionProfile("Player");

	Box->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::CollisionBegin);
	Box->SetCollisionEndFunction<CPlayer>(this, &CPlayer::CollisionEnd);

	Box = AddCollider<CColliderBox>("Body");

	Box->SetExtent(36.f, 31.f);
	Box->SetOffset(5.f, -15.5f);
	Box->SetCollisionProfile("Player");

	Box->SetCollisionBeginFunction<CPlayer>(this, &CPlayer::CollisionBegin);
	Box->SetCollisionEndFunction<CPlayer>(this, &CPlayer::CollisionEnd);



	CInput::GetInst()->AddBindFunction<CPlayer>("MoveFront",
		Input_Type::Push, this, &CPlayer::MoveFront, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("MoveBack",
		Input_Type::Push, this, &CPlayer::MoveBack, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("GunRotation",
		Input_Type::Push, this, &CPlayer::GunRotation, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("GunRotationInv",
		Input_Type::Push, this, &CPlayer::GunRotationInv, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Fire",
		Input_Type::Down, this, &CPlayer::Fire, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Skill1",
		Input_Type::Down, this, &CPlayer::Skill1, m_Scene);

	CInput::GetInst()->AddBindFunction<CPlayer>("Skill2",
		Input_Type::Down, this, &CPlayer::Skill2, m_Scene);

	return true;
} 

void CPlayer::Update(float DeltaTime)
{
	CCharacter::Update(DeltaTime * m_TimeScale);

	DeltaTime *= m_TimeScale;

	size_t Size = m_vecCoolDown.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecCoolDown[i].CoolDownEnable)
		{
			m_vecCoolDown[i].CoolDown -= DeltaTime;

			if (m_vecCoolDown[i].CoolDown <= 0.f)
				m_vecCoolDown[i].CoolDownEnable = false;
		}
	}

	// VK_RETRUN : Enter키
	// VK_F1 : F1키
	// VK_LBUTTON : 왼쪽 마우스 버튼
	// GetAsyncKeyState : 키의 상태를 체크할 수 있다.
	// 반환값
	// 0 : 해당 키가 눌러진 상태가 아니다.
	// 0x8000 : 해당 키를 누르고 있는 상태이다.
	// 0x1 : 해당 키를 이전 프레임에 눌렀다.


	m_GunPos.x = m_Pos.x + cosf(DegreeToRadian(m_GunAngle)) * m_GunLength;
	m_GunPos.y = m_Pos.y + sinf(DegreeToRadian(m_GunAngle)) * m_GunLength;
	
	
	if (m_SolSkillOn)
	{
		m_SolLength += m_SolSkillDir *(m_SolLengthMax - m_SolLengthMin) / 2.f * DeltaTime;
		
		if (m_SolLength >= m_SolLengthMax)
			m_SolLength = m_SolLengthMax;

		else if (m_SolLength <= m_SolLengthMin)
		{
			m_SolRotationSpeed = 60.f;
			m_SolLength = m_SolLengthMin;
			m_SolSkillOn = false;
		}

		m_SolSkillTime += DeltaTime;

		if (m_SolSkillTime >= 5.f)
		{
			m_SolSkillDir = -1.f;
		}
	
	}

	for (int i = 0; i < 3; ++i)
	{
		m_SolAngle[i] += m_SolRotationSpeed * DeltaTime;

		Vector2	Pos;
		Pos.x = m_Pos.x + cosf(DegreeToRadian(m_SolAngle[i])) * m_SolLength;
		Pos.y = m_Pos.y + sinf(DegreeToRadian(m_SolAngle[i])) * m_SolLength;

		m_Sol[i]->SetPos(Pos);
	}
}

void CPlayer::PostUpdate(float DeltaTime)
{
	CCharacter::PostUpdate(DeltaTime);

	if (m_PlayerDir == 1)
	{
		// 0일 경우라면 오른쪽으로 이동중 멈췄다는 것이다.
		if (m_Move.x < 0.f)
			m_PlayerDir = -1;
	}
	else
	{
		if (m_Move.x > 0.f)
			m_PlayerDir = 1;
	}

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;

	if (m_Move.x != 0.f || m_Move.y != 0.f)
	{
		// 이동을 할 겨9ㅇ우 공격중이더라도 공격을 취소한다.
		m_Attack = false;
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][1]);
	}
	else if(!m_Attack)
		ChangeAnimation(m_vecSequenceKey[AnimDirIndex][0]);
}


void CPlayer::Render(HDC hDC, float DeltaTime)
{
	CCharacter::Render(hDC, DeltaTime);
	
	Vector2 Pos = m_Pos - m_Scene->GetCamera()->GetPos();
	Vector2 GunPos = m_GunPos - m_Scene->GetCamera()->GetPos();

	MoveToEx(hDC, (int)Pos.x, (int)Pos.y, nullptr);
	LineTo(hDC, (int)GunPos.x, (int)GunPos.y);
}

void CPlayer::MoveFront()
{ 
	//Move(m_GunAngle);
	MoveDir(Vector2(0.f, -1.f));
	 // Vector2 Dir;
	 // Dir.x = cosf(DegreeToRadian(m_GunAngle));
	 // Dir.y = sinf(DegreeToRadian(m_GunAngle));

	 // m_Pos += Dir * 400.f * DELTA_TIME * m_TimeScale;

}

void CPlayer::MoveBack()
{
	//Move(m_GunAngle + 180.f);
	MoveDir(Vector2(0.f, 1.f));
	// Vector2 Dir;
	// Dir.x = cosf(DegreeToRadian(m_GunAngle));
	// Dir.y = sinf(DegreeToRadian(m_GunAngle));

	// m_Pos -= Dir * 400.f * DELTA_TIME * m_TimeScale;

}

void CPlayer::GunRotation()
{
	// m_GunAngle += 180.f * DELTA_TIME * m_TimeScale;
	MoveDir(Vector2(1.f, 0.f));
}

void CPlayer::GunRotationInv()
{
	// m_GunAngle -= 180.f * DELTA_TIME * m_TimeScale;
	MoveDir(Vector2(-1.f, 0.f));
}

void CPlayer::Fire()
{
	if (m_Attack)
		return;

	m_Attack = true;

	int AnimDirIndex = 0;

	if (m_PlayerDir == -1)
		AnimDirIndex = 1;
	
	ChangeAnimation(m_vecSequenceKey[AnimDirIndex][2]);

	
}

void CPlayer::Skill1()
{
	if (m_vecCoolDown[0].CoolDownEnable)
		return;

	m_vecCoolDown[0].CoolDownEnable = true;
	m_vecCoolDown[0].CoolDown = 5.f;

	CTornado* Bullet = m_Scene->CreateObject<CTornado>("Tornado");

	Bullet->SetAngle(m_GunAngle);

	Bullet->SetPos(m_GunPos);
}


void CPlayer::Skill2()
{
	if (m_vecCoolDown[1].CoolDownEnable)
		return;

	m_vecCoolDown[1].CoolDownEnable = true;
	m_vecCoolDown[1].CoolDown = 10.f;

	m_SolSkillOn = true;

	m_SolRotationSpeed = 180.f;

	m_SolSkillTime = 0.f;
	m_SolSkillDir = 1.f;
}

void CPlayer::AttackEnd()
{
	
	m_Attack = false;
}

void CPlayer::Attack()
{
	CBullet* Bullet = m_Scene->CreateObject<CBullet>("Bullet");

	Bullet->SetAngle(m_GunAngle);

	Bullet->SetPos(m_GunPos);

	CCollider* BulletCol = Bullet->FindCollider("Body");

	BulletCol->SetCollisionProfile("PlayerAttack");
}

void CPlayer::CollisionBegin(CCollider* Src, CCollider* Dest)
{
}

void CPlayer::CollisionEnd(CCollider* Src, CCollider* Dest)
{
}
