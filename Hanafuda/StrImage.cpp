#include "StdAfx.h"
#include "StrImage.h"

// 描画
void StrImage::Paint(Graphics* g)
{
	Character::Paint(g);
}

// 初期化
void StrImage::Init(void)
{
	Character::Init();
}

// 初期化
void StrImage::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
}

// 初期化
void StrImage::Init(Point pt)
{
	Character::Init(pt);
}
