#include "StdAfx.h"
#include "Character.h"

UINT Character::uiCID = 0;

Character::Character(int width, int height)
:XImage(width,height)
{
	this->m_bExistFlg = FALSE;
	this->m_bInvisibleFlg = FALSE;
	this->m_dScale = 1.0;
	this->_x = 0;
	this->_y = 0;
	this->_width = width;
	this->_height = height;
	this->m_uiCID = ++Character::uiCID;
}

Character::~Character(void)
{
}

//リソースから画像をロード
bool Character::Load(UINT pResourceID, LPCTSTR pResourseType, HMODULE hInstance)
{
	bool rtnFlg = XImage::Load(pResourceID,pResourseType,hInstance);
	this->_width = XImage::GetWidth();
	this->_height = XImage::GetHeight();

	return rtnFlg;
}

//存在フラグ
BOOL Character::GetExistFlg(void)
{ 
	return this->m_bExistFlg;
}


//特殊描画
void Character::ExtraPaint(Graphics* g,float red,float green,float blue,float alpha)
{
	if(this->m_bExistFlg){
		ColorMatrix cmat = {
			red, 0.0f, 0.0f, 0.0f, 0.0f,   // Red
			0.0f, green, 0.0f, 0.0f, 0.0f,   // Green
			0.0f, 0.0f, blue, 0.0f, 0.0f,   // Blue
			0.0f, 0.0f, 0.0f, alpha, 0.0f,   // Alpha
			0.0f, 0.0f, 0.0f, 0.0f, 1.0f    // must be 1
		};
		ImageAttributes attr;
		attr.SetColorMatrix(&cmat,ColorMatrixFlagsDefault,ColorAdjustTypeBitmap);
		g->DrawImage(this->GetBitmap(),
			Rect(this->_x, this->_y, this->GetWidth(), this->GetHeight()),
			0, 0, this->GetWidth(), this->GetHeight(),
			UnitPixel, &attr, NULL, NULL);
	}
}

//ブレンド描画
void Character::BlendPaint(void)
{
	if(this->m_bExistFlg){
		Mediator::GetInstance()->BlendImages(this->_x,this->_y,this->GetBitmap());
	}
}


// 描画
void Character::Paint(Graphics* g)
{
	if(this->m_bExistFlg){
		if(this->m_bInvisibleFlg){//不可視状態
			this->BlendPaint();
		}else{
			g->DrawImage(this->GetBitmap(),this->_x,this->_y,this->GetWidth(),this->GetHeight());
		}
	}

}


// 座標設定
void Character::SetPos(Point pt)
{
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
	this->_x = pt.X;
	this->_y = pt.Y;
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//座標＆サイズ設定
void Character::SetPos(Point pt,double scale)
{
	this->SetPos(pt);
	this->m_dScale = scale;
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

// 座標取得
Point Character::GetPos(void)
{
	return Point(this->_x,this->_y);
}

//幅
int Character::GetWidth(void)
{
//	return static_cast<int>(XImage::GetWidth()*this->m_dScale);
	return static_cast<int>(this->_width*this->m_dScale);
}

//高さ
int Character::GetHeight(void)
{
//	return static_cast<int>(XImage::GetHeight()*this->m_dScale);
	return static_cast<int>(this->_height*this->m_dScale);
}


//初期化
void Character::Init(void)
{
	this->m_bExistFlg = TRUE;

	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

// 初期化
void Character::Init(UINT _x, UINT _y)
{
	this->_x = _x;
	this->_y = _y;

	this->Init();
}

// 初期化
void Character::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}


//中央に配置
void Character::Centering(UINT _x,UINT _y,UINT width,UINT height){

	if(!width) width = Mediator::WND_WIDTH;
	if(!height) height = Mediator::WND_HEIGHT;
	if(_x == NULL) _x = (width - this->GetWidth())/2;
	if(_y == NULL) _y = (height - this->GetHeight())/2;

	this->Init(_x,_y);
}

// リセット
void Character::Reset(void)
{
	if(this->m_bExistFlg){
		this->m_bExistFlg = FALSE;
		this->SetScale(1.0);
		this->SetupInvisible(FALSE);
		Mediator::GetInstance()->SetupRepaint(this->returnRect());
	}
}

//画像範囲を返す
RECT Character::returnRect(void)
{
	RECT rect;
	::SetRect(&rect,this->_x,this->_y,this->_x + this->GetWidth(),this->_y + this->GetHeight());
	return rect;
}



// 同位置に置き換える
void Character::Trace(Character* lpch)
{
	lpch->Init(this->_x,this->_y);
	this->Reset();
}

//可視状態を切り替え
void Character::SetupInvisible(BOOL flag)
{
	this->m_bInvisibleFlg = flag;
}

//拡大縮小指数
void Character::SetScale(double scale)
{
	this->m_dScale = scale;
}

//RGBA値を設定
void Character::SetRGBA(float red,float green,float blue,float alpha)
{
	this->m_fRGBA[0] = red;
	this->m_fRGBA[1] = green;
	this->m_fRGBA[2] = blue;
	this->m_fRGBA[3] = alpha;
}
