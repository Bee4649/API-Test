#pragma once

#include "GameInfo.h"



class CRef
{
public:
	CRef();
	//복사 생성자
	CRef(const CRef& ref);
	virtual ~CRef();


protected:
	int		m_RefCount;
	std::string m_Name;
	std::string m_TypeName;
	size_t	m_TypeID;
	bool m_Enable;	// 활성, 비활성
	bool m_Active;	// 살아 있는지 죽었는지

public:
	void AddRef();
	int Release();

public:
	int GetRefCount()	const
	{
		return m_RefCount;
	}

	bool GetEnable() const
	{
		return m_Enable;
	}

	bool GetActive() const
	{
		return m_Active;
	}
public:
	size_t GetTypeID()		const
	{
		return m_TypeID;
	}

	const std::string& GetTypeName()	const
	{
		return m_TypeName;
	}

	void SetEnable(bool Enable)
	{
		m_Enable = Enable; 
	}

	void SetActive(bool Active)
	{
		m_Active = Active;
	}

	const std::string& GetName()	const
	{
		return m_Name;
	}

	void SetName(const std::string& Name)
	{
		m_Name = Name;
	}
	
	template<typename T>
	bool CheckTypeID()	const
	{
		return m_TypeID == typeid(T).hash_code();
	}

public:
	template<typename T>
	void SetTypeID()
	{
		// 타입 이름을 문자열로 얻어온다.
		m_TypeName = typeid(T).name();

		// 타입의 고유한 번호를 얻어온다.
		m_TypeID = typeid(T).hash_code();
	}
};

