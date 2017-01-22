#pragma once
#include "Character.h"
#include "MouseListner.h"

class SelectImage :
	public Character,public MouseListner
{
public:
	SelectImage(void);
	~SelectImage(void);
protected:
	// ロールオーバー時の動作
	void OnRollOver(double x, double y);
	// ロールアウト時の動作
	void OnRollOut(double x, double y);
	// マウスクリック時の動作
	void OnMouseClick(double x, double y);
public:
	// 描画
	void Paint(Graphics* g);
	// マウス移動時の処理
	BOOL MouseMove(double x, double y);
	// マウスクリック時の処理
	BOOL MouseClick(double x, double y);
	//初期化
	void Init(void);
	//初期化
	void Init(UINT _x,UINT _y);
	//初期化
	void Init(Point pt);
	// リセット
	void Reset(void);
};
