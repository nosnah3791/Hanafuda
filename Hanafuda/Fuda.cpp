#include "StdAfx.h"
#include "Fuda.h"


Fuda::Fuda(void){}

Fuda::~Fuda(void){}

// �`��
void Fuda::Paint(Graphics* g)
{
	//�摜�̕`��
	Character::Paint(g);
	//�O�g�̕`��
	if(this->m_bRolloverFlg){//�I����
		//���F�ŕ`��
		g->DrawRectangle(&Pen(Color(0,255, 255), 3),this->_x+1,this->_y+1,this->GetWidth()-4,this->GetHeight()-4);
	}else if(this->m_bSelectableFlg){//�I���\��
		//���F�ŕ`��
		g->DrawRectangle(&Pen(Color(255,255,0), 3),this->_x+2,this->_y+2,this->GetWidth()-5,this->GetHeight()-5);
		Mediator::GetInstance()->SetupRepaint(this->returnRect());
	}
}

// ���W�ݒ�
void Fuda::SetPos(Point pt)
{
	Character::SetPos(pt);
	this->rect = this->returnRect();
}

// ���W���T�C�Y�ݒ�
void Fuda::SetPos(Point pt,double scale)
{
	Character::SetPos(pt,scale);
	this->rect = this->returnRect();
}

//������
void Fuda::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// ������
void Fuda::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	
	this->rect = this->returnRect();
}

// ������
void Fuda::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//���Z�b�g
void Fuda::Reset(void)
{
	this->ResetSelect();
	Character::Reset();
}

// ���ʒu�ɒu��������
void Fuda::Trace(Character* lpch)
{
	Fuda* lpFuda = (Fuda*)lpch;
	lpFuda->Init(this->_x,this->_y);
	this->Reset();
}

//�����ɔz�u
void Fuda::Centering(UINT _x,UINT _y){

	Character::Centering(_x,_y);
	this->rect = this->returnRect();
}


//���[���I�[�o�[���̏���
void Fuda::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,TRUE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//���[���A�E�g���̏���
void Fuda::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,FALSE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//�}�E�X�N���b�N���̏���
void Fuda::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//�J�[�\����߂�
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//�v���p�e�B
void Fuda::SetProperty(UINT id,UINT kind,UINT pResourceID,HMODULE hInstance)
{
	this->m_uiID = id;
	this->m_uiKind = kind;
	this->Load(pResourceID,_T("PNG"),hInstance);
}



//�}�E�X�ړ����̏���
BOOL Fuda::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//�}�E�X�N���b�N���̏���
BOOL Fuda::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//�I����Ԃ��w��
void Fuda::SetupSelected(BOOL onFlg)
{
	this->m_bRolloverFlg = onFlg;
	//�ĕ`��̈���Z�b�g
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//�I���\��Ԃ��w��
void Fuda::SetupSelectable(BOOL onFlg){
	this->m_bSelectableFlg = onFlg;
	//�ĕ`��̈���Z�b�g
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//��ԃt���O�����Z�b�g
void Fuda::ResetSelect(void)
{
	this->SetupSelected(FALSE);
	this->SetupSelectable(FALSE);
	Character::SetupInvisible(FALSE);
}

//ID���擾
UINT Fuda::GetID(void)
{
	return this->m_uiID;
}

//��ʂ��擾
UINT Fuda::GetKind(void)
{
	return this->m_uiKind;
}

