#include "StdAfx.h"
#include "Layer.h"


Layer::Layer(int width,int height):Character(width,height),
								   m_ptCom(146,0),m_ptYou(292,0),
								   m_ptMonth(0,35),
								   m_ptCScore(210,35),m_ptMScore(356,35)
{
	//�X�N���[��
	this->m_gScreen = Graphics::FromImage(this->GetBitmap());

	Mediator* md = Mediator::GetInstance();
	//������摜
	this->m_siCom = &md->m_siCom;//COM
	this->m_siYou = &md->m_siYou;//���Ȃ�
	int i = 0;
	for(i=0;i<MONCNT;i++){
		this->m_siMonth[i] = &md->m_siMonth[i];//��
	}
	for(i=0;i<MONCNT+1;i++){
		this->m_siMon[i][0] = this->m_siMon[i][1] = md->m_siMonB;//��
	}
	this->m_siScore.reserve(50);//�X�R�A�[
	this->m_siGoukei = &md->m_siGoukeiB;//���v
}

Layer::~Layer(void)
{
}

//������
void Layer::Init(void)
{
	Mediator* md = Mediator::GetInstance();

	this->m_siCom->Init(this->m_ptCom);//COM
	this->m_siYou->Init(this->m_ptYou);//���Ȃ�
	int i = 0;
	for(i=0;i<MONCNT+1;i++){
		Point ptMonth(this->m_ptMonth.X,this->m_ptMonth.Y + DIF_Y*i);
		Point ptCScore(this->m_ptCScore.X,this->m_ptCScore.Y + DIF_Y*i);
		Point ptMScore(this->m_ptMScore.X,this->m_ptMScore.Y + DIF_Y*i);

		if(i==MONCNT){
			this->m_siGoukei->Init(ptMonth);//���v
			//�X�R�A�[
			int cScore = md->GetScore().X;
			int mScore = md->GetScore().Y;
			md->HouseScore(cScore,this->m_siScore,ptCScore);//COM
			md->HouseScore(mScore,this->m_siScore,ptMScore);//���Ȃ�
			//���b�Z�[�W
			this->m_siMsg = (mScore>cScore)?&md->m_siLaS[MSG_ANK]:(mScore<cScore)?&md->m_siLaS[MSG_WAK]:&md->m_siLaS[MSG_DRAW];
		}else{
			this->m_siMonth[i]->Init(ptMonth);//��
			md->HouseScore(md->m_uiScoreTable[i][0],this->m_siScore,ptCScore);//COM
			md->HouseScore(md->m_uiScoreTable[i][1],this->m_siScore,ptMScore);//���Ȃ�
		}
		//��
		this->m_siMon[i][0].Init(ptCScore);//COM
		this->m_siMon[i][1].Init(ptMScore);//���Ȃ�
	}
	//���b�Z�[�W
	this->m_siMsg->Centering(NULL,Y_MSG,this->GetWidth(),this->GetHeight());

	Character::Init();
}

// ������
void Layer::Init(UINT _x, UINT _y)
{
	this->Init();
	Character::Init(_x,_y);
}

//������
void Layer::Init(Point pt)
{
	this->Init(pt.X,pt.Y);
}

//�����ɔz�u
void Layer::Centering(UINT _x,UINT _y,UINT width,UINT height)
{
	this->Init();
	Character::Centering(_x,_y);
}

// ���Z�b�g
void Layer::Reset(void)
{
	//����
	this->m_siCom->Reset();//COM
	this->m_siYou->Reset();//���Ȃ�
	for(int i=0;i<MONCNT+1;i++){
		if(i==MONCNT) this->m_siGoukei->Reset();//���v
		else this->m_siMonth[i]->Reset();//��
		//��
		this->m_siMon[i][0].Reset();//COM
		this->m_siMon[i][1].Reset();//���Ȃ�
	}
	//�X�R�A�[
	this->m_siScore.clear();
	//���b�Z�[�W
	this->m_siMsg->Reset();

	Character::Reset();
}

// �`��
void Layer::Paint(Graphics* g)
{
	if(this->GetExistFlg()){
		Mediator* md = Mediator::GetInstance();

		//�w�i
		this->m_gScreen->Clear(Color::Transparent);
		//����
		this->m_siCom->Paint(this->m_gScreen);//COM
		this->m_siYou->Paint(this->m_gScreen);//���Ȃ�
		for(int i=0;i<MONCNT+1;i++){
			if(i==MONCNT) this->m_siGoukei->Paint(this->m_gScreen);//���v
			else this->m_siMonth[i]->Paint(this->m_gScreen);//��
			//��
			this->m_siMon[i][0].Paint(this->m_gScreen);//COM
			this->m_siMon[i][1].Paint(this->m_gScreen);//���Ȃ�
		}
		//�X�R�A�[
		md->PaintStrImage(this->m_siScore,this->m_gScreen);
		
		//���b�Z�[�W
		this->m_siMsg->Paint(this->m_gScreen);
	}

	Character::ExtraPaint(g,this->m_fRGBA[0],this->m_fRGBA[1],this->m_fRGBA[2],this->m_fRGBA[3]);
}
