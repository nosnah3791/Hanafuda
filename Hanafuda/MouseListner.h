#pragma once

class MouseListner
{
public:
	MouseListner(void);
	~MouseListner(void);
protected:
	RECT rect;//矩形領域
	//ロールオーバーフラグ
	BOOL m_bRolloverFlg;
private:
	// 指定座標の画像矩形内有無を判定
	BOOL InRect(double x, double y);
protected:
	// ロールオーバー時の動作
	virtual void OnRollOver(double x, double y) = 0;
	// ロールアウト時の動作
	virtual void OnRollOut(double x, double y) = 0;
	// マウスクリック時の動作
	virtual void OnMouseClick(double x, double y) = 0;
public:
	// マウス移動時の処理
	BOOL MouseMove(double x, double y);
	//ロールオーバー時の処理
	BOOL RollOver(double x, double y);
	//ロールアウト時の処理
	BOOL RollOut(double x, double y);
	// マウスクリック時の処理
	BOOL MouseClick(double x, double y);
};
