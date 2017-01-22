#include "StdAfx.h"
#include "Layer.h"


Layer::Layer(int width,int height):Character(width,height),
								   m_ptCom(146,0),m_ptYou(292,0),
								   m_ptMonth(0,35),
								   m_ptCScore(210,35),m_ptMScore(356,35)
{
	//スクリーン
	this->m_gScreen = Graphics::FromImage(this->GetBitmap());

	Mediator* md = Mediator::GetInstance();
	//文字列画像
	this->m_siCom = &md->m_siCom;//COM
	this->m_siYou = &md->m_siYou;//あなた
	int i = 0;
	for(i=0;i<MONCNT;i++){
		this->m_siMonth[i] = &md->m_siMonth[i];//月
	}
	for(i=0;i<MONCNT+1;i++){
		this->m_siMon[i][0] = this->m_siMon[i][1] = md->m_siMonB;//文
	}
	this->m_siScore.reserve(50);//スコアー
	this->m_siGoukei = &md->m_siGoukeiB;//合計
}

Layer::~Layer(void)
{
}

//初期化
void Layer::Init(void)
{
	Mediator* md = Mediator::GetInstance();

	this->m_siCom->Init(this->m_ptCom);//COM
	this->m_siYou->Init(this->m_ptYou);//あなた
	int i = 0;
	for(i=0;i<MONCNT+1;i++){
		Point ptMonth(this->m_ptMonth.X,this->m_ptMonth.Y + DIF_Y*i);
		Point ptCScore(this->m_ptCScore.X,this->m_ptCScore.Y + DIF_Y*i);
		Point ptMScore(this->m_ptMScore.X,this->m_ptMScore.Y + DIF_Y*i);

		if(i==MONCNT){
			this->m_siGoukei->Init(ptMonth);//合計
			//スコアー
			int cScore = md->GetScore().X;
			int mScore = md->GetScore().Y;
			md->HouseScore(cScore,this->m_siScore,ptCScore);//COM
			md->HouseScore(mScore,this->m_siScore,ptMScore);//あなた
			//メッセージ
			this->m_siMsg = (mScore>cScore)?&md->m_siLaS[MSG_ANK]:(mScore<cScore)?&md->m_siLaS[MSG_WAK]:&md->m_siLaS[MSG_DRAW];
		}else{
			this->m_siMonth[i]->Init(ptMonth);//月
			md->HouseScore(md->m_uiScoreTable[i][0],this->m_siScore,ptCScore);//COM
			md->HouseScore(md->m_uiScoreTable[i][1],this->m_siScore,ptMScore);//あなた
		}
		//文
		this->m_siMon[i][0].Init(ptCScore);//COM
		this->m_siMon[i][1].Init(ptMScore);//あなた
	}
	//メッセージ
	this->m_siMsg->Centering(NULL,Y_MSG,this->GetWidth(),this->GetHeight());

	Character::Init();
}

// 初期化
void Layer::Init(UINT _x, UINT _y)
{
	this->Init();
	Character::Init(_x,_y);
}

//初期化
void Layer::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//中央に配置
void Layer::Centering(UINT _x,UINT _y,UINT width,UINT height)
{
	this->Init();
	Character::Centering(_x,_y);
}

// リセット
void Layer::Reset(void)
{
	//氏名
	this->m_siCom->Reset();//COM
	this->m_siYou->Reset();//あなた
	for(int i=0;i<MONCNT+1;i++){
		if(i==MONCNT) this->m_siGoukei->Reset();//合計
		else this->m_siMonth[i]->Reset();//月
		//文
		this->m_siMon[i][0].Reset();//COM
		this->m_siMon[i][1].Reset();//あなた
	}
	//スコアー
	this->m_siScore.clear();
	//メッセージ
	this->m_siMsg->Reset();

	Character::Reset();
}

// 描画
void Layer::Paint(Graphics* g)
{
	if(this->GetExistFlg()){
		Mediator* md = Mediator::GetInstance();

		//背景
		this->m_gScreen->Clear(Color::Transparent);
		//氏名
		this->m_siCom->Paint(this->m_gScreen);//COM
		this->m_siYou->Paint(this->m_gScreen);//あなた
		for(int i=0;i<MONCNT+1;i++){
			if(i==MONCNT) this->m_siGoukei->Paint(this->m_gScreen);//合計
			else this->m_siMonth[i]->Paint(this->m_gScreen);//月
			//文
			this->m_siMon[i][0].Paint(this->m_gScreen);//COM
			this->m_siMon[i][1].Paint(this->m_gScreen);//あなた
		}
		//スコアー
		md->PaintStrImage(this->m_siScore,this->m_gScreen);
		
		//メッセージ
		this->m_siMsg->Paint(this->m_gScreen);
	}

	Character::ExtraPaint(g,this->m_fRGBA[0],this->m_fRGBA[1],this->m_fRGBA[2],this->m_fRGBA[3]);
}
