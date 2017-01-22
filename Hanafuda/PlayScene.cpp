#include "StdAfx.h"
#include "PlayScene.h"
#include "LastScene.h"
#include "Strategy.h"
#include "Random.h"
#include <algorithm>
#include <math.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

PlayScene::PlayScene(void): m_ptCF(20,20),m_ptCF2(260,20),
							m_ptBFt(50,146),m_ptBFt2(182,146),
							m_ptDF(404,252),m_ptMonth(300,283),
							m_ptBFb(50,358),m_ptBFb2(182,358),
							m_ptMF(20,484),m_ptMF2(260,484),
							m_ptGCF(520,106),m_ptGMF(520,398),
							m_ptCom(520,10),m_ptYou(520,302),
							m_ptCY(520,43),m_ptMY(520,335),
							m_ptYSize(75,20),
							m_ptCScore(788,10),m_ptMScore(788,302),
							m_ptSquare(35,131),m_ptSqSize(450,338),
							m_ptHai(134,332),m_ptIie(286,332)
{
	Mediator* md = Mediator::GetInstance();
	//札番号格納
	this->m_vNo.reserve(md->ALLFUDACNT);
	int i;
	for(i=0;i<md->ALLFUDACNT;i++){
		this->m_vNo.push_back(i);
	}
	//札領域確保
	for(i=0;i<UFUDACNT;i++){
		this->m_UFuda[i] = md->m_UraFuda;
	}
	this->m_MyFuda.reserve(TEFUDACNT);
	this->m_ComFuda.reserve(TEFUDACNT);
	this->m_BaFuda.reserve(BAFUDACNT);
	this->m_DaiFuda.reserve(DAIFUDACNT);
	//役一覧
	this->m_ylComYaku = md->m_siYakuList;
	this->m_ylMyYaku = md->m_siYakuList;
	//***文字列画像***
	//名前
	this->m_siCom = &md->m_siCom;
	this->m_siYou = &md->m_siYou;
	//スコアー
	this->m_siScore.reserve(10);
	//「文」
	for(i=0;i<2;i++) this->m_siMon[i] = md->m_siMonB;
	//中背景
	this->m_square = new Square(this->m_ptSqSize.X,this->m_ptSqSize.Y);
	//選択画像
	this->m_slHai = &md->m_slHai;
	this->m_slIie = &md->m_slIie;
	//BGM
	this->m_sound = &md->m_sndAcheive;
	//レイヤー
	this->m_vLayer.reserve(4);
	//リソースファイル
	this->m_hModule = ::LoadLibrary(_T("RS.dll"));
}

PlayScene::~PlayScene(void)
{
	//リソースファイル
	::FreeLibrary(this->m_hModule);
}

//札をシャッフルする
void PlayScene::Shuffle(void)
{
	Mediator* md = Mediator::GetInstance();
	int monAry[md->MONCNT];
	int i=0;
	BOOL cmtFlg = FALSE;

	//場札に同月が4枚ある場合、再シャッフル
	do{
		for(i=0;i<md->MONCNT;i++) monAry[i] = 0;
		Random r;
		std::random_shuffle(this->m_vNo.begin(),this->m_vNo.end(),r);
		cmtFlg = TRUE;
		for(i=0;i<TEFUDACNT;i++){
			int mon = this->m_vNo[i]/FUDACNT;
			if(++(monAry[mon]) >= FUDACNT) cmtFlg = FALSE;
		}
	}while(!cmtFlg);
}

//札をセットする
void PlayScene::SetupFuda(void)

{
	Mediator* md = Mediator::GetInstance();
	//札をシャッフル
	this->Shuffle();
	//札初期化
	int i;
	for(i=0;i<UFUDACNT;i++){//裏札
		if(i==0) this->m_UFuda[i].Init(this->m_ptDF);
		else this->m_UFuda[i].SetPos(this->m_ptDF);
	}
	//格納
	for(i=0;i<DAIFUDACNT;i++){
		if(i<TEFUDACNT){
			this->m_BaFuda.push_back(&md->m_Fuda[this->m_vNo[i]]);
			this->m_ComFuda.push_back(&md->m_Fuda[this->m_vNo[i+TEFUDACNT]]);
			this->m_MyFuda.push_back(&md->m_Fuda[this->m_vNo[i+TEFUDACNT*2]]);
		}
		this->m_DaiFuda.push_back(&md->m_Fuda[this->m_vNo[i+DAIFUDACNT]]);
	}
}


//移動経路を確定する
void PlayScene::SetupRoute(Fuda* lpFuda,Point* ePt,UINT cnt,UINT step)
{
	UINT i,j;
	int x,y;

	//Vector初期化
	for(i=0;i<this->m_ptMove.size();i++) this->m_ptMove[i].clear();
	this->m_ptMove.clear();
	this->m_ptMove.resize(cnt);

	//座標配列を格納
	for(i=0;i<cnt;i++){
		this->m_ptMove[i].resize(step);
		double bX = static_cast<double>((lpFuda+i)->_x);
		double bY = static_cast<double>((lpFuda+i)->_y);
		double eX = static_cast<double>((ePt+i)->X);
		double eY = static_cast<double>((ePt+i)->Y);
		double dX = (eX - bX)/static_cast<double>(step);
		double dY = (eY - bY)/static_cast<double>(step);
		for(j=0;j<step;j++){
			x = static_cast<int>(bX + dX*(j+1) + 0.5);
			y = static_cast<int>(bY + dY*(j+1) + 0.5);
			this->m_ptMove[i][j] = Point(x,y);
		}
	}
}


//移動経路を確定する
void PlayScene::SetupRoute(Fuda** lplpFuda,Point* ePt,UINT cnt,UINT step)
{
	Fuda* fuda = new Fuda[cnt];
	UINT i = 0;
	for(i=0;i<cnt;i++){
		fuda[i] = **(lplpFuda++);
	}
	this->SetupRoute(fuda,ePt,cnt,step);
	delete [] fuda;
}


//合わせ札の移動経路を確定する
void PlayScene::SetupAFRoute(BOOL teFlg,UINT step)
{
	Mediator* md = Mediator::GetInstance();
	int order = md->ExtBitOrder(this->m_iBFbitBlank,BAFUDACNT);
	int dif = 0;
	if((teFlg && (this->m_iBFbit1>0)) || (!teFlg && (this->m_iBFbit2>0))){
		dif = BAFUDADIF;
		order = (teFlg)?md->ExtBitOrder(this->m_iBFbit1,BAFUDACNT):md->ExtBitOrder(this->m_iBFbit2,BAFUDACNT);
	}
	int turn = (FUDACNT*2-1)*2;
	int dx = (order/turn)?order%turn:order/2;
	int x = this->m_ptBFt.X+dx*(this->m_UFuda[0].GetWidth()+BAFUDADIF);
	int y = (order/turn)?this->m_ptDF.Y:(order%2)?this->m_ptBFb.Y:this->m_ptBFt.Y;
	Fuda* fuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	this->SetupRoute(fuda,&Point(x+dif,y+dif),1,step);
}

//獲得札の移動経路を確定する
void PlayScene::SetupGFRoute(Fuda** lplpFuda,UINT cnt,UINT step,UINT ratio)
{
	Mediator* md = Mediator::GetInstance();
	BOOL myFlg = this->IsMyTurn();
	Point oPt = (myFlg)?this->m_ptGMF:this->m_ptGCF;
	Point* ePt = new Point[cnt];
	UINT i = 0;
	UINT kasuCnt = 0;
	for(i=0;i<cnt;i++){
		if(lplpFuda[i] == NULL) break;
		UINT kind = lplpFuda[i]->GetKind();
		UINT size = (myFlg)?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
		if(kind==md->KIND_KASU) size += kasuCnt++;
		if(size+1>=GFUDACNT) size = GFUDACNT-1;
		int x = oPt.X + size*this->m_UFuda[0].GetWidth()/ratio;
		int y = oPt.Y + kind*this->m_UFuda[0].GetHeight()/ratio;
		ePt[i] = Point(x,y);

	}
	this->SetupRoute(lplpFuda,ePt,i,step);

	delete [] ePt;
}

//札を指定枚数分、1フレームずつ縮小しながらずらす
BOOL PlayScene::SlideReduce(Fuda** lplpFuda,UINT cnt,UINT step,UINT ratio)
{
	//移動
	if(this->m_uiFrameNo++ < step){
		Mediator* md = Mediator::GetInstance();
		UINT i;
		for(i=0;i<cnt;i++){
			if(lplpFuda[i] == NULL) break;
			md->SetupRepaint(lplpFuda[i]->returnRect());
			lplpFuda[i]->SetPos(this->m_ptMove[i][this->m_uiFrameNo-1],1.0-(double)this->m_uiFrameNo/(double)(ratio*step));
			md->SetupRepaint(lplpFuda[i]->returnRect());
		}
		return TRUE;
	}
	
	//移動完了
	return FALSE;
}



//裏札を指定枚数分、1フレームずらす
BOOL PlayScene::Slide(Fuda* lpUFuda,UINT cnt,UINT step)
{
	//移動
	if(this->m_uiFrameNo++ < step){
		Mediator* md = Mediator::GetInstance();
		UINT i;
		for(i=0;i<cnt;i++){
			md->SetupRepaint((lpUFuda+i)->returnRect());
			(lpUFuda+i)->SetPos(this->m_ptMove[i][this->m_uiFrameNo-1]);
			md->SetupRepaint((lpUFuda+i)->returnRect());
		}
		return TRUE;
	}
	
	//移動完了
	return FALSE;
}


//札を配る
void PlayScene::Deal(void){
	Mediator* md = Mediator::GetInstance();
	Fuda* fuda = this->m_UFuda;
	Point ePt[FUDACNT];
	UINT i;

	switch(this->m_uiCutNo){
		case 0://手札4枚(先攻)左
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT*2]:&this->m_UFuda[1];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptMF:this->m_ptCF;
			break;
		case 1://手札4枚(後攻)左
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1]:&this->m_UFuda[1+FUDACNT*2];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptCF:this->m_ptMF;
			break;
		case 2://場札4枚(上2枚下2枚)左
			fuda = &this->m_UFuda[1+FUDACNT*4];
			if(!this->m_uiFrameNo){
				ePt[0] = this->m_ptBFt;
				ePt[1] = this->m_ptBFb;
				ePt[2] = Point(this->m_ptBFt.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFt.Y);
				ePt[3] = Point(this->m_ptBFb.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFb.Y); 
			}
			break;
		case 3://手札4枚(先攻)右
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT*3]:&this->m_UFuda[1+FUDACNT];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptMF2:this->m_ptCF2;
			break;
		case 4://手札4枚(後攻)右
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT]:&this->m_UFuda[1+FUDACNT*3];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptCF2:this->m_ptMF2;
			break;
		case 5://場札4枚(上2枚下2枚)右
			fuda = &this->m_UFuda[1+FUDACNT*4];
			if(!this->m_uiFrameNo){
				ePt[0] = this->m_ptBFt2;
				ePt[1] = this->m_ptBFb2;
				ePt[2] = Point(this->m_ptBFt2.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFt2.Y);
				ePt[3] = Point(this->m_ptBFb2.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFb2.Y); 
			}
			break;
		default://対戦シーンへ
			this->m_uiCutNo = 0;
			this->m_uiSceneNo = SCENE_PLAY;
			fuda = &this->m_UFuda[1+FUDACNT*2];
			//手札をめくる
			for(i=0;i<TEFUDACNT;i++){
				(fuda+i)->Trace(this->m_MyFuda[i]);
				(fuda+i)->Init(this->m_ptDF);
				//※デバッグ用
				if(md->m_bDebugFlg){
					this->m_ComFuda[i]->Init(this->m_ptCF.X + fuda->GetWidth()*i,this->m_ptCF.Y);
				}
			}
			this->m_uiStopCnt = 10;
			return;
	}

	//手札を横一列に並べる
	BOOL bafudaFlg = (this->m_uiCutNo % 3 == 2)?TRUE:FALSE;
	if(!bafudaFlg && !this->m_uiFrameNo){
		for(i=0;i<FUDACNT;i++){
			ePt[i] = Point(ePt[0].X + fuda->GetWidth()*i,ePt[0].Y);
		}
	}

	//移動経路を確定する
	if(!this->m_uiFrameNo){
		this->SetupRoute(fuda,ePt,FUDACNT);
		for(i=0;i<FUDACNT;i++){
			(fuda+i)->Init();
		}
	}

	//移動
	if(!this->Slide(fuda,FUDACNT)){
		//効果音
		PlaySound(MAKEINTRESOURCE(IDR_WAV_PUT2),this->m_hModule,SND_RESOURCE | SND_ASYNC);


		//場札をめくる
		if(bafudaFlg){
			for(i=0;i<FUDACNT;i++){
				(fuda+i)->Trace(this->m_BaFuda[i+FUDACNT*(this->m_uiCutNo/3)]);
				(fuda+i)->Init(this->m_ptDF);
			}
		}

		//次のカットへ
		this->m_uiFrameNo = 0;
		this->m_uiCutNo++;
	}
}


//対戦
BOOL PlayScene::Play(void){

	switch(this->m_uiCutNo){
		case CUT_TSELECT://札選択
			return this->PlayTSelect();
		case CUT_BSELECT1://場札選択
			return this->PlayBSelect(TRUE);
		case CUT_TPUT://手札合わせ
			return this->PlayPut(TRUE);
		case CUT_DOPEN://台札めくり
			return this->PlayDOpen();
		case CUT_DPUT://台札合わせ
			return this->PlayPut(FALSE);
		case CUT_TGET://手札獲得
			return this->PlayGet(&this->m_GetTFuda);
		case CUT_B1GET://場札1獲得
			return this->PlayGet(this->m_GetBFuda1,FUDACNT-1);
		case CUT_DGET://台札獲得
			return this->PlayGet(&this->m_GetDFuda);
		case CUT_B2GET://場札2獲得
			return this->PlayGet(this->m_GetBFuda2,FUDACNT-1);
		case CUT_SORT://並び替え
			return this->PlaySort();
		case CUT_ACHIEVE://役達成
			return this->PlayAcheive();
		case CUT_KOIKOI://こいこい
			return this->PlayKoikoi();
		case CUT_RESULT://結果
			return this->PlayResult();
	}
	return TRUE;
}


//手札選択
BOOL PlayScene::PlayTSelect(void){

	BOOL rtnFlg = TRUE;

	//中背景リセット
	this->m_square->Reset();
	//レイヤーリセット
	this->m_vLayer.clear();

	//自分の番なら、スレッドを止める
	if(this->IsMyTurn()){
		//選択可能な場札にフラグを立てる
		this->SetupSelectable();
		rtnFlg = FALSE;
	}else{
		//コンピュータが次の札を選択
		UINT i  = Strategy::GetInstance()->SelectCF();
		//次の札をめくる
		this->m_GetTFuda = this->m_ComFuda[i];
		this->m_UFuda[i+1].Trace(this->m_GetTFuda);
		//選択場札を格納
		if(this->HouseBF(TRUE,this->m_GetTFuda->GetID()/FUDACNT)){
			this->m_uiCutNo = CUT_TPUT;//手札移動カットへ
			this->m_uiStopCnt = 10;
		}else{//同月が二つの場合
			this->m_uiCutNo = CUT_BSELECT1;//場札選択カットへ
		}
		//選択手札を削除
		this->AllFudaFunc(this->m_ComFuda,FUNC_ERASE,(double)i);
	}

	return rtnFlg;
}
//場札選択
BOOL PlayScene::PlayBSelect(BOOL teFlg){

	BOOL rtnFlg = TRUE;

	Fuda** lplpFuda = (teFlg)?this->m_GetBFuda1:this->m_GetBFuda2;
	if(this->IsMyTurn()){//自分の番
		//選択場札以外を透過表示
		this->AllFudaFunc(this->m_BaFuda,FUNC_INVISIBLE,lplpFuda[0]->GetID()/FUDACNT,TRUE);
		lplpFuda[0]->SetupSelectable(TRUE);
		lplpFuda[1]->SetupSelectable(TRUE);
		//場札選択カットへ
		this->m_uiCutNo = (teFlg)?CUT_BSELECT1:CUT_BSELECT2;
		rtnFlg = FALSE;
	}else{//COMの番
		//非選択札を場札に戻す
		if(!Strategy::GetInstance()->AlternativeBF(lplpFuda[0]->GetID(),lplpFuda[0]->GetKind(),lplpFuda[1]->GetID(),lplpFuda[1]->GetKind())){
			this->ReturnBF(teFlg,lplpFuda[1],1);
		}else{
			this->ReturnBF(teFlg,lplpFuda[0],0);
			lplpFuda[0] = lplpFuda[1];
		}
		lplpFuda[1] = NULL;
		//手札移動カットへ
		this->m_uiCutNo = (teFlg)?CUT_TPUT:CUT_DPUT;
		this->m_uiStopCnt = 10;
	}

	return rtnFlg;
}
//札合わせ
BOOL PlayScene::PlayPut(BOOL teFlg){

	//経路設定
	if(!this->m_uiFrameNo) this->SetupAFRoute(teFlg);
	//移動
	Fuda* lpFuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	if(!this->Slide(lpFuda,1)){
		Mediator* md = Mediator::GetInstance();
		UINT id = 0;
		//未選択の札を場札に格納
		if((teFlg && !this->m_iBFbit1) || (!teFlg && !this->m_iBFbit2)){
			//効果音
			id = IDR_WAV_PUT2;
			this->AddBF(teFlg);
			lpFuda = NULL;
		}else id = IDR_WAV_PUT1;//this->m_sound = &md->m_sndPut1;
		//効果音
		PlaySound(MAKEINTRESOURCE(id),this->m_hModule,SND_RESOURCE | SND_ASYNC);
		//次のカットへ
		this->m_uiCutNo = (teFlg)?CUT_DOPEN:(this->m_iBFbit1)?CUT_TGET:(this->m_iBFbit2)?CUT_DGET:CUT_SORT;
		this->m_uiFrameNo = 0;
		this->m_uiStopCnt = 10;
	}

	return TRUE;
}
//台札めくり
BOOL PlayScene::PlayDOpen(void){

	BOOL rtnFlg = TRUE;

	//台札めくる
	this->m_GetDFuda = this->m_DaiFuda.front();
	this->m_GetDFuda->Init(this->m_ptDF);
	this->m_DaiFuda.erase(this->m_DaiFuda.begin());

	//獲得場札を格納
	if(this->HouseBF(FALSE,this->m_GetDFuda->GetID()/FUDACNT)){
		//台札合わせカットへ
		this->m_uiCutNo = CUT_DPUT;
		this->m_uiStopCnt = 10;
	}else{//同月が二つの場合
		//場札選択
		rtnFlg = this->PlayBSelect(FALSE);
	}
	return rtnFlg;
}
//札獲得
BOOL PlayScene::PlayGet(Fuda** lplpFuda,UINT cnt){

	//経路設定
	if(!this->m_uiFrameNo){
		this->SetupGFRoute(lplpFuda,cnt);
		this->SortLayer(this->m_uiCutNo);
	}
	//移動
	if(!this->SlideReduce(lplpFuda,cnt)){
		Mediator* md = Mediator::GetInstance();
		//効果音
		PlaySound(MAKEINTRESOURCE(IDR_WAV_PUT2),this->m_hModule,SND_RESOURCE | SND_ASYNC);
		//獲得済み場札を格納
		this->HouseGF(lplpFuda,cnt);
		//次カットへ
		switch(this->m_uiCutNo){
			case CUT_TGET://手札獲得
				this->m_uiCutNo = CUT_B1GET;//場札1獲得へ
				break;
			case CUT_B1GET://場札1獲得
				this->m_uiCutNo = (this->m_iBFbit2)?CUT_DGET:CUT_SORT;//台札獲得or並び替えへ
				break;
			case CUT_DGET://台札獲得
				this->m_uiCutNo = CUT_B2GET;//場札2獲得へ
				break;
			case CUT_B2GET://場札2獲得
				this->m_uiCutNo = CUT_SORT;//並び替えへ
				break;
		}
		this->m_uiFrameNo = 0;
		//並び替えの前に、一時停止
		if(this->m_uiCutNo == CUT_SORT) this->m_uiStopCnt = 10;
	}

	return TRUE;
}

//並び替え
BOOL PlayScene::PlaySort(void){

	Mediator* md = Mediator::GetInstance();
	int bit = this->m_iBFbit1 | this->m_iBFbit2;

	if(this->IsMyTurn()){//自分のターン
		//手札のソート
		this->AllFudaFunc(this->m_MyFuda,FUNC_SORT,SORT_TEFUDA);
		//獲得済み札(カスのみ)
		if(bit && (this->m_GMyFuda[md->KIND_KASU].size()>GFUDACNT))
			this->AllFudaFunc(this->m_GMyFuda[md->KIND_KASU],FUNC_SORT,SORT_GETFUDA);
	}else{//相手のターン
		//相手札(裏札)のソート
		UINT i;
		for(i=1;i<TEFUDACNT+1;i++){
			if(i<TEFUDACNT-this->m_uiTurnNo/2) this->m_UFuda[i].Init();
			else this->m_UFuda[i].Reset();
		}
		//相手札(表札)のソート※デバッグ用
		if(md->m_bDebugFlg) this->AllFudaFunc(this->m_ComFuda,FUNC_SORT,SORT_COMFUDA);
		//獲得済み札(カスのみ)
		if(bit && (this->m_GComFuda[md->KIND_KASU].size()>GFUDACNT))
			this->AllFudaFunc(this->m_GComFuda[md->KIND_KASU],FUNC_SORT,SORT_GETFUDA);
	}
	//獲得ビットリセット
	this->m_iBFbit1 = this->m_iBFbit2 = this->m_iBFbitBlank = 0;

	//役判定
	this->m_iAchieveBit = Strategy::GetInstance()->Judge(this->IsMyTurn());
	if(this->m_iAchieveBit){
		//役達成カットへ
		this->m_uiCutNo = CUT_ACHIEVE;
		this->AddAcheiveBit(this->m_iAchieveBit);
	}else{
		//札終了→結果カット else 手札カットへ
		this->m_uiCutNo = (++this->m_uiTurnNo >= 2*TEFUDACNT)?CUT_RESULT:CUT_TSELECT;
		this->m_uiStopCnt = 10;
	}

	return TRUE;
}
//役達成
BOOL PlayScene::PlayAcheive(void){

	Mediator* md = Mediator::GetInstance();
	//次のカットへ
	if(!this->m_iAchieveBit){
		this->m_uiCutNo = CUT_RESULT;
		//最終ターンより前
		if(this->m_uiTurnNo/2+1 < TEFUDACNT){
			//自分の番or(COMの番&「こいこいする」)→こいこいカットへ
			if(this->IsMyTurn() || Strategy::GetInstance()->IsKoiKoi()) this->m_uiCutNo = CUT_KOIKOI;
		}
	}else{
		//中背景配置
		if(!this->m_uiFrameNo){
			this->m_square->Reset();
			this->m_square->Init(this->m_ptSquare);
		}
		//中背景表示
		if(++this->m_uiFrameNo < FADECNT){
			this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
		}else{
			//勝ち越しマーク
			this->m_uiWin = (this->IsMyTurn())?WIN_ME:WIN_COM;
			//達成した役を並べる
			this->DispAcheiveFuda();
			this->m_uiStopCnt = 150;
			this->m_uiFrameNo = 0;
			//BGM
			this->m_sound = &md->m_sndAcheive;
			SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
		}
	}
	//再表示
	md->SetupRepaint(this->m_square->returnRect());

	return TRUE;
}
//こいこい
BOOL PlayScene::PlayKoikoi(void){

	BOOL rtnFlg = TRUE;

	//中背景初期化
	if(!this->m_uiFrameNo){
		this->m_square->Reset();
		this->m_square->Init(this->m_ptSquare);
	}
	//中背景表示
	if(++this->m_uiFrameNo < FADECNT){
		this->m_square->ReduceArea(1.0f-(0.33f/FADECNT)*this->m_uiFrameNo);
		this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
	}else{
		//「こいこいします」or「こいこいしますか？」
		this->m_square->ChangeMode(this->m_square->MODE_KOIKOI);
		this->m_uiFrameNo = 0;
		//こいこい選択
		if(this->IsMyTurn()){//自分の番
			//「はい」「いいえ」
			this->m_slHai->Init(this->m_ptHai);
			this->m_slIie->Init(this->m_ptIie);
			rtnFlg = FALSE;
		}else{//相手の番
			//「こいこいします」
			this->m_uiStopCnt = 50;
			this->m_uiTurnNo++;//次のターンへ
			this->m_uiCutNo = CUT_TSELECT;//手札選択カットへ
		}

	}
	//再描画
	Mediator::GetInstance()->SetupRepaint(this->m_square->returnRect());

	return rtnFlg;
}

//結果
BOOL PlayScene::PlayResult(void){

	Mediator* md = Mediator::GetInstance();
	BOOL rtnFlg = TRUE;

	//中背景初期化
	if(!this->m_uiFrameNo){
		this->m_square->Reset();
		this->m_square->Init(this->m_ptSquare);
		this->m_square->ReduceArea(1.0);
	}
	//中背景表示
	if(++this->m_uiFrameNo < FADECNT){
		this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
	}else{
		//モード変更
		int bit =(this->m_uiWin == WIN_ME)?this->m_iMyAchieveBit:(this->m_uiWin == WIN_COM)?this->m_iComAchieveBit:0;
		this->m_square->ChangeMode(this->m_square->MODE_RESULT,bit);
		//BGM
		this->m_sound = &md->m_sndWin;
		SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
		rtnFlg = FALSE;
	}
	//再描画
	md->SetupRepaint(this->m_square->returnRect());

	return rtnFlg;
}


//自分の番かどうか
BOOL PlayScene::IsMyTurn(void)
{
	return (this->m_uiTurnNo + Mediator::GetInstance()->GetOyaFlg())%2;
}

//札ベクトルを一括処理
BOOL PlayScene::AllFudaFunc(std::vector<Fuda*> &vFuda,int func,double param,BOOL flag)
{

	std::vector<Fuda*>::iterator it = vFuda.begin();
	BOOL rtnFlg = FALSE;
	int i=0;
	for(it=vFuda.begin();it != vFuda.end();it++){
		Fuda* fuda = *it;
		if(fuda){
			switch(func){
				case FUNC_PAINT://描画
					fuda->Paint(Mediator::GetInstance()->m_gOffScreen);
					break;
				case FUNC_SELECTABLE://選択可能札のフラグを切り替え
					if(this->IsSameMonth(fuda,(UINT)param)){
						fuda->SetupSelectable(flag);					
						rtnFlg = TRUE;
					}
					break;
				case FUNC_SELECTED://選択中札のフラグを切り替え
					if(this->IsSameMonth(fuda,(UINT)param)){
						fuda->SetupSelected(flag);					
						rtnFlg = TRUE;
					}
					break;
				case FUNC_RESET://選択リセット
					fuda->ResetSelect();
					break;
				case FUNC_INVISIBLE://非選択場札を不可視にする
					if(!this->IsSameMonth(fuda,(UINT)param)) fuda->SetupInvisible(flag);
					break;
				case FUNC_SORT://札並び替え
					this->SortFuda(fuda,(int)param,i);
					break;
				case FUNC_ERASE://削除
					if(i==(int)param){
						vFuda.erase(it);
						return TRUE;
					}
					break;
				case FUNC_COPY://コピー
					UINT id = fuda->GetID();
					this->m_square->m_AFuda.push_back(Mediator::GetInstance()->m_Fuda[id]);
					break;
			}
		}
		i++;
	}

	return rtnFlg;
}


//選択可能な札にフラグを立てる
void PlayScene::SetupSelectable(void)
{
	std::vector<UINT> rptMon;//重複月
	std::vector<Fuda*>::iterator itMF;

	itMF = this->m_MyFuda.begin();
	for(itMF=this->m_MyFuda.begin();itMF != this->m_MyFuda.end();itMF++){
		Fuda* mf = *itMF;
		UINT mon = mf->GetID()/FUDACNT;
		BOOL repFlg = FALSE;
		if(!rptMon.empty()){//重複月は省略
			std::vector<UINT>::iterator itMon = rptMon.begin();
			for(itMon=rptMon.begin();itMon!=rptMon.end();itMon++){
				UINT* tMon = &*itMon;
				if(mon==*tMon){
					mf->SetupSelectable(TRUE);
					continue;
				}
			}
		}
		//選択可能な場札にフラグを立てる
		if(this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTABLE,mon,TRUE)){
			//手札にフラグを立てる
			rptMon.push_back(mon);
			mf->SetupSelectable(TRUE);
		}
	}
}


//同じ月の札か？
BOOL PlayScene::IsSameMonth(Fuda* fuda,UINT mon)
{	
	if(mon==fuda->GetID()/FUDACNT) return TRUE;
	return FALSE;
}


//合わせる場札を格納する
BOOL PlayScene::HouseBF(BOOL teFlg,UINT mon)
{
	int bit = 1;
	UINT cnt = 0;
	Fuda** lplpBaFuda = (teFlg)?this->m_GetBFuda1:this->m_GetBFuda2;
	int* lpBFbit = (teFlg)?&this->m_iBFbit1:&this->m_iBFbit2;
	int layer = (teFlg)?CUT_B1GET:CUT_B2GET;
	this->m_iBFbitBlank = 0;

	try{
		std::vector<Fuda*>::iterator it = this->m_BaFuda.begin();
		while(it != this->m_BaFuda.end()){
			Fuda* bf = *it;
			if(!bf){
				if(!(bit & (this->m_iBFbit1 | this->m_iBFbit2))) this->m_iBFbitBlank += bit;
			}else{
				if(this->IsSameMonth(bf,mon)){
					//獲得場札を格納
					lplpBaFuda[cnt++] = bf;
					//ビット加算
					*lpBFbit += bit;
					//場札削除
					*it = NULL;
					//場札レイヤーを重ねる
					if(cnt==1) this->m_vLayer.insert(this->m_vLayer.begin(),layer);
				}
			}
			bit <<= 1;
			it++;
		}
	}
	catch(...)
	{
		// 処理A
		OutputDebugString(L"HouseBF");

	}
	
	//最後尾の空白ビットをセットする
	if(!this->m_iBFbitBlank) this->m_iBFbitBlank += 1 << this->m_BaFuda.size();

	//手札台札レイヤーを重ねる
	layer = (teFlg)?CUT_TGET:CUT_DGET;	
	this->m_vLayer.insert(this->m_vLayer.begin(),layer);

	//2枚なら選択カットへ
	if(cnt == 2) return FALSE;

	return TRUE;
}



//選択しない方の場札を元に戻す
BOOL PlayScene::ReturnBF(BOOL teFlg,Fuda* lpBaFuda,UINT i)
{
	int* lpBFbit = (teFlg)?&this->m_iBFbit1:&this->m_iBFbit2;
	int sort = Mediator::GetInstance()->ExtBitOrder(*lpBFbit,BAFUDACNT,i);
	if(sort<0) return FALSE;
	int bit = 1 << sort;

	//札を戻す
	this->m_BaFuda[sort] = lpBaFuda;
	//ビットを戻す
	*lpBFbit -= bit;

	return TRUE;
}


//選択不可の札を場札に加える
void PlayScene::AddBF(BOOL teFlg)
{

	int bit = 1;
	Fuda* lpFuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	BOOL addFlg = FALSE;

	//空いてる場所に挿入
	for(UINT i=0;i<this->m_BaFuda.size();i++){
		if(!this->m_BaFuda[i]){
			if(teFlg || !(this->m_iBFbit1 & (bit << i))){
				this->m_BaFuda[i] = lpFuda;
				return;
			}
		}
	}

	//空いてないので最後尾に入れる
	this->m_BaFuda.push_back(lpFuda);
}


//獲得済札を格納する
void PlayScene::HouseGF(Fuda **lplpFuda,UINT cnt)
{
	Mediator* md = Mediator::GetInstance();
	UINT i;
	for(i=0;i<cnt;i++){
		if(lplpFuda[i] == NULL) break;
		UINT kind = lplpFuda[i]->GetKind();
		if(this->IsMyTurn()) this->m_GMyFuda[kind].push_back(lplpFuda[i]);
		else this->m_GComFuda[kind].push_back(lplpFuda[i]);
		Strategy::GetInstance()->SetupGFudaBit(this->IsMyTurn(),lplpFuda[i]->GetID(),lplpFuda[i]->GetKind());
		lplpFuda[i] = NULL;
	}
}


//札を並び替える
void PlayScene::SortFuda(Fuda* fuda,int fudakind,int i)
{
	int nX;
	int size;
	int maxSize;
	switch(fudakind){
		case SORT_TEFUDA://手札
			nX = this->m_ptMF.X;
			size = this->m_MyFuda.size();
			maxSize = TEFUDACNT;
			break;
		case SORT_COMFUDA://相手札※デバッグ用
			nX = this->m_ptCF.X;
			size = this->m_ComFuda.size();
			maxSize = TEFUDACNT;
			break;
		case SORT_GETFUDA://獲得済み札(カス)
			int kind;
			kind = Mediator::GetInstance()->KIND_KASU;
			nX = this->m_ptGMF.X;
			size = (this->IsMyTurn())?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
			maxSize = GFUDACNT;
			break;
	}
	//maxサイズ超えたら、詰めて表示
	nX += (size <= maxSize)?i*fuda->GetWidth():(int)(i*fuda->GetWidth()*(double)(maxSize-1)/(size-1));
	//再表示
	if(fuda->GetPos().X != nX){
		Mediator::GetInstance()->SetupRepaint(fuda->returnRect());
		fuda->SetPos(Point(nX,fuda->_y));
		Mediator::GetInstance()->SetupRepaint(fuda->returnRect());
	}

}

//指定のレイヤーを一番上にする
void PlayScene::SortLayer(int layer)
{
	std::vector<int>::iterator it = std::find(this->m_vLayer.begin(),this->m_vLayer.end(),layer);
	std::rotate(this->m_vLayer.begin(),it,this->m_vLayer.end());
}


//達成した役札を表示する
void PlayScene::DispAcheiveFuda(void)
{
	Strategy* st = Strategy::GetInstance();
	Mediator* md = Mediator::GetInstance();
	std::vector<Fuda*>* gFuda = (this->IsMyTurn())?this->m_GMyFuda:this->m_GComFuda;//獲得済札

	//表示役札を格納
	int order = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT);
	switch(order){
		case st->YAKU_GOKO:
		case st->YAKU_SHIKO:
		case st->YAKU_AMESHIKO:
		case st->YAKU_SANKO:
			this->AllFudaFunc(gFuda[md->KIND_GOKO],FUNC_COPY);
			break;
		case st->YAKU_HANAMI:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_SAKURA]);
			break;
		case st->YAKU_TSUKIMI:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_TSUKI]);
			break;
		case st->YAKU_AKATAN:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA1]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA2]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA3]);
			break;
		case st->YAKU_AOTAN:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO1]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO2]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO3]);
			break;
		case st->YAKU_INOSHIKA:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_INO]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_SHIKA]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_CHOU]);
			break;
		case st->YAKU_TANE:
			this->AllFudaFunc(gFuda[md->KIND_TANE],FUNC_COPY);
			break;
		case st->YAKU_TAN:
			this->AllFudaFunc(gFuda[md->KIND_TAN],FUNC_COPY);
			break;
		case st->YAKU_KASU:
			this->AllFudaFunc(gFuda[md->KIND_KASU],FUNC_COPY);
			if(st->IsBakeFuda(this->IsMyTurn())) this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			break;
	}

	//札を並べる
	this->m_square->Sort();
	//モード変更
	this->m_square->ChangeMode(this->m_square->MODE_ACHIEVE,this->m_iAchieveBit);
	//ビットをクリアー
	this->m_iAchieveBit ^= 1 << order;	

}

//達成した約ビットを加算する
void PlayScene::AddAcheiveBit(int bit)
{
	Strategy* st = Strategy::GetInstance();

	int* lpABit = (this->IsMyTurn())?&this->m_iMyAchieveBit:&this->m_iComAchieveBit;
	//三光以上のビットを一旦オフ
	int gokoBit = (1 << st->YAKU_GOKO) | (1 << st->YAKU_SHIKO) | (1 << st->YAKU_AMESHIKO) | (1 << st->YAKU_SANKO);
	if(bit & gokoBit) *lpABit &= ~gokoBit;
	//加算
	*lpABit |= bit;
}


//初期化
void PlayScene::Init(void){

	Mediator* md = Mediator::GetInstance();

	//札をセットする
	this->SetupFuda();
	//役一覧
	this->m_ylComYaku.Init(this->m_ptCY);
	this->m_ylMyYaku.Init(this->m_ptMY);
	//***文字列画像***
	//月
	this->m_siMonth = &md->m_siMonth[md->NxtKyoku(FALSE)];
	this->m_siMonth->Init(this->m_ptMonth);
	//名前
	this->m_siCom->Init(this->m_ptCom);
	this->m_siYou->Init(this->m_ptYou);
	//スコアー
	this->m_siScore.clear();
	Point score = md->GetScore();
	md->HouseScore(score.X,this->m_siScore,this->m_ptCScore);
	md->HouseScore(score.Y,this->m_siScore,this->m_ptMScore);
	//「文」
	this->m_siMon[0].Init(this->m_ptCScore);
	this->m_siMon[1].Init(this->m_ptMScore);
	//勝ち越しマーク
	this->m_uiWin = WIN_OFF;
	//シーンセット
	this->m_uiSceneNo = SCENE_DEAL;
	this->m_uiCutNo = CUT_TSELECT;
	this->m_uiFrameNo = 0;
	this->m_uiTurnNo = 0;
	//ビット
	this->m_iBFbit1 = 0;//場札獲得ビット1
	this->m_iBFbit2 = 0;//場札獲得ビット2
	this->m_iBFbitBlank = 0;//場札空白ビット
	this->m_iAchieveBit = 0;//達成役ビット
	this->m_iMyAchieveBit = 0;//達成役ビット(自分)
	this->m_iComAchieveBit = 0;//達成役ビット(COM)

}

//リセット
void PlayScene::Reset(void){

	Mediator* md = Mediator::GetInstance();
	//札
	int i;
	for(i=0;i<UFUDACNT;i++){//裏札
		this->m_UFuda[i].Reset();
	}
	for(i=0;i<md->ALLFUDACNT;i++){//表札
		md->m_Fuda[i].Reset();
	}
	this->m_BaFuda.clear();//場札
	this->m_ComFuda.clear();//相手札
	this->m_MyFuda.clear();//自札
	this->m_DaiFuda.clear();//台札
	for(i=0;i<FUDACNT;i++){
		this->m_GMyFuda[i].clear();//獲得済手札
		this->m_GComFuda[i].clear();//獲得済相手札
	}
	//役一覧
	this->m_ylComYaku.Reset();
	this->m_ylMyYaku.Reset();
	//***文字列画像***
	//名前
	this->m_siCom->Reset();
	this->m_siYou->Reset();
	//スコアー
	this->m_siScore.clear();
	//「文」
	this->m_siMon[0].Reset();
	this->m_siMon[1].Reset();
	//月
	this->m_siMonth->Reset();
	//中背景
	this->m_square->Reset();
	//「はい」「いいえ」
	this->m_slHai->Reset();
	this->m_slIie->Reset();
	//戦略
	Strategy::GetInstance()->Reset();
	//効果音
	SendMessage(md->m_hWnd,WM_SOUND,FALSE,NULL);
}

//マウス移動時の処理
BOOL PlayScene::MouseMove(double x,double y){

	//手札選択時
	BOOL rtnFlg = FALSE;
	if(this->m_uiCutNo==CUT_TSELECT){
		std::vector<Fuda*>::iterator it = this->m_MyFuda.begin();
		UINT sMon = -1;
		for(it=this->m_MyFuda.begin();it!=this->m_MyFuda.end();it++){
			Fuda* fuda = *it;
			UINT mon = fuda->GetID()/FUDACNT;
			if(fuda->RollOver(x,y)){
				//選択場札にフラグを立てる
				if(this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTED,mon,TRUE)) sMon = mon;
				//上に札をずらす
				fuda->SetPos(Point(fuda->_x,fuda->_y-10));
				rtnFlg = TRUE;
			}else if(fuda->RollOut(x,y)){
				//選択場札のフラグを戻す
				if(sMon != mon) this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTED,mon,FALSE);
				//下に札を戻す
				fuda->SetPos(Point(fuda->_x,fuda->_y+10));
				rtnFlg = TRUE;
			}
		}
	}else if(this->m_uiCutNo==CUT_BSELECT1 || this->m_uiCutNo==CUT_BSELECT2){//場札選択時
		Fuda* bFuda0 = (this->m_uiCutNo==CUT_BSELECT1)?this->m_GetBFuda1[0]:this->m_GetBFuda2[0];
		Fuda* bFuda1 = (this->m_uiCutNo==CUT_BSELECT1)?this->m_GetBFuda1[1]:this->m_GetBFuda2[1];
		if(bFuda0->MouseMove(x,y)) rtnFlg = TRUE;
		else if(bFuda1->MouseMove(x,y)) rtnFlg = TRUE;
	}else if(this->m_uiCutNo==CUT_KOIKOI && this->IsMyTurn()){//こいこい選択時
		if(this->m_slHai->MouseMove(x,y)) rtnFlg = TRUE;
		else if(this->m_slIie->MouseMove(x,y)) rtnFlg = TRUE;
	}else if(this->m_uiCutNo==CUT_RESULT){//結果表示時
		this->m_square->MouseMove(x,y);//カーソル変更
	}
	return rtnFlg;
}

//マウスクリック時の処理
BOOL PlayScene::MouseClick(double x,double y){

	BOOL rtnFlg = FALSE;
	if(this->m_uiCutNo==CUT_TSELECT){//手札選択時
		std::vector<Fuda*>::iterator it = this->m_MyFuda.begin();
		for(it=this->m_MyFuda.begin();it!=this->m_MyFuda.end();it++){
			Fuda* fuda = *it;
			if(fuda->MouseClick(x,y)){
				//全札の選択状態をリセットする
				this->AllFudaFunc(this->m_MyFuda,FUNC_RESET);
				this->AllFudaFunc(this->m_BaFuda,FUNC_RESET);
				//獲得場札を格納
				if(this->HouseBF(TRUE,fuda->GetID()/FUDACNT)){
					this->m_uiCutNo = CUT_TPUT;//手札移動カットへ
					rtnFlg = TRUE;//スレッド再開
				}else{
					this->m_uiCutNo = CUT_BSELECT1;//場札選択カットへ
					this->Play();
					Mediator::GetInstance()->Repaint();
					rtnFlg = FALSE;//スレッドはそのまま
				}
				//獲得手札を格納
				this->m_GetTFuda = fuda;
				this->m_MyFuda.erase(it);
				return rtnFlg;
			}
		}
	}else if(this->m_uiCutNo==CUT_BSELECT1 || this->m_uiCutNo==CUT_BSELECT2){//場札選択時
		Fuda** bFuda0 = (this->m_uiCutNo==CUT_BSELECT1)?&this->m_GetBFuda1[0]:&this->m_GetBFuda2[0];
		Fuda** bFuda1 = (this->m_uiCutNo==CUT_BSELECT1)?&this->m_GetBFuda1[1]:&this->m_GetBFuda2[1];
		BOOL teFlg = (this->m_uiCutNo==CUT_BSELECT1)?TRUE:FALSE;

		if((*bFuda0)->MouseClick(x,y)){
			rtnFlg = this->ReturnBF(teFlg,*bFuda1,1);
		}else if((*bFuda1)->MouseClick(x,y)){
			rtnFlg = this->ReturnBF(teFlg,*bFuda0,0);
			*bFuda0 = *bFuda1;
		}
		if(rtnFlg){
			(*bFuda0)->ResetSelect();
			(*bFuda1) = NULL;
			this->AllFudaFunc(this->m_BaFuda,FUNC_RESET);
			this->m_uiCutNo = (this->m_uiCutNo==CUT_BSELECT1)?CUT_TPUT:CUT_DPUT;//手札or台札移動カットへ
		}

	}else if(this->m_uiCutNo==CUT_KOIKOI && this->IsMyTurn()){//こいこい選択時
		if(this->m_slHai->MouseClick(x,y)){//はい
			rtnFlg = TRUE;
			this->m_uiCutNo=CUT_TSELECT;//手札選択カットへ
			this->m_uiTurnNo++;//次のターンへ
		}else if(this->m_slIie->MouseClick(x,y)){//いいえ
			rtnFlg = TRUE;
			this->m_uiCutNo=CUT_RESULT;//結果カットへ
		}
		this->m_slHai->Reset();
		this->m_slIie->Reset();
	}else if(this->m_uiCutNo==CUT_RESULT){//結果表示
		if(this->m_square->MouseClick(x,y)){
			this->Reset();
			Mediator* md = Mediator::GetInstance();
			if(md->NxtKyoku(TRUE) == md->MONCNT){//ゲーム終了
				//最終シーンへ
				md->ChangeScene(LastScene::GetInstance());
			}else{
				this->Init();
			}
			rtnFlg = TRUE;
		}
	}
	return rtnFlg;
}


// 札を動かす
void PlayScene::Move(void){
	if(this->m_uiSceneNo == SCENE_DEAL) this->Deal();//配札
	else if(this->m_uiSceneNo == SCENE_PLAY){//対戦
		//札選択時、スレッド一時停止
		if(!this->Stop()){
			if(!this->Play()){
				Mediator* md = Mediator::GetInstance();
				md->Repaint();
				md->RunThread(FALSE);
				//ロールオーバー確認
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(md->m_hWnd,&pt);
				this->MouseMove(pt.x,pt.y);
				md->Repaint();
				SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
			}
		}
	}
}

// 描画
void PlayScene::Paint(void){
	Mediator* md = Mediator::GetInstance();
	int i;

	//裏札(台札0＋相手札1~8)
	for(i=0;i<TEFUDACNT+1;i++) this->m_UFuda[i].Paint(md->m_gOffScreen);
	//場札
	this->AllFudaFunc(this->m_BaFuda,FUNC_PAINT);
	//月
	this->m_siMonth->Paint(md->m_gOffScreen);
	//氏名
	this->m_siCom->Paint(md->m_gOffScreen);
	this->m_siYou->Paint(md->m_gOffScreen);
	//得点
	md->PaintStrImage(this->m_siScore,md->m_gOffScreen);
	//文
	for(i=0;i<2;i++) this->m_siMon[i].Paint(md->m_gOffScreen);
	//役一覧
	this->m_ylComYaku.Paint(md->m_gOffScreen);
	this->m_ylMyYaku.Paint(md->m_gOffScreen);

	//配札シーン
	if(this->m_uiSceneNo == SCENE_DEAL){
		//裏札(手札9~17＋場札18~21)
		for(i=TEFUDACNT+1;i<UFUDACNT;i++) this->m_UFuda[i].Paint(md->m_gOffScreen);
	}else{//対戦シーン

		//手札
		this->AllFudaFunc(this->m_MyFuda,FUNC_PAINT);
		//相手札※デバッグ用
		if(md->m_bDebugFlg) this->AllFudaFunc(this->m_ComFuda,FUNC_PAINT);
		//獲得済札
		for(i=0;i<FUDAKIND;i++){
			this->AllFudaFunc(this->m_GMyFuda[i],FUNC_PAINT);
			this->AllFudaFunc(this->m_GComFuda[i],FUNC_PAINT);
		}

		//獲得札(レイヤー順に描画する)
		try{
			std::vector<int>::reverse_iterator rit = this->m_vLayer.rbegin();
			while(rit != this->m_vLayer.rend()){
				int layer = *rit;
				switch(layer){
					case CUT_B1GET://獲得場札1
						for(i=0;i<FUDACNT-1;i++) if(this->m_GetBFuda1[i]) this->m_GetBFuda1[i]->Paint(md->m_gOffScreen);
						break;
					case CUT_B2GET://獲得場札2
						for(i=0;i<FUDACNT-1;i++) if(this->m_GetBFuda2[i]) this->m_GetBFuda2[i]->Paint(md->m_gOffScreen);
						break;
					case CUT_TGET://獲得手札
						if(this->m_GetTFuda) this->m_GetTFuda->Paint(md->m_gOffScreen);
						break;
					case CUT_DGET://獲得台札
						if(this->m_GetDFuda) this->m_GetDFuda->Paint(md->m_gOffScreen);
						break;
				}
				rit++;
			}
		}catch(...){
			OutputDebugString(L"Paint");
		}
		//中背景
		this->m_square->Paint(md->m_gOffScreen);
		//こいこい選択
		if(this->m_uiCutNo == CUT_KOIKOI && this->IsMyTurn()){
			this->m_slHai->Paint(md->m_gOffScreen);
			this->m_slIie->Paint(md->m_gOffScreen);
		}
	}

}


//達成した約の文数を取得
UINT PlayScene::GetAcheiveScore(int bit)
{
	Mediator* md = Mediator::GetInstance();
	Strategy* st = Strategy::GetInstance();

	//親権
	if(!bit) return md->SCORE_OYAKEN;

	int kind = 0;
	int size = 0;
	int order =md->ExtBitOrder(bit,md->YAKUCNT);
	UINT score = md->m_uiScore[order];
	BOOL myFlg = (this->m_uiWin==WIN_ME)?TRUE:FALSE;
	//タネ、タン、カス→枚数分を加算
	switch(order){
		case st->YAKU_TANE:
			kind = md->KIND_TANE;
			break;
		case st->YAKU_TAN:
			kind = md->KIND_TAN;
			break;
		case st->YAKU_KASU:
			kind = md->KIND_KASU;
			if(st->IsBakeFuda(myFlg)) size = 1;
			break;
	}
	if(kind){
		size += (myFlg)?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
		score += (order == st->YAKU_KASU)?size-10:size-5;
	}

	return score;
}


//達成役の合計スコアーを追加
void PlayScene::AddScore(int score)
{
	Mediator* md = Mediator::GetInstance();
	BOOL myFlg = (this->m_uiWin==WIN_COM)?FALSE:(this->m_uiWin==WIN_ME)?TRUE:md->GetOyaFlg();
	//加算
	md->AddScore(score,myFlg);
	//取得
	Point pt = md->GetScore();
	//格納
	this->m_siScore.clear();
	md->HouseScore(pt.X,this->m_siScore,this->m_ptCScore);
	md->HouseScore(pt.Y,this->m_siScore,this->m_ptMScore);
}

//ターンNo
UINT PlayScene::GetTurnNo(void)
{
	return this->m_uiTurnNo;
}
