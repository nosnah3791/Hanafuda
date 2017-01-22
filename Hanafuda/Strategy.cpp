#include "StdAfx.h"
#include "Strategy.h"
#include "PlayScene.h"


Strategy::~Strategy(void){}

//選択する札順を返す
UINT Strategy::SelectCF(void)
{
	PlayScene* ps = PlayScene::GetInstance();
	std::vector<Fuda*>::iterator cIT = ps->m_ComFuda.begin();
	int maxIndex = 0;//選択指数最大値
	UINT rtnSort = 0;//札順
	UINT i=0;

	for(cIT=ps->m_ComFuda.begin();cIT != ps->m_ComFuda.end();cIT++){
		Fuda* cFuda = *cIT;
		int index = 0;//選択指数
		if(cFuda){
			int bIndex = 0;//場札選択指数
			std::vector<Fuda*>::iterator bIT = ps->m_BaFuda.begin();
			while(bIT != ps->m_BaFuda.end()){
				Fuda* bFuda = *bIT;
				if(bFuda){
					//同月場札が存在する
					if(ps->IsSameMonth(bFuda,cFuda->GetID()/ps->FUDACNT)){
						//場札の選択指数を算出
						bIndex = max(bIndex,this->CulcIndex(bFuda->GetID(),bFuda->GetKind()));							
					}
				}
				bIT++;
			}
			//場札の選択指数を加算
			index += bIndex;
			//手札の選択指数を加算
			BOOL matchFlg = (!bIndex)?FALSE:TRUE;
			index += this->CulcIndex(cFuda->GetID(),cFuda->GetKind(),matchFlg);

			//※デバッグ用
			if(Mediator::GetInstance()->m_bDebugFlg){
				WCHAR dbg[50];
				wsprintf(dbg,_T("%d:%d\r\n"),i,index);
				OutputDebugString(dbg);
			}

			//選択指数を比較
			if(i == 0 || maxIndex < index){
				maxIndex = index;
				rtnSort = i;
			}
		}

		i++;
	}

	return rtnSort;
}

//選択指数を算出する
int Strategy::CulcIndex(UINT id,UINT kind,BOOL matchFlg)
{
	int index = 0;
	UINT fudaBit = 0;
	UINT yakuBit = 0;

	//ビットを立てる
	this->SetupBit(&fudaBit,&yakuBit,id,kind);

	//算出
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


//五光札の選択指数を算出する
int Strategy::CulcGoko(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//五光チェック
	UINT compBit = (1 << YAKUBIT_AME) | (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_GOKO);
	if(yakuBit & compBit){

		//雨？
		UINT pt = (yakuBit & (1 << YAKUBIT_AME))?1:2;

		//五光札の獲得状況を取得する(雨:1pt,雨以外:2pt)
		WPARAM cnt = this->CheckGetGoko(this->m_uiComBit,this->m_uiMyBit);
		WORD comCnt = LOWORD(cnt);
		WORD myCnt = HIWORD(cnt);
		UINT kouCnt = (matchFlg)?comCnt:myCnt;
		UINT otsuCnt = (matchFlg)?myCnt:comCnt;
		int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;

		//(0)札指数(五光=20点)を加算or減算
		//※「雨取得＋自分二光＋相手二光」時を除く
		if(!(pt == 1 && kouCnt == 4 && otsuCnt == 4)) index += (matchFlg)?INDEX_GOKO:-INDEX_GOKO;

		//***獲得時***
		//(1)役達成→獲得見込み文数×100点
		//(2)リーチがかかる→獲得見込み文数×50点
		//***捨札時***
		//(1)役達成リスク→獲得見込み文数×(-100)点
		//(2)リーチリスク→獲得見込み文数×(-50)点
		switch(kouCnt + pt){
			case 4://三光リーチ
				if(otsuCnt < 4) index += Mediator::SCORE_SANKO * score;//(2)
				break;
			case 5://雨四光リーチ
				if(otsuCnt < 4) index += Mediator::SCORE_AMESHIKO * score;//(2)
				break;
			case 6://三光
				index += Mediator::SCORE_SANKO * score * 2;//(1)
				break;
			case 7://雨四光
				index += Mediator::SCORE_AMESHIKO * score * 2;//(1)
				break;
			case 8://四光
				index += Mediator::SCORE_SHIKO * score * 2;//(1)
				break;
			case 9://五光
				index += Mediator::SCORE_GOKO * score * 2;//(1)
				break;
		}

		//***獲得時***
		//(3)達成阻止→獲得見込み文数×100点
		//***捨札時***
		//(3)達成阻止リスク→→獲得見込み文数×100点
		switch(otsuCnt + pt){
			case 6://三光
				index += Mediator::SCORE_SANKO * score * 2;
				break;
			case 7://雨四光
				index += Mediator::SCORE_AMESHIKO * score * 2;
				break;
			case 8://四光
				index += Mediator::SCORE_SHIKO * score * 2;
				break;
			case 9://五光
				index += Mediator::SCORE_GOKO * score * 2;
				break;
		}
	}

	return index;
}


//花見月見札の選択指数を算出する
int Strategy::CulcBake(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//花見月見チェック
	UINT compBit = (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_BAKE);
	if(yakuBit & compBit){

		//花見月見札の獲得状況を取得する
		int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;
		UINT kouBit = (matchFlg)?this->m_uiComBit:this->m_uiMyBit;
		UINT otsuBit = (!matchFlg)?this->m_uiComBit:this->m_uiMyBit;

		//***獲得時***
		//(1)役達成→獲得見込み文数×100点
		//(2)リーチがかかる→獲得見込み文数×50点
		//(3)達成阻止→獲得見込み文数×100点
		//***捨札時***
		//(1)役達成リスク→獲得見込み文数×(-100)点
		//(2)リーチリスク→獲得見込み文数×(-50)点
		//(3)達成阻止リスク→→獲得見込み文数×100点
		if(yakuBit & (1 << YAKUBIT_BAKE)){//化け札獲得or捨て

			//(1)花見達成or花見達成リスク
			if(kouBit & (1 << FUDABIT_SAKURA)) index += score * 2 * Mediator::SCORE_HANAMI;
			//(2)花見リーチor花見リーチリスク
			else if(!(otsuBit & (1 << FUDABIT_SAKURA))) index += score * Mediator::SCORE_HANAMI;

			//(3)達成阻止or達成阻止リスク
			if(otsuBit & (1 << FUDABIT_SAKURA)) index += score * 2 * Mediator::SCORE_HANAMI;

			//(1)月見達成or月見達成リスク
			if(kouBit & (1 << FUDABIT_TSUKI)) index += score * 2 * Mediator::SCORE_TSUKIMI;
			//月見リーチ
			else if(!(otsuBit & (1 << FUDABIT_TSUKI))) index += score * Mediator::SCORE_TSUKIMI;

			//(3)達成阻止or達成阻止リスク
			if(otsuBit & (1 << FUDABIT_TSUKI)) index += score * 2 * Mediator::SCORE_TSUKIMI;

		}
		else if(yakuBit & (1 << YAKUBIT_SAKURA)){//桜獲得or捨て

			//(1)花見達成or花見達成リスク
			if(kouBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_HANAMI;
			//(2)花見リーチor花見リーチリスク
			else if(!(otsuBit & (1 << FUDABIT_BAKE))) index += score * Mediator::SCORE_HANAMI;

			//(3)達成阻止or達成阻止リスク
			if(otsuBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_HANAMI;
		}
		else if(yakuBit & (1 << YAKUBIT_TSUKI)){//月獲得or捨て

			//(1)月見達成or月見達成リスク
			if(kouBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_TSUKIMI;
			//(2)月見リーチor月見リーチリスク
			else if(!(otsuBit & (1 << FUDABIT_BAKE))) index += score * Mediator::SCORE_TSUKIMI;

			//(3)達成阻止or達成阻止リスク
			if(otsuBit & (1 << FUDABIT_BAKE)) index += score * 2 * Mediator::SCORE_TSUKIMI;
		}

	}

	return index;
}

//赤短札の選択指数を算出する
int Strategy::CulcAkatan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_AKATAN))?this->CulcTriple(yakuBit,matchFlg):0;
}

//青短札の選択指数を算出する
int Strategy::CulcAotan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_AOTAN))?this->CulcTriple(yakuBit,matchFlg):0;
}

//猪鹿蝶札の選択指数を算出する
int Strategy::CulcInoshika(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_INOSHIKA))?this->CulcTriple(yakuBit,matchFlg):0;
}

//3枚役札(赤短・青短・猪鹿蝶)の選択指数を算出する
int Strategy::CulcTriple(UINT yakuBit,BOOL matchFlg)
{

	int index = 0;

	//札の獲得状況を取得する
	WPARAM cnt = 0;
	int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;

	if(yakuBit & (1 << YAKUBIT_AKATAN)){//赤短
		cnt = this->CheckGetAkatan(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_AKATAN;
	}else if(yakuBit & (1 << YAKUBIT_AOTAN)){//青短
		cnt = this->CheckGetAotan(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_AOTAN;
	}else if(yakuBit & (1 << YAKUBIT_INOSHIKA)){//猪鹿蝶
		cnt = this->CheckGetInoshika(this->m_uiComBit,this->m_uiMyBit);
		score *= Mediator::SCORE_INOSHIKA;
	}

	WORD comCnt = LOWORD(cnt);
	WORD myCnt = HIWORD(cnt);
	UINT kouCnt = (matchFlg)?comCnt:myCnt;
	UINT otsuCnt = (matchFlg)?myCnt:comCnt;

	//(0)札指数(10点)を加算or減算
	//※「自分一枚＋相手一枚」時を除く
	if(!(kouCnt == 1 && otsuCnt == 1)) index += (matchFlg)?INDEX_YAKU:-INDEX_YAKU;
	//***獲得時***
	//(1)役達成→獲得見込み文数×100点
	//(2)リーチがかかる→獲得見込み文数×50点
	//(3)達成阻止→獲得見込み文数×100点
	//***捨札時***
	//(1)役達成リスク→獲得見込み文数×(-100)点
	//(2)リーチリスク→獲得見込み文数×(-50)点
	//(3)達成阻止リスク→獲得見込み文数×(-100)点
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



//タネの選択指数を算出する
int Strategy::CulcTane(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_TANE))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//タン札の選択指数を算出する
int Strategy::CulcTan(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_TAN))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//カス札の選択指数を算出する
int Strategy::CulcKasu(UINT yakuBit,BOOL matchFlg)
{
	return (yakuBit & (1 << YAKUBIT_KASU))?this->CulcTaneTanKasu(yakuBit,matchFlg):0;
}

//タネタンカス札の選択指数を算出する
int Strategy::CulcTaneTanKasu(UINT yakuBit,BOOL matchFlg)
{
	int index = 0;

	//札の獲得状況を取得する
	WPARAM cnt = 0;
	int score = (matchFlg)?INDEX_STRATEGY:-INDEX_STRATEGY;
	UINT acheiveCnt = 0;

	//(0)札指数(タネタン5点,カス1点)を加算or減算
	if(yakuBit & (1 << YAKUBIT_TANE)){//タネ
		cnt = this->CheckGetTane(FALSE);
		score *= Mediator::SCORE_TANE;
		index += (matchFlg)?INDEX_TANE:-INDEX_TANE;//(0)
		acheiveCnt = ACHVTANECNT;
	}else if(yakuBit & (1 << YAKUBIT_TAN)){//タン
		cnt = this->CheckGetTan(FALSE);
		score *= Mediator::SCORE_TAN;
		index += (matchFlg)?INDEX_TAN:-INDEX_TAN;//(0)
		acheiveCnt = ACHVTANCNT;
	}else if(yakuBit & (1 << YAKUBIT_KASU)){//カス
		cnt = this->CheckGetKasu(FALSE);
		score *= Mediator::SCORE_KASU;
		index += (matchFlg)?INDEX_KASU:-INDEX_KASU;//(0)
		acheiveCnt = ACHVKASUCNT;
	}

	WORD comCnt = LOWORD(cnt);
	WORD myCnt = HIWORD(cnt);
	UINT kouCnt = (matchFlg)?comCnt:myCnt;
	UINT otsuCnt = (matchFlg)?myCnt:comCnt;

	//***獲得時***
	//(1)役達成→獲得見込み文数×100点
	//(2)リーチがかかる→獲得見込み文数×50点
	//***捨札時***
	//(1)役達成リスク→獲得見込み文数×(-100)点
	//(2)リーチリスク→獲得見込み文数×(-50)点
	if(kouCnt + 1 >= acheiveCnt){//(1)
		index += score * 2;
	}else if(kouCnt + 1 == acheiveCnt-1){//(2)
		index += score;
	}

	return index;
}

//どちらか一方の場札を選択する
UINT Strategy::AlternativeBF(UINT id1,UINT kind1,UINT id2,UINT kind2)
{
	int index1 = this->CulcIndex(id1,kind1);
	int index2 = this->CulcIndex(id2,kind2);
	UINT sort = (index1 >= index2)?0:1;
	return sort;
}

//こいこいするか？
BOOL Strategy::IsKoiKoi(void)
{
	PlayScene* ps = PlayScene::GetInstance();

	//(1)相手のリーチ役の有無を確認
	WPARAM cnt = 0;
	WORD comCnt = 0;
	WORD myCnt = 0;
	UINT reachBit = 0;

	//タネ札の獲得状況を取得→リーチならこいこいしない
	cnt = this->CheckGetTane(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLTANECNT) && (myCnt >= ACHVTANECNT-1)) return FALSE;

	//タン札の獲得状況を取得→リーチならこいこいしない
	cnt = this->CheckGetTan(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLTANCNT) && (myCnt >= ACHVTANCNT-1)) return FALSE;

	//カス札の獲得状況を取得→リーチならこいこいしない
	cnt = this->CheckGetKasu(FALSE);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < ALLKASUCNT+1) && (myCnt >= ACHVKASUCNT-2)) return FALSE;

	//五光札の獲得状況を取得する(雨:1pt,五光:2pt)
	cnt = this->CheckGetGoko(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((myCnt + comCnt < 9) && (comCnt < 4)){//五光札残りあり
		//相手がリーチ
		if(myCnt > 4) reachBit |= (1 << YAKU_GOKO);//雨含む
		else if(myCnt == 4) reachBit |= (1 << YAKU_SANKO);//雨含まない
	}
	//花見月見の獲得状況を取得する(※4ターン目から)
	if(ps->GetTurnNo()/2 >= 3){
		if(this->m_uiMyBit & (1 << FUDABIT_BAKE)){//相手が化け札を持つ
			//二人とも桜を持っていない→花見リーチ
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_SAKURA))) reachBit |= (1 << YAKU_HANAMI);
			//二人とも月を持っていない→月見リーチ
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_TSUKI))) reachBit |= (1 << YAKU_TSUKIMI);
		}
		if(this->m_uiMyBit & (1 << FUDABIT_SAKURA)){//相手が桜を持つ
			//二人とも化け札を持っていない→花見リーチ
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_BAKE))) reachBit |= (1 << YAKU_HANAMI);
		}
		if(this->m_uiMyBit & (1 << FUDABIT_TSUKI)){//相手が月を持つ
			//二人とも化け札を持っていない→月見リーチ
			if(!((this->m_uiMyBit | this->m_uiComBit) & (1 << FUDABIT_BAKE))) reachBit |= (1 << YAKU_TSUKIMI);
		}
	}
	//赤短札の獲得状況を取得する
	cnt = this->CheckGetAkatan(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_AKATAN);
	//青短札の獲得状況を取得する
	cnt = this->CheckGetAotan(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_AOTAN);
	//猪鹿蝶札の獲得状況を取得する
	cnt = this->CheckGetInoshika(this->m_uiComBit,this->m_uiMyBit);
	comCnt = LOWORD(cnt);
	myCnt = HIWORD(cnt);
	if((comCnt + myCnt < 3) && (myCnt == 2)) reachBit |= (1 << YAKU_INOSHIKA);

	//(2)場札にロン札があればこいこいしない
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

//指定札毎にビットを立てる
void Strategy::SetupBit(UINT* lpFudaBit,UINT* lpYakuBit,UINT id,UINT kind)
{
	Mediator* md = Mediator::GetInstance();
	int bit = 1;

	//役札
	switch(id){
		case FUDA_AME:
			*lpFudaBit |= bit << FUDABIT_AME;
			*lpYakuBit |= bit << YAKUBIT_AME;//五光(雨)
			break;
		case FUDA_SAKURA:
			*lpFudaBit |= bit << FUDABIT_SAKURA;
			*lpYakuBit |= bit << YAKUBIT_SAKURA;//五光(桜)
			break;
		case FUDA_TSUKI:
			*lpFudaBit |= bit << FUDABIT_TSUKI;
			*lpYakuBit |= bit << YAKUBIT_TSUKI;//五光(月)
			break;
		case FUDA_TSURU:
			*lpFudaBit |= bit << FUDABIT_TSURU;
			*lpYakuBit |= bit << YAKUBIT_GOKO;//五光(鶴桐)
			break;
		case FUDA_KIRI:
			*lpFudaBit |= bit << FUDABIT_KIRI;
			*lpYakuBit |= bit << YAKUBIT_GOKO;//五光(鶴桐)
			break;
		case FUDA_BAKE:
			*lpFudaBit |= bit << FUDABIT_BAKE;
			*lpYakuBit |= bit << YAKUBIT_BAKE;//化け札
			*lpYakuBit |= bit << YAKUBIT_KASU;//カス
			break;
		case FUDA_INO:
			*lpFudaBit |= bit << FUDABIT_INO;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//猪鹿蝶
			break;
		case FUDA_SHIKA:
			*lpFudaBit |= bit << FUDABIT_SHIKA;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//猪鹿蝶
			break;
		case FUDA_CHOU:
			*lpFudaBit |= bit << FUDABIT_CHOU;
			*lpYakuBit |= bit << YAKUBIT_INOSHIKA;//猪鹿蝶
			break;
		case FUDA_AKA1:
			*lpFudaBit |= bit << FUDABIT_AKA1;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//赤短
			break;
		case FUDA_AKA2:
			*lpFudaBit |= bit << FUDABIT_AKA2;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//赤短
			break;
		case FUDA_AKA3:
			*lpFudaBit |= bit << FUDABIT_AKA3;
			*lpYakuBit |= bit << YAKUBIT_AKATAN;//赤短
			break;
		case FUDA_AO1:
			*lpFudaBit |= bit << FUDABIT_AO1;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//青短
			break;
		case FUDA_AO2:
			*lpFudaBit |= bit << FUDABIT_AO2;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//青短
			break;
		case FUDA_AO3:
			*lpFudaBit |= bit << FUDABIT_AO3;
			*lpYakuBit |= bit << YAKUBIT_AOTAN;//青短
			break;
	}

	//タネ・タン・カス
	switch(kind){
		case md->KIND_TANE:
			*lpYakuBit |= bit << YAKUBIT_TANE;//タネ
			break;
		case md->KIND_TAN:
			*lpYakuBit |= bit << YAKUBIT_TAN;//タン
			break;
		case md->KIND_KASU:
			*lpYakuBit |= bit << YAKUBIT_KASU;//カス
			break;
	}	
}

//獲得札毎にビットを立てる
void Strategy::SetupGFudaBit(BOOL myFlg,UINT id,UINT kind)
{
	UINT* lpFudaBit = (myFlg)?&this->m_uiMyBit:&this->m_uiComBit;

	this->SetupBit(lpFudaBit,&this->m_uiYakuBit,id,kind);
}



//役判定
UINT Strategy::Judge(BOOL myFlg)
{
	PlayScene* ps = PlayScene::GetInstance();
	YakuList* lpYLKou = (myFlg)?&ps->m_ylMyYaku:&ps->m_ylComYaku;
	YakuList* lpYLOtsu = (!myFlg)?&ps->m_ylMyYaku:&ps->m_ylComYaku;
	UINT kouBit = (myFlg)?this->m_uiMyBit:this->m_uiComBit;
	UINT otsuBit = (!myFlg)?this->m_uiMyBit:this->m_uiComBit;
	int bit = 0;

	//五光チェック
	bit |= this->JudgeGoko(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//花見月見チェック
	bit |= this->JudgeBake(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//赤短チェック
	bit |= this->JudgeAkatan(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//青短チェック
	bit |= this->JudgeAotan(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//猪鹿蝶チェック
	bit |= this->JudgeInoshika(lpYLKou,lpYLOtsu,kouBit,otsuBit);
	//タネチェック
	bit |= this->JudgeTane(lpYLKou,lpYLOtsu,myFlg);
	//タンチェック
	bit |= this->JudgeTan(lpYLKou,lpYLOtsu,myFlg);
	//カスチェック
	bit |= this->JudgeKasu(lpYLKou,lpYLOtsu,myFlg);

	//役ビットリセット
	this->m_uiYakuBit = 0;

	return bit;
}


//五光札の獲得状況を返す
/*****獲得ポイント(雨:1Pt,他:2Pt)******/
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

//赤短札の獲得状況を返す
WPARAM Strategy::CheckGetAkatan(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_AKATAN);
}

//青短札の獲得状況を返す
WPARAM Strategy::CheckGetAotan(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_AOTAN);
}

//猪鹿蝶札の獲得状況を返す
WPARAM Strategy::CheckGetInoshika(UINT kouBit,UINT otsuBit){
	return this->CheckGetTriple(kouBit,otsuBit,YAKU_INOSHIKA);
}

//3枚役札(赤短・青短・猪鹿蝶)の獲得状況(枚数)を返す
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


//タネ札の獲得状況を返す
WPARAM Strategy::CheckGetTane(BOOL myFlg){
	return this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_TANE);
}

//タン札の獲得状況を返す
WPARAM Strategy::CheckGetTan(BOOL myFlg){
	return this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_TAN);
}

//カス札の獲得状況を返す
WPARAM Strategy::CheckGetKasu(BOOL myFlg){

	WPARAM cnt = this->CheckGetTaneTanKasu(myFlg,Mediator::GetInstance()->KIND_KASU);
	//化け札分を加算する
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);
	int bakeBits = 1 << FUDABIT_BAKE;
	if((myFlg && (this->m_uiMyBit & bakeBits)) || (!myFlg && (this->m_uiComBit & bakeBits))) kouCnt++;
	if((!myFlg && (this->m_uiMyBit & bakeBits)) || (myFlg && (this->m_uiComBit & bakeBits))) otsuCnt++;

	return MAKEWPARAM(kouCnt,otsuCnt);
}


//タネ・タン・カスの獲得枚数を返す
WPARAM Strategy::CheckGetTaneTanKasu(BOOL myFlg,UINT kind){

	PlayScene* ps = PlayScene::GetInstance();
	WORD kouCnt = (myFlg)?ps->m_GMyFuda[kind].size():ps->m_GComFuda[kind].size();
	WORD otsuCnt = (!myFlg)?ps->m_GMyFuda[kind].size():ps->m_GComFuda[kind].size();

	return MAKEWPARAM(kouCnt,otsuCnt);
}


//五光系の判定
UINT Strategy::JudgeGoko(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//五光チェック
	UINT compBit = (1 << YAKUBIT_AME) | (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_GOKO);
	if(!(this->m_uiYakuBit & compBit)) return 0;

	//五光札獲得ポイント(雨:1Pt,他:2Pt)
	UINT bit = 0;
	UINT gokoPt = 2;
	UINT amePt = 1;
	WPARAM cnt = this->CheckGetGoko(kouBit,otsuBit);
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);

	//一枚→相手の五光消滅
	lpYLOtsu->SetReachBit(YAKU_GOKO,FALSE);
	//雨一枚→相手の雨四光消滅
	if(kouCnt % gokoPt == amePt) lpYLOtsu->SetReachBit(YAKU_AMESHIKO,FALSE);
	//五光札一枚→相手の四光消滅
	if(kouCnt > amePt) lpYLOtsu->SetReachBit(YAKU_SHIKO,FALSE);
	//五光札2枚→相手の三光&雨四光消滅
	if(kouCnt >= 2*gokoPt){
		lpYLOtsu->SetReachBit(YAKU_SANKO,FALSE);
		lpYLOtsu->SetReachBit(YAKU_AMESHIKO,FALSE);
	}		

	//五光札二枚→三光リーチ
	if((kouCnt == 2*gokoPt) && (otsuCnt < 2*gokoPt)) lpYLKou->SetReachBit(YAKU_SANKO,TRUE);
	//雨＋五光札二枚
	else if((kouCnt == 2*gokoPt+amePt) && (otsuCnt < 2*gokoPt)){
		//雨四光リーチ
		lpYLKou->SetReachBit(YAKU_AMESHIKO,TRUE);
		//三光消滅
		lpYLKou->SetReachBit(YAKU_SANKO,FALSE);
	}
	//三光以上
	else if(kouCnt >= 3*gokoPt){
		//三光リーチ消滅
		lpYLKou->SetReachBit(YAKU_SANKO,FALSE);

		//三光達成
		if(kouCnt == 3*gokoPt){
			bit += 1 << YAKU_SANKO;
			lpYLKou->SetAchieveBit(YAKU_SANKO,TRUE);
			//雨四光リーチ
			if(otsuCnt%gokoPt != amePt) lpYLKou->SetReachBit(YAKU_AMESHIKO,TRUE);
			//四光リーチ
			if(otsuCnt < gokoPt) lpYLKou->SetReachBit(YAKU_SHIKO,TRUE);
		}
		//三光より上
		else if(kouCnt > 3*gokoPt){
			//三光消滅
			lpYLKou->SetAchieveBit(YAKU_SANKO,FALSE);
			//雨四光リーチ＆四光リーチ消滅
			lpYLKou->SetReachBit(YAKU_AMESHIKO,FALSE);
			lpYLKou->SetReachBit(YAKU_SHIKO,FALSE);
			//五光リーチ
			if(!otsuCnt) lpYLKou->SetReachBit(YAKU_GOKO,TRUE);

			//雨四光達成
			if(kouCnt == 3*gokoPt+amePt){
				bit += 1 << YAKU_AMESHIKO;
				lpYLKou->SetAchieveBit(YAKU_AMESHIKO,TRUE);
			}
			//四光達成
			else if(kouCnt == 4*gokoPt){
				bit += 1 << YAKU_SHIKO;
				lpYLKou->SetAchieveBit(YAKU_SHIKO,TRUE);
			}
			//五光達成
			else{
				bit += 1 << YAKU_GOKO;
				lpYLKou->SetAchieveBit(YAKU_GOKO,TRUE);
				//雨四光＆四光消滅
				lpYLKou->SetAchieveBit(YAKU_AMESHIKO,FALSE);
				lpYLKou->SetAchieveBit(YAKU_SHIKO,FALSE);
				//五光リーチ消滅
				lpYLKou->SetReachBit(YAKU_GOKO,FALSE);
			}
		}
	}

	return bit;

}

//花見月見の判定
UINT Strategy::JudgeBake(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//花見月見チェック
	UINT compBit = (1 << YAKUBIT_SAKURA) | (1 << YAKUBIT_TSUKI) | (1 << YAKUBIT_BAKE);
	if(!(this->m_uiYakuBit & compBit)) return 0;

	UINT bit = 0;
	BOOL hanamiFlg = FALSE;
	BOOL tsukimiFlg = FALSE;

	if(this->m_uiYakuBit & (1 << YAKUBIT_BAKE)){//化け札獲得
		//相手のリーチ消滅
		lpYLOtsu->SetReachBit(YAKU_TSUKIMI,FALSE);
		lpYLOtsu->SetReachBit(YAKU_HANAMI,FALSE);
		
		//花見達成
		if(kouBit & (1 << FUDABIT_SAKURA)) hanamiFlg = TRUE;
		//花見リーチ
		else if(!(otsuBit & (1 << FUDABIT_SAKURA))) lpYLKou->SetReachBit(YAKU_HANAMI,TRUE);
		//月見達成
		if(kouBit & (1 << FUDABIT_TSUKI)) tsukimiFlg = TRUE;
		//月見リーチ
		else if(!(otsuBit & (1 << FUDABIT_TSUKI))) lpYLKou->SetReachBit(YAKU_TSUKIMI,TRUE);
	}
	if(!hanamiFlg && (this->m_uiYakuBit & (1 << YAKUBIT_SAKURA))){//桜獲得
		//相手の花見リーチ消滅
		lpYLOtsu->SetReachBit(YAKU_HANAMI,FALSE);

		//花見達成
		if(kouBit & (1 << FUDABIT_BAKE)) hanamiFlg = TRUE;
		//花見リーチ
		else if(!(otsuBit & (1 << FUDABIT_BAKE))) lpYLKou->SetReachBit(YAKU_HANAMI,TRUE);
	}
	if(!tsukimiFlg && (this->m_uiYakuBit & (1 << YAKUBIT_TSUKI))){//月獲得
		//相手の月見リーチ消滅
		lpYLOtsu->SetReachBit(YAKU_TSUKIMI,FALSE);

		//月見達成
		if(kouBit & (1 << FUDABIT_BAKE)) tsukimiFlg = TRUE;
		//月見リーチ
		else if(!(otsuBit & (1 << FUDABIT_BAKE))) lpYLKou->SetReachBit(YAKU_TSUKIMI,TRUE);
	}

	//花見達成
	if(hanamiFlg){
		bit += 1 << YAKU_HANAMI;
		lpYLKou->SetAchieveBit(YAKU_HANAMI,TRUE);
	}
	//月見達成
	if(tsukimiFlg){
		bit += 1 << YAKU_TSUKIMI;
		lpYLKou->SetAchieveBit(YAKU_TSUKIMI,TRUE);
	}
		
	return bit;
}

//赤短の判定
UINT Strategy::JudgeAkatan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//赤短チェック
	UINT compBit = 1 << YAKUBIT_AKATAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_AKATAN);
}

//青短の判定
UINT Strategy::JudgeAotan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//青短チェック
	UINT compBit = 1 << YAKUBIT_AOTAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_AOTAN);
}

//猪鹿蝶の判定
UINT Strategy::JudgeInoshika(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit)
{
	//猪鹿蝶チェック
	UINT compBit = 1 << YAKUBIT_INOSHIKA;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTriple(lpYLKou,lpYLOtsu,kouBit,otsuBit,YAKU_INOSHIKA);
}

//3枚役(赤短・青短・猪鹿蝶)の判定
UINT Strategy::JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT yakuNo)
{

	UINT bit = 0;
	WPARAM cnt = this->CheckGetTriple(kouBit,otsuBit,yakuNo);
	WORD kouCnt = LOWORD(cnt);
	WORD otsuCnt = HIWORD(cnt);

	if(kouCnt == 3){//達成
		bit = 1 << yakuNo;
		lpYLKou->SetAchieveBit(yakuNo,TRUE);
	}else if(kouCnt == 2 && otsuCnt == 0){//リーチ
		lpYLKou->SetReachBit(yakuNo,TRUE);
	}else if(kouCnt == 1 && otsuCnt == 2){//相手のリーチ消滅
		lpYLOtsu->SetReachBit(yakuNo,FALSE);
	}

	return bit;
}


//タネの判定
UINT Strategy::JudgeTane(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//タネチェック
	UINT compBit = 1 << YAKUBIT_TANE;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_TANE);

}


//タンの判定
UINT Strategy::JudgeTan(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//タンチェック
	UINT compBit = 1 << YAKUBIT_TAN;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_TAN);

}
	
//カスの判定
UINT Strategy::JudgeKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg)
{
	//カスチェック
	UINT compBit = 1 << YAKUBIT_KASU;
	if(!(this->m_uiYakuBit & compBit)) return 0;

	return this->JudgeTaneTanKasu(lpYLKou,lpYLOtsu,myFlg,YAKU_KASU);

}

//タネ・タン・カスの判定
UINT Strategy::JudgeTaneTanKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg,UINT yakuNo)
{
	UINT bit = 0;
	WPARAM cnt = 0;
	UINT achvCnt = 0;
	UINT allCnt = 0;

	switch(yakuNo){
		case YAKU_TANE://タネ
			cnt = this->CheckGetTane(myFlg);
			achvCnt = ACHVTANECNT;
			allCnt = ALLTANECNT;
			break;
		case YAKU_TAN://タン
			cnt = this->CheckGetTan(myFlg);
			achvCnt = ACHVTANCNT;
			allCnt = ALLTANCNT;
			break;
		case YAKU_KASU://カス
			cnt = this->CheckGetKasu(myFlg);
			achvCnt = ACHVKASUCNT;
			allCnt = ALLKASUCNT + 1;//※化け札分を加算
			break;
	}
	WORD kouCnt = LOWORD(cnt);

	if(kouCnt >= achvCnt){//達成
		bit = 1 << yakuNo;
		lpYLKou->SetAchieveBit(yakuNo,TRUE);
		//相手のリーチ消滅
		if(kouCnt > allCnt - achvCnt) lpYLOtsu->SetReachBit(yakuNo,FALSE);
	}else if(kouCnt == achvCnt - 1){//リーチ
		//札が残っていたらリーチ
		WORD otsuCnt = HIWORD(cnt);
		if(otsuCnt < allCnt - achvCnt + 1) lpYLKou->SetReachBit(yakuNo,TRUE);
	}

	return bit;
}


//化け札を持っているか？
BOOL Strategy::IsBakeFuda(BOOL myFlg)
{
	UINT bit = (myFlg)?this->m_uiMyBit:this->m_uiComBit;

	return (bit & (1 << FUDABIT_BAKE))?TRUE:FALSE;
}

//リセット
void Strategy::Reset(void)
{
	//ビットクリアー
	this->m_uiComBit = this->m_uiMyBit = this->m_uiYakuBit = 0;
}
