#pragma once
#include "character.h"
#include <vector>

class Square :
	public Character,public MouseListner
{
public:
	Square(void){}
	Square(int width,int height);
	~Square(void);
private:
	//スクリーン
	Graphics* m_gScreen;
	enum{
		YFUDACNT = 6,//役達成札数
		FUNC_PAINT = 0,//描画関数
		FUNC_SORT//札並び替え関数
	};
	//モード
	UINT m_uiMode;
	//達成役ビット
	int m_iAchieveBit;
	//描画領域
	int m_iX;
	int m_iY;
	int m_iWidth;
	int m_iHeight;
	//座標
	const Point m_ptYName;//役名
	const Point m_ptYF;//役札
	const Point m_ptScore;//スコアー
	const Point m_ptSumScore;//合計スコアー
	const Point m_ptGoukei;//「合計」
	const Point m_ptKoikoi;//こいこいしますか？
	//文字列
	std::vector<StrImage> m_siYname;//役名
	std::vector<StrImage> m_siMon;//文
	std::vector<StrImage> m_siScore;//スコアー
	StrImage* m_siKoikoi;//こいこい
	StrImage* m_siGoukei;//合計
public:
	enum{
		MODE_ACHIEVE = 1,//役達成モード
		MODE_KOIKOI,//こいこいモード
		MODE_RESULT//結果モード
	};
	std::vector<Fuda> m_AFuda;//達成表示札
private:
	//札ベクトルを一括処理
	BOOL AllFudaFunc(std::vector<Fuda> &vFuda,int func,double param=0,BOOL flag=FALSE);
	//札を並び替える
	void SortFuda(Fuda* fuda,int i);
	//描画範囲を設定
	void SetArea(int x,int y,int width,int height);
protected:
	// ロールオーバー時の動作
	void OnRollOver(double x, double y);
	// ロールアウト時の動作
	void OnRollOut(double x, double y);
	// マウスクリック時の動作
	void OnMouseClick(double x, double y);
public:
	//札を並べる
	void Sort(void);
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
	//モード変更
	void ChangeMode(int mode,int bit=0);
	//描画位置を中央に固定かつ、領域を指定倍に縮小する
	void ReduceArea(float scale);
};
