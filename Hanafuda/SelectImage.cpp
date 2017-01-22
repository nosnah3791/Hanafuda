#include "StdAfx.h"
#include "SelectImage.h"

SelectImage::SelectImage(void)
{
}

SelectImage::~SelectImage(void)
{
}

// �`��
void SelectImage::Paint(Graphics* g)
{
	//�I����
	if(this->m_bRolloverFlg) this->ExtraPaint(g,1.0,1.0,0.0,1.0);//���F
	else Character::Paint(g);
}

//���[���I�[�o�[���̏���
void SelectImage::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,TRUE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//���[���A�E�g���̏���
void SelectImage::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,FALSE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//�}�E�X�N���b�N���̏���
void SelectImage::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//�J�[�\����߂�
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//�}�E�X�ړ����̏���
BOOL SelectImage::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//�}�E�X�N���b�N���̏���
BOOL SelectImage::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//������
void SelectImage::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// ������
void SelectImage::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	this->rect = this->returnRect();
}

// ������
void SelectImage::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//���Z�b�g
void SelectImage::Reset(void)
{
	this->m_bRolloverFlg = FALSE;
	Character::Reset();
}
