#include "StdAfx.h"
#include "Mediator.h"
#include "ChooseScene.h"
#include "Strategy.h"
#include <process.h>

const LPCWSTR Mediator::EVENT_NAME = L"Thread";

Mediator::~Mediator(void)
{
	// �C�x���g�̔j��
	CloseHandle(this->m_hEvent);
	// �X���b�h�̔j��
	CloseHandle(this->m_thread);
}

//�D���
const UINT Mediator::m_uiFKind[ALLFUDACNT] = { 
	  KIND_GOKO,KIND_TAN,KIND_KASU,KIND_KASU,//�ꌎ(���ɒ�)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//��(�~����)
	  KIND_GOKO,KIND_TAN,KIND_KASU,KIND_KASU,//�O��(���ɖ�)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//�l��(���Ƀz�g�g�M�X)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//�܌�(�Ҋ��ɔ���)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//�Z��(���O�ɒ�)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//����(���ɒ�)
	  KIND_GOKO,KIND_TANE,KIND_KASU,KIND_KASU,//����(�V��Ɍ�)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//�㌎(�e�ɔt)
	  KIND_TANE,KIND_TAN,KIND_KASU,KIND_KASU,//�\��(�g�t�Ɏ�)
	  KIND_GOKO,KIND_KASU,KIND_KASU,KIND_KASU, //�\�ꌎ(�˂ɖP��)
	  KIND_GOKO,KIND_TANE,KIND_TAN,KIND_KASU//�\��(��(�J))
};

//���_�ꗗ
const UINT Mediator::m_uiScore[YAKUCNT] = {
	SCORE_GOKO,//�܌�(15��)
	SCORE_SHIKO,//�l��(10��)
	SCORE_AMESHIKO,//�J�l��(8��)
	SCORE_SANKO,//�O��(6��)
	SCORE_HANAMI,//�Ԍ�(3��)
	SCORE_TSUKIMI,//����(3��)
	SCORE_AKATAN,//�ԒZ(6��)
	SCORE_AOTAN,//�Z(6��)
	SCORE_INOSHIKA,//������(5��)
	SCORE_TANE,//�^�l(1��)
	SCORE_TAN,//�^��(1��)
	SCORE_KASU//�J�X(1��)
};

//�I�t�X�N���[���Ƀu�����h�`��
void Mediator::BlendImages(int x,int y,Bitmap* srcBitmap)
{
	this->m_XG.BlendImages(x,y,XGraphics::BLEND_ALPHA,srcBitmap);
}

// ���C���X�N���[���`��
void Mediator::Paint(void)
{
	PAINTSTRUCT ps;

	//�`��J�n
	HDC hdc = BeginPaint(this->m_XG.m_hWnd,&ps);

	//�w�i
	this->m_gOffScreen->DrawImage(this->m_imgBG,0,0,this->m_imgBG.GetWidth(),this->m_imgBG.GetHeight());

	//�V�[�����ɕ`��
	this->m_lpScene->Paint();

	//�I�t�X�N���[����`��
	this->m_XG.Update(hdc);

	//�`��I��
	EndPaint(this->m_XG.m_hWnd,&ps);

}

//�ĕ`��
void Mediator::Repaint(void)
{
	InvalidateRect(this->m_XG.m_hWnd,&this->m_rctRepaint,FALSE);
	UpdateWindow(this->m_XG.m_hWnd);
	SetRectEmpty(&this->m_rctRepaint);
}

//�ĕ`��̈���Z�b�g
void Mediator::SetupRepaint(int left,int top,int right,int bottom)
{
	//��`����������
	int l = left;
	int t = top;
	int r = right;
	int b = bottom;
	if(!IsRectEmpty(&this->m_rctRepaint)){
		l = min(this->m_rctRepaint.left,l);
		t = min(this->m_rctRepaint.top,t);
		r = max(this->m_rctRepaint.right,r);
		b = max(this->m_rctRepaint.bottom,b);
	}

	SetRect(&this->m_rctRepaint,l,t,r,b);
}

//�ĕ`��̈���Z�b�g
void Mediator::SetupRepaint(RECT rect)
{
	this->SetupRepaint(rect.left,rect.top,rect.right,rect.bottom);
}

// ������
void Mediator::Init(HWND hWnd)
{
	int i;
	Strategy* st = Strategy::GetInstance();

	//���f�o�b�O�p�t���O
	this->m_bDebugFlg = FALSE;

	//�E�B���h�E�n���h��
	this->m_hWnd = hWnd;

	//�O���t�B�b�N�X����
	this->m_XG.SetProperty(hWnd);
	this->m_gOffScreen = this->m_XG.m_gOffScreen;

	//�ĕ`��̈�
	SetRectEmpty(&this->m_rctRepaint);

	//***�摜�o�^***
	HMODULE hModule;
	hModule = ::LoadLibrary(_T("RS.dll"));
	this->m_imgBG.Load(IDB_BG,_T("PNG"),hModule);//�w�i
	for(i=0;i<ALLFUDACNT;i++){//�D
		this->m_Fuda[i].SetProperty(i,Mediator::m_uiFKind[i],IDB_FUDA1 + i,hModule);
	}
	this->m_UraFuda.SetProperty(NULL,NULL,IDB_URA,hModule);
	//===������摜�o�^===
	this->m_siCom.Load(IDB_STR_COM,_T("PNG"),hModule);//COM
	this->m_siYou.Load(IDB_STR_YOU,_T("PNG"),hModule);//���Ȃ�
	for(i=0;i<MONCNT;i++){//��
		this->m_siMonth[i].Load(IDB_STR_1G + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_CHSCNT;i++){//�e���߃V�[��
		this->m_siChS[i].Load(IDB_STR_ANO + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_PLSCNT;i++){//�΋ǃV�[��
		this->m_siPlS[i].Load(IDB_STR_KOIKOI1 + i,_T("PNG"),hModule);
	}
	for(i=0;i<S_LASCNT;i++){//�ŏI�V�[��
		this->m_siLaS[i].Load(IDB_STR_WAK + i,_T("PNG"),hModule);
	}
	for(i=0;i<YAKUCNT;i++){//��
		this->m_siYName[i].Load(IDB_STR_GOKO + i,_T("PNG"),hModule);
	}
	this->m_siOyaken.Load(IDB_STR_OYAKEN,_T("PNG"),hModule);//�e��
	for(i=0;i<10;i++){//��
		this->m_siNumberB[i].Load(IDB_STR_B_0 + i,_T("PNG"),hModule);//��
		this->m_siNumberW[i].Load(IDB_STR_W_0 + i,_T("PNG"),hModule);//��
	}
	this->m_siMonB.Load(IDB_STR_B_MON,_T("PNG"),hModule);//��(��)
	this->m_siMonW.Load(IDB_STR_W_MON,_T("PNG"),hModule);//��(��)
	this->m_siGoukeiB.Load(IDB_STR_B_GOUKEI,_T("PNG"),hModule);//���v(��)
	this->m_siGoukeiW.Load(IDB_STR_W_GOUKEI,_T("PNG"),hModule);//���v(��)
	//===�I���摜===
	this->m_slHai.Load(IDB_STR_HAI,_T("PNG"),hModule);//�͂�
	this->m_slIie.Load(IDB_STR_IIE,_T("PNG"),hModule);//������
	//===���ꗗ�摜===
	this->m_siYakuList.Load(IDB_YAKU,_T("PNG"),hModule);
	//===���ʉ�===
//	this->m_sndPut1.Load(IDR_MP3_PUT1,hModule,_T("MP3"),_T("MPEGVIDEO"));
//	this->m_sndPut1.Load(IDR_WAV_PUT1,hModule);
//	this->m_sndPut2.Load(IDR_MP3_PUT2,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndAcheive.Load(IDR_MP3_ACHEIVE,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndWin.Load(IDR_MP3_WIN,hModule,_T("MP3"),_T("MPEGVIDEO"));
	this->m_sndEnding.Load(IDR_MP3_ENDING,hModule,_T("MP3"),_T("MPEGVIDEO"));

	::FreeLibrary(hModule);

	//�V�[���ݒ�
	this->ChangeScene(ChooseScene::GetInstance());
	//�X�R�A�[���Z�b�g
	this->ResetScore();

	//�X���b�h�X�^�[�g
	this->m_hEvent = CreateEvent(NULL, TRUE, FALSE, EVENT_NAME);
	unsigned (__stdcall *func)( void* v ) = Mediator::Run;
	this->m_thread = (HANDLE)::_beginthreadex(NULL,0,func,NULL,(unsigned)0, NULL);
}

//���Z�b�g
void Mediator::Reset(void)
{
	//�V�[���ݒ�
	if(this->m_lpScene){
		this->m_lpScene->Reset();
		//�X���b�h��~
		this->RunThread(FALSE);
	}
	this->ChangeScene(ChooseScene::GetInstance());
	//�X�R�A�[���Z�b�g
	this->ResetScore();
	this->SetupRepaint(0,0,WND_WIDTH,WND_HEIGHT);
	this->Repaint();
}

//�X�R�A�[���Z�b�g
void Mediator::ResetScore(void)
{
	int i;
	for(i=0;i<MONCNT;i++) this->m_uiScoreTable[i][0] = this->m_uiScoreTable[i][1] = 0;
	this->m_uiKyoku = 0;
}

// �}�E�X�ړ����̏���
void Mediator::MouseMove(double x, double y)
{
	if(!this->m_bThreadFlg){
		if(this->m_lpScene->MouseMove(x,y)){
			this->Repaint();
		}
	}
}

// �}�E�X�N���b�N�̏���
void Mediator::MouseClick(double x, double y)
{
	if(!this->m_bThreadFlg){
		if(this->m_lpScene->MouseClick(x,y)){
			//�ĕ`��
			InvalidateRect(this->m_XG.m_hWnd,&this->m_rctRepaint,FALSE);
			//�X���b�h�X�^�[�g
			this->RunThread(TRUE);
		}
	}
}

//���ʉ�
void Mediator::Sound(BOOL playFlg)
{
	this->m_lpScene->Sound(playFlg);
}

//�X���b�h�֐�
unsigned __stdcall Mediator::Run(LPVOID param)
{
	Mediator* md = Mediator::GetInstance();

	HANDLE hEvent = OpenEvent(EVENT_ALL_ACCESS, FALSE, EVENT_NAME);


	UINT i = 0;
	while(TRUE){
		WaitForSingleObject(hEvent, INFINITE);
		md->m_tc.TimeRegular();
		md->m_lpScene->Move();
		if(md->m_tc.GetDrawFlg()){
			//�ĕ`��
			if(!IsRectEmpty(&md->m_rctRepaint)){
				md->Repaint();
			}
		}
	}

	return 0;
}

//�X���b�h�����s
void Mediator::RunThread(BOOL onFlg)
{
	this->m_bThreadFlg = onFlg;
	if(onFlg){
		Mediator::GetInstance()->m_tc.ResetCounter(TRUE);
		SetEvent(this->m_hEvent);
	}else{
		ResetEvent(this->m_hEvent);
		this->SetupRepaint(0,0,WND_WIDTH,WND_HEIGHT);
		this->Repaint();
	}
}


//�V�[���ύX
void Mediator::ChangeScene(Scene* scene)
{
	this->m_lpScene = scene;
	this->m_lpScene->Init();
}

//�e�t���O
BOOL Mediator::GetOyaFlg(void)
{
	return this->m_bOyaFlg;
}
void Mediator::SetOyaFlg(BOOL oyaFlg)
{
	this->m_bOyaFlg = oyaFlg;
}

//���[���I�[�o�[ID���擾
UINT Mediator::GetROID(void)
{
	return this->m_uiROID;
}

//���[���I�[�o�[ID��ύX
void Mediator::ChangeROID(UINT roID,BOOL roFlg)
{
	if(roFlg) this->m_uiROID = roID;
	else{
		if(!roID || roID == this->m_uiROID) this->m_uiROID = NULL;
	}
}

//���ǂɐi�߂�
UINT Mediator::NxtKyoku(BOOL nxtFlg)
{
	if(nxtFlg) this->m_uiKyoku++;
	return this->m_uiKyoku;
}

//���_�����Z
void Mediator::AddScore(int score,BOOL myFlg)
{
	int lose = (myFlg+1)%2;
	this->m_uiScoreTable[this->m_uiKyoku][myFlg] = score;
	this->m_uiScoreTable[this->m_uiKyoku][lose] = 0;
	//�e��ύX
	this->m_bOyaFlg = myFlg;
}

//�݌v���_���擾
Point Mediator::GetScore(void)
{
	UINT i,cnt;
	Point pt(0,0);
	cnt = (this->m_uiKyoku>=MONCNT)?MONCNT:this->m_uiKyoku;
	for(i=0;i<cnt;i++){
		pt.X += this->m_uiScoreTable[i][0];
		pt.Y += this->m_uiScoreTable[i][1];
	}
	return pt;
}

//�w��r�b�g����w��̏��Ԃ��擾
int Mediator::ExtBitOrder(int bit,int total,int i)
{
	if(bit){
		//�r�b�g���珇�Ԃ��Z�o
		int sort = -1;
		int cnt = -1;
		while(++sort < total){
			if(bit & (1 << sort)){
				if(++cnt == i) return sort;
			}
		}
	}

	return -1;
}

//�X�R�A�[���l�摜���i�[
void Mediator::HouseScore(UINT score,std::vector<StrImage> &vSI,Point rPos,BOOL whiteFlg,double scale)
{
	//1�̈ʂ��珇�ɕ��ׂĕ`��
	int i=0,no=0;
	do{
		no = score%10;
		if(!whiteFlg) vSI.push_back(this->m_siNumberB[no]);
		else vSI.push_back(this->m_siNumberW[no]);
		StrImage* temp = &vSI.back();
		temp->SetScale(scale);
		temp->Init(rPos.X - (++i)*temp->GetWidth(),rPos.Y);
		score /= 10; 
	}while(score);	
}

//������摜���ꊇ�`��
void Mediator::PaintStrImage(std::vector<StrImage> &vSI,Graphics* g)
{
	std::vector<StrImage>::iterator it = vSI.begin();
	int i=0;
	for(it=vSI.begin();it != vSI.end();it++){
		StrImage* si = &*it;
		if(si) si->Paint(g);
		i++;
	}
}

