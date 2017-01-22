#pragma once
#include "Scene.h"
#include "Fuda.h"
#include "StrImage.h"

class ChooseScene:public Scene
{
private:
	ChooseScene(void);
	ChooseScene(const ChooseScene& chooseScene) {}
	ChooseScene& operator=(const ChooseScene& chooseScene) {}
public:
	static ChooseScene* GetInstance(void){
		static ChooseScene chooseScene;
		return &chooseScene;
	}
	~ChooseScene(void);
private:
	enum{
		STR_ANAOYA = 0,
		STR_ERA,
		STR_WATOYA,
		X_LFUDA = 250,//左札座標x
		X_RFUDA = 530,//右札座標x
		Y_MSG = 114//メッセージ座標y
	};
	Fuda m_LFuda;//左札
	Fuda m_RFuda;//右札
	StrImage* m_siMsg;//メッセージ
	StrImage* m_siLMON;//月(左)
	StrImage* m_siRMON;//月(右)
	StrImage* m_siLWho;//誰(左)
	StrImage* m_siRWho;//誰(右)
	//座標
	const Point m_ptLU,m_ptRD;
private:
	//親決め抽選
	Point Oyagime(void);
public:
	//初期化
	void Init(void);
	//リセット
	void Reset(void);
	// マウス移動時の処理
	BOOL MouseMove(double x, double y);
	// マウスクリック時の処理
	BOOL MouseClick(double x, double y);
	// 札を動かす
	void Move(void);
	// 描画
	void Paint(void);
};

