#pragma once

#include "GameInfo.h"
#include "SingletonMacro.h"
#include "Ref.h"

enum class Input_Type
{
	Down,
	Push,
	Up,
	End
	
};

// �Լ� �����͸� �̿��ؼ� Ű�� ����ϴ� �ý����� ������̴�.

struct KeyState
{
	unsigned char key;
	bool Down;	// ������ �����Ҷ�
	bool Push;	// ������ ������
	bool Up;	// ������ �ִ� Ű�� �ö�ö�

	KeyState() :
		key(0),
		Down(false),
		Push(false),
		Up(false)
	{}

};

struct BindFunction
{
	void* Obj;
	std::function<void()>	func;
	class CScene* Scene;

	BindFunction() :
		Obj(nullptr),
		Scene(nullptr)
	{
	}
};

struct BindKey
{
	// �̸�
	std::string Name;
	// � Ű�� ����ϴ���.
	KeyState* key;
	bool Ctrl;
	bool Alt;
	bool Shift;

	std::vector<BindFunction*>	vecFunction[(int)Input_Type::End];
	
	
	BindKey() :
		key(nullptr),
		Ctrl(false),
		Alt(false),
		Shift(false)
	{}

};


class CInput
{

private:
	// unorderd_map ���� ��� Ű �ߺ��� ���� �ȵȴ�.
	// Ű �ߺ��Ϸ��� multimap �̿��ؾ� �Ѵ�.

	std::unordered_map<unsigned char, KeyState*>	m_mapKeyState;
	std::unordered_map<std::string, BindKey*>		m_mapBindKey;
	bool	m_Ctrl;
	bool	m_Alt;
	bool	m_Shift;



public:
	bool Init();
	void Update(float DeltaTime);

private:
	void UpdateMouse(float DeltaTime);
	void UpdatekeyState(float DeltaTime);
	void UpdateBindKey(float DeltaTime);

	void SetKeyCtrl(const std::string& Name, bool Ctrl = true);
	void SetKeyAlt(const std::string& Name, bool Alt = true);
	void SetKeyShift(const std::string& Name, bool Shift = true);

	KeyState* FindKeyState(unsigned char Key);
	BindKey* FindBindKey(const std::string& Name);


public:
	bool AddBindKey(const std::string& Name, unsigned char Key);
	
public:
	template <typename T>
	void AddBindFunction(const std::string& KeyName,
		Input_Type Type,
		T* Object, void (T::* Func)(), class CScene* Scene)
	{
		BindKey* Key = FindBindKey(KeyName);

		if (!Key)
			return;

		BindFunction* Function = new BindFunction;

		Function->Obj = Object;
		// ����Լ��� ����Ҷ� �Լ��ּ�, ��ü�ּҸ� ����ؾ� �Ѵ�.
		Function->func = std::bind(Func, Object);
		Function->Scene = Scene;

		Key->vecFunction[(int)Type].push_back(Function);
	}


	DECLARE_SINGLE(CInput)

};
