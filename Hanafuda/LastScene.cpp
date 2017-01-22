#include "StdAfx.h"
#include "LastScene.h"


LastScene::LastScene(void)
{
	this->m_layer = new Layer(438,555);
	this->m_sound = &Mediator::GetInstance()->m_sndEnding;
}

LastScene::~LastScene(void)
{
}

//初期化
void LastScene::Init(void)
{
	this->m_uiFrameNo = 0;
}

//リセット
void LastScene::Reset(void)
{
	Mediator* md = Mediator::GetInstance();
	this->m_layer->Reset();
	//効果音
	SendMessage(md->m_hWnd,WM_SOUND,FALSE,NULL);
	
}

// マウス移動時の処理
BOOL LastScene::MouseMove(double x, double y)
{
	return FALSE;
}

// マウスクリック時の処理
BOOL LastScene::MouseClick(double x, double y)
{
	return FALSE;
}

//再生
BOOL LastScene::Play(void)
{
	Mediator* md = Mediator::GetInstance();
	if(!this->m_uiFrameNo){
		this->m_layer->Centering(NULL,NULL);
		//効果音
		SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
	}

	if(++this->m_uiFrameNo <= FADECNT){
		this->m_layer->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
		//再描画
		md->SetupRepaint(this->m_layer->returnRect());
		return TRUE;
	}
	return FALSE; 
}

//スレッド回転
void LastScene::Move(void)
{
	if(!this->Play()){
		Mediator* md = Mediator::GetInstance();
		md->Repaint();
		md->RunThread(FALSE);
	}
}

// 描画
void LastScene::Paint(void)
{
	this->m_layer->Paint(Mediator::GetInstance()->m_gOffScreen);
}

