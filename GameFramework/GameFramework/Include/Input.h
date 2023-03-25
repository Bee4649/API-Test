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

// 함수 포인터를 이용해서 키를 등록하는 시스템을 만들것이다.

struct KeyState
{
	unsigned char key;
	bool Down;	// 누르기 시작할때
	bool Push;	// 누르고 있을때
	bool Up;	// 누르고 있던 키가 올라올때

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
	// 이름
	std::string Name;
	// 어떤 키를 사용하는지.
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
	// unorderd_map 같은 경우 키 중복이 전혀 안된다.
	// 키 중복하려면 multimap 이용해야 한다.

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
		// 멤버함수를 등록할때 함수주소, 객체주소를 등록해야 한다.
		Function->func = std::bind(Func, Object);
		Function->Scene = Scene;

		Key->vecFunction[(int)Type].push_back(Function);
	}


	DECLARE_SINGLE(CInput)

};

