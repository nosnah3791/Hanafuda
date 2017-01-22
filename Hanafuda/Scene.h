#pragma once
#include "stdAfx.h"


class Scene
{
public:
	Scene(void);
	~Scene(void);
protected:
	//一時停止指数
	UINT m_uiStopCnt;
	//効果音
	XSound* m_sound;
public:
	// 初期化
	virtual void Init(void)=0;
	// リセット
	virtual void Reset(void)=0;
	// マウス移動時の処理
	virtual BOOL MouseMove(double x, double y) = 0;
	// マウスクリック時の処理
	virtual BOOL MouseClick(double x, double y) = 0;
	// 札を動かす
	virtual void Move(void)=0;
	// 描画
	virtual void Paint(void)=0;
	//効果音
	void Sound(BOOL playFlg);
	//一時停止
	BOOL Stop(void);
};
