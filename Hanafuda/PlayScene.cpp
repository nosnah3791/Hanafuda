#include "StdAfx.h"
#include "PlayScene.h"
#include "LastScene.h"
#include "Strategy.h"
#include "Random.h"
#include <algorithm>
#include <math.h>
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

PlayScene::PlayScene(void): m_ptCF(20,20),m_ptCF2(260,20),
							m_ptBFt(50,146),m_ptBFt2(182,146),
							m_ptDF(404,252),m_ptMonth(300,283),
							m_ptBFb(50,358),m_ptBFb2(182,358),
							m_ptMF(20,484),m_ptMF2(260,484),
							m_ptGCF(520,106),m_ptGMF(520,398),
							m_ptCom(520,10),m_ptYou(520,302),
							m_ptCY(520,43),m_ptMY(520,335),
							m_ptYSize(75,20),
							m_ptCScore(788,10),m_ptMScore(788,302),
							m_ptSquare(35,131),m_ptSqSize(450,338),
							m_ptHai(134,332),m_ptIie(286,332)
{
	Mediator* md = Mediator::GetInstance();
	//�D�ԍ��i�[
	this->m_vNo.reserve(md->ALLFUDACNT);
	int i;
	for(i=0;i<md->ALLFUDACNT;i++){
		this->m_vNo.push_back(i);
	}
	//�D�̈�m��
	for(i=0;i<UFUDACNT;i++){
		this->m_UFuda[i] = md->m_UraFuda;
	}
	this->m_MyFuda.reserve(TEFUDACNT);
	this->m_ComFuda.reserve(TEFUDACNT);
	this->m_BaFuda.reserve(BAFUDACNT);
	this->m_DaiFuda.reserve(DAIFUDACNT);
	//���ꗗ
	this->m_ylComYaku = md->m_siYakuList;
	this->m_ylMyYaku = md->m_siYakuList;
	//***������摜***
	//���O
	this->m_siCom = &md->m_siCom;
	this->m_siYou = &md->m_siYou;
	//�X�R�A�[
	this->m_siScore.reserve(10);
	//�u���v
	for(i=0;i<2;i++) this->m_siMon[i] = md->m_siMonB;
	//���w�i
	this->m_square = new Square(this->m_ptSqSize.X,this->m_ptSqSize.Y);
	//�I���摜
	this->m_slHai = &md->m_slHai;
	this->m_slIie = &md->m_slIie;
	//BGM
	this->m_sound = &md->m_sndAcheive;
	//���C���[
	this->m_vLayer.reserve(4);
	//���\�[�X�t�@�C��
	this->m_hModule = ::LoadLibrary(_T("RS.dll"));
}

PlayScene::~PlayScene(void)
{
	//���\�[�X�t�@�C��
	::FreeLibrary(this->m_hModule);
}

//�D���V���b�t������
void PlayScene::Shuffle(void)
{
	Mediator* md = Mediator::GetInstance();
	int monAry[md->MONCNT];
	int i=0;
	BOOL cmtFlg = FALSE;

	//��D�ɓ�����4������ꍇ�A�ăV���b�t��
	do{
		for(i=0;i<md->MONCNT;i++) monAry[i] = 0;
		Random r;
		std::random_shuffle(this->m_vNo.begin(),this->m_vNo.end(),r);
		cmtFlg = TRUE;
		for(i=0;i<TEFUDACNT;i++){
			int mon = this->m_vNo[i]/FUDACNT;
			if(++(monAry[mon]) >= FUDACNT) cmtFlg = FALSE;
		}
	}while(!cmtFlg);
}

//�D���Z�b�g����
void PlayScene::SetupFuda(void)

{
	Mediator* md = Mediator::GetInstance();
	//�D���V���b�t��
	this->Shuffle();
	//�D������
	int i;
	for(i=0;i<UFUDACNT;i++){//���D
		if(i==0) this->m_UFuda[i].Init(this->m_ptDF);
		else this->m_UFuda[i].SetPos(this->m_ptDF);
	}
	//�i�[
	for(i=0;i<DAIFUDACNT;i++){
		if(i<TEFUDACNT){
			this->m_BaFuda.push_back(&md->m_Fuda[this->m_vNo[i]]);
			this->m_ComFuda.push_back(&md->m_Fuda[this->m_vNo[i+TEFUDACNT]]);
			this->m_MyFuda.push_back(&md->m_Fuda[this->m_vNo[i+TEFUDACNT*2]]);
		}
		this->m_DaiFuda.push_back(&md->m_Fuda[this->m_vNo[i+DAIFUDACNT]]);
	}
}


//�ړ��o�H���m�肷��
void PlayScene::SetupRoute(Fuda* lpFuda,Point* ePt,UINT cnt,UINT step)
{
	UINT i,j;
	int x,y;

	//Vector������
	for(i=0;i<this->m_ptMove.size();i++) this->m_ptMove[i].clear();
	this->m_ptMove.clear();
	this->m_ptMove.resize(cnt);

	//���W�z����i�[
	for(i=0;i<cnt;i++){
		this->m_ptMove[i].resize(step);
		double bX = static_cast<double>((lpFuda+i)->_x);
		double bY = static_cast<double>((lpFuda+i)->_y);
		double eX = static_cast<double>((ePt+i)->X);
		double eY = static_cast<double>((ePt+i)->Y);
		double dX = (eX - bX)/static_cast<double>(step);
		double dY = (eY - bY)/static_cast<double>(step);
		for(j=0;j<step;j++){
			x = static_cast<int>(bX + dX*(j+1) + 0.5);
			y = static_cast<int>(bY + dY*(j+1) + 0.5);
			this->m_ptMove[i][j] = Point(x,y);
		}
	}
}


//�ړ��o�H���m�肷��
void PlayScene::SetupRoute(Fuda** lplpFuda,Point* ePt,UINT cnt,UINT step)
{
	Fuda* fuda = new Fuda[cnt];
	UINT i = 0;
	for(i=0;i<cnt;i++){
		fuda[i] = **(lplpFuda++);
	}
	this->SetupRoute(fuda,ePt,cnt,step);
	delete [] fuda;
}


//���킹�D�̈ړ��o�H���m�肷��
void PlayScene::SetupAFRoute(BOOL teFlg,UINT step)
{
	Mediator* md = Mediator::GetInstance();
	int order = md->ExtBitOrder(this->m_iBFbitBlank,BAFUDACNT);
	int dif = 0;
	if((teFlg && (this->m_iBFbit1>0)) || (!teFlg && (this->m_iBFbit2>0))){
		dif = BAFUDADIF;
		order = (teFlg)?md->ExtBitOrder(this->m_iBFbit1,BAFUDACNT):md->ExtBitOrder(this->m_iBFbit2,BAFUDACNT);
	}
	int turn = (FUDACNT*2-1)*2;
	int dx = (order/turn)?order%turn:order/2;
	int x = this->m_ptBFt.X+dx*(this->m_UFuda[0].GetWidth()+BAFUDADIF);
	int y = (order/turn)?this->m_ptDF.Y:(order%2)?this->m_ptBFb.Y:this->m_ptBFt.Y;
	Fuda* fuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	this->SetupRoute(fuda,&Point(x+dif,y+dif),1,step);
}

//�l���D�̈ړ��o�H���m�肷��
void PlayScene::SetupGFRoute(Fuda** lplpFuda,UINT cnt,UINT step,UINT ratio)
{
	Mediator* md = Mediator::GetInstance();
	BOOL myFlg = this->IsMyTurn();
	Point oPt = (myFlg)?this->m_ptGMF:this->m_ptGCF;
	Point* ePt = new Point[cnt];
	UINT i = 0;
	UINT kasuCnt = 0;
	for(i=0;i<cnt;i++){
		if(lplpFuda[i] == NULL) break;
		UINT kind = lplpFuda[i]->GetKind();
		UINT size = (myFlg)?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
		if(kind==md->KIND_KASU) size += kasuCnt++;
		if(size+1>=GFUDACNT) size = GFUDACNT-1;
		int x = oPt.X + size*this->m_UFuda[0].GetWidth()/ratio;
		int y = oPt.Y + kind*this->m_UFuda[0].GetHeight()/ratio;
		ePt[i] = Point(x,y);

	}
	this->SetupRoute(lplpFuda,ePt,i,step);

	delete [] ePt;
}

//�D���w�薇�����A1�t���[�����k�����Ȃ��炸�炷
BOOL PlayScene::SlideReduce(Fuda** lplpFuda,UINT cnt,UINT step,UINT ratio)
{
	//�ړ�
	if(this->m_uiFrameNo++ < step){
		Mediator* md = Mediator::GetInstance();
		UINT i;
		for(i=0;i<cnt;i++){
			if(lplpFuda[i] == NULL) break;
			md->SetupRepaint(lplpFuda[i]->returnRect());
			lplpFuda[i]->SetPos(this->m_ptMove[i][this->m_uiFrameNo-1],1.0-(double)this->m_uiFrameNo/(double)(ratio*step));
			md->SetupRepaint(lplpFuda[i]->returnRect());
		}
		return TRUE;
	}
	
	//�ړ�����
	return FALSE;
}



//���D���w�薇�����A1�t���[�����炷
BOOL PlayScene::Slide(Fuda* lpUFuda,UINT cnt,UINT step)
{
	//�ړ�
	if(this->m_uiFrameNo++ < step){
		Mediator* md = Mediator::GetInstance();
		UINT i;
		for(i=0;i<cnt;i++){
			md->SetupRepaint((lpUFuda+i)->returnRect());
			(lpUFuda+i)->SetPos(this->m_ptMove[i][this->m_uiFrameNo-1]);
			md->SetupRepaint((lpUFuda+i)->returnRect());
		}
		return TRUE;
	}
	
	//�ړ�����
	return FALSE;
}


//�D��z��
void PlayScene::Deal(void){
	Mediator* md = Mediator::GetInstance();
	Fuda* fuda = this->m_UFuda;
	Point ePt[FUDACNT];
	UINT i;

	switch(this->m_uiCutNo){
		case 0://��D4��(��U)��
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT*2]:&this->m_UFuda[1];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptMF:this->m_ptCF;
			break;
		case 1://��D4��(��U)��
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1]:&this->m_UFuda[1+FUDACNT*2];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptCF:this->m_ptMF;
			break;
		case 2://��D4��(��2����2��)��
			fuda = &this->m_UFuda[1+FUDACNT*4];
			if(!this->m_uiFrameNo){
				ePt[0] = this->m_ptBFt;
				ePt[1] = this->m_ptBFb;
				ePt[2] = Point(this->m_ptBFt.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFt.Y);
				ePt[3] = Point(this->m_ptBFb.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFb.Y); 
			}
			break;
		case 3://��D4��(��U)�E
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT*3]:&this->m_UFuda[1+FUDACNT];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptMF2:this->m_ptCF2;
			break;
		case 4://��D4��(��U)�E
			fuda = (md->GetOyaFlg())? &this->m_UFuda[1+FUDACNT]:&this->m_UFuda[1+FUDACNT*3];
			if(!this->m_uiFrameNo) ePt[0] = (md->GetOyaFlg())? this->m_ptCF2:this->m_ptMF2;
			break;
		case 5://��D4��(��2����2��)�E
			fuda = &this->m_UFuda[1+FUDACNT*4];
			if(!this->m_uiFrameNo){
				ePt[0] = this->m_ptBFt2;
				ePt[1] = this->m_ptBFb2;
				ePt[2] = Point(this->m_ptBFt2.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFt2.Y);
				ePt[3] = Point(this->m_ptBFb2.X + fuda->GetWidth()+BAFUDADIF,this->m_ptBFb2.Y); 
			}
			break;
		default://�ΐ�V�[����
			this->m_uiCutNo = 0;
			this->m_uiSceneNo = SCENE_PLAY;
			fuda = &this->m_UFuda[1+FUDACNT*2];
			//��D���߂���
			for(i=0;i<TEFUDACNT;i++){
				(fuda+i)->Trace(this->m_MyFuda[i]);
				(fuda+i)->Init(this->m_ptDF);
				//���f�o�b�O�p
				if(md->m_bDebugFlg){
					this->m_ComFuda[i]->Init(this->m_ptCF.X + fuda->GetWidth()*i,this->m_ptCF.Y);
				}
			}
			this->m_uiStopCnt = 10;
			return;
	}

	//��D�������ɕ��ׂ�
	BOOL bafudaFlg = (this->m_uiCutNo % 3 == 2)?TRUE:FALSE;
	if(!bafudaFlg && !this->m_uiFrameNo){
		for(i=0;i<FUDACNT;i++){
			ePt[i] = Point(ePt[0].X + fuda->GetWidth()*i,ePt[0].Y);
		}
	}

	//�ړ��o�H���m�肷��
	if(!this->m_uiFrameNo){
		this->SetupRoute(fuda,ePt,FUDACNT);
		for(i=0;i<FUDACNT;i++){
			(fuda+i)->Init();
		}
	}

	//�ړ�
	if(!this->Slide(fuda,FUDACNT)){
		//���ʉ�
		PlaySound(MAKEINTRESOURCE(IDR_WAV_PUT2),this->m_hModule,SND_RESOURCE | SND_ASYNC);


		//��D���߂���
		if(bafudaFlg){
			for(i=0;i<FUDACNT;i++){
				(fuda+i)->Trace(this->m_BaFuda[i+FUDACNT*(this->m_uiCutNo/3)]);
				(fuda+i)->Init(this->m_ptDF);
			}
		}

		//���̃J�b�g��
		this->m_uiFrameNo = 0;
		this->m_uiCutNo++;
	}
}


//�ΐ�
BOOL PlayScene::Play(void){

	switch(this->m_uiCutNo){
		case CUT_TSELECT://�D�I��
			return this->PlayTSelect();
		case CUT_BSELECT1://��D�I��
			return this->PlayBSelect(TRUE);
		case CUT_TPUT://��D���킹
			return this->PlayPut(TRUE);
		case CUT_DOPEN://��D�߂���
			return this->PlayDOpen();
		case CUT_DPUT://��D���킹
			return this->PlayPut(FALSE);
		case CUT_TGET://��D�l��
			return this->PlayGet(&this->m_GetTFuda);
		case CUT_B1GET://��D1�l��
			return this->PlayGet(this->m_GetBFuda1,FUDACNT-1);
		case CUT_DGET://��D�l��
			return this->PlayGet(&this->m_GetDFuda);
		case CUT_B2GET://��D2�l��
			return this->PlayGet(this->m_GetBFuda2,FUDACNT-1);
		case CUT_SORT://���ёւ�
			return this->PlaySort();
		case CUT_ACHIEVE://��B��
			return this->PlayAcheive();
		case CUT_KOIKOI://��������
			return this->PlayKoikoi();
		case CUT_RESULT://����
			return this->PlayResult();
	}
	return TRUE;
}


//��D�I��
BOOL PlayScene::PlayTSelect(void){

	BOOL rtnFlg = TRUE;

	//���w�i���Z�b�g
	this->m_square->Reset();
	//���C���[���Z�b�g
	this->m_vLayer.clear();

	//�����̔ԂȂ�A�X���b�h���~�߂�
	if(this->IsMyTurn()){
		//�I���\�ȏ�D�Ƀt���O�𗧂Ă�
		this->SetupSelectable();
		rtnFlg = FALSE;
	}else{
		//�R���s���[�^�����̎D��I��
		UINT i  = Strategy::GetInstance()->SelectCF();
		//���̎D���߂���
		this->m_GetTFuda = this->m_ComFuda[i];
		this->m_UFuda[i+1].Trace(this->m_GetTFuda);
		//�I����D���i�[
		if(this->HouseBF(TRUE,this->m_GetTFuda->GetID()/FUDACNT)){
			this->m_uiCutNo = CUT_TPUT;//��D�ړ��J�b�g��
			this->m_uiStopCnt = 10;
		}else{//��������̏ꍇ
			this->m_uiCutNo = CUT_BSELECT1;//��D�I���J�b�g��
		}
		//�I����D���폜
		this->AllFudaFunc(this->m_ComFuda,FUNC_ERASE,(double)i);
	}

	return rtnFlg;
}
//��D�I��
BOOL PlayScene::PlayBSelect(BOOL teFlg){

	BOOL rtnFlg = TRUE;

	Fuda** lplpFuda = (teFlg)?this->m_GetBFuda1:this->m_GetBFuda2;
	if(this->IsMyTurn()){//�����̔�
		//�I����D�ȊO�𓧉ߕ\��
		this->AllFudaFunc(this->m_BaFuda,FUNC_INVISIBLE,lplpFuda[0]->GetID()/FUDACNT,TRUE);
		lplpFuda[0]->SetupSelectable(TRUE);
		lplpFuda[1]->SetupSelectable(TRUE);
		//��D�I���J�b�g��
		this->m_uiCutNo = (teFlg)?CUT_BSELECT1:CUT_BSELECT2;
		rtnFlg = FALSE;
	}else{//COM�̔�
		//��I���D����D�ɖ߂�
		if(!Strategy::GetInstance()->AlternativeBF(lplpFuda[0]->GetID(),lplpFuda[0]->GetKind(),lplpFuda[1]->GetID(),lplpFuda[1]->GetKind())){
			this->ReturnBF(teFlg,lplpFuda[1],1);
		}else{
			this->ReturnBF(teFlg,lplpFuda[0],0);
			lplpFuda[0] = lplpFuda[1];
		}
		lplpFuda[1] = NULL;
		//��D�ړ��J�b�g��
		this->m_uiCutNo = (teFlg)?CUT_TPUT:CUT_DPUT;
		this->m_uiStopCnt = 10;
	}

	return rtnFlg;
}
//�D���킹
BOOL PlayScene::PlayPut(BOOL teFlg){

	//�o�H�ݒ�
	if(!this->m_uiFrameNo) this->SetupAFRoute(teFlg);
	//�ړ�
	Fuda* lpFuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	if(!this->Slide(lpFuda,1)){
		Mediator* md = Mediator::GetInstance();
		UINT id = 0;
		//���I���̎D����D�Ɋi�[
		if((teFlg && !this->m_iBFbit1) || (!teFlg && !this->m_iBFbit2)){
			//���ʉ�
			id = IDR_WAV_PUT2;
			this->AddBF(teFlg);
			lpFuda = NULL;
		}else id = IDR_WAV_PUT1;//this->m_sound = &md->m_sndPut1;
		//���ʉ�
		PlaySound(MAKEINTRESOURCE(id),this->m_hModule,SND_RESOURCE | SND_ASYNC);
		//���̃J�b�g��
		this->m_uiCutNo = (teFlg)?CUT_DOPEN:(this->m_iBFbit1)?CUT_TGET:(this->m_iBFbit2)?CUT_DGET:CUT_SORT;
		this->m_uiFrameNo = 0;
		this->m_uiStopCnt = 10;
	}

	return TRUE;
}
//��D�߂���
BOOL PlayScene::PlayDOpen(void){

	BOOL rtnFlg = TRUE;

	//��D�߂���
	this->m_GetDFuda = this->m_DaiFuda.front();
	this->m_GetDFuda->Init(this->m_ptDF);
	this->m_DaiFuda.erase(this->m_DaiFuda.begin());

	//�l����D���i�[
	if(this->HouseBF(FALSE,this->m_GetDFuda->GetID()/FUDACNT)){
		//��D���킹�J�b�g��
		this->m_uiCutNo = CUT_DPUT;
		this->m_uiStopCnt = 10;
	}else{//��������̏ꍇ
		//��D�I��
		rtnFlg = this->PlayBSelect(FALSE);
	}
	return rtnFlg;
}
//�D�l��
BOOL PlayScene::PlayGet(Fuda** lplpFuda,UINT cnt){

	//�o�H�ݒ�
	if(!this->m_uiFrameNo){
		this->SetupGFRoute(lplpFuda,cnt);
		this->SortLayer(this->m_uiCutNo);
	}
	//�ړ�
	if(!this->SlideReduce(lplpFuda,cnt)){
		Mediator* md = Mediator::GetInstance();
		//���ʉ�
		PlaySound(MAKEINTRESOURCE(IDR_WAV_PUT2),this->m_hModule,SND_RESOURCE | SND_ASYNC);
		//�l���ςݏ�D���i�[
		this->HouseGF(lplpFuda,cnt);
		//���J�b�g��
		switch(this->m_uiCutNo){
			case CUT_TGET://��D�l��
				this->m_uiCutNo = CUT_B1GET;//��D1�l����
				break;
			case CUT_B1GET://��D1�l��
				this->m_uiCutNo = (this->m_iBFbit2)?CUT_DGET:CUT_SORT;//��D�l��or���ёւ���
				break;
			case CUT_DGET://��D�l��
				this->m_uiCutNo = CUT_B2GET;//��D2�l����
				break;
			case CUT_B2GET://��D2�l��
				this->m_uiCutNo = CUT_SORT;//���ёւ���
				break;
		}
		this->m_uiFrameNo = 0;
		//���ёւ��̑O�ɁA�ꎞ��~
		if(this->m_uiCutNo == CUT_SORT) this->m_uiStopCnt = 10;
	}

	return TRUE;
}

//���ёւ�
BOOL PlayScene::PlaySort(void){

	Mediator* md = Mediator::GetInstance();
	int bit = this->m_iBFbit1 | this->m_iBFbit2;

	if(this->IsMyTurn()){//�����̃^�[��
		//��D�̃\�[�g
		this->AllFudaFunc(this->m_MyFuda,FUNC_SORT,SORT_TEFUDA);
		//�l���ςݎD(�J�X�̂�)
		if(bit && (this->m_GMyFuda[md->KIND_KASU].size()>GFUDACNT))
			this->AllFudaFunc(this->m_GMyFuda[md->KIND_KASU],FUNC_SORT,SORT_GETFUDA);
	}else{//����̃^�[��
		//����D(���D)�̃\�[�g
		UINT i;
		for(i=1;i<TEFUDACNT+1;i++){
			if(i<TEFUDACNT-this->m_uiTurnNo/2) this->m_UFuda[i].Init();
			else this->m_UFuda[i].Reset();
		}
		//����D(�\�D)�̃\�[�g���f�o�b�O�p
		if(md->m_bDebugFlg) this->AllFudaFunc(this->m_ComFuda,FUNC_SORT,SORT_COMFUDA);
		//�l���ςݎD(�J�X�̂�)
		if(bit && (this->m_GComFuda[md->KIND_KASU].size()>GFUDACNT))
			this->AllFudaFunc(this->m_GComFuda[md->KIND_KASU],FUNC_SORT,SORT_GETFUDA);
	}
	//�l���r�b�g���Z�b�g
	this->m_iBFbit1 = this->m_iBFbit2 = this->m_iBFbitBlank = 0;

	//�𔻒�
	this->m_iAchieveBit = Strategy::GetInstance()->Judge(this->IsMyTurn());
	if(this->m_iAchieveBit){
		//��B���J�b�g��
		this->m_uiCutNo = CUT_ACHIEVE;
		this->AddAcheiveBit(this->m_iAchieveBit);
	}else{
		//�D�I�������ʃJ�b�g else ��D�J�b�g��
		this->m_uiCutNo = (++this->m_uiTurnNo >= 2*TEFUDACNT)?CUT_RESULT:CUT_TSELECT;
		this->m_uiStopCnt = 10;
	}

	return TRUE;
}
//��B��
BOOL PlayScene::PlayAcheive(void){

	Mediator* md = Mediator::GetInstance();
	//���̃J�b�g��
	if(!this->m_iAchieveBit){
		this->m_uiCutNo = CUT_RESULT;
		//�ŏI�^�[�����O
		if(this->m_uiTurnNo/2+1 < TEFUDACNT){
			//�����̔�or(COM�̔�&�u������������v)�����������J�b�g��
			if(this->IsMyTurn() || Strategy::GetInstance()->IsKoiKoi()) this->m_uiCutNo = CUT_KOIKOI;
		}
	}else{
		//���w�i�z�u
		if(!this->m_uiFrameNo){
			this->m_square->Reset();
			this->m_square->Init(this->m_ptSquare);
		}
		//���w�i�\��
		if(++this->m_uiFrameNo < FADECNT){
			this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
		}else{
			//�����z���}�[�N
			this->m_uiWin = (this->IsMyTurn())?WIN_ME:WIN_COM;
			//�B������������ׂ�
			this->DispAcheiveFuda();
			this->m_uiStopCnt = 150;
			this->m_uiFrameNo = 0;
			//BGM
			this->m_sound = &md->m_sndAcheive;
			SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
		}
	}
	//�ĕ\��
	md->SetupRepaint(this->m_square->returnRect());

	return TRUE;
}
//��������
BOOL PlayScene::PlayKoikoi(void){

	BOOL rtnFlg = TRUE;

	//���w�i������
	if(!this->m_uiFrameNo){
		this->m_square->Reset();
		this->m_square->Init(this->m_ptSquare);
	}
	//���w�i�\��
	if(++this->m_uiFrameNo < FADECNT){
		this->m_square->ReduceArea(1.0f-(0.33f/FADECNT)*this->m_uiFrameNo);
		this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
	}else{
		//�u�����������܂��vor�u�����������܂����H�v
		this->m_square->ChangeMode(this->m_square->MODE_KOIKOI);
		this->m_uiFrameNo = 0;
		//���������I��
		if(this->IsMyTurn()){//�����̔�
			//�u�͂��v�u�������v
			this->m_slHai->Init(this->m_ptHai);
			this->m_slIie->Init(this->m_ptIie);
			rtnFlg = FALSE;
		}else{//����̔�
			//�u�����������܂��v
			this->m_uiStopCnt = 50;
			this->m_uiTurnNo++;//���̃^�[����
			this->m_uiCutNo = CUT_TSELECT;//��D�I���J�b�g��
		}

	}
	//�ĕ`��
	Mediator::GetInstance()->SetupRepaint(this->m_square->returnRect());

	return rtnFlg;
}

//����
BOOL PlayScene::PlayResult(void){

	Mediator* md = Mediator::GetInstance();
	BOOL rtnFlg = TRUE;

	//���w�i������
	if(!this->m_uiFrameNo){
		this->m_square->Reset();
		this->m_square->Init(this->m_ptSquare);
		this->m_square->ReduceArea(1.0);
	}
	//���w�i�\��
	if(++this->m_uiFrameNo < FADECNT){
		this->m_square->SetRGBA(1.0,1.0,1.0,(1.0f/FADECNT)*this->m_uiFrameNo);
	}else{
		//���[�h�ύX
		int bit =(this->m_uiWin == WIN_ME)?this->m_iMyAchieveBit:(this->m_uiWin == WIN_COM)?this->m_iComAchieveBit:0;
		this->m_square->ChangeMode(this->m_square->MODE_RESULT,bit);
		//BGM
		this->m_sound = &md->m_sndWin;
		SendMessage(md->m_hWnd,WM_SOUND,TRUE,NULL);
		rtnFlg = FALSE;
	}
	//�ĕ`��
	md->SetupRepaint(this->m_square->returnRect());

	return rtnFlg;
}


//�����̔Ԃ��ǂ���
BOOL PlayScene::IsMyTurn(void)
{
	return (this->m_uiTurnNo + Mediator::GetInstance()->GetOyaFlg())%2;
}

//�D�x�N�g�����ꊇ����
BOOL PlayScene::AllFudaFunc(std::vector<Fuda*> &vFuda,int func,double param,BOOL flag)
{

	std::vector<Fuda*>::iterator it = vFuda.begin();
	BOOL rtnFlg = FALSE;
	int i=0;
	for(it=vFuda.begin();it != vFuda.end();it++){
		Fuda* fuda = *it;
		if(fuda){
			switch(func){
				case FUNC_PAINT://�`��
					fuda->Paint(Mediator::GetInstance()->m_gOffScreen);
					break;
				case FUNC_SELECTABLE://�I���\�D�̃t���O��؂�ւ�
					if(this->IsSameMonth(fuda,(UINT)param)){
						fuda->SetupSelectable(flag);					
						rtnFlg = TRUE;
					}
					break;
				case FUNC_SELECTED://�I�𒆎D�̃t���O��؂�ւ�
					if(this->IsSameMonth(fuda,(UINT)param)){
						fuda->SetupSelected(flag);					
						rtnFlg = TRUE;
					}
					break;
				case FUNC_RESET://�I�����Z�b�g
					fuda->ResetSelect();
					break;
				case FUNC_INVISIBLE://��I����D��s���ɂ���
					if(!this->IsSameMonth(fuda,(UINT)param)) fuda->SetupInvisible(flag);
					break;
				case FUNC_SORT://�D���ёւ�
					this->SortFuda(fuda,(int)param,i);
					break;
				case FUNC_ERASE://�폜
					if(i==(int)param){
						vFuda.erase(it);
						return TRUE;
					}
					break;
				case FUNC_COPY://�R�s�[
					UINT id = fuda->GetID();
					this->m_square->m_AFuda.push_back(Mediator::GetInstance()->m_Fuda[id]);
					break;
			}
		}
		i++;
	}

	return rtnFlg;
}


//�I���\�ȎD�Ƀt���O�𗧂Ă�
void PlayScene::SetupSelectable(void)
{
	std::vector<UINT> rptMon;//�d����
	std::vector<Fuda*>::iterator itMF;

	itMF = this->m_MyFuda.begin();
	for(itMF=this->m_MyFuda.begin();itMF != this->m_MyFuda.end();itMF++){
		Fuda* mf = *itMF;
		UINT mon = mf->GetID()/FUDACNT;
		BOOL repFlg = FALSE;
		if(!rptMon.empty()){//�d�����͏ȗ�
			std::vector<UINT>::iterator itMon = rptMon.begin();
			for(itMon=rptMon.begin();itMon!=rptMon.end();itMon++){
				UINT* tMon = &*itMon;
				if(mon==*tMon){
					mf->SetupSelectable(TRUE);
					continue;
				}
			}
		}
		//�I���\�ȏ�D�Ƀt���O�𗧂Ă�
		if(this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTABLE,mon,TRUE)){
			//��D�Ƀt���O�𗧂Ă�
			rptMon.push_back(mon);
			mf->SetupSelectable(TRUE);
		}
	}
}


//�������̎D���H
BOOL PlayScene::IsSameMonth(Fuda* fuda,UINT mon)
{	
	if(mon==fuda->GetID()/FUDACNT) return TRUE;
	return FALSE;
}


//���킹���D���i�[����
BOOL PlayScene::HouseBF(BOOL teFlg,UINT mon)
{
	int bit = 1;
	UINT cnt = 0;
	Fuda** lplpBaFuda = (teFlg)?this->m_GetBFuda1:this->m_GetBFuda2;
	int* lpBFbit = (teFlg)?&this->m_iBFbit1:&this->m_iBFbit2;
	int layer = (teFlg)?CUT_B1GET:CUT_B2GET;
	this->m_iBFbitBlank = 0;

	try{
		std::vector<Fuda*>::iterator it = this->m_BaFuda.begin();
		while(it != this->m_BaFuda.end()){
			Fuda* bf = *it;
			if(!bf){
				if(!(bit & (this->m_iBFbit1 | this->m_iBFbit2))) this->m_iBFbitBlank += bit;
			}else{
				if(this->IsSameMonth(bf,mon)){
					//�l����D���i�[
					lplpBaFuda[cnt++] = bf;
					//�r�b�g���Z
					*lpBFbit += bit;
					//��D�폜
					*it = NULL;
					//��D���C���[���d�˂�
					if(cnt==1) this->m_vLayer.insert(this->m_vLayer.begin(),layer);
				}
			}
			bit <<= 1;
			it++;
		}
	}
	catch(...)
	{
		// ����A
		OutputDebugString(L"HouseBF");

	}
	
	//�Ō���̋󔒃r�b�g���Z�b�g����
	if(!this->m_iBFbitBlank) this->m_iBFbitBlank += 1 << this->m_BaFuda.size();

	//��D��D���C���[���d�˂�
	layer = (teFlg)?CUT_TGET:CUT_DGET;	
	this->m_vLayer.insert(this->m_vLayer.begin(),layer);

	//2���Ȃ�I���J�b�g��
	if(cnt == 2) return FALSE;

	return TRUE;
}



//�I�����Ȃ����̏�D�����ɖ߂�
BOOL PlayScene::ReturnBF(BOOL teFlg,Fuda* lpBaFuda,UINT i)
{
	int* lpBFbit = (teFlg)?&this->m_iBFbit1:&this->m_iBFbit2;
	int sort = Mediator::GetInstance()->ExtBitOrder(*lpBFbit,BAFUDACNT,i);
	if(sort<0) return FALSE;
	int bit = 1 << sort;

	//�D��߂�
	this->m_BaFuda[sort] = lpBaFuda;
	//�r�b�g��߂�
	*lpBFbit -= bit;

	return TRUE;
}


//�I��s�̎D����D�ɉ�����
void PlayScene::AddBF(BOOL teFlg)
{

	int bit = 1;
	Fuda* lpFuda = (teFlg)?this->m_GetTFuda:this->m_GetDFuda;
	BOOL addFlg = FALSE;

	//�󂢂Ă�ꏊ�ɑ}��
	for(UINT i=0;i<this->m_BaFuda.size();i++){
		if(!this->m_BaFuda[i]){
			if(teFlg || !(this->m_iBFbit1 & (bit << i))){
				this->m_BaFuda[i] = lpFuda;
				return;
			}
		}
	}

	//�󂢂ĂȂ��̂ōŌ���ɓ����
	this->m_BaFuda.push_back(lpFuda);
}


//�l���ώD���i�[����
void PlayScene::HouseGF(Fuda **lplpFuda,UINT cnt)
{
	Mediator* md = Mediator::GetInstance();
	UINT i;
	for(i=0;i<cnt;i++){
		if(lplpFuda[i] == NULL) break;
		UINT kind = lplpFuda[i]->GetKind();
		if(this->IsMyTurn()) this->m_GMyFuda[kind].push_back(lplpFuda[i]);
		else this->m_GComFuda[kind].push_back(lplpFuda[i]);
		Strategy::GetInstance()->SetupGFudaBit(this->IsMyTurn(),lplpFuda[i]->GetID(),lplpFuda[i]->GetKind());
		lplpFuda[i] = NULL;
	}
}


//�D����ёւ���
void PlayScene::SortFuda(Fuda* fuda,int fudakind,int i)
{
	int nX;
	int size;
	int maxSize;
	switch(fudakind){
		case SORT_TEFUDA://��D
			nX = this->m_ptMF.X;
			size = this->m_MyFuda.size();
			maxSize = TEFUDACNT;
			break;
		case SORT_COMFUDA://����D���f�o�b�O�p
			nX = this->m_ptCF.X;
			size = this->m_ComFuda.size();
			maxSize = TEFUDACNT;
			break;
		case SORT_GETFUDA://�l���ςݎD(�J�X)
			int kind;
			kind = Mediator::GetInstance()->KIND_KASU;
			nX = this->m_ptGMF.X;
			size = (this->IsMyTurn())?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
			maxSize = GFUDACNT;
			break;
	}
	//max�T�C�Y��������A�l�߂ĕ\��
	nX += (size <= maxSize)?i*fuda->GetWidth():(int)(i*fuda->GetWidth()*(double)(maxSize-1)/(size-1));
	//�ĕ\��
	if(fuda->GetPos().X != nX){
		Mediator::GetInstance()->SetupRepaint(fuda->returnRect());
		fuda->SetPos(Point(nX,fuda->_y));
		Mediator::GetInstance()->SetupRepaint(fuda->returnRect());
	}

}

//�w��̃��C���[����ԏ�ɂ���
void PlayScene::SortLayer(int layer)
{
	std::vector<int>::iterator it = std::find(this->m_vLayer.begin(),this->m_vLayer.end(),layer);
	std::rotate(this->m_vLayer.begin(),it,this->m_vLayer.end());
}


//�B���������D��\������
void PlayScene::DispAcheiveFuda(void)
{
	Strategy* st = Strategy::GetInstance();
	Mediator* md = Mediator::GetInstance();
	std::vector<Fuda*>* gFuda = (this->IsMyTurn())?this->m_GMyFuda:this->m_GComFuda;//�l���ώD

	//�\�����D���i�[
	int order = md->ExtBitOrder(this->m_iAchieveBit,md->YAKUCNT);
	switch(order){
		case st->YAKU_GOKO:
		case st->YAKU_SHIKO:
		case st->YAKU_AMESHIKO:
		case st->YAKU_SANKO:
			this->AllFudaFunc(gFuda[md->KIND_GOKO],FUNC_COPY);
			break;
		case st->YAKU_HANAMI:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_SAKURA]);
			break;
		case st->YAKU_TSUKIMI:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_TSUKI]);
			break;
		case st->YAKU_AKATAN:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA1]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA2]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AKA3]);
			break;
		case st->YAKU_AOTAN:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO1]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO2]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_AO3]);
			break;
		case st->YAKU_INOSHIKA:
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_INO]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_SHIKA]);
			this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_CHOU]);
			break;
		case st->YAKU_TANE:
			this->AllFudaFunc(gFuda[md->KIND_TANE],FUNC_COPY);
			break;
		case st->YAKU_TAN:
			this->AllFudaFunc(gFuda[md->KIND_TAN],FUNC_COPY);
			break;
		case st->YAKU_KASU:
			this->AllFudaFunc(gFuda[md->KIND_KASU],FUNC_COPY);
			if(st->IsBakeFuda(this->IsMyTurn())) this->m_square->m_AFuda.push_back(md->m_Fuda[st->FUDA_BAKE]);
			break;
	}

	//�D����ׂ�
	this->m_square->Sort();
	//���[�h�ύX
	this->m_square->ChangeMode(this->m_square->MODE_ACHIEVE,this->m_iAchieveBit);
	//�r�b�g���N���A�[
	this->m_iAchieveBit ^= 1 << order;	

}

//�B��������r�b�g�����Z����
void PlayScene::AddAcheiveBit(int bit)
{
	Strategy* st = Strategy::GetInstance();

	int* lpABit = (this->IsMyTurn())?&this->m_iMyAchieveBit:&this->m_iComAchieveBit;
	//�O���ȏ�̃r�b�g����U�I�t
	int gokoBit = (1 << st->YAKU_GOKO) | (1 << st->YAKU_SHIKO) | (1 << st->YAKU_AMESHIKO) | (1 << st->YAKU_SANKO);
	if(bit & gokoBit) *lpABit &= ~gokoBit;
	//���Z
	*lpABit |= bit;
}


//������
void PlayScene::Init(void){

	Mediator* md = Mediator::GetInstance();

	//�D���Z�b�g����
	this->SetupFuda();
	//���ꗗ
	this->m_ylComYaku.Init(this->m_ptCY);
	this->m_ylMyYaku.Init(this->m_ptMY);
	//***������摜***
	//��
	this->m_siMonth = &md->m_siMonth[md->NxtKyoku(FALSE)];
	this->m_siMonth->Init(this->m_ptMonth);
	//���O
	this->m_siCom->Init(this->m_ptCom);
	this->m_siYou->Init(this->m_ptYou);
	//�X�R�A�[
	this->m_siScore.clear();
	Point score = md->GetScore();
	md->HouseScore(score.X,this->m_siScore,this->m_ptCScore);
	md->HouseScore(score.Y,this->m_siScore,this->m_ptMScore);
	//�u���v
	this->m_siMon[0].Init(this->m_ptCScore);
	this->m_siMon[1].Init(this->m_ptMScore);
	//�����z���}�[�N
	this->m_uiWin = WIN_OFF;
	//�V�[���Z�b�g
	this->m_uiSceneNo = SCENE_DEAL;
	this->m_uiCutNo = CUT_TSELECT;
	this->m_uiFrameNo = 0;
	this->m_uiTurnNo = 0;
	//�r�b�g
	this->m_iBFbit1 = 0;//��D�l���r�b�g1
	this->m_iBFbit2 = 0;//��D�l���r�b�g2
	this->m_iBFbitBlank = 0;//��D�󔒃r�b�g
	this->m_iAchieveBit = 0;//�B�����r�b�g
	this->m_iMyAchieveBit = 0;//�B�����r�b�g(����)
	this->m_iComAchieveBit = 0;//�B�����r�b�g(COM)

}

//���Z�b�g
void PlayScene::Reset(void){

	Mediator* md = Mediator::GetInstance();
	//�D
	int i;
	for(i=0;i<UFUDACNT;i++){//���D
		this->m_UFuda[i].Reset();
	}
	for(i=0;i<md->ALLFUDACNT;i++){//�\�D
		md->m_Fuda[i].Reset();
	}
	this->m_BaFuda.clear();//��D
	this->m_ComFuda.clear();//����D
	this->m_MyFuda.clear();//���D
	this->m_DaiFuda.clear();//��D
	for(i=0;i<FUDACNT;i++){
		this->m_GMyFuda[i].clear();//�l���ώ�D
		this->m_GComFuda[i].clear();//�l���ϑ���D
	}
	//���ꗗ
	this->m_ylComYaku.Reset();
	this->m_ylMyYaku.Reset();
	//***������摜***
	//���O
	this->m_siCom->Reset();
	this->m_siYou->Reset();
	//�X�R�A�[
	this->m_siScore.clear();
	//�u���v
	this->m_siMon[0].Reset();
	this->m_siMon[1].Reset();
	//��
	this->m_siMonth->Reset();
	//���w�i
	this->m_square->Reset();
	//�u�͂��v�u�������v
	this->m_slHai->Reset();
	this->m_slIie->Reset();
	//�헪
	Strategy::GetInstance()->Reset();
	//���ʉ�
	SendMessage(md->m_hWnd,WM_SOUND,FALSE,NULL);
}

//�}�E�X�ړ����̏���
BOOL PlayScene::MouseMove(double x,double y){

	//��D�I����
	BOOL rtnFlg = FALSE;
	if(this->m_uiCutNo==CUT_TSELECT){
		std::vector<Fuda*>::iterator it = this->m_MyFuda.begin();
		UINT sMon = -1;
		for(it=this->m_MyFuda.begin();it!=this->m_MyFuda.end();it++){
			Fuda* fuda = *it;
			UINT mon = fuda->GetID()/FUDACNT;
			if(fuda->RollOver(x,y)){
				//�I����D�Ƀt���O�𗧂Ă�
				if(this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTED,mon,TRUE)) sMon = mon;
				//��ɎD�����炷
				fuda->SetPos(Point(fuda->_x,fuda->_y-10));
				rtnFlg = TRUE;
			}else if(fuda->RollOut(x,y)){
				//�I����D�̃t���O��߂�
				if(sMon != mon) this->AllFudaFunc(this->m_BaFuda,FUNC_SELECTED,mon,FALSE);
				//���ɎD��߂�
				fuda->SetPos(Point(fuda->_x,fuda->_y+10));
				rtnFlg = TRUE;
			}
		}
	}else if(this->m_uiCutNo==CUT_BSELECT1 || this->m_uiCutNo==CUT_BSELECT2){//��D�I����
		Fuda* bFuda0 = (this->m_uiCutNo==CUT_BSELECT1)?this->m_GetBFuda1[0]:this->m_GetBFuda2[0];
		Fuda* bFuda1 = (this->m_uiCutNo==CUT_BSELECT1)?this->m_GetBFuda1[1]:this->m_GetBFuda2[1];
		if(bFuda0->MouseMove(x,y)) rtnFlg = TRUE;
		else if(bFuda1->MouseMove(x,y)) rtnFlg = TRUE;
	}else if(this->m_uiCutNo==CUT_KOIKOI && this->IsMyTurn()){//���������I����
		if(this->m_slHai->MouseMove(x,y)) rtnFlg = TRUE;
		else if(this->m_slIie->MouseMove(x,y)) rtnFlg = TRUE;
	}else if(this->m_uiCutNo==CUT_RESULT){//���ʕ\����
		this->m_square->MouseMove(x,y);//�J�[�\���ύX
	}
	return rtnFlg;
}

//�}�E�X�N���b�N���̏���
BOOL PlayScene::MouseClick(double x,double y){

	BOOL rtnFlg = FALSE;
	if(this->m_uiCutNo==CUT_TSELECT){//��D�I����
		std::vector<Fuda*>::iterator it = this->m_MyFuda.begin();
		for(it=this->m_MyFuda.begin();it!=this->m_MyFuda.end();it++){
			Fuda* fuda = *it;
			if(fuda->MouseClick(x,y)){
				//�S�D�̑I����Ԃ����Z�b�g����
				this->AllFudaFunc(this->m_MyFuda,FUNC_RESET);
				this->AllFudaFunc(this->m_BaFuda,FUNC_RESET);
				//�l����D���i�[
				if(this->HouseBF(TRUE,fuda->GetID()/FUDACNT)){
					this->m_uiCutNo = CUT_TPUT;//��D�ړ��J�b�g��
					rtnFlg = TRUE;//�X���b�h�ĊJ
				}else{
					this->m_uiCutNo = CUT_BSELECT1;//��D�I���J�b�g��
					this->Play();
					Mediator::GetInstance()->Repaint();
					rtnFlg = FALSE;//�X���b�h�͂��̂܂�
				}
				//�l����D���i�[
				this->m_GetTFuda = fuda;
				this->m_MyFuda.erase(it);
				return rtnFlg;
			}
		}
	}else if(this->m_uiCutNo==CUT_BSELECT1 || this->m_uiCutNo==CUT_BSELECT2){//��D�I����
		Fuda** bFuda0 = (this->m_uiCutNo==CUT_BSELECT1)?&this->m_GetBFuda1[0]:&this->m_GetBFuda2[0];
		Fuda** bFuda1 = (this->m_uiCutNo==CUT_BSELECT1)?&this->m_GetBFuda1[1]:&this->m_GetBFuda2[1];
		BOOL teFlg = (this->m_uiCutNo==CUT_BSELECT1)?TRUE:FALSE;

		if((*bFuda0)->MouseClick(x,y)){
			rtnFlg = this->ReturnBF(teFlg,*bFuda1,1);
		}else if((*bFuda1)->MouseClick(x,y)){
			rtnFlg = this->ReturnBF(teFlg,*bFuda0,0);
			*bFuda0 = *bFuda1;
		}
		if(rtnFlg){
			(*bFuda0)->ResetSelect();
			(*bFuda1) = NULL;
			this->AllFudaFunc(this->m_BaFuda,FUNC_RESET);
			this->m_uiCutNo = (this->m_uiCutNo==CUT_BSELECT1)?CUT_TPUT:CUT_DPUT;//��Dor��D�ړ��J�b�g��
		}

	}else if(this->m_uiCutNo==CUT_KOIKOI && this->IsMyTurn()){//���������I����
		if(this->m_slHai->MouseClick(x,y)){//�͂�
			rtnFlg = TRUE;
			this->m_uiCutNo=CUT_TSELECT;//��D�I���J�b�g��
			this->m_uiTurnNo++;//���̃^�[����
		}else if(this->m_slIie->MouseClick(x,y)){//������
			rtnFlg = TRUE;
			this->m_uiCutNo=CUT_RESULT;//���ʃJ�b�g��
		}
		this->m_slHai->Reset();
		this->m_slIie->Reset();
	}else if(this->m_uiCutNo==CUT_RESULT){//���ʕ\��
		if(this->m_square->MouseClick(x,y)){
			this->Reset();
			Mediator* md = Mediator::GetInstance();
			if(md->NxtKyoku(TRUE) == md->MONCNT){//�Q�[���I��
				//�ŏI�V�[����
				md->ChangeScene(LastScene::GetInstance());
			}else{
				this->Init();
			}
			rtnFlg = TRUE;
		}
	}
	return rtnFlg;
}


// �D�𓮂���
void PlayScene::Move(void){
	if(this->m_uiSceneNo == SCENE_DEAL) this->Deal();//�z�D
	else if(this->m_uiSceneNo == SCENE_PLAY){//�ΐ�
		//�D�I�����A�X���b�h�ꎞ��~
		if(!this->Stop()){
			if(!this->Play()){
				Mediator* md = Mediator::GetInstance();
				md->Repaint();
				md->RunThread(FALSE);
				//���[���I�[�o�[�m�F
				POINT pt;
				GetCursorPos(&pt);
				ScreenToClient(md->m_hWnd,&pt);
				this->MouseMove(pt.x,pt.y);
				md->Repaint();
				SendMessage(md->m_hWnd,WM_SETCURSOR,NULL,NULL);
			}
		}
	}
}

// �`��
void PlayScene::Paint(void){
	Mediator* md = Mediator::GetInstance();
	int i;

	//���D(��D0�{����D1~8)
	for(i=0;i<TEFUDACNT+1;i++) this->m_UFuda[i].Paint(md->m_gOffScreen);
	//��D
	this->AllFudaFunc(this->m_BaFuda,FUNC_PAINT);
	//��
	this->m_siMonth->Paint(md->m_gOffScreen);
	//����
	this->m_siCom->Paint(md->m_gOffScreen);
	this->m_siYou->Paint(md->m_gOffScreen);
	//���_
	md->PaintStrImage(this->m_siScore,md->m_gOffScreen);
	//��
	for(i=0;i<2;i++) this->m_siMon[i].Paint(md->m_gOffScreen);
	//���ꗗ
	this->m_ylComYaku.Paint(md->m_gOffScreen);
	this->m_ylMyYaku.Paint(md->m_gOffScreen);

	//�z�D�V�[��
	if(this->m_uiSceneNo == SCENE_DEAL){
		//���D(��D9~17�{��D18~21)
		for(i=TEFUDACNT+1;i<UFUDACNT;i++) this->m_UFuda[i].Paint(md->m_gOffScreen);
	}else{//�ΐ�V�[��

		//��D
		this->AllFudaFunc(this->m_MyFuda,FUNC_PAINT);
		//����D���f�o�b�O�p
		if(md->m_bDebugFlg) this->AllFudaFunc(this->m_ComFuda,FUNC_PAINT);
		//�l���ώD
		for(i=0;i<FUDAKIND;i++){
			this->AllFudaFunc(this->m_GMyFuda[i],FUNC_PAINT);
			this->AllFudaFunc(this->m_GComFuda[i],FUNC_PAINT);
		}

		//�l���D(���C���[���ɕ`�悷��)
		try{
			std::vector<int>::reverse_iterator rit = this->m_vLayer.rbegin();
			while(rit != this->m_vLayer.rend()){
				int layer = *rit;
				switch(layer){
					case CUT_B1GET://�l����D1
						for(i=0;i<FUDACNT-1;i++) if(this->m_GetBFuda1[i]) this->m_GetBFuda1[i]->Paint(md->m_gOffScreen);
						break;
					case CUT_B2GET://�l����D2
						for(i=0;i<FUDACNT-1;i++) if(this->m_GetBFuda2[i]) this->m_GetBFuda2[i]->Paint(md->m_gOffScreen);
						break;
					case CUT_TGET://�l����D
						if(this->m_GetTFuda) this->m_GetTFuda->Paint(md->m_gOffScreen);
						break;
					case CUT_DGET://�l����D
						if(this->m_GetDFuda) this->m_GetDFuda->Paint(md->m_gOffScreen);
						break;
				}
				rit++;
			}
		}catch(...){
			OutputDebugString(L"Paint");
		}
		//���w�i
		this->m_square->Paint(md->m_gOffScreen);
		//���������I��
		if(this->m_uiCutNo == CUT_KOIKOI && this->IsMyTurn()){
			this->m_slHai->Paint(md->m_gOffScreen);
			this->m_slIie->Paint(md->m_gOffScreen);
		}
	}

}


//�B��������̕������擾
UINT PlayScene::GetAcheiveScore(int bit)
{
	Mediator* md = Mediator::GetInstance();
	Strategy* st = Strategy::GetInstance();

	//�e��
	if(!bit) return md->SCORE_OYAKEN;

	int kind = 0;
	int size = 0;
	int order =md->ExtBitOrder(bit,md->YAKUCNT);
	UINT score = md->m_uiScore[order];
	BOOL myFlg = (this->m_uiWin==WIN_ME)?TRUE:FALSE;
	//�^�l�A�^���A�J�X�������������Z
	switch(order){
		case st->YAKU_TANE:
			kind = md->KIND_TANE;
			break;
		case st->YAKU_TAN:
			kind = md->KIND_TAN;
			break;
		case st->YAKU_KASU:
			kind = md->KIND_KASU;
			if(st->IsBakeFuda(myFlg)) size = 1;
			break;
	}
	if(kind){
		size += (myFlg)?this->m_GMyFuda[kind].size():this->m_GComFuda[kind].size();
		score += (order == st->YAKU_KASU)?size-10:size-5;
	}

	return score;
}


//�B�����̍��v�X�R�A�[��ǉ�
void PlayScene::AddScore(int score)
{
	Mediator* md = Mediator::GetInstance();
	BOOL myFlg = (this->m_uiWin==WIN_COM)?FALSE:(this->m_uiWin==WIN_ME)?TRUE:md->GetOyaFlg();
	//���Z
	md->AddScore(score,myFlg);
	//�擾
	Point pt = md->GetScore();
	//�i�[
	this->m_siScore.clear();
	md->HouseScore(pt.X,this->m_siScore,this->m_ptCScore);
	md->HouseScore(pt.Y,this->m_siScore,this->m_ptMScore);
}

//�^�[��No
UINT PlayScene::GetTurnNo(void)
{
	return this->m_uiTurnNo;
}
