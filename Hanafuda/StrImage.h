#pragma once
#include "character.h"

class StrImage :
	public Character
{
public:
	StrImage(void){}
	~StrImage(void){}
public:
	// 描画
	void Paint(Graphics* g);
	// 初期化
	void Init(void);
	// 初期化
	void Init(UINT _x, UINT _y);
	// 初期化
	void Init(Point pt);
};
