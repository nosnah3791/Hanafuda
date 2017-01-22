#pragma once
#include "stdAfx.h"
#include "Character.h"
#include "Fuda.h"
#include "StrImage.h"
#include "SelectImage.h"
#include "YakuList.h"
#include "XSound.h"
#include "Scene.h"
#include <vector>
#include "TimeControl.h"

class Mediator
{
private:
	Mediator(void) {}
	Mediator(const Mediator& mediator) {}
	Mediator& operator=(const Mediator& mediator) {}
public:
	static Mediator* GetInstance(void){
		static Mediator mediator;
		return &mediator;
	}
	~Mediator(void);
public:
	//※デバッグ用フラグ
	BOOL m_bDebugFlg;
	//定数
	enum{
		ORG_X = 0,//画面原点X
		ORG_Y = 0,//画面原点Y
		WND_WIDTH = 840,//ウィンドウ幅
		WND_HEIGHT = 600,//ウィンドウ高さ
		FUDACNT = 4,//月毎の札数
		YAKUCNT = 12,//役数
		MONCNT = 12,//月数
		ALLFUDACNT = FUDACNT*MONCNT,//札総数
		KIND_GOKO = 0,//札種別(五光)
		KIND_TANE = 1,//札種別(タネ)
		KIND_TAN = 2,//札種別(タン)
		KIND_KASU = 3,//札種別(カス)
		S_CHSCNT = 3,//親決めシーン文字列数
		S_PLSCNT = 2,//対局シーン文字列数
		S_LASCNT = 3,//最終シーン文字列数
		SCORE_GOKO = 15,//五光(15文)
		SCORE_SHIKO = 10,//四光(10文)
		SCORE_AMESHIKO = 8,//雨四光(8文)
		SCORE_SANKO = 6,//三光(6文)
		SCORE_HANAMI = 3,//花見(3文)
		SCORE_TSUKIMI = 3,//月見(3文)
		SCORE_AKATAN = 6,//赤短(6文)
		SCORE_AOTAN = 6,//青短(6文)
		SCORE_INOSHIKA = 5,//猪鹿蝶(5文)
		SCORE_TANE = 1,//タネ(1文)
		SCORE_TAN = 1,//タン(1文)
		SCORE_KASU = 1,//カス(1文)
		SCORE_OYAKEN = 3//親権(3文)
	};
	//ウィンドウハンドル
	HWND m_hWnd;
	//オフスクリーン
	Graphics* m_gOffScreen;
	// 札
	Fuda m_Fuda[ALLFUDACNT];
	//裏札
	Fuda m_UraFuda;
	//***文字列***
	StrImage m_siCom;//COM
	StrImage m_siYou;//あなた
	StrImage m_siMonth[MONCNT];//月
	StrImage m_siChS[S_CHSCNT];//親決めシーン
	StrImage m_siPlS[S_PLSCNT];//対局シーン
	StrImage m_siLaS[S_LASCNT];//最終シーン
	StrImage m_siYName[YAKUCNT];//役名
	StrImage m_siOyaken;//親権
	StrImage m_siNumberB[10];//数(黒)
	StrImage m_siMonB;//文(黒)
	StrImage m_siGoukeiB;//合計(黒)
	StrImage m_siNumberW[10];//数(白)
	StrImage m_siMonW;//文(白)
	StrImage m_siGoukeiW;//合計(白)
	//選択画像
	SelectImage m_slHai;//はい
	SelectImage m_slIie;//いいえ
	//役一覧表
	YakuList m_siYakuList;
	//BGM
//	XSound m_sndPut1;//札を重ねる
//	XSound m_sndPut2;//札を置く
	XSound m_sndAcheive;//役達成
	XSound m_sndWin;//勝利
	XSound m_sndEnding;//エンディング
	//得点一覧
	static const UINT m_uiScore[YAKUCNT];
	//得点表
	UINT m_uiScoreTable[MONCNT][2];
private:
	// 背景画像
	XImage m_imgBG;
	// シーンID
	UINT sceneID;
	// グラフィックス
	XGraphics m_XG;
	//再描画領域
	RECT m_rctRepaint;
	//シーンオブジェクト
	Scene* m_lpScene;
	//スレッドハンドル
	HANDLE m_thread;
	//Event
	HANDLE m_hEvent;
	//スレッドタイマー
	TimeControl m_tc;
	//イベント名
	static const LPCWSTR EVENT_NAME;
	//親フラグ
	BOOL m_bOyaFlg;
	//局
	UINT m_uiKyoku;
	//ロールオーバーID
	UINT m_uiROID;
	//スレッドフラグ
	BOOL m_bThreadFlg;
	//スレッド関数
	static unsigned __stdcall Run(LPVOID param);
	//札種別
	static const UINT m_uiFKind[ALLFUDACNT];
public:
	//オフスクリーンにブレンド描画
	void BlendImages(int x,int y,Bitmap* srcBitmap);
	// 初期化
	void Init(HWND hWnd);
	//リセット
	void Reset(void);
	//スコアーリセット
	void ResetScore(void);
	// メインスクリーン描画
	void Paint(void);
	//再描画
	void Repaint(void);
	//再描画領域をセット
	void SetupRepaint(int left,int top,int right,int bottom);
	//再描画領域をセット
	void SetupRepaint(RECT rect);
	// マウス移動時の処理
	void MouseMove(double x, double y);
	// マウスクリック時の処理
	void MouseClick(double x, double y);
	//効果音
	void Sound(BOOL playFlg);
	//スレッドを実行
	void RunThread(BOOL onFlg);
	//シーン変更
	void ChangeScene(Scene* scene);
	//親フラグ
	BOOL GetOyaFlg(void);
	void SetOyaFlg(BOOL oyaFlg);
	//次局に進める
	UINT NxtKyoku(BOOL nxtFlg);
	//ロールオーバーIDを取得
	UINT GetROID(void);
	//ロールオーバーIDを変更
	void ChangeROID(UINT roID,BOOL roFlg);
	//得点を加算
	void AddScore(int score,BOOL myFlg);
	//累計得点を取得
	Point GetScore(void);
	//指定ビットから指定の順番を取得
	int ExtBitOrder(int bit,int total,int i=0);
	//スコアー数値画像を格納
	void HouseScore(UINT score,std::vector<StrImage> &vSI,Point rPos,BOOL whiteFlg=FALSE,double scale=1.0);
	//文字列画像を一括描画
	void PaintStrImage(std::vector<StrImage> &vSI,Graphics* g);
};
