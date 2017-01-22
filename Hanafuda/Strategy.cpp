#include "StdAfx.h"
#include "Strategy.h"
#include "PlayScene.h"


Strategy::~Strategy(void){}

//�I������D����Ԃ�
UINT Strategy::SelectCF(void)
{
	PlayScene* ps = PlayScene::GetInstance();
	std::vector<Fuda*>::iterator cIT = ps->m_ComFuda.begin();
	int maxIndex = 0;//�I���w���ő�l
	UINT rtnSort = 0;//�D��
	UINT i=0;

	for(cIT=ps->m_ComFuda.begin();cIT != ps->m_ComFuda.end();cIT++){
		Fuda* cFuda = *cIT;
		int index = 0;//�I���w��
		if(cFuda){
			int bIndex = 0;//��D�I���w��
			std::vector<Fuda*>::iterator bIT = ps->m_BaFuda.begin();
			while(bIT != ps->m_BaFuda.end()){
				Fuda* bFuda = *bIT;
				if(bFuda){
					//������D�����݂���
					if(ps->IsSameMonth(bFuda,cFuda->GetID()/ps->FUDACNT)){
						//��D�̑I���w�����Z�o
						bIndex = max(bIndex,this->CulcIndex(bFuda->GetID(),bFuda->GetKind()));							
					}
				}
				bIT++;
			}
			//��D�̑I���w�������Z
			index += bIndex;
			//��D�̑I���w�������Z
			BOOL matchFlg = (!bIndex)?FALSE:TRUE;
			index += this->CulcIndex(cFuda->GetID(),cFuda->GetKind(),matchFlg);

			//���f�o�b�O�p
			if(Mediator::GetInstance()->m_bDebugFlg){
				WCHAR dbg[50];
				wsprintf(dbg,_T("%d:%d\r\n"),i,index);
				OutputDebugString(dbg);
			}

			//�I���w�����r
			if(i == 0 || maxIndex < index){
				maxIndex = index;
				rtnSort = i;
			}
		}

		i++;
	}

	return rtnSort;
}

//�I���w�����Z�o����
int Strategy::CulcIndex(UINT id,UINT kind,BOOL matchFlg)
{
	int index = 0;
	UINT fudaBit = 0;
	UINT yakuBit = 0;

	//�r�b�g�𗧂Ă�
	this->SetupBit(&fudaBit,&yakuBit,id,kind);

	//�Z�o
	index += this->CulcGoko(yakuBit,matchFlg);
	index += this->CulcBake(yakuBit,matchFlg);
	index += this->CulcAkatan(yakuBit,matchFlg);
	index += this->CulcAotan(yakuBit,matchFlg);
	index += this->CulcInoshika(yakuBit,matchFlg);
	index += this->CulcTane(yakuBit,matchFlg);
	index += this->CulcTan(yakuBit,matchFlg);
	index += this->CulcKasu(yakuBit,matchFlg);

	return index;
}


//�܌��D�̑I���w�����Z�o����
int Strategy::CulcGoko(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//�܌��`�F�b�N
	UINT compBit = (1 << YAKUBIT_AME) | (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_GOKO);
	if(yakuBit & compBit){

		//�J�H
		UINT pt = (yakuBit & (1 << YAKUBIT_AME))?1:2;

		//�܌��D�̊l���󋵂��擾����(�J:1pt,�J�ȊO:2pt)
		WPARAM cnt = this->CheckGetGoko(this->m_uiComBit,this->m_uiMyBit);
		WORD comCnt = LOWORD(cnt);
		WORD myCnt = HIWORD(cnt);
		UINT kouCnt = (matchFlg)?comCnt:myCnt;
		UINT otsuCnt = (matchFlg)?myCnt:comCnt;
		int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;

		//(0)�D�w��(�܌�=20�_)�����Zor���Z
		//���u�J�擾�{��������{�������v��������
		if(!(pt == 1 && kouCnt == 4 && otsuCnt == 4)) index += (matchFlg)?INDEX_GOKO:-INDEX_GOKO;

		//***�l����***
		//(1)��B�����l�������ݕ����~100�_
		//(2)���[�`�������遨�l�������ݕ����~50�_
		//***�̎D��***
		//(1)��B�����X�N���l�������ݕ����~(-100)�_
		//(2)���[�`���X�N���l�������ݕ����~(-50)�_
		switch(kouCnt + pt){
			case 4://�O�����[�`
				if(otsuCnt < 4) index += Mediator::SCORE_SANKO * score;//(2)
				break;
			case 5://�J�l�����[�`
				if(otsuCnt < 4) index += Mediator::SCORE_AMESHIKO * score;//(2)
				break;
			case 6://�O��
				index += Mediator::SCORE_SANKO * score * 2;//(1)
				break;
			case 7://�J�l��
				index += Mediator::SCORE_AMESHIKO * score * 2;//(1)
				break;
			case 8://�l��
				index += Mediator::SCORE_SHIKO * score * 2;//(1)
				break;
			case 9://�܌�
				index += Mediator::SCORE_GOKO * score * 2;//(1)
				break;
		}

		//***�l����***
		//(3)�B���j�~���l�������ݕ����~100�_
		//***�̎D��***
		//(3)�B���j�~���X�N�����l�������ݕ����~100�_
		switch(otsuCnt + pt){
			case 6://�O��
				index += Mediator::SCORE_SANKO * score * 2;
				break;
			case 7://�J�l��
				index += Mediator::SCORE_AMESHIKO * score * 2;
				break;
			case 8://�l��
				index += Mediator::SCORE_SHIKO * score * 2;
				break;
			case 9://�܌�
				index += Mediator::SCORE_GOKO * score * 2;
				break;
		}
	}

	return index;
}


//�Ԍ������D�̑I���w�����Z�o����
int Strategy::CulcBake(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//�Ԍ������`�F�b�N
	UINT compBit = (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_BAKE);
	if(yakuBit & compBit){

		//�Ԍ������D�̊l���󋵂��擾����
		int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;
		UINT kouBit = (matchFlg)?this->m_uiComBit:this->m_uiMyBit;
		UINT otsuBit = (!matchFlg)?this->m_uiComBit:this->m_uiMyBit;

		//***�l����***
		//(1)��B�����l�������ݕ����~100�_
		//(2)���[�`�������遨�l�������ݕ����~50�_
		//(3)�B���j�~���l�������ݕ����~100�_
		//***�̎D��***
		//(1)��B�����X�N���l�������ݕ����~(-100)�_
		//(2)���[�`���X�N���l�������ݕ����~(-50)�_
		//(3)�B���j�~���X�N�����l�������ݕ����~100�_
		if(yakuBit & (1 << YAKUBIT_BAKE)){//�����D�l��or�̂�

			//(1)�Ԍ��B��or�Ԍ��B�����X�N
			if(kouBit & (1 << FUDABIT_SAKURA)) index += score * 2 * Mediator::SCORE_HANAMI;
			//(2)�Ԍ����[�`or�Ԍ����[�`���X�N
			else if(!(otsuBit & (1 << FUDABIT_SAKURA))) index += score * Mediator::SCORE_HANAMI;

			//(3)�B���j�~or�B���j�~���X�N
			if(otsuBit & (1 << FUDABIT_SAKURA)) index += score * 2 * Mediator::SCORE_HANAMI;

			//(1)�����B��or�����B�����X�N
			if(kouBit & (1 << FUDABIT_TSUKI)) index += score * 2 * Mediator::SCORE_TSUKIMI;
			//�������[�`
			else if(!(otsuBit & (1 << FUDABIT_TSUKI))) index += score * Mediator::SCORE_TSUKIMI;

			//(3)�B���j�~or�B���j�~���X�N
			if(otsuBit & (1 << FUDABIT_TSUKI)) index += score * 2 * Mediator::SCORE_TSUKIMI;

		}
		else if(yakuBit & (1 << YAKUBIT_SAKURA)){//���l��or�̂�

			//(1)�Ԍ��B��or�Ԍ��B�����X�N
			if(kouBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_HANAMI;
			//(2)�Ԍ����[�`or�Ԍ����[�`���X�N
			else if(!(otsuBit & (1 << FUDABIT_BAKE))) index += score * Mediator::SCORE_HANAMI;

			//(3)�B���j�~or�B���j�~���X�N
			if(otsuBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_HANAMI;
		}
		else if(yakuBit & (1 << YAKUBIT_TSUKI)){//���l��or�̂�

			//(1)�����B��or�����B�����X�N
			if(kouBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_TSUKIMI;
			//(2)�������[�`or�������[�`���X�N
			else if(!(otsuBit & (1 << FUDABIT_BAKE))) index += score * Mediator::SCORE_TSUKIMI;

			//(3)�B���j�~or�B���j�~���X�N
			if(otsuBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_TSUKIMI;
		}

	}

	return index;
}

//�ԒZ�D�̑I���w�����Z�o����
int Strategy::CulcAkatan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_AKATAN))?this->CulcTriple(yakuBit,matchFlg):0;
}

//�Z�D�̑I���w�����Z�o����
int Strategy::CulcAotan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_AOTAN))?this->CulcTriple(yakuBit,matchFlg):0;
}

//�������D�̑I���w�����Z�o����
int Strategy::CulcInoshika(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_INOSHIKA))?this->CulcTriple(yakuBit,matchFlg):0;
}

//3�����D(�ԒZ�E�Z�E������)�̑I���w�����Z�o����
int Strategy::CulcTriple(UINT yakuBit,BOOL matchFlg)
{

	int index = 0;

	//�D�̊l���󋵂��擾����
	WPARAM cnt = 0;
	int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;

	if(yakuBit & (1 << YAKUBIT_AKATAN)){//�ԒZ
		cnt = this->CheckGetAkatan(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_AKATAN;
	}else if(yakuBit & (1 << YAKUBIT_AOTAN)){//�Z
		cnt = this->CheckGetAotan(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_AOTAN;
	}else if(yakuBit & (1 << YAKUBIT_INOSHIKA)){//������
		cnt = this->CheckGetInoshika(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_INOSHIKA;
	}

	WORD comCnt = LOWORD(cnt);
	WORD myCnt = HIWORD(cnt);
	UINT kouCnt = (matchFlg)?comCnt:myCnt;
	UINT otsuCnt = (matchFlg)?myCnt:comCnt;

	//(0)�D�w��(10�_)�����Zor���Z
	//���u�����ꖇ�{����ꖇ�v��������
	if(!(kouCnt == 1 && otsuCnt == 1)) index += (matchFlg)?INDEX_YAKU:-INDEX_YAKU;
	//***�l����***
	//(1)��B�����l�������ݕ����~100�_
	//(2)���[�`�������遨�l�������ݕ����~50�_
	//(3)�B���j�~���l�������ݕ����~100�_
	//***�̎D��***
	//(1)��B�����X�N���l�������ݕ����~(-100)�_
	//(2)���[�`���X�N���l�������ݕ����~(-50)�_
	//(3)�B���j�~���X�N���l�������ݕ����~(-100)�_
	switch(kouCnt + 1){
		case 3://(1)
			index += score * 2;
			break;
		case 2://(2)
			if(!otsuCnt) index += score;
			break;
		case 1://(3)
			if(otsuCnt == 2) index += score * 2;
			break;
	}

	return index;
}	



//�^�l�̑I���w�����Z�o����
int Strategy::CulcTane(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_TANE))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//�^���D�̑I���w�����Z�o����
int Strategy::CulcTan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_TAN))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//�J�X�D�̑I���w�����Z�o����
int Strategy::CulcKasu(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_KASU))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//�^�l�^���J�X�D�̑I���w�����Z�o����
int Strategy::CulcTaneTanKasu(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//�D�̊l���󋵂��擾����
	WPARAM cnt = 0;
	int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;
	UINT acheiveCnt = 0;

	//(0)�D�w��(�^�l�^��5�_,�J�X1�_)�����Zor���Z
	if(yakuBit & (1 << YAKUBIT_TANE)){//�^�l
		cnt = this->CheckGetTane(FALSE);
		score *= Mediator::SCORE_TANE;
		index += (matchFlg)?INDEX_TANE:-INDEX_TANE;//(0)
		acheiveCnt = ACHVTANECNT;
	}else if(yakuBit & (1 << YAKUBIT_TAN)){//�^��
		cnt = this->CheckGetTan(FALSE);
		score *= Mediator::SCORE_TAN;
		index += (matchFlg)?INDEX_TAN:-INDEX_TAN;//(0)
		acheiveCnt = ACHVTANCNT;
	}else if(yakuBit & (1 << YAKUBIT_KASU)){//�J�X
		cnt = this->CheckGetKasu(FALSE);
		score *= Mediator::SCORE_KASU;
		index += (matchFlg)?INDEX_KASU:-INDEX_KASU;//(0)
		acheiveCnt = ACHVKASUCNT;
	}

	WORD comCnt = LOWORD(cnt);
	WORD myCnt = HIWORD(cnt);
	UINT kouCnt = (matchFlg)?comCnt:myCnt;
	UINT otsuCnt = (matchFlg)?myCnt:comCnt;

	//***�l����***
	//(1)��B�����l�������ݕ����~100�_
	//(2)���[�`�������遨�l�������ݕ����~50�_
	//***�̎D��***
	//(1)��B�����X�N���l�������ݕ����~(-100)�_
	//(2)���[�`���X�N���l�������ݕ����~(-50)�_
	if(kouCnt + 1 >= acheiveCnt){//(1)
		index += score * 2;
	}else if(kouCnt + 1 == acheiveCnt-1){//(2)
		index += score;
	}

	return index;
}

//�ǂ��炩����̏�D��I������
UINT Strategy::AlternativeBF(UINT id1,UINT kind1,UINT id2,UINT kind2)
{
	int index1 = this->CulcIndex(id1,kind1);
	int index2 = this->CulcIndex(id2,kind2);
	UINT sort = (index1 >= index2)?0:1;
	return sort;
}

//�����������邩�H
BOOL Strategy::IsKoiKoi(void)
{
	PlayScene* ps = PlayScene::GetInstance();

	//(1)����̃��[�`���̗L�����m�F
	WPARAM cnt = 0;
	WORD comCnt = 0;
	WORD myCnt = 0;
	UINT reachBit = 0;

	//�^�l�D�̊l���󋵂��擾�����[�`�Ȃ炱���������Ȃ�
	cnt = this->CheckGetTane(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLTANECNT) && (myCnt >= ACHVTANECNT-1)) return FALSE;

	//�^���D�̊l���󋵂��擾�����[�`�Ȃ炱���������Ȃ�
	cnt = this->CheckGetTan(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLTANCNT) && (myCnt >= ACHVTANCNT-1)) return FALSE;

	//�J�X�D�̊l���󋵂��擾�����[�`�Ȃ炱���������Ȃ�
	cnt = this->CheckGetKasu(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLKASUCNT+1) && (myCnt >= ACHVKASUCNT-2)) return FALSE;

	//�܌��D�̊l���󋵂��擾����(�J:1pt,�܌�:2pt)
	cnt = this->CheckGetGoko(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((myCnt + comCnt < 9) && (comCnt < 4)){//�܌��D�c�肠��
		//���肪���[�`
		if(myCnt > 4) reachBit |= (1 << YAKU_GOKO);//�J�܂�
		else if(myCnt == 4) reachBit |= (1 << YAKU_SANKO);//�J�܂܂Ȃ�
	}
	//�Ԍ������̊l���󋵂��擾����(��4�^�[���ڂ���)
	if(ps->GetTurnNo()/2 >= 3){
		if(this->m_uiMyBit & (1 << FUDABIT_BAKE)){//���肪�����D������
			//��l�Ƃ����������Ă��Ȃ����Ԍ����[�`
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_SAKURA))) reachBit |= (1 << YAKU_HANAMI);
			//��l�Ƃ����������Ă��Ȃ����������[�`
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_TSUKI))) reachBit |= (1 << YAKU_TSUKIMI);
		}
		if(this->m_uiMyBit & (1 << FUDABIT_SAKURA)){//���肪��������
			//��l�Ƃ������D�������Ă��Ȃ����Ԍ����[�`
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_BAKE))) reachBit |= (1 << YAKU_HANAMI);
		}
		if(this->m_uiMyBit & (1 << FUDABIT_TSUKI)){//���肪��������
			//��l�Ƃ������D�������Ă��Ȃ����������[�`
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_BAKE))) reachBit |= (1 << YAKU_TSUKIMI);
		}
	}
	//�ԒZ�D�̊l���󋵂��擾����
	cnt = this->CheckGetAkatan(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_AKATAN);
	//�Z�D�̊l���󋵂��擾����
	cnt = this->CheckGetAotan(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_AOTAN);
	//�������D�̊l���󋵂��擾����
	cnt = this->CheckGetInoshika(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_INOSHIKA);

	//(2)��D�Ƀ����D������΂����������Ȃ�
	if(reachBit){

		std::vector<Fuda*>::iterator bIT = ps->m_BaFuda.begin();
		while(bIT != ps->m_BaFuda.end()){
			Fuda* bFuda = *bIT;
			if(bFuda){
				switch(bFuda->GetID()){
					case FUDA_AME:
						if(reachBit & (1 << YAKU_GOKO)) return FALSE;
						break;
					case FUDA_SAKURA:
						if(reachBit & ((1 << YAKU_GOKO) | (1 << YAKU_SANKO) | (1 << YAKU_HANAMI))) return FALSE;
						break;
					case FUDA_TSUKI:
						if(reachBit & ((1 << YAKU_GOKO) | (1 << YAKU_SANKO) | (1 << YAKU_TSUKIMI))) return FALSE;
						break;
					case FUDA_TSURU:
					case FUDA_KIRI:
						if(reachBit & ((1 << YAKU_GOKO) | (1 << YAKU_SANKO))) return FALSE;
						break;
					case FUDA_BAKE:
						if(reachBit & ((1 << YAKU_TSUKIMI) | (1 << YAKU_HANAMI))) return FALSE;
						break;
					case FUDA_AKA1:
					case FUDA_AKA2:
					case FUDA_AKA3:
						if(reachBit & (1 << YAKU_AKATAN)) return FALSE;
						break;
					case FUDA_AO1:
					case FUDA_AO2:
					case FUDA_AO3:
						if(reachBit & (1 << YAKU_AOTAN)) return FALSE;
						break;
					case FUDA_INO:
					case FUDA_SHIKA:
					case FUDA_CHOU:
						if(reachBit & (1 << YAKU_INOSHIKA)) return FALSE;
						break;
				}
			}
			bIT++;
		}
	}

	return TRUE;
}

//�w��D���Ƀr�b�g�𗧂Ă�
void Strategy::SetupBit(UINT* lpFudaBit,UINT* lpYakuBit,UINT id,UINT kind)
{
	Mediator* md = Mediator::GetInstance();
	int bit = 1;

	//���D
	switch(id){
		case FUDA_AME:
			*lpFudaBit |= bit << FUDABIT_AME;
			*lpYakuBit |= bit << YAKUBIT_AME;//�܌�(�J)
			break;
		case FUDA_SAKURA:
			*lpFudaBit |= bit << FUDABIT_SAKURA;
			*lpYakuBit |= bit << YAKUBIT_SAKURA;//�܌�(��)
			break;
		case FUDA_TSUKI:
			*lpFudaBit |= bit << FUDABIT_TSUKI;
			*lpYakuBit |= bit << YAKUBIT_TSUKI;//�܌�(��)
			break;
		case FUDA_TSURU:
			*lpFudaBit |= bit << FUDABIT_TSURU;
			*lpYakuBit |= bit << YAKUBIT_GOKO;//�܌�(�ߋ�)
			break;
		case FUDA_KIRI:
			*lpFudaBit |= bit << FUDABIT_KIRI;
			*lpYakuBit |= bit << YAKUBIT_GOKO;//�܌�(�ߋ�)
			break;
		case FUDA_BAKE:
			*lpFudaBit |= bit << FUDABIT_BAKE;
			*lpYakuBit |= bit << YAKUBIT_BAKE;//�����D
			*lpYakuBit |= bit << YAKUBIT_KASU;//�J�X
			break;
		case FUDA_INO:
			*lpFudaBit |= bit << FUDABIT_INO;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//������
			break;
		case FUDA_SHIKA:
			*lpFudaBit |= bit << FUDABIT_SHIKA;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//������
			break;
		case FUDA_CHOU:
			*lpFudaBit |= bit << FUDABIT_CHOU;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//������
			break;
		case FUDA_AKA1:
			*lpFudaBit |= bit << FUDABIT_AKA1;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//�ԒZ
			break;
		case FUDA_AKA2:
			*lpFudaBit |= bit << FUDABIT_AKA2;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//�ԒZ
			break;
		case FUDA_AKA3:
			*lpFudaBit |= bit << FUDABIT_AKA3;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//�ԒZ
			break;
		case FUDA_AO1:
			*lpFudaBit |= bit << FUDABIT_AO1;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//�Z
			break;
		case FUDA_AO2:
			*lpFudaBit |= bit << FUDABIT_AO2;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//�Z
			break;
		case FUDA_AO3:
			*lpFudaBit |= bit << FUDABIT_AO3;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//�Z
			break;
	}

	//�^�l�E�^���E�J�X
	switch(kind){
		case md->KIND_TANE:
			*lpYakuBit |= bit << YAKUBIT_TANE;//�^�l
			break;
		case md->KIND_TAN:
			*lpYakuBit |= bit << YAKUBIT_TAN;//�^��
			break;
		case md->KIND_KASU:
			*lpYakuBit |= bit << YAKUBIT_KASU;//�J�X
			break;
	}	
}

//�l���D���Ƀr�b�g�𗧂Ă�
void Strategy::SetupGFudaBit(BOOL myFlg,UINT id,UINT kind)
{
	UINT* lpFudaBit = (myFlg)?&this->m_uiMyBit:&this->m_uiComBit;

	this->SetupBit(lpFudaBit,&this->m_uiYakuBit,id,kind);
}



//�𔻒�
UINT Strategy::Judge(BOOL myFlg)
{
	PlayScene* ps = PlayScene::GetInstance();
	YakuList* lpYLKou = (myFlg)?&ps->m_ylMyYaku:&ps->m_ylComYaku;
	YakuList* lpYLOtsu = (!myFlg)?&ps->m_ylMyYaku:&ps->m_ylComYaku;
	UINT kouBit = (myFlg)?this->m_uiMyBit:this->m_uiComBit;
	UINT otsuBit = (!myFlg)?this->m_uiMyBit:this->m_uiComBit;
	int bit = 0;

	//�܌��`�F�b�N
	bit |= this->JudgeGoko(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//�Ԍ������`�F�b�N
	bit |= this->JudgeBake(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//�ԒZ�`�F�b�N
	bit |= this->JudgeAkatan(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//�Z�`�F�b�N
	bit |= this->JudgeAotan(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//�������`�F�b�N
	bit |= this->JudgeInoshika(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//�^�l�`�F�b�N
	bit |= this->JudgeTane(lpYLKou,lpYLOtsu,myFlg);
	//�^���`�F�b�N
	bit |= this->JudgeTan(lpYLKou,lpYLOtsu,myFlg);
	//�J�X�`�F�b�N
	bit |= this->JudgeKasu(lpYLKou,lpYLOtsu,myFlg);

	//���r�b�g���Z�b�g
	this->m_uiYakuBit = 0;

	return bit;
}


//�܌��D�̊l���󋵂�Ԃ�
/*****�l���|�C���g(�J:1Pt,��:2Pt)******/
WPARAM Strategy::CheckGetGoko(UINT kouBit,UINT otsuBit)
{
	WORD kouCnt = 0;
	WORD otsuCnt = 0;
	WORD amePt = 1;
	WORD gokoPt = 2;
	UINT bit = 0;
	UINT i=0;
	if(kouBit & (1 << FUDABIT_AME)) kouCnt += amePt;
	if(otsuBit & (1 << FUDABIT_AME)) otsuCnt += amePt;
	for(i=0;i<4;i++){
		if(kouBit & (1 << (FUDABIT_SAKURA + i))) kouCnt += gokoPt;
		if(otsuBit & (1 << (FUDABIT_SAKURA + i))) otsuCnt += gokoPt;
	}

	return MAKEWPARAM(kouCnt,otsuCnt);
}

//�ԒZ�D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetAkatan(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_AKATAN);
}

//�Z�D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetAotan(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_AOTAN);
}

//�������D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetInoshika(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_INOSHIKA);
}

//3�����D(�ԒZ�E�Z�E������)�̊l����(����)��Ԃ�
WPARAM Strategy::CheckGetTriple(UINT kouBit,UINT otsuBit,UINT yakuNo){

	WORD kouCnt = 0;
	WORD otsuCnt = 0;
	int fudabit = 0;
	switch(yakuNo){
		case YAKU_AKATAN:
			fudabit = FUDABIT_AKA1;
			break;
		case YAKU_AOTAN:
			fudabit = FUDABIT_AO1;
			break;
		case YAKU_INOSHIKA:
			fudabit = FUDABIT_INO;
			break;
	}

	for(UINT i=0;i<3;i++){
		int compBit = 1 << (fudabit + i);
		if(kouBit & compBit) kouCnt++;
		else if(otsuBit & compBit) otsuCnt++;
	}

	return MAKEWPARAM(kouCnt,otsuCnt);
}


//�^�l�D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetTane(BOOL myFlg){
	return this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_TANE);
}

//�^���D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetTan(BOOL myFlg){
	return this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_TAN);
}

//�J�X�D�̊l���󋵂�Ԃ�
WPARAM Strategy::CheckGetKasu(BOOL myFlg){

	WPARAM cnt = this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_KASU);
	//�����D�������Z����
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);
	int bakeBits = 1 << FUDABIT_BAKE;
	if((myFlg && (this->m_uiMyBit & bakeBits)) || (!myFlg && (this->m_uiComBit & bakeBits))) kouCnt++;
	if((!myFlg && (this->m_uiMyBit & bakeBits)) || (myFlg && (this->m_uiComBit & bakeBits))) otsuCnt++;

	return MAKEWPARAM(kouCnt,otsuCnt);
}


//�^�l�E�^���E�J�X�̊l��������Ԃ�
WPARAM Strategy::CheckGetTaneTanKasu(BOOL myFlg,UINT kind){

	PlayScene* ps = PlayScene::GetInstance();
	WORD kouCnt = (myFlg)?ps->m_GMyFuda[kind].size():ps->m_GComFuda[kind].size();
	WORD otsuCnt = (!myFlg)?ps->m_GMyFuda[kind].size():ps->m_GComFuda[kind].size();

	return MAKEWPARAM(kouCnt,otsuCnt);
}


//�܌��n�̔���
UINT Strategy::JudgeGoko(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//�܌��`�F�b�N
	UINT compBit = (1 << YAKUBIT_AME) | (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_GOKO);
	if(!(this->m_uiYakuBit & compBit)) return 0;

	//�܌��D�l���|�C���g(�J:1Pt,��:2Pt)
	UINT bit = 0;
	UINT gokoPt = 2;
	UINT amePt = 1;
	WPARAM cnt = this->CheckGetGoko(kouBit,otsuBit);
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);

	//�ꖇ������̌܌�����
	lpYLOtsu->SetReachBit(YAKU_GOKO,FALSE);
	//�J�ꖇ������̉J�l������
	if(kouCnt % gokoPt == amePt) lpYLOtsu->SetReachBit(YAKU_AMESHIKO,FALSE);
	//�܌��D�ꖇ������̎l������
	if(kouCnt > amePt) lpYLOtsu->SetReachBit(YAKU_SHIKO,FALSE);
	//�܌��D2��������̎O��&�J�l������
	if(kouCnt >= 2*gokoPt){
		lpYLOtsu->SetReachBit(YAKU_SANKO,FALSE);
		lpYLOtsu->SetReachBit(YAKU_AMESHIKO,FALSE);
	}		

	//�܌��D�񖇁��O�����[�`
	if((kouCnt == 2*gokoPt) && (otsuCnt < 2*gokoPt)) lpYLKou->SetReachBit(YAKU_SANKO,TRUE);
	//�J�{�܌��D��
	else if((kouCnt == 2*gokoPt+amePt) && (otsuCnt < 2*gokoPt)){
		//�J�l�����[�`
		lpYLKou->SetReachBit(YAKU_AMESHIKO,TRUE);
		//�O������
		lpYLKou->SetReachBit(YAKU_SANKO,FALSE);
	}
	//�O���ȏ�
	else if(kouCnt >= 3*gokoPt){
		//�O�����[�`����
		lpYLKou->SetReachBit(YAKU_SANKO,FALSE);

		//�O���B��
		if(kouCnt == 3*gokoPt){
			bit += 1 << YAKU_SANKO;
			lpYLKou->SetAchieveBit(YAKU_SANKO,TRUE);
			//�J�l�����[�`
			if(otsuCnt%gokoPt != amePt) lpYLKou->SetReachBit(YAKU_AMESHIKO,TRUE);
			//�l�����[�`
			if(otsuCnt < gokoPt) lpYLKou->SetReachBit(YAKU_SHIKO,TRUE);
		}
		//�O������
		else if(kouCnt > 3*gokoPt){
			//�O������
			lpYLKou->SetAchieveBit(YAKU_SANKO,FALSE);
			//�J�l�����[�`���l�����[�`����
			lpYLKou->SetReachBit(YAKU_AMESHIKO,FALSE);
			lpYLKou->SetReachBit(YAKU_SHIKO,FALSE);
			//�܌����[�`
			if(!otsuCnt) lpYLKou->SetReachBit(YAKU_GOKO,TRUE);

			//�J�l���B��
			if(kouCnt == 3*gokoPt+amePt){
				bit += 1 << YAKU_AMESHIKO;
				lpYLKou->SetAchieveBit(YAKU_AMESHIKO,TRUE);
			}
			//�l���B��
			else if(kouCnt == 4*gokoPt){
				bit += 1 << YAKU_SHIKO;
				lpYLKou->SetAchieveBit(YAKU_SHIKO,TRUE);
			}
			//�܌��B��
			else{
				bit += 1 << YAKU_GOKO;
				lpYLKou->SetAchieveBit(YAKU_GOKO,TRUE);
				//�J�l�����l������
				lpYLKou->SetAchieveBit(YAKU_AMESHIKO,FALSE);
				lpYLKou->SetAchieveBit(YAKU_SHIKO,FALSE);
				//�܌����[�`����
				lpYLKou->SetReachBit(YAKU_GOKO,FALSE);
			}
		}
	}

	return bit;

}

//�Ԍ������̔���
UINT Strategy::JudgeBake(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//�Ԍ������`�F�b�N
	UINT compBit = (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_BAKE);
	if(!(this->m_uiYakuBit & compBit)) return 0;

	UINT bit = 0;
	BOOL hanamiFlg = FALSE;
	BOOL tsukimiFlg = FALSE;

	if(this->m_uiYakuBit & (1 << YAKUBIT_BAKE)){//�����D�l��
		//����̃��[�`����
		lpYLOtsu->SetReachBit(YAKU_TSUKIMI,FALSE);
		lpYLOtsu->SetReachBit(YAKU_HANAMI,FALSE);
		
		//�Ԍ��B��
		if(kouBit & (1 << FUDABIT_SAKURA)) hanamiFlg = TRUE;
		//�Ԍ����[�`
		else if(!(otsuBit & (1 << FUDABIT_SAKURA))) lpYLKou->SetReachBit(YAKU_HANAMI,TRUE);
		//�����B��
		if(kouBit & (1 << FUDABIT_TSUKI)) tsukimiFlg = TRUE;
		//�������[�`
		else if(!(otsuBit & (1 << FUDABIT_TSUKI))) lpYLKou->SetReachBit(YAKU_TSUKIMI,TRUE);
	}
	if(!hanamiFlg && (this->m_uiYakuBit & (1 << YAKUBIT_SAKURA))){//���l��
		//����̉Ԍ����[�`����
		lpYLOtsu->SetReachBit(YAKU_HANAMI,FALSE);

		//�Ԍ��B��
		if(kouBit & (1 << FUDABIT_BAKE)) hanamiFlg = TRUE;
		//�Ԍ����[�`
		else if(!(otsuBit & (1 << FUDABIT_BAKE))) lpYLKou->SetReachBit(YAKU_HANAMI,TRUE);
	}
	if(!tsukimiFlg && (this->m_uiYakuBit & (1 << YAKUBIT_TSUKI))){//���l��
		//����̌������[�`����
		lpYLOtsu->SetReachBit(YAKU_TSUKIMI,FALSE);

		//�����B��
		if(kouBit & (1 << FUDABIT_BAKE)) tsukimiFlg = TRUE;
		//�������[�`
		else if(!(otsuBit & (1 << FUDABIT_BAKE))) lpYLKou->SetReachBit(YAKU_TSUKIMI,TRUE);
	}

	//�Ԍ��B��
	if(hanamiFlg){
		bit += 1 << YAKU_HANAMI;
		lpYLKou->SetAchieveBit(YAKU_HANAMI,TRUE);
	}
	//�����B��
	if(tsukimiFlg){
		bit += 1 << YAKU_TSUKIMI;
		lpYLKou->SetAchieveBit(YAKU_TSUKIMI,TRUE);
	}
		
	return bit;
}

//�ԒZ�̔���
UINT Strategy::JudgeAkatan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//�ԒZ�`�F�b�N
	UINT compBit = 1 << YAKUBIT_AKATAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_AKATAN);
}

//�Z�̔���
UINT Strategy::JudgeAotan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//�Z�`�F�b�N
	UINT compBit = 1 << YAKUBIT_AOTAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_AOTAN);
}

//�������̔���
UINT Strategy::JudgeInoshika(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//�������`�F�b�N
	UINT compBit = 1 << YAKUBIT_INOSHIKA;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_INOSHIKA);
}

//3����(�ԒZ�E�Z�E������)�̔���
UINT Strategy::JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT yakuNo)
{

	UINT bit = 0;
	WPARAM cnt = this->CheckGetTriple(kouBit,otsuBit,yakuNo);
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);

	if(kouCnt == 3){//�B��
		bit = 1 << yakuNo;
		lpYLKou->SetAchieveBit(yakuNo,TRUE);
	}else if(kouCnt == 2 && otsuCnt == 0){//���[�`
		lpYLKou->SetReachBit(yakuNo,TRUE);
	}else if(kouCnt == 1 && otsuCnt == 2){//����̃��[�`����
		lpYLOtsu->SetReachBit(yakuNo,FALSE);
	}

	return bit;
}


//�^�l�̔���
UINT Strategy::JudgeTane(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//�^�l�`�F�b�N
	UINT compBit = 1 << YAKUBIT_TANE;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_TANE);

}


//�^���̔���
UINT Strategy::JudgeTan(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//�^���`�F�b�N
	UINT compBit = 1 << YAKUBIT_TAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_TAN);

}
	
//�J�X�̔���
UINT Strategy::JudgeKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//�J�X�`�F�b�N
	UINT compBit = 1 << YAKUBIT_KASU;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_KASU);

}

//�^�l�E�^���E�J�X�̔���
UINT Strategy::JudgeTaneTanKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg,UINT yakuNo)
{
	UINT bit = 0;
	WPARAM cnt = 0;
	UINT achvCnt = 0;
	UINT allCnt = 0;

	switch(yakuNo){
		case YAKU_TANE://�^�l
			cnt = this->CheckGetTane(myFlg);
			achvCnt = ACHVTANECNT;
			allCnt = ALLTANECNT;
			break;
		case YAKU_TAN://�^��
			cnt = this->CheckGetTan(myFlg);
			achvCnt = ACHVTANCNT;
			allCnt = ALLTANCNT;
			break;
		case YAKU_KASU://�J�X
			cnt = this->CheckGetKasu(myFlg);
			achvCnt = ACHVKASUCNT;
			allCnt = ALLKASUCNT + 1;//�������D�������Z
			break;
	}
	WORD kouCnt = LOWORD(cnt);

	if(kouCnt >= achvCnt){//�B��
		bit = 1 << yakuNo;
		lpYLKou->SetAchieveBit(yakuNo,TRUE);
		//����̃��[�`����
		if(kouCnt > allCnt - achvCnt) lpYLOtsu->SetReachBit(yakuNo,FALSE);
	}else if(kouCnt == achvCnt - 1){//���[�`
		//�D���c���Ă����烊�[�`
		WORD otsuCnt = HIWORD(cnt);
		if(otsuCnt < allCnt - achvCnt + 1) lpYLKou->SetReachBit(yakuNo,TRUE);
	}

	return bit;
}


//�����D�������Ă��邩�H
BOOL Strategy::IsBakeFuda(BOOL myFlg)
{
	UINT bit = (myFlg)?this->m_uiMyBit:this->m_uiComBit;

	return (bit & (1 << FUDABIT_BAKE))?TRUE:FALSE;
}

//���Z�b�g
void Strategy::Reset(void)
{
	//�r�b�g�N���A�[
	this->m_uiComBit = this->m_uiMyBit = this->m_uiYakuBit = 0;
}
