#pragma once
#include "character.h"

class StrImage :
	public Character
{
public:
	StrImage(void){}
	~StrImage(void){}
public:
	// �`��
	void Paint(Graphics* g);
	// ������
	void Init(void);
	// ������
	void Init(UINT _x, UINT _y);
	// ������
	void Init(Point pt);
};
