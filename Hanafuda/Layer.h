#pragma once
#include "character.h"

class Layer :
	public Character
{
public:
	Layer(void){}
	Layer(int width,int height);
	~Layer(void);
private:
	//スクリーン
	Graphics* m_gScreen;
	enum{
		MONCNT = 12,
		Y_MSG = 510,
		DIF_Y = 35,
		MSG_WAK = 0,
		MSG_ANK,
		MSG_DRAW
	};
	//座標
	const Point m_ptCom,m_ptYou;//「COM」「あなた」
	const Point m_ptMonth;//月
	const Point m_ptCScore,m_ptMScore;//スコアー
	//文字列画像
	StrImage* m_siCom;//COM
	StrImage* m_siYou;//あなた
	StrImage* m_siMonth[MONCNT];//月
	StrImage m_siMon[MONCNT+1][2];//文
	std::vector<StrImage> m_siScore;//スコアー
	StrImage* m_siGoukei;//合計
	StrImage* m_siMsg;//メッセージ
public:
	//初期化
	void Init(void);
	//初期化
	void Init(UINT _x,UINT _y);
	//初期化
	void Init(Point pt);
	//中央に配置
	void Centering(UINT _x,UINT _y,UINT width=0,UINT height=0);
	// リセット
	void Reset(void);
	// 描画
	void Paint(Graphics* g);
};
