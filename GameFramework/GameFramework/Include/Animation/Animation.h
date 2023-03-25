#pragma once


#include "AnimationInfo.h"


class CAnimation
{
	friend class CGameObject;


private:
	CAnimation();
	~CAnimation();

private:
	class CGameObject* m_Owner;
	class CScene* m_Scene;
	std::unordered_map<std::string, CAnimationInfo*>	m_mapAnimation;
	CAnimationInfo* m_CurrentAnimation; // 현재 동작해있는 애니메이션 주소
	std::function<void()>	m_CurrentAnimationEndFunc;

public:
	void AddAnimation(const std::string& SequenceName, bool Loop = true,
		float PlayTime = 1.f, float PlayScale = 1.f,
		bool Reverse = false);
	void SetPlayTime(const std::string& Name, float PlayTime);
	void SetPlayScale(const std::string& Name, float PlayScale);
	void SetPlayLoop(const std::string& Name, bool Loop);
	void SetPlayReverse(const std::string& Name, float Reverse);
	// 현재 동작되는 애니메이션 지정하는 기능 
	void SetCurrentAnimation(std::string& Name);
	// 맵에 바꾸고 싶은 애니메이션 있다면 이름만 바꾸면 자동으로 바뀜
	void ChangeAnimation(const std::string& Name);
	// 현재 동작되고 있는 크런트 애니메이션이 무엇인지.
	bool CheckCurrentAnimation(const std::string& Name);
	void Update(float DeltaTime);

private:
	// 내부에서 접근하는 정도로 찾으려면
	CAnimationInfo* FindInfo(const std::string& Name);



public:
	template <typename T>
	void SetEndFunction(const std::string&Name, T* Obj, void(T::* Func)())
	{
		CAnimationInfo* Info = FindInfo(Name);

		if (!Info)
			return;

		Info->SetEndFunction<T>(Obj, Func);
		
	}


	template <typename T>
	void AddNotify(const std::string& Name, int Frame, T* Obj, void(T::* Func)())
	{

		CAnimationInfo* Info = FindInfo(Name);

		if (!Info)
			return;

		Info->AddNotify<T>(Frame, Obj, Func);

	}
	template <typename T>
	void SetCurrentAnimationEndFunction(T* Obj, void(T::* Func)())
	{
		m_CurrentAnimationEndFunc = std::bind(Func, Obj);
	}


};

