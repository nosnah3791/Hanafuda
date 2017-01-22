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

//���\�[�X����摜�����[�h
bool Character::Load(UINT pResourceID, LPCTSTR pResourseType, HMODULE hInstance)
{
	bool rtnFlg = XImage::Load(pResourceID,pResourseType,hInstance);
	this->_width = XImage::GetWidth();
	this->_height = XImage::GetHeight();

	return rtnFlg;
}

//���݃t���O
BOOL Character::GetExistFlg(void)
{ 
	return this->m_bExistFlg;
}


//����`��
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

//�u�����h�`��
void Character::BlendPaint(void)
{
	if(this->m_bExistFlg){
		Mediator::GetInstance()->BlendImages(this->_x,this->_y,this->GetBitmap());
	}
}


// �`��
void Character::Paint(Graphics* g)
{
	if(this->m_bExistFlg){
		if(this->m_bInvisibleFlg){//�s�����
			this->BlendPaint();
		}else{
			g->DrawImage(this->GetBitmap(),this->_x,this->_y,this->GetWidth(),this->GetHeight());
		}
	}

}


// ���W�ݒ�
void Character::SetPos(Point pt)
{
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
	this->_x = pt.X;
	this->_y = pt.Y;
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//���W���T�C�Y�ݒ�
void Character::SetPos(Point pt,double scale)
{
	this->SetPos(pt);
	this->m_dScale = scale;
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

// ���W�擾
Point Character::GetPos(void)
{
	return Point(this->_x,this->_y);
}

//��
int Character::GetWidth(void)
{
//	return static_cast<int>(XImage::GetWidth()*this->m_dScale);
	return static_cast<int>(this->_width*this->m_dScale);
}

//����
int Character::GetHeight(void)
{
//	return static_cast<int>(XImage::GetHeight()*this->m_dScale);
	return static_cast<int>(this->_height*this->m_dScale);
}


//������
void Character::Init(void)
{
	this->m_bExistFlg = TRUE;

	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

// ������
void Character::Init(UINT _x, UINT _y)
{
	this->_x = _x;
	this->_y = _y;

	this->Init();
}

// ������
void Character::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}


//�����ɔz�u
void Character::Centering(UINT _x,UINT _y,UINT width,UINT height){

	if(!width) width = Mediator::WND_WIDTH;
	if(!height) height = Mediator::WND_HEIGHT;
	if(_x == NULL) _x = (width - this->GetWidth())/2;
	if(_y == NULL) _y = (height - this->GetHeight())/2;

	this->Init(_x,_y);
}

// ���Z�b�g
void Character::Reset(void)
{
	if(this->m_bExistFlg){
		this->m_bExistFlg = FALSE;
		this->SetScale(1.0);
		this->SetupInvisible(FALSE);
		Mediator::GetInstance()->SetupRepaint(this->returnRect());
	}
}

//�摜�͈͂�Ԃ�
RECT Character::returnRect(void)
{
	RECT rect;
	::SetRect(&rect,this->_x,this->_y,this->_x + this->GetWidth(),this->_y + this->GetHeight());
	return rect;
}



// ���ʒu�ɒu��������
void Character::Trace(Character* lpch)
{
	lpch->Init(this->_x,this->_y);
	this->Reset();
}

//����Ԃ�؂�ւ�
void Character::SetupInvisible(BOOL flag)
{
	this->m_bInvisibleFlg = flag;
}

//�g��k���w��
void Character::SetScale(double scale)
{
	this->m_dScale = scale;
}

//RGBA�l��ݒ�
void Character::SetRGBA(float red,float green,float blue,float alpha)
{
	this->m_fRGBA[0] = red;
	this->m_fRGBA[1] = green;
	this->m_fRGBA[2] = blue;
	this->m_fRGBA[3] = alpha;
}
