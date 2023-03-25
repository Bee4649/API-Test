#pragma once

// 한장 여러가지 있는 타입 : Sprite
// 한장 한장 나누어져 있는 타입 :Frame
enum class ETexture_Type
{
	Sprite,
	Frame
};

enum class ERender_Layer
{
	Back,
	Default,
	Effect,
	Max
};

enum class ECollision_Channel
{
	Default,
	Player,
	Monster,
	PlayerAttack,
	MonsterAttack,
	Max
};

enum class ECollision_Interaction
{
	Ignore, // 무시
	Collision // 충돌
};

enum class ECollider_Type 
{
	Box,
	Circle
};

enum class EBrush_Type
{
	Red,
	Green,
	Black,
	Blue,
	Yellow,
	Max
};