#include "StdAfx.h"
#include "SelectImage.h"

SelectImage::SelectImage(void)
{
}

SelectImage::~SelectImage(void)
{
}

// 描画
void SelectImage::Paint(Graphics* g)
{
	//選択時
	if(this->m_bRolloverFlg) this->ExtraPaint(g,1.0,1.0,0.0,1.0);//黄色
	else Character::Paint(g);
}

//ロールオーバー時の処理
void SelectImage::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,TRUE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//ロールアウト時の処理
void SelectImage::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,FALSE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//マウスクリック時の処理
void SelectImage::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//カーソルを戻す
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//マウス移動時の処理
BOOL SelectImage::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//マウスクリック時の処理
BOOL SelectImage::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//初期化
void SelectImage::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// 初期化
void SelectImage::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	this->rect = this->returnRect();
}

// 初期化
void SelectImage::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//リセット
void SelectImage::Reset(void)
{
	this->m_bRolloverFlg = FALSE;
	Character::Reset();
}
