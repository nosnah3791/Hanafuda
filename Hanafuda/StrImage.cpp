#include "StdAfx.h"
#include "StrImage.h"

// �`��
void StrImage::Paint(Graphics* g)
{
	Character::Paint(g);
}

// ������
void StrImage::Init(void)
{
	Character::Init();
}

// ������
void StrImage::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
}

// ������
void StrImage::Init(Point pt)
{
	Character::Init(pt);
}
