#include "StdAfx.h"
#include "YakuList.h"

YakuList::YakuList(void)
{
}

YakuList::~YakuList(void)
{
}

//�`��
void YakuList::Paint(Graphics *g){

	int i;
	int bit = 1;
	for(i=0;i<Mediator::GetInstance()->YAKUCNT;i++){
		if(this->m_uiAchieveBit & (bit << i)){
			//�B��
			g->FillRectangle(&SolidBrush(Color(153,102,0)),_x+(i%4)*YL_WIDTH,_y+(i/4)*YL_HEIGHT,YL_WIDTH,YL_HEIGHT);
		}else if(this->m_uiReachBit & (bit << i)){
			//���[�`
			g->FillRectangle(&SolidBrush(Color(204,153,102)),_x+(i%4)*YL_WIDTH,_y+(i/4)*YL_HEIGHT,YL_WIDTH,YL_HEIGHT);
		}
	}
	Character::Paint(g);
}

//���Z�b�g
void YakuList::Reset(void)
{
	this->m_uiAchieveBit = this->m_uiReachBit = 0;
	Character::Reset();
}

//�w��ʒu�̃��[�`�r�b�g��ύX
void YakuList::SetReachBit(UINT no,BOOL flg)
{
	if(flg) this->m_uiReachBit |= 1 << no;
	else this->m_uiReachBit &= ~(1 << no);
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//�w��ʒu�̃��[�`�r�b�g���擾
BOOL YakuList::GetReachBit(UINT no){
	return (this->m_uiReachBit >> no) & 1;
}

//�w��ʒu�̓��B�r�b�g��ύX
void YakuList::SetAchieveBit(UINT no,BOOL flg)
{
	if(flg) this->m_uiAchieveBit |= 1 << no;
	else this->m_uiAchieveBit &= ~(1 << no);
	Mediator::GetInstance()->SetupRepaint(this->returnRect());
}

//�w��ʒu�̓��B�r�b�g���擾
BOOL YakuList::GetAchieveBit(UINT no){
	return (this->m_uiAchieveBit >> no) & 1;
}

