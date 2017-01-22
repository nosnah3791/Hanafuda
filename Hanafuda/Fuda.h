#pragma once
#include "Character.h"
#include "MouseListner.h"

class Fuda:public Character,public MouseListner
{
public:
	Fuda(void);
	~Fuda(void);
private:
	// ID
	UINT m_uiID;
	//種別
	UINT m_uiKind;//0:五光 1:タネ 2:タン 3:カス
	//選択可能フラグ
	BOOL m_bSelectableFlg;
	//不可視フラグ
	BOOL m_bInvisibleFlg;
protected:
	// ロールオーバー時の動作
	void OnRollOver(double x, double y);
	// ロールアウト時の動作
	void OnRollOut(double x, double y);
	// マウスクリック時の動作
	void OnMouseClick(double x, double y);
public:
	void SetProperty(UINT id,UINT kind,UINT pResourceID,HMODULE hInstance);
	// 描画
	void Paint(Graphics* g);
	// 座標設定
	void SetPos(Point pt);
	// 座標＆サイズ設定
	void SetPos(Point pt,double scale);
	//初期化
	void Init(void);
	//初期化
	void Init(UINT _x,UINT _y);
	//初期化
	void Init(Point pt);
	// リセット
	void Reset(void);
	// 同位置に置き換える
	void Trace(Character* lpch);
	//中央に配置
	void Centering(UINT _x,UINT _y);
	// マウス移動時の処理
	BOOL MouseMove(double x, double y);
	// マウスクリック時の処理
	BOOL MouseClick(double x, double y);
	//選択状態を指定
	void SetupSelected(BOOL onFlg);
	//選択可能状態を指定
	void SetupSelectable(BOOL onFlg);
	//状態フラグをリセット
	void ResetSelect(void);
	//ID取得
	UINT GetID(void);
	//種別取得
	UINT GetKind(void);
};
