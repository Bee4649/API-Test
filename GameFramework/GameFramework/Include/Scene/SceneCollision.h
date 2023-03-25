#pragma once

#include "../GameInfo.h"

class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	std::vector<class CCollider*>	m_vecCollider;
	class CCollider* m_MouseCollision;
	class CWidget* m_MouseCollisionWidget;

public:
	void AddCollider(class CCollider* Collider);
	void Collision(float DeltaTime);

};

