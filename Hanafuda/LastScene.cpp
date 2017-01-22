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

//������
void LastScene::Init(void)
{
	this->m_uiFrameNo = 0;
}

//���Z�b�g
void LastScene::Reset(void)
{
	Mediator* md = Mediator::GetInstance();
	this->m_layer->Reset();
	//���ʉ�
	SendMessage(md->m_hWnd,WM_SOUND,FALSE,NULL);
	
}

// �}�E�X�ړ����̏���
BOOL LastScene::MouseMove(double x, double y)
{
	return FALSE;
}

// �}�E�X�N���b�N���̏���
BOOL LastScene::MouseClick(double x, double y)
{
	return FALSE;
}

//�Đ�
BOOL LastScene::Play(void)
{
	Mediator* md = Mediator::GetInstance();
	if(!this->m_uiFrameNo){
		this->m_layer->Centering(NULL,NULL);
		//���ʉ�
		SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
	}

	if(++this->m_uiFrameNo <= FADECNT){
		this->m_layer->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
		//�ĕ`��
		md->SetupRepaint(this->m_layer->returnRect());
		return TRUE;
	}
	return FALSE; 
}

//�X���b�h��]
void LastScene::Move(void)
{
	if(!this->Play()){
		Mediator* md = Mediator::GetInstance();
		md->Repaint();
		md->RunThread(FALSE);
	}
}

// �`��
void LastScene::Paint(void)
{
	this->m_layer->Paint(Mediator::GetInstance()->m_gOffScreen);
}

