#pragma once
#include "character.h"

class YakuList :
	public Character
{
public:
	YakuList(void);
	~YakuList(void);
private:
	UINT m_uiReachBit;//リーチビット
	UINT m_uiAchieveBit;//達成ビット
	//定数
	enum{
		YL_WIDTH = 75,//幅
		YL_HEIGHT = 20//高さ
	};
public:
	// 描画
	void Paint(Graphics* g);
	//リセット
	void Reset(void);
	//指定位置のリーチビットを変更
	void SetReachBit(UINT no,BOOL flg);
	//指定位置のリーチビットを取得
	BOOL GetReachBit(UINT no);
	//指定位置の到達ビットを変更
	void SetAchieveBit(UINT no,BOOL flg);
	//指定位置の到達ビットを取得
	BOOL GetAchieveBit(UINT no);
};
