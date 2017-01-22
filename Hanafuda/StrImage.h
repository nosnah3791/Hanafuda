#pragma once
#include "character.h"

class StrImage :
	public Character
{
public:
	StrImage(void){}
	~StrImage(void){}
public:
	// •`‰æ
	void Paint(Graphics* g);
	// ‰Šú‰»
	void Init(void);
	// ‰Šú‰»
	void Init(UINT _x, UINT _y);
	// ‰Šú‰»
	void Init(Point pt);
};
