#include "StdAfx.h"
#include "MouseListner.h"

MouseListner::MouseListner():m_bRolloverFlg(FALSE)
{
}

MouseListner::~MouseListner(void)
{
}

// �w����W�̉摜��`���L���𔻒�
BOOL MouseListner::InRect(double x, double y)
{
	if(this->rect.left <= x && x < this->rect.right && this->rect.top <= y && y < this->rect.bottom)
		return TRUE;
	return FALSE;
}


//�}�E�X�ړ����̏���
BOOL MouseListner::MouseMove(double x, double y)
{
	if(!this->m_bRolloverFlg) return this->RollOver(x,y);
	else return this->RollOut(x,y);
}

//���[���I�[�o�[���̏���
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

//���[���A�E�g���̏���
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

//�}�E�X�N���b�N���̏���
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

