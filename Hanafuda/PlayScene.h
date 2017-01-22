#pragma once
#include "Scene.h"
#include "Fuda.h"
#include "Square.h"
#include <vector>

class PlayScene:public Scene
{
private:
	PlayScene(void);
	PlayScene(const PlayScene& playScene) {}
	PlayScene& operator=(const PlayScene& playScene) {}
public:
	static PlayScene* GetInstance(void){
		static PlayScene playScene;
		return &playScene;
	}
	~PlayScene(void);
public:
	enum{
		FUDACNT = 4,//配手札数
		FUDAKIND = 4,//札種別
		STR_KOIKOI1 = 0,//文字列(こいこいしますか？)
		STR_KOIKOI2//文字列(こいこいします)
	};
	std::vector<Fuda*> m_ComFuda;//相手札
	std::vector<Fuda*> m_BaFuda;//場札
	std::vector<Fuda*> m_GMyFuda[FUDAKIND];//獲得済手札
	std::vector<Fuda*> m_GComFuda[FUDAKIND];//獲得済相手札
	//役一覧
	YakuList m_ylComYaku;
	YakuList m_ylMyYaku;
private:
	enum{
		FADECNT = 10,//フェードアウトフレーム数
		TEFUDACNT = FUDACNT*2,//手札数
		BAFUDACNT = 18,//場札数
		DAIFUDACNT = TEFUDACNT*3,//台札数
		UFUDACNT = 21,//裏札数
		GFUDACNT = 10,//獲得済札数
		YFUDACNT = 6,//役達成札数
		MOVESTEPCNT = 8,//ステップ数
		BAFUDADIF = 6,//場札の隙間
		FUNC_PAINT = 0,//描画関数
		FUNC_SELECTABLE,//選択可能フラグ切り替え関数
		FUNC_SELECTED,//選択中フラグ切り替え関数
		FUNC_RESET,//選択リセット関数
		FUNC_INVISIBLE,//不可視状態切り替え関数
		FUNC_INSERT,//場札挿入関数
		FUNC_SORT,//札並び替え関数
		FUNC_ERASE,//削除関数
		FUNC_COPY,//コピー関数
		SCENE_DEAL = 0,//配るシーン
		SCENE_PLAY,//対戦シーン
		CUT_TSELECT = 0,//手札選択カット
		CUT_BSELECT1,//場札選択カット1
		CUT_TPUT,//手札合わせカット
		CUT_DOPEN,//台札めくりカット
		CUT_BSELECT2,//場札選択カット2
		CUT_DPUT,//台札合わせカット
		CUT_TGET,//手札獲得カット
		CUT_B1GET,//場札1獲得カット
		CUT_DGET,//台札獲得カット
		CUT_B2GET,//場札2獲得カット
		CUT_SORT,//札並び替えカット
		CUT_ACHIEVE,//役達成カット
		CUT_KOIKOI,//こいこいカット
		CUT_RESULT,//結果カット
		SORT_TEFUDA=0,//手札ソート
		SORT_COMFUDA,//相手札ソート(※デバッグ用)
		SORT_GETFUDA,//獲得済札ソート
		WIN_OFF = 0,//勝ち越しマーク(未)
		WIN_COM,//勝ち越しマーク(COM)
		WIN_ME//勝ち越しマーク(自分)
	};
	//札
	Fuda m_UFuda[UFUDACNT];//裏札
	std::vector<Fuda*> m_MyFuda;//手札
	std::vector<Fuda*> m_DaiFuda;//台札
	Fuda* m_GetTFuda;//獲得手札
	Fuda* m_GetDFuda;//獲得台札
	Fuda* m_GetBFuda1[FUDACNT-1];//獲得場札1
	Fuda* m_GetBFuda2[FUDACNT-1];//獲得場札2
	//ビット
	int m_iBFbit1;//場札獲得ビット1
	int m_iBFbit2;//場札獲得ビット2
	int m_iBFbitBlank;//場札空白ビット
	int m_iAchieveBit;//達成役ビット
	int m_iMyAchieveBit;//達成役ビット(自分)
	int m_iComAchieveBit;//達成役ビット(COM)
	//勝ち越しマーク
	UINT m_uiWin;//0:未 1:COM 2:自分
	//札番号
	std::vector<int> m_vNo;
	//札移動座標配列
	std::vector<std::vector<Point>> m_ptMove;
	//文字列画像
	StrImage* m_siMonth;//月
	StrImage* m_siCom;//COM
	StrImage* m_siYou;//あなた
	StrImage m_siMon[2];//文
	std::vector<StrImage> m_siScore;//スコアー
	//中背景
	Square* m_square;
	//選択画像
	SelectImage* m_slHai;//はい
	SelectImage* m_slIie;//いいえ
	//座標
	const Point m_ptCF,m_ptCF2;//相手札
	const Point m_ptBFt,m_ptBFt2;//場札上段
	const Point m_ptDF;//台札
	const Point m_ptMonth;//月
	const Point m_ptBFb,m_ptBFb2;//場札下段
	const Point m_ptMF,m_ptMF2;//手札
	const Point m_ptGMF;//獲得手札
	const Point m_ptGCF;//獲得相手札
	const Point m_ptCom,m_ptYou;//氏名
	const Point m_ptCY,m_ptMY;//役一覧
	const Point m_ptYSize;//役表示枠サイズ
	const Point m_ptCScore,m_ptMScore;//スコアー
	const Point m_ptSquare,m_ptSqSize;//中背景
	const Point m_ptHai,m_ptIie;//選択画像
	//シーンNo.
	UINT m_uiSceneNo;//0:配る,1:対戦
	//カットNo.
	UINT m_uiCutNo;
	//フレームNo.
	UINT m_uiFrameNo;
	//ターンNo.
	UINT m_uiTurnNo;
	//レイヤー
	std::vector<int> m_vLayer;
	//リソースファイル
	HMODULE m_hModule;
private:
	//札をシャッフルする
	void Shuffle(void);
	//札をセットする
	void SetupFuda(void);
	//移動経路を確定する
	void SetupRoute(Fuda* lpFuda,Point* ePt,UINT cnt,UINT step=MOVESTEPCNT);
	//移動経路を確定する
	void SetupRoute(Fuda** lplpFuda,Point* ePt,UINT cnt,UINT step=MOVESTEPCNT);
	//合わせ札の移動経路を確定する
	void SetupAFRoute(BOOL teFlg,UINT step=MOVESTEPCNT);
	//獲得札の移動経路を確定する
	void SetupGFRoute(Fuda** lplpFuda,UINT cnt=1,UINT step=MOVESTEPCNT,UINT ratio=2);
	//札を指定枚数分、1フレームずつ縮小しながらずらす
	BOOL SlideReduce(Fuda** lplpFuda,UINT cnt=1,UINT step=MOVESTEPCNT,UINT ratio=2);
	//裏札を指定枚数分、1フレームずらす
	BOOL Slide(Fuda* lpUFuda,UINT cnt,UINT step=MOVESTEPCNT);
	//札を配る
	void Deal(void);
	//対戦
	BOOL Play(void);
	//手札選択
	BOOL PlayTSelect(void);
	//場札選択
	BOOL PlayBSelect(BOOL teFlg);
	//札合わせ
	BOOL PlayPut(BOOL teFlg);
	//台札めくり
	BOOL PlayDOpen(void);
	//札獲得
	BOOL PlayGet(Fuda** lplpFuda,UINT cnt=1);
	//並び替え
	BOOL PlaySort(void);
	//役達成
	BOOL PlayAcheive(void);
	//こいこい
	BOOL PlayKoikoi(void);
	//結果
	BOOL PlayResult(void);
	//札ベクトルを一括処理
	BOOL AllFudaFunc(std::vector<Fuda*> &vFuda,int func,double param = 0,BOOL flag = FALSE);
	//選択可能な札にフラグを立てる
	void SetupSelectable(void);
	//合わせる場札を格納する
	BOOL HouseBF(BOOL teFlg,UINT mon);
	//選択しない方の場札を元に戻す
	BOOL ReturnBF(BOOL teFlg,Fuda* lpBaFuda,UINT i);
	//選択不可の札を場札に加える
	void AddBF(BOOL teFlg);
	//獲得済札を格納する
	void HouseGF(Fuda** lplpFuda,UINT cnt=1);
	//札を並び替える
	void SortFuda(Fuda* fuda,int fudakind,int i=0);
	//指定のレイヤーを一番上にする
	void SortLayer(int layer);
	//達成した役札を表示する
	void DispAcheiveFuda(void);
	//達成した約ビットを加算する
	void AddAcheiveBit(int bit);
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
	//達成した役の文数を取得
	UINT GetAcheiveScore(int bit);
	//達成役の合計スコアーを追加
	void AddScore(int score);
	//自分の番かどうか
	BOOL IsMyTurn(void);
	//同じ月の札か？
	BOOL IsSameMonth(Fuda* fuda,UINT mon);
	//ターンNo
	UINT GetTurnNo(void);
};
