#include "StdAfx.h"
#include "StrImage.h"

// •`‰æ
void StrImage::Paint(Graphics* g)
{
	Character::Paint(g);
}

// ‰Šú‰»
void StrImage::Init(void)
{
	Character::Init();
}

// ‰Šú‰»
void StrImage::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
}

// ‰Šú‰»
void StrImage::Init(Point pt)
{
	Character::Init(pt);
}
