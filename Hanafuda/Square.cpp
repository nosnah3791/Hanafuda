#include "StdAfx.h"
#include "Square.h"
#include "Strategy.h"
#include "PlayScene.h"


Square::Square(int width,int height):Character(width,height),
									 m_ptYF(45,121),
									 m_ptYName(30,30),
									 m_ptScore(384,30),
									 m_ptSumScore(384,271),
									 m_ptGoukei(261,271),
									 m_ptKoikoi(79,100)
{
	//スクリーン
	this->m_gScreen = Graphics::FromImage(this->GetBitmap());
	//描画領域
	this->SetArea(0,0,width,height);
	//vector領域確保
	this->m_AFuda.reserve(25);
	this->m_siYname.reserve(10);
	this->m_siScore.reserve(20);
	this->m_siMon.reserve(10);
	//ロールオーバーフラグ
	this->m_bRolloverFlg = FALSE;
}

Square::~Square(void)
{
}

//札ベクトルを一括処理
BOOL Square::AllFudaFunc(std::vector<Fuda> &vFuda,int func,double param,BOOL flag)
{

	std::vector<Fuda>::iterator it = vFuda.begin();
	BOOL rtnFlg = FALSE;
	int i=0;
	for(it=vFuda.begin();it != vFuda.end();it++){
		Fuda* fuda = &*it;
		if(fuda){
			switch(func){
				case FUNC_PAINT://描画
					fuda->Paint(this->m_gScreen);
					break;
				case FUNC_SORT://札並び替え
					this->SortFuda(fuda,i);
					break;
			}
		}
		i++;
	}

	return rtnFlg;
}

//札を並び替える
void Square::SortFuda(Fuda* fuda,int i)
{
	int size = this->m_AFuda.size();
	int maxSize = YFUDACNT;
	fuda->SetScale(1.0);
	int dx = fuda->GetWidth();
	int nX,nY;

	if(size <= maxSize){ 
		//センタリング
		nX = (int)(this->m_ptYF.X + dx*(i+(double)(maxSize - size)/2));
	}
	//maxサイズ超えたら、詰めて表示
	else nX = (int)(this->m_ptYF.X + i*dx*(double)(maxSize-1)/(size-1));
	nY = this->m_ptYF.Y;

	fuda->SetPos(Point(nX,nY));

}

//札を並べる
void Square::Sort(void)
{
	this->AllFudaFunc(this->m_AFuda,FUNC_SORT);
}

// 描画
void Square::Paint(Gdiplus::Graphics *g)
{
	if(this->GetExistFlg()){
		Mediator* md = Mediator::GetInstance();
		PlayScene* ps = PlayScene::GetInstance();

		//背景
		this->m_gScreen->Clear(Color::Transparent);
		this->m_gScreen->FillRectangle(&SolidBrush(Color(0,0,0)),this->m_iX,this->m_iY,this->m_iWidth,this->m_iHeight);

		if(this->m_uiMode == MODE_ACHIEVE){//役達成モード
			//札
			this->AllFudaFunc(this->m_AFuda,FUNC_PAINT);
			//役名
			this->m_siYname.back().Paint(this->m_gScreen);
			//文数
			md->PaintStrImage(this->m_siScore,this->m_gScreen);
			//文
			this->m_siMon.back().Paint(this->m_gScreen);
		}else if(this->m_uiMode == MODE_KOIKOI){//こいこいモード
			this->m_siKoikoi->Paint(this->m_gScreen);
		}else if(this->m_uiMode == MODE_RESULT){//結果モード
			//役名
			md->PaintStrImage(this->m_siYname,this->m_gScreen);
			//文数
			md->PaintStrImage(this->m_siScore,this->m_gScreen);
			//文
			md->PaintStrImage(this->m_siMon,this->m_gScreen);
			//合計
			this->m_siGoukei->Paint(this->m_gScreen);
		}
	
		Character::ExtraPaint(g,this->m_fRGBA[0],this->m_fRGBA[1],this->m_fRGBA[2],this->m_fRGBA[3]);

	}
}

//ロールオーバー時の処理
void Square::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,TRUE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//ロールアウト時の処理
void Square::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,FALSE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//マウスクリック時の処理
void Square::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//カーソルを戻す
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//マウス移動時の処理
BOOL Square::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//マウスクリック時の処理
BOOL Square::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//初期化
void Square::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// 初期化
void Square::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	
	this->rect = this->returnRect();
}

//初期化
void Square::Init(Point pt)
{
	//モード
	this->m_uiMode = 0;

	this->Init(pt.X,pt.Y);
}

// リセット
void Square::Reset(void)
{
	if(this->GetExistFlg()){
		//描画領域
		this->SetArea(0,0,this->GetWidth(),this->GetHeight());
		//達成役ビット
		this->m_iAchieveBit = 0;
		//役札削除
		this->m_AFuda.clear();
		//役名削除
		this->m_siYname.clear();
		//スコアー削除
		this->m_siScore.clear();
		//文削除
		this->m_siMon.clear();

		Character::Reset();
	}
}

//モード変更
void Square::ChangeMode(int mode,int bit)
{
	Mediator* md = Mediator::GetInstance();
	PlayScene* ps = PlayScene::GetInstance();

	this->m_uiMode = mode;
	this->m_iAchieveBit = bit;
	if(this->m_uiMode == MODE_ACHIEVE){//役達成モード
		//役名
		int i = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT);
		this->m_siYname.push_back(md->m_siYName[i]);
		this->m_siYname.back().Init(this->m_ptYName);
		//スコアー
		int score = ps->GetAcheiveScore(this->m_iAchieveBit);
		md->HouseScore(score,this->m_siScore,this->m_ptSumScore,TRUE);
		//文
		this->m_siMon.push_back(md->m_siMonW);
		this->m_siMon.back().Init(this->m_ptSumScore);
	}else if(this->m_uiMode == MODE_KOIKOI){//こいこいモード
		if(ps->IsMyTurn()){//自分の番
			this->m_siKoikoi = &md->m_siPlS[ps->STR_KOIKOI1];
			this->m_siKoikoi->Init(this->m_ptKoikoi);
		}else{//相手の番
			this->m_siKoikoi = &md->m_siPlS[ps->STR_KOIKOI2];
			this->m_siKoikoi->Centering(NULL,NULL,this->GetWidth(),this->GetHeight());
		}
	}else if(this->m_uiMode == MODE_RESULT){//結果モード
		int i=0;
		int sum=0;
		double scale = 1.0;

		//親権
		if(!this->m_iAchieveBit){
			md->m_siOyaken.Init(this->m_ptYName);
			this->m_siYname.push_back(md->m_siOyaken);
			sum += ps->GetAcheiveScore(this->m_iAchieveBit);
		}else{
			//役の個数が6つ以上なら、縮小表示
			int j;
			for(j=0;j<4;j++){
				if(md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT,5+j)>=0) scale -= 0.1;
				else break;
			}
		}
			
		//格納
		while(this->m_iAchieveBit){
			int order = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT,i);
			if(order < 0) break;
			
			//役名
			this->m_siYname.push_back(md->m_siYName[order]);
			this->m_siYname.back().SetScale(scale);
			int dY = (this->m_siYname.back().GetHeight()+3)*i;
			this->m_siYname.back().Init(this->m_ptYName.X,this->m_ptYName.Y + dY);
			//文数
			int score = ps->GetAcheiveScore((1 << order));
			Point ptScore(this->m_ptScore.X,this->m_ptScore.Y + dY);
			md->HouseScore(score,this->m_siScore,ptScore,TRUE,scale);
			sum += score;
			//「文」
			this->m_siMon.push_back(md->m_siMonW);
			this->m_siMon.back().SetScale(scale);
			this->m_siMon.back().Init(this->m_ptScore.X,this->m_ptScore.Y + dY);
			i++;
		}
		//「合計」
		this->m_siGoukei = &md->m_siGoukeiW;
		this->m_siGoukei->Init(this->m_ptGoukei);
		ps->AddScore(sum);
		//合計文数
		md->HouseScore(sum,this->m_siScore,this->m_ptSumScore,TRUE);
		//「文」
		this->m_siMon.push_back(md->m_siMonW);
		this->m_siMon.back().Init(this->m_ptSumScore);
	}

}

//描画範囲を設定
void Square::SetArea(int x,int y,int width,int height)
{
	this->m_iX = x;
	this->m_iY = y;
	this->m_iWidth = width;
	this->m_iHeight = height;
}

//描画位置を中央に固定かつ、領域を指定倍に縮小する
void Square::ReduceArea(float scale)
{
	int x = static_cast<int>(this->GetWidth()*(1-scale)/2);
	int y = static_cast<int>(this->GetHeight()*(1-scale)/2);
	int width = static_cast<int>(this->GetWidth()*scale);
	int height = static_cast<int>(this->GetHeight()*scale);
	this->SetArea(x,y,width,height);
}

