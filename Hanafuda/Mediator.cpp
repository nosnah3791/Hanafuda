#include "StdAfx.h"
#include "Mediator.h"
#include "ChooseScene.h"
#include "Strategy.h"
#include <process.h>

const LPCWSTR Mediator::EVENT_NAME = L"Thread";

Mediator::~Mediator(void)
{
	// イベントの破棄
	CloseHandle(this->m_hEvent);
	// スレッドの破棄
	CloseHandle(this->m_thread);
}

//札種別
const UINT Mediator::m_uiFKind[ALLFUDACNT] = { 
	  KIND_GOKO,KIND_TAN,KIND_KASU,KIND_KASU,//一月(松に鶴)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//二月(梅に鶯)
	  KIND_GOKO,KIND_TAN,KIND_KASU,KIND_KASU,//三月(桜に幕)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//四月(藤にホトトギス)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//五月(菖蒲に八橋)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//六月(牡丹に蝶)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//七月(萩に猪)
	  KIND_GOKO,KIND_TANE,KIND_KASU,KIND_KASU,//八月(坊主に月)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//九月(菊に杯)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//十月(紅葉に鹿)
	  KIND_GOKO,KIND_KASU,KIND_KASU,KIND_KASU, //十一月(桐に鳳凰)
	  KIND_GOKO,KIND_TANE,KIND_TAN,KIND_KASU//十二月(柳(雨))
};

//得点一覧
const UINT Mediator::m_uiScore[YAKUCNT] = {
	SCORE_GOKO,//五光(15文)
	SCORE_SHIKO,//四光(10文)
	SCORE_AMESHIKO,//雨四光(8文)
	SCORE_SANKO,//三光(6文)
	SCORE_HANAMI,//花見(3文)
	SCORE_TSUKIMI,//月見(3文)
	SCORE_AKATAN,//赤短(6文)
	SCORE_AOTAN,//青短(6文)
	SCORE_INOSHIKA,//猪鹿蝶(5文)
	SCORE_TANE,//タネ(1文)
	SCORE_TAN,//タン(1文)
	SCORE_KASU//カス(1文)
};

//オフスクリーンにブレンド描画
void Mediator::BlendImages(int x,int y,Bitmap* srcBitmap)
{
	this->m_XG.BlendImages(x,y,XGraphics::BLEND_ALPHA,srcBitmap);
}

// メインスクリーン描画
void Mediator::Paint(void)
{
	PAINTSTRUCT ps;

	//描画開始
	HDC hdc = BeginPaint(this->m_XG.m_hWnd,&ps);

	//背景
	this->m_gOffScreen->DrawImage(this->m_imgBG,0,0,this->m_imgBG.GetWidth(),this->m_imgBG.GetHeight());

	//シーン毎に描画
	this->m_lpScene->Paint();

	//オフスクリーンを描画
	this->m_XG.Update(hdc);

	//描画終了
	EndPaint(this->m_XG.m_hWnd,&ps);

}

//再描画
void Mediator::Repaint(void)
{
	InvalidateRect(this->m_XG.m_hWnd,&this->m_rctRepaint,FALSE);
	UpdateWindow(this->m_XG.m_hWnd);
	SetRectEmpty(&this->m_rctRepaint);
}

//再描画領域をセット
void Mediator::SetupRepaint(int left,int top,int right,int bottom)
{
	//矩形を合成する
	int l = left;
	int t = top;
	int r = right;
	int b = bottom;
	if(!IsRectEmpty(&this->m_rctRepaint)){
		l = min(this->m_rctRepaint.left,l);
		t = min(this->m_rctRepaint.top,t);
		r = max(this->m_rctRepaint.right,r);
		b = max(this->m_rctRepaint.bottom,b);
	}

	SetRect(&this->m_rctRepaint,l,t,r,b);
}

//再描画領域をセット
void Mediator::SetupRepaint(RECT rect)
{
	this->SetupRepaint(rect.left,rect.top,rect.right,rect.bottom);
}

// 初期化
void Mediator::Init(HWND hWnd)
{
	int i;
	Strategy* st = Strategy::GetInstance();

	//※デバッグ用フラグ
	this->m_bDebugFlg = FALSE;

	//ウィンドウハンドル
	this->m_hWnd = hWnd;

	//グラフィックス生成
	this->m_XG.SetProperty(hWnd);
	this->m_gOffScreen = this->m_XG.m_gOffScreen;

	//再描画領域
	SetRectEmpty(&this->m_rctRepaint);

	//***画像登録***
	HMODULE hModule;
	hModule = ::LoadLibrary(_T("RS.dll"));
	this->m_imgBG.Load(IDB_BG,_T("PNG"),hModule);//背景
	for(i=0;i<ALLFUDACNT;i++){//札
		this->m_Fuda[i].SetProperty(i,Mediator::m_uiFKind[i],IDB_FUDA1 + i,hModule);
	}
	this->m_UraFuda.SetProperty(NULL,NULL,IDB_URA,hModule);
	//===文字列画像登録===
	this->m_siCom.Load(IDB_STR_COM,_T("PNG"),hModule);//COM
	this->m_siYou.Load(IDB_STR_YOU,_T("PNG"),hModule);//あなた
	for(i=0;i<MONCNT;i++){//月
		this->m_siMonth[i].Load(IDB_STR_1G + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_CHSCNT;i++){//親決めシーン
		this->m_siChS[i].Load(IDB_STR_ANO + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_PLSCNT;i++){//対局シーン
		this->m_siPlS[i].Load(IDB_STR_KOIKOI1 + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_LASCNT;i++){//最終シーン
		this->m_siLaS[i].Load(IDB_STR_WAK + i,_T("PNG"),hModule);
	}
	for(i=0;i<YAKUCNT;i++){//役名
		this->m_siYName[i].Load(IDB_STR_GOKO + i,_T("PNG"),hModule);
	}
	this->m_siOyaken.Load(IDB_STR_OYAKEN,_T("PNG"),hModule);//親権
	for(i=0;i<10;i++){//数
		this->m_siNumberB[i].Load(IDB_STR_B_0 + i,_T("PNG"),hModule);//黒
		this->m_siNumberW[i].Load(IDB_STR_W_0 + i,_T("PNG"),hModule);//白
	}
	this->m_siMonB.Load(IDB_STR_B_MON,_T("PNG"),hModule);//文(黒)
	this->m_siMonW.Load(IDB_STR_W_MON,_T("PNG"),hModule);//文(白)
	this->m_siGoukeiB.Load(IDB_STR_B_GOUKEI,_T("PNG"),hModule);//合計(黒)
	this->m_siGoukeiW.Load(IDB_STR_W_GOUKEI,_T("PNG"),hModule);//合計(白)
	//===選択画像===
	this->m_slHai.Load(IDB_STR_HAI,_T("PNG"),hModule);//はい
	this->m_slIie.Load(IDB_STR_IIE,_T("PNG"),hModule);//いいえ
	//===役一覧画像===
	this->m_siYakuList.Load(IDB_YAKU,_T("PNG"),hModule);
	//===効果音===
//	this->m_sndPut1.Load(IDR_MP3_PUT1,hModule,_T("MP3"),_T("MPEGVIDEO"));
//	this->m_sndPut1.Load(IDR_WAV_PUT1,hModule);
//	this->m_sndPut2.Load(IDR_MP3_PUT2,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndAcheive.Load(IDR_MP3_ACHEIVE,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndWin.Load(IDR_MP3_WIN,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndEnding.Load(IDR_MP3_ENDING,hModule,_T("MP3"),_T("MPEGVIDEO"));

	::FreeLibrary(hModule);

	//シーン設定
	this->ChangeScene(ChooseScene::GetInstance());
	//スコアーリセット
	this->ResetScore();

	//スレッドスタート
	this->m_hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
	unsigned (__stdcall *func)( void* v ) = Mediator::Run;
	this->m_thread = (HANDLE)::_beginthreadex(NULL,0,func,NULL,(unsigned)0, NULL);
}

//リセット
void Mediator::Reset(void)
{
	//シーン設定
	if(this->m_lpScene){
		this->m_lpScene->Reset();
		//スレッド停止
		this->RunThread(FALSE);
	}
	this->ChangeScene(ChooseScene::GetInstance());
	//スコアーリセット
	this->ResetScore();
	this->SetupRepaint(0,0,WND_WIDTH,WND_HEIGHT);
	this->Repaint();
}

//スコアーリセット
void Mediator::ResetScore(void)
{
	int i;
	for(i=0;i<MONCNT;i++) this->m_uiScoreTable[i][0] = this->m_uiScoreTable[i][1] = 0;
	this->m_uiKyoku = 0;
}

// マウス移動時の処理
void Mediator::MouseMove(double x, double y)
{
	if(!this->m_bThreadFlg){
		if(this->m_lpScene->MouseMove(x,y)){
			this->Repaint();
		}
	}
}

// マウスクリックの処理
void Mediator::MouseClick(double x, double y)
{
	if(!this->m_bThreadFlg){
		if(this->m_lpScene->MouseClick(x,y)){
			//再描画
			InvalidateRect(this->m_XG.m_hWnd,&this->m_rctRepaint,FALSE);
			//スレッドスタート
			this->RunThread(TRUE);
		}
	}
}

//効果音
void Mediator::Sound(BOOL playFlg)
{
	this->m_lpScene->Sound(playFlg);
}

//スレッド関数
unsigned __stdcall Mediator::Run(LPVOID param)
{
	Mediator* md = Mediator::GetInstance();

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);


	UINT i = 0;
	while(TRUE){
		WaitForSingleObject(hEvent, INFINITE);
		md->m_tc.TimeRegular();
		md->m_lpScene->Move();
		if(md->m_tc.GetDrawFlg()){
			//再描画
			if(!IsRectEmpty(&md->m_rctRepaint)){
				md->Repaint();
			}
		}
	}

	return 0;
}

//スレッドを実行
void Mediator::RunThread(BOOL onFlg)
{
	this->m_bThreadFlg = onFlg;
	if(onFlg){
		Mediator::GetInstance()->m_tc.ResetCounter(TRUE);
		SetEvent(this->m_hEvent);
	}else{
		ResetEvent(this->m_hEvent);
		this->SetupRepaint(0,0,WND_WIDTH,WND_HEIGHT);
		this->Repaint();
	}
}


//シーン変更
void Mediator::ChangeScene(Scene* scene)
{
	this->m_lpScene = scene;
	this->m_lpScene->Init();
}

//親フラグ
BOOL Mediator::GetOyaFlg(void)
{
	return this->m_bOyaFlg;
}
void Mediator::SetOyaFlg(BOOL oyaFlg)
{
	this->m_bOyaFlg = oyaFlg;
}

//ロールオーバーIDを取得
UINT Mediator::GetROID(void)
{
	return this->m_uiROID;
}

//ロールオーバーIDを変更
void Mediator::ChangeROID(UINT roID,BOOL roFlg)
{
	if(roFlg) this->m_uiROID = roID;
	else{
		if(!roID || roID == this->m_uiROID) this->m_uiROID = NULL;
	}
}

//次局に進める
UINT Mediator::NxtKyoku(BOOL nxtFlg)
{
	if(nxtFlg) this->m_uiKyoku++;
	return this->m_uiKyoku;
}

//得点を加算
void Mediator::AddScore(int score,BOOL myFlg)
{
	int lose = (myFlg+1)%2;
	this->m_uiScoreTable[this->m_uiKyoku][myFlg] = score;
	this->m_uiScoreTable[this->m_uiKyoku][lose] = 0;
	//親を変更
	this->m_bOyaFlg = myFlg;
}

//累計得点を取得
Point Mediator::GetScore(void)
{
	UINT i,cnt;
	Point pt(0,0);
	cnt = (this->m_uiKyoku>=MONCNT)?MONCNT:this->m_uiKyoku;
	for(i=0;i<cnt;i++){
		pt.X += this->m_uiScoreTable[i][0];
		pt.Y += this->m_uiScoreTable[i][1];
	}
	return pt;
}

//指定ビットから指定の順番を取得
int Mediator::ExtBitOrder(int bit,int total,int i)
{
	if(bit){
		//ビットから順番を算出
		int sort = -1;
		int cnt = -1;
		while(++sort < total){
			if(bit & (1 << sort)){
				if(++cnt == i) return sort;
			}
		}
	}

	return -1;
}

//スコアー数値画像を格納
void Mediator::HouseScore(UINT score,std::vector<StrImage> &vSI,Point rPos,BOOL whiteFlg,double scale)
{
	//1の位から順に並べて描画
	int i=0,no=0;
	do{
		no = score%10;
		if(!whiteFlg) vSI.push_back(this->m_siNumberB[no]);
		else vSI.push_back(this->m_siNumberW[no]);
		StrImage* temp = &vSI.back();
		temp->SetScale(scale);
		temp->Init(rPos.X - (++i)*temp->GetWidth(),rPos.Y);
		score /= 10; 
	}while(score);	
}

//文字列画像を一括描画
void Mediator::PaintStrImage(std::vector<StrImage> &vSI,Graphics* g)
{
	std::vector<StrImage>::iterator it = vSI.begin();
	int i=0;
	for(it=vSI.begin();it != vSI.end();it++){
		StrImage* si = &*it;
		if(si) si->Paint(g);
		i++;
	}
}

