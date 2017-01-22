#pragma once

class Strategy
{
private:
	Strategy(void):m_uiComBit(0),
				   m_uiMyBit(0),
				   m_uiYakuBit(0)
	{}
	Strategy(const Strategy& strategy) {}
	Strategy& operator=(const Strategy& strategy) {}
public:
	static Strategy* GetInstance(void){
		static Strategy strategy;
		return &strategy;
	}
	~Strategy(void);
private:
	/****札ビット*****/
	//0:雨
	//1:桜
	//2:月
	//3:鶴
	//4:桐
	//5:化け札
	//6:猪
	//7:鹿
	//8:蝶
	//9:赤(松)
	//10:赤(梅)
	//11:赤(桜)
	//12:青(牡丹)
	//13:青(紅葉)
	//14:青(菊)
	//札ビット(COM)
	UINT m_uiComBit;
	//札ビット(自分)
	UINT m_uiMyBit;
	/****役ビット*****/
	//0:雨
	//1:桜
	//2:月
	//3:鶴or桐
	//4:化け札
	//5:猪鹿蝶
	//6:赤短
	//7:青短
	//8:タネ
	//9:タン
	//10:カス
	UINT m_uiYakuBit;
public:
	//定数
	enum{
		FUDA_BAKE = 36,//化け札
		FUDA_AME = 44,//雨
		FUDA_SAKURA = 8,//桜
		FUDA_TSUKI = 28,//月
		FUDA_TSURU = 0,//鶴
		FUDA_KIRI = 40,//桐
		FUDA_INO = 24,//猪
		FUDA_SHIKA = 32,//鹿
		FUDA_CHOU = 20,//蝶
		FUDA_AKA1 = 1,//赤短(松)
		FUDA_AKA2 = 5,//赤短(梅)
		FUDA_AKA3 = 9,//赤短(桜)
		FUDA_AO1 = 21,//青短(牡丹)
		FUDA_AO2 = 33,//青短(紅葉)
		FUDA_AO3 = 37,//青短(菊)
		FUDABIT_AME = 0,//雨
		FUDABIT_SAKURA,//桜
		FUDABIT_TSUKI,//月
		FUDABIT_TSURU,//鶴
		FUDABIT_KIRI,//桐
		FUDABIT_BAKE,//化け札
		FUDABIT_INO,//猪
		FUDABIT_SHIKA,//鹿
		FUDABIT_CHOU,//蝶
		FUDABIT_AKA1,//赤短(松)
		FUDABIT_AKA2,//赤短(梅)
		FUDABIT_AKA3,//赤短(桜)
		FUDABIT_AO1,//青短(牡丹)
		FUDABIT_AO2,//青短(紅葉)
		FUDABIT_AO3,//青短(菊)
		YAKUBIT_AME = 0,//五光(雨)
		YAKUBIT_SAKURA,//五光(桜)
		YAKUBIT_TSUKI,//五光(月)
		YAKUBIT_GOKO,//五光(鶴桐)
		YAKUBIT_BAKE,//化け札
		YAKUBIT_INOSHIKA,//猪鹿蝶
		YAKUBIT_AKATAN,//赤短
		YAKUBIT_AOTAN,//青短
		YAKUBIT_TANE,//タネ
		YAKUBIT_TAN,//短札
		YAKUBIT_KASU,//カス
		YAKU_GOKO = 0,//五光
		YAKU_SHIKO,//四光
		YAKU_AMESHIKO,//雨四光
		YAKU_SANKO,//三光
		YAKU_HANAMI,//花見
		YAKU_TSUKIMI,//月見
		YAKU_AKATAN,//赤短
		YAKU_AOTAN,//青短
		YAKU_INOSHIKA,//猪鹿蝶
		YAKU_TANE,//タネ
		YAKU_TAN,//タン
		YAKU_KASU,//カス
		ACHVTANECNT = 5,//タネ達成数
		ACHVTANCNT = 5,//タン達成数
		ACHVKASUCNT = 10,//カス達成数
		ALLTANECNT = 9,//タネ総数
		ALLTANCNT = 10,//タン総数
		ALLKASUCNT = 24,//カス総数
		INDEX_STRATEGY = 50,//ストラテジー指数
		INDEX_GOKO = 20,//ストラテジー指数(五光札)
		INDEX_YAKU = 10,//ストラテジー指数(役札)
		INDEX_TANE = 5,//ストラテジー指数(タネ札)
		INDEX_TAN = 5,//ストラテジー指数(タン札)
		INDEX_KASU = 1//ストラテジー指数(カス札)
	};
private:
//選択指数を算出する
int CulcIndex(UINT id,UINT kind,BOOL matchFlg = TRUE);
//五光札の選択指数を算出する
int CulcGoko(UINT yakuBit,BOOL matchFlg = TRUE);
//花見月見札の選択指数を算出する
int CulcBake(UINT yakuBit,BOOL matchFlg = TRUE);
//赤短札の選択指数を算出する
int CulcAkatan(UINT yakuBit,BOOL matchFlg = TRUE);
//青短札の選択指数を算出する
int CulcAotan(UINT yakuBit,BOOL matchFlg = TRUE);
//猪鹿蝶札の選択指数を算出する
int CulcInoshika(UINT yakuBit,BOOL matchFlg = TRUE);
//3枚役札(赤短・青短・猪鹿蝶)の選択指数を算出する
int CulcTriple(UINT yakuBit,BOOL matchFlg = TRUE);
//タネの選択指数を算出する
int CulcTane(UINT yakuBit,BOOL matchFlg = TRUE);
//タン札の選択指数を算出する
int CulcTan(UINT yakuBit,BOOL matchFlg = TRUE);
//カス札の選択指数を算出する
int CulcKasu(UINT yakuBit,BOOL matchFlg = TRUE);
//タネタンカス札の選択指数を算出する
int CulcTaneTanKasu(UINT yakuBit,BOOL matchFlg = TRUE);
//五光札の獲得状況を返す
WPARAM CheckGetGoko(UINT kouBit,UINT otsuBit);
//赤短札の獲得状況を返す
WPARAM CheckGetAkatan(UINT kouBit,UINT otsuBit);
//青短札の獲得状況を返す
WPARAM CheckGetAotan(UINT kouBit,UINT otsuBit);
//猪鹿蝶札の獲得状況を返す
WPARAM CheckGetInoshika(UINT kouBit,UINT otsuBit);
//タネ札の獲得状況を返す
WPARAM CheckGetTane(BOOL myFlg);
//タン札の獲得状況を返す
WPARAM CheckGetTan(BOOL myFlg);
//カス札の獲得状況を返す
WPARAM CheckGetKasu(BOOL myFlg);
//3枚役札(赤短・青短・猪鹿蝶)の獲得状況を返す
//WPARAM CheckGetTriple(UINT kouBit,UINT otsuBit,UINT kind);
WPARAM CheckGetTriple(UINT kouBit,UINT otsuBit,UINT yakuNo);
//タネ・タン・カスの獲得枚数を返す
WPARAM CheckGetTaneTanKasu(BOOL myFlg,UINT kind);
//五光系の判定
UINT JudgeGoko(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//花見月見の判定
UINT JudgeBake(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//赤短の判定
UINT JudgeAkatan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//青短の判定
UINT JudgeAotan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//猪鹿蝶の判定
UINT JudgeInoshika(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//タネの判定
UINT JudgeTane(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//タンの判定
UINT JudgeTan(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//カスの判定
UINT JudgeKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//3枚役(赤短・青短・猪鹿蝶)の判定
//UINT JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT kind);
UINT JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT yakuNo);
//タネ・タン・カスの判定
UINT JudgeTaneTanKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg,UINT yakuNo);
//指定札毎にビットを立てる
void SetupBit(UINT* lpFudaBit,UINT* lpYakuBit,UINT id,UINT kind);
public:
//選択する札順を返す
UINT SelectCF(void);
//どちらか一方の場札を選択する
UINT AlternativeBF(UINT id1,UINT kind1,UINT id2,UINT kind2);
//こいこいするか？
BOOL IsKoiKoi(void);
//獲得札毎にビットを立てる
void SetupGFudaBit(BOOL myFlg,UINT id,UINT kind);
//役判定
UINT Judge(BOOL myFlg);
//化け札を持っているか？
BOOL IsBakeFuda(BOOL myFlg);
//リセット
void Reset(void);
};
