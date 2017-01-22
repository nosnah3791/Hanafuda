#include "StdAfx.h"
#include "ChooseScene.h"
#include "PlayScene.h"
#include "Random.h"


ChooseScene::ChooseScene(void): m_ptLU(232,200),m_ptRD(512,367)
{
}

ChooseScene::~ChooseScene(void)
{
}

//�e���ߒ��I
Point ChooseScene::Oyagime(void)
{

	//0�`11�ňقȂ�񐮐��𒊑I
	Random lRd,rRd;
	UINT lMon,rMon;
	lMon = lRd(Mediator::MONCNT);
	rMon = rRd(Mediator::MONCNT-1);
	if(lMon <= rMon) rMon++;
	
	return Point(lMon,rMon);
	//return Point(1,2);
}


//������
void ChooseScene::Init(void){
	Mediator* md = Mediator::GetInstance();
	this->m_LFuda = md->m_UraFuda;//���D
	this->m_RFuda = md->m_UraFuda;//�E�D
	this->m_siMsg = &md->m_siChS[STR_ERA];//���b�Z�[�W
	this->m_LFuda.Centering(X_LFUDA,NULL);
	this->m_RFuda.Centering(X_RFUDA,NULL);
	this->m_siMsg->Centering(NULL,Y_MSG);
}

//���Z�b�g
void ChooseScene::Reset(void){
	this->m_LFuda.Reset();
	this->m_RFuda.Reset();
	this->m_siMsg->Reset();
	if(this->m_siLMON) this->m_siLMON->Reset();
	if(this->m_siRMON) this->m_siRMON->Reset();
	if(this->m_siLWho) this->m_siLWho->Reset();
	if(this->m_siRWho) this->m_siRWho->Reset();
}

//�}�E�X�ړ����̏���
BOOL ChooseScene::MouseMove(double x,double y){
	if(this->m_LFuda.MouseMove(x,y)) return TRUE;
	if(this->m_RFuda.MouseMove(x,y)) return TRUE;
	return FALSE;
}

//�}�E�X�N���b�N���̏���
BOOL ChooseScene::MouseClick(double x,double y){
	int chFuda = 0;
	if(this->m_LFuda.MouseClick(x,y)) chFuda++;
	else if(this->m_RFuda.MouseClick(x,y)) chFuda--;
	if(chFuda != 0){//�D�I��
		Mediator* md = Mediator::GetInstance();
		//�D���I
		Point pt = this->Oyagime();
		//�D�߂���
		Fuda lFuda = this->m_LFuda;
		Fuda rFuda = this->m_RFuda;
		this->m_LFuda = md->m_Fuda[pt.X * md->FUDACNT];
		this->m_RFuda = md->m_Fuda[pt.Y * md->FUDACNT];
		lFuda.Trace(&this->m_LFuda);
		rFuda.Trace(&this->m_RFuda);
		//���b�Z�[�W�ύX
		int y = this->m_siMsg->_y;
		this->m_siMsg->Reset();
		if(chFuda*(pt.Y - pt.X) > 0){
			md->SetOyaFlg(TRUE);
			this->m_siMsg = &md->m_siChS[this->STR_ANAOYA];
		}else{
			md->SetOyaFlg(FALSE);
			this->m_siMsg = &md->m_siChS[this->STR_WATOYA];
		}
		this->m_siMsg->Centering(NULL,y);
		//��
		this->m_siLMON = &md->m_siMonth[pt.X];//��(��)
		this->m_siLMON->Init(this->m_ptLU);
		this->m_siRMON = &md->m_siMonth[pt.Y];//��(�E)
		this->m_siRMON->Init(this->m_ptRD.X,this->m_ptLU.Y);
		//�N
		this->m_siLWho = (chFuda > 0)?&md->m_siYou:&md->m_siCom;//�N(��)
		this->m_siLWho->Init(this->m_ptLU.X,this->m_ptRD.Y);
		this->m_siRWho = (chFuda < 0)?&md->m_siYou:&md->m_siCom;//�N(�E)
		this->m_siRWho->Init(this->m_ptRD);

		this->m_uiStopCnt = 50;

		return TRUE;
	}
	return FALSE;
}


// �D�𓮂���
void ChooseScene::Move(void){
	if(!this->Stop()){
		//�V�[���ύX
		Mediator* md = Mediator::GetInstance();
		this->Reset();
		md->ChangeScene(PlayScene::GetInstance());
	}
}

// �`��
void ChooseScene::Paint(void){
	Mediator* md = Mediator::GetInstance();
	this->m_LFuda.Paint(md->m_gOffScreen);
	this->m_RFuda.Paint(md->m_gOffScreen);
	this->m_siMsg->Paint(md->m_gOffScreen);
	if(this->m_siLMON) this->m_siLMON->Paint(md->m_gOffScreen);
	if(this->m_siRMON) this->m_siRMON->Paint(md->m_gOffScreen);
	if(this->m_siLWho) this->m_siLWho->Paint(md->m_gOffScreen);
	if(this->m_siRWho) this->m_siRWho->Paint(md->m_gOffScreen);
}

