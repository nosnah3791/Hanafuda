#include "StdAfx.h"
#include "Fuda.h"


Fuda::Fuda(void){}

Fuda::~Fuda(void){}

// 描画
void Fuda::Paint(Graphics* g)
{
	//画像の描画
	Character::Paint(g);
	//外枠の描画
	if(this->m_bRolloverFlg){//選択時
		//水色で描画
		g->DrawRectangle(&Pen(Color(0,255, 255), 3),this->_x+1,this->_y+1,this->GetWidth()-4,this->GetHeight()-4);
	}else if(this->m_bSelectableFlg){//選択可能時
		//黄色で描画
		g->DrawRectangle(&Pen(Color(255,255,0), 3),this->_x+2,this->_y+2,this->GetWidth()-5,this->GetHeight()-5);
		Mediator::GetInstance()->SetupRepaint(this->returnRect());
	}
}

// 座標設定
void Fuda::SetPos(Point pt)
{
	Character::SetPos(pt);
	this->rect = this->returnRect();
}

// 座標＆サイズ設定
void Fuda::SetPos(Point pt,double scale)
{
	Character::SetPos(pt,scale);
	this->rect = this->returnRect();
}

//初期化
void Fuda::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// 初期化
void Fuda::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	
	this->rect = this->returnRect();
}

// 初期化
void Fuda::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//リセット
void Fuda::Reset(void)
{
	this->ResetSelect();
	Character::Reset();
}

// 同位置に置き換える
void Fuda::Trace(Character* lpch)
{
	Fuda* lpFuda = (Fuda*)lpch;
	lpFuda->Init(this->_x,this->_y);
	this->Reset();
}

//中央に配置
void Fuda::Centering(UINT _x,UINT _y){

	Character::Centering(_x,_y);
	this->rect = this->returnRect();
}


//ロールオーバー時の処理
void Fuda::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,TRUE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//ロールアウト時の処理
void Fuda::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//ロールオーバーIDを変更
	md->ChangeROID(this->m_uiCID,FALSE);
	//再描画領域をセット
	md->SetupRepaint(this->returnRect());
}

//マウスクリック時の処理
void Fuda::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//カーソルを戻す
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//プロパティ
void Fuda::SetProperty(UINT id,UINT kind,UINT pResourceID,HMODULE hInstance)
{
	this->m_uiID = id;
	this->m_uiKind = kind;
	this->Load(pResourceID,_T("PNG"),hInstance);
}



//マウス移動時の処理
BOOL Fuda::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//マウスクリック時の処理
BOOL Fuda::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//選択状態を指定
void Fuda::SetupSelected(BOOL onFlg)
{
	this->m_bRolloverFlg = onFlg;
	//再描画領域をセット
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//選択可能状態を指定
void Fuda::SetupSelectable(BOOL onFlg){
	this->m_bSelectableFlg = onFlg;
	//再描画領域をセット
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//状態フラグをリセット
void Fuda::ResetSelect(void)
{
	this->SetupSelected(FALSE);
	this->SetupSelectable(FALSE);
	Character::SetupInvisible(FALSE);
}

//IDを取得
UINT Fuda::GetID(void)
{
	return this->m_uiID;
}

//種別を取得
UINT Fuda::GetKind(void)
{
	return this->m_uiKind;
}

