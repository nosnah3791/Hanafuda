#include "StdAfx.h"
#include "MouseListner.h"

MouseListner::MouseListner():m_bRolloverFlg(FALSE)
{
}

MouseListner::~MouseListner(void)
{
}

// 指定座標の画像矩形内有無を判定
BOOL MouseListner::InRect(double x, double y)
{
	if(this->rect.left <= x && x < this->rect.right && this->rect.top <= y && y < this->rect.bottom)
		return TRUE;
	return FALSE;
}


//マウス移動時の処理
BOOL MouseListner::MouseMove(double x, double y)
{
	if(!this->m_bRolloverFlg) return this->RollOver(x,y);
	else return this->RollOut(x,y);
}

//ロールオーバー時の処理
BOOL MouseListner::RollOver(double x, double y)
{
	if(!this->m_bRolloverFlg){
		if(this->InRect(x,y)){
			this->m_bRolloverFlg = TRUE;
			this->OnRollOver(x,y);
			return TRUE;
		}
	}
	return FALSE;
}

//ロールアウト時の処理
BOOL MouseListner::RollOut(double x, double y)
{
	if(this->m_bRolloverFlg){
		if(!this->InRect(x,y)){
			this->m_bRolloverFlg = FALSE;
			this->OnRollOut(x,y);
			return TRUE;
		}
	}
	return FALSE;
}

//マウスクリック時の処理
BOOL MouseListner::MouseClick(double x, double y)
{
	if(this->m_bRolloverFlg){
		if(this->InRect(x,y)){
			this->OnMouseClick(x,y);
			this->m_bRolloverFlg = FALSE;
			return TRUE;
		}
	}
	return FALSE;
}

