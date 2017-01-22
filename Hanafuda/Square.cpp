#include "StdAfx.h"
#include "Square.h"
#include "Strategy.h"
#include "PlayScene.h"


Square::Square(int width,int height):Character(width,height),
									 m_ptYF(45,121),
									 m_ptYName(30,30),
									 m_ptScore(384,30),
									 m_ptSumScore(384,271),
									 m_ptGoukei(261,271),
									 m_ptKoikoi(79,100)
{
	//�X�N���[��
	this->m_gScreen = Graphics::FromImage(this->GetBitmap());
	//�`��̈�
	this->SetArea(0,0,width,height);
	//vector�̈�m��
	this->m_AFuda.reserve(25);
	this->m_siYname.reserve(10);
	this->m_siScore.reserve(20);
	this->m_siMon.reserve(10);
	//���[���I�[�o�[�t���O
	this->m_bRolloverFlg = FALSE;
}

Square::~Square(void)
{
}

//�D�x�N�g�����ꊇ����
BOOL Square::AllFudaFunc(std::vector<Fuda> &vFuda,int func,double param,BOOL flag)
{

	std::vector<Fuda>::iterator it = vFuda.begin();
	BOOL rtnFlg = FALSE;
	int i=0;
	for(it=vFuda.begin();it != vFuda.end();it++){
		Fuda* fuda = &*it;
		if(fuda){
			switch(func){
				case FUNC_PAINT://�`��
					fuda->Paint(this->m_gScreen);
					break;
				case FUNC_SORT://�D���ёւ�
					this->SortFuda(fuda,i);
					break;
			}
		}
		i++;
	}

	return rtnFlg;
}

//�D����ёւ���
void Square::SortFuda(Fuda* fuda,int i)
{
	int size = this->m_AFuda.size();
	int maxSize = YFUDACNT;
	fuda->SetScale(1.0);
	int dx = fuda->GetWidth();
	int nX,nY;

	if(size <= maxSize){ 
		//�Z���^�����O
		nX = (int)(this->m_ptYF.X + dx*(i+(double)(maxSize - size)/2));
	}
	//max�T�C�Y��������A�l�߂ĕ\��
	else nX = (int)(this->m_ptYF.X + i*dx*(double)(maxSize-1)/(size-1));
	nY = this->m_ptYF.Y;

	fuda->SetPos(Point(nX,nY));

}

//�D����ׂ�
void Square::Sort(void)
{
	this->AllFudaFunc(this->m_AFuda,FUNC_SORT);
}

// �`��
void Square::Paint(Gdiplus::Graphics *g)
{
	if(this->GetExistFlg()){
		Mediator* md = Mediator::GetInstance();
		PlayScene* ps = PlayScene::GetInstance();

		//�w�i
		this->m_gScreen->Clear(Color::Transparent);
		this->m_gScreen->FillRectangle(&SolidBrush(Color(0,0,0)),this->m_iX,this->m_iY,this->m_iWidth,this->m_iHeight);

		if(this->m_uiMode == MODE_ACHIEVE){//��B�����[�h
			//�D
			this->AllFudaFunc(this->m_AFuda,FUNC_PAINT);
			//��
			this->m_siYname.back().Paint(this->m_gScreen);
			//����
			md->PaintStrImage(this->m_siScore,this->m_gScreen);
			//��
			this->m_siMon.back().Paint(this->m_gScreen);
		}else if(this->m_uiMode == MODE_KOIKOI){//�����������[�h
			this->m_siKoikoi->Paint(this->m_gScreen);
		}else if(this->m_uiMode == MODE_RESULT){//���ʃ��[�h
			//��
			md->PaintStrImage(this->m_siYname,this->m_gScreen);
			//����
			md->PaintStrImage(this->m_siScore,this->m_gScreen);
			//��
			md->PaintStrImage(this->m_siMon,this->m_gScreen);
			//���v
			this->m_siGoukei->Paint(this->m_gScreen);
		}
	
		Character::ExtraPaint(g,this->m_fRGBA[0],this->m_fRGBA[1],this->m_fRGBA[2],this->m_fRGBA[3]);

	}
}

//���[���I�[�o�[���̏���
void Square::OnRollOver(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,TRUE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//���[���A�E�g���̏���
void Square::OnRollOut(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//���[���I�[�o�[ID��ύX
	md->ChangeROID(this->m_uiCID,FALSE);
	//�ĕ`��̈���Z�b�g
	md->SetupRepaint(this->returnRect());
}

//�}�E�X�N���b�N���̏���
void Square::OnMouseClick(double x, double y){
	Mediator* md = Mediator::GetInstance();
	//�J�[�\����߂�
	md->ChangeROID(NULL,FALSE);
	SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
}

//�}�E�X�ړ����̏���
BOOL Square::MouseMove(double x, double y){
	return MouseListner::MouseMove(x,y);
}

//�}�E�X�N���b�N���̏���
BOOL Square::MouseClick(double x, double y){
	return MouseListner::MouseClick(x,y);
}

//������
void Square::Init(void)
{
	Character::Init();
	this->rect = this->returnRect();
}

// ������
void Square::Init(UINT _x, UINT _y)
{
	Character::Init(_x,_y);
	
	this->rect = this->returnRect();
}

//������
void Square::Init(Point pt)
{
	//���[�h
	this->m_uiMode = 0;

	this->Init(pt.X,pt.Y);
}

// ���Z�b�g
void Square::Reset(void)
{
	if(this->GetExistFlg()){
		//�`��̈�
		this->SetArea(0,0,this->GetWidth(),this->GetHeight());
		//�B�����r�b�g
		this->m_iAchieveBit = 0;
		//���D�폜
		this->m_AFuda.clear();
		//�𖼍폜
		this->m_siYname.clear();
		//�X�R�A�[�폜
		this->m_siScore.clear();
		//���폜
		this->m_siMon.clear();

		Character::Reset();
	}
}

//���[�h�ύX
void Square::ChangeMode(int mode,int bit)
{
	Mediator* md = Mediator::GetInstance();
	PlayScene* ps = PlayScene::GetInstance();

	this->m_uiMode = mode;
	this->m_iAchieveBit = bit;
	if(this->m_uiMode == MODE_ACHIEVE){//��B�����[�h
		//��
		int i = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT);
		this->m_siYname.push_back(md->m_siYName[i]);
		this->m_siYname.back().Init(this->m_ptYName);
		//�X�R�A�[
		int score = ps->GetAcheiveScore(this->m_iAchieveBit);
		md->HouseScore(score,this->m_siScore,this->m_ptSumScore,TRUE);
		//��
		this->m_siMon.push_back(md->m_siMonW);
		this->m_siMon.back().Init(this->m_ptSumScore);
	}else if(this->m_uiMode == MODE_KOIKOI){//�����������[�h
		if(ps->IsMyTurn()){//�����̔�
			this->m_siKoikoi = &md->m_siPlS[ps->STR_KOIKOI1];
			this->m_siKoikoi->Init(this->m_ptKoikoi);
		}else{//����̔�
			this->m_siKoikoi = &md->m_siPlS[ps->STR_KOIKOI2];
			this->m_siKoikoi->Centering(NULL,NULL,this->GetWidth(),this->GetHeight());
		}
	}else if(this->m_uiMode == MODE_RESULT){//���ʃ��[�h
		int i=0;
		int sum=0;
		double scale = 1.0;

		//�e��
		if(!this->m_iAchieveBit){
			md->m_siOyaken.Init(this->m_ptYName);
			this->m_siYname.push_back(md->m_siOyaken);
			sum += ps->GetAcheiveScore(this->m_iAchieveBit);
		}else{
			//���̌���6�ȏ�Ȃ�A�k���\��
			int j;
			for(j=0;j<4;j++){
				if(md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT,5+j)>=0) scale -= 0.1;
				else break;
			}
		}
			
		//�i�[
		while(this->m_iAchieveBit){
			int order = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT,i);
			if(order < 0) break;
			
			//��
			this->m_siYname.push_back(md->m_siYName[order]);
			this->m_siYname.back().SetScale(scale);
			int dY = (this->m_siYname.back().GetHeight()+3)*i;
			this->m_siYname.back().Init(this->m_ptYName.X,this->m_ptYName.Y + dY);
			//����
			int score = ps->GetAcheiveScore((1 << order));
			Point ptScore(this->m_ptScore.X,this->m_ptScore.Y + dY);
			md->HouseScore(score,this->m_siScore,ptScore,TRUE,scale);
			sum += score;
			//�u���v
			this->m_siMon.push_back(md->m_siMonW);
			this->m_siMon.back().SetScale(scale);
			this->m_siMon.back().Init(this->m_ptScore.X,this->m_ptScore.Y + dY);
			i++;
		}
		//�u���v�v
		this->m_siGoukei = &md->m_siGoukeiW;
		this->m_siGoukei->Init(this->m_ptGoukei);
		ps->AddScore(sum);
		//���v����
		md->HouseScore(sum,this->m_siScore,this->m_ptSumScore,TRUE);
		//�u���v
		this->m_siMon.push_back(md->m_siMonW);
		this->m_siMon.back().Init(this->m_ptSumScore);
	}

}

//�`��͈͂�ݒ�
void Square::SetArea(int x,int y,int width,int height)
{
	this->m_iX = x;
	this->m_iY = y;
	this->m_iWidth = width;
	this->m_iHeight = height;
}

//�`��ʒu�𒆉��ɌŒ肩�A�̈���w��{�ɏk������
void Square::ReduceArea(float scale)
{
	int x = static_cast<int>(this->GetWidth()*(1-scale)/2);
	int y = static_cast<int>(this->GetHeight()*(1-scale)/2);
	int width = static_cast<int>(this->GetWidth()*scale);
	int height = static_cast<int>(this->GetHeight()*scale);
	this->SetArea(x,y,width,height);
}

