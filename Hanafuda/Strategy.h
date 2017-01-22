#pragma once

class Strategy
{
private:
	Strategy(void):m_uiComBit(0),
				   m_uiMyBit(0),
				   m_uiYakuBit(0)
	{}
	Strategy(const Strategy& strategy) {}
	Strategy& operator=(const Strategy& strategy) {}
public:
	static Strategy* GetInstance(void){
		static Strategy strategy;
		return &strategy;
	}
	~Strategy(void);
private:
	/****�D�r�b�g*****/
	//0:�J
	//1:��
	//2:��
	//3:��
	//4:��
	//5:�����D
	//6:��
	//7:��
	//8:��
	//9:��(��)
	//10:��(�~)
	//11:��(��)
	//12:��(���O)
	//13:��(�g�t)
	//14:��(�e)
	//�D�r�b�g(COM)
	UINT m_uiComBit;
	//�D�r�b�g(����)
	UINT m_uiMyBit;
	/****���r�b�g*****/
	//0:�J
	//1:��
	//2:��
	//3:��or��
	//4:�����D
	//5:������
	//6:�ԒZ
	//7:�Z
	//8:�^�l
	//9:�^��
	//10:�J�X
	UINT m_uiYakuBit;
public:
	//�萔
	enum{
		FUDA_BAKE = 36,//�����D
		FUDA_AME = 44,//�J
		FUDA_SAKURA = 8,//��
		FUDA_TSUKI = 28,//��
		FUDA_TSURU = 0,//��
		FUDA_KIRI = 40,//��
		FUDA_INO = 24,//��
		FUDA_SHIKA = 32,//��
		FUDA_CHOU = 20,//��
		FUDA_AKA1 = 1,//�ԒZ(��)
		FUDA_AKA2 = 5,//�ԒZ(�~)
		FUDA_AKA3 = 9,//�ԒZ(��)
		FUDA_AO1 = 21,//�Z(���O)
		FUDA_AO2 = 33,//�Z(�g�t)
		FUDA_AO3 = 37,//�Z(�e)
		FUDABIT_AME = 0,//�J
		FUDABIT_SAKURA,//��
		FUDABIT_TSUKI,//��
		FUDABIT_TSURU,//��
		FUDABIT_KIRI,//��
		FUDABIT_BAKE,//�����D
		FUDABIT_INO,//��
		FUDABIT_SHIKA,//��
		FUDABIT_CHOU,//��
		FUDABIT_AKA1,//�ԒZ(��)
		FUDABIT_AKA2,//�ԒZ(�~)
		FUDABIT_AKA3,//�ԒZ(��)
		FUDABIT_AO1,//�Z(���O)
		FUDABIT_AO2,//�Z(�g�t)
		FUDABIT_AO3,//�Z(�e)
		YAKUBIT_AME = 0,//�܌�(�J)
		YAKUBIT_SAKURA,//�܌�(��)
		YAKUBIT_TSUKI,//�܌�(��)
		YAKUBIT_GOKO,//�܌�(�ߋ�)
		YAKUBIT_BAKE,//�����D
		YAKUBIT_INOSHIKA,//������
		YAKUBIT_AKATAN,//�ԒZ
		YAKUBIT_AOTAN,//�Z
		YAKUBIT_TANE,//�^�l
		YAKUBIT_TAN,//�Z�D
		YAKUBIT_KASU,//�J�X
		YAKU_GOKO = 0,//�܌�
		YAKU_SHIKO,//�l��
		YAKU_AMESHIKO,//�J�l��
		YAKU_SANKO,//�O��
		YAKU_HANAMI,//�Ԍ�
		YAKU_TSUKIMI,//����
		YAKU_AKATAN,//�ԒZ
		YAKU_AOTAN,//�Z
		YAKU_INOSHIKA,//������
		YAKU_TANE,//�^�l
		YAKU_TAN,//�^��
		YAKU_KASU,//�J�X
		ACHVTANECNT = 5,//�^�l�B����
		ACHVTANCNT = 5,//�^���B����
		ACHVKASUCNT = 10,//�J�X�B����
		ALLTANECNT = 9,//�^�l����
		ALLTANCNT = 10,//�^������
		ALLKASUCNT = 24,//�J�X����
		INDEX_STRATEGY = 50,//�X�g���e�W�[�w��
		INDEX_GOKO = 20,//�X�g���e�W�[�w��(�܌��D)
		INDEX_YAKU = 10,//�X�g���e�W�[�w��(���D)
		INDEX_TANE = 5,//�X�g���e�W�[�w��(�^�l�D)
		INDEX_TAN = 5,//�X�g���e�W�[�w��(�^���D)
		INDEX_KASU = 1//�X�g���e�W�[�w��(�J�X�D)
	};
private:
//�I���w�����Z�o����
int CulcIndex(UINT id,UINT kind,BOOL matchFlg = TRUE);
//�܌��D�̑I���w�����Z�o����
int CulcGoko(UINT yakuBit,BOOL matchFlg = TRUE);
//�Ԍ������D�̑I���w�����Z�o����
int CulcBake(UINT yakuBit,BOOL matchFlg = TRUE);
//�ԒZ�D�̑I���w�����Z�o����
int CulcAkatan(UINT yakuBit,BOOL matchFlg = TRUE);
//�Z�D�̑I���w�����Z�o����
int CulcAotan(UINT yakuBit,BOOL matchFlg = TRUE);
//�������D�̑I���w�����Z�o����
int CulcInoshika(UINT yakuBit,BOOL matchFlg = TRUE);
//3�����D(�ԒZ�E�Z�E������)�̑I���w�����Z�o����
int CulcTriple(UINT yakuBit,BOOL matchFlg = TRUE);
//�^�l�̑I���w�����Z�o����
int CulcTane(UINT yakuBit,BOOL matchFlg = TRUE);
//�^���D�̑I���w�����Z�o����
int CulcTan(UINT yakuBit,BOOL matchFlg = TRUE);
//�J�X�D�̑I���w�����Z�o����
int CulcKasu(UINT yakuBit,BOOL matchFlg = TRUE);
//�^�l�^���J�X�D�̑I���w�����Z�o����
int CulcTaneTanKasu(UINT yakuBit,BOOL matchFlg = TRUE);
//�܌��D�̊l���󋵂�Ԃ�
WPARAM CheckGetGoko(UINT kouBit,UINT otsuBit);
//�ԒZ�D�̊l���󋵂�Ԃ�
WPARAM CheckGetAkatan(UINT kouBit,UINT otsuBit);
//�Z�D�̊l���󋵂�Ԃ�
WPARAM CheckGetAotan(UINT kouBit,UINT otsuBit);
//�������D�̊l���󋵂�Ԃ�
WPARAM CheckGetInoshika(UINT kouBit,UINT otsuBit);
//�^�l�D�̊l���󋵂�Ԃ�
WPARAM CheckGetTane(BOOL myFlg);
//�^���D�̊l���󋵂�Ԃ�
WPARAM CheckGetTan(BOOL myFlg);
//�J�X�D�̊l���󋵂�Ԃ�
WPARAM CheckGetKasu(BOOL myFlg);
//3�����D(�ԒZ�E�Z�E������)�̊l���󋵂�Ԃ�
//WPARAM CheckGetTriple(UINT kouBit,UINT otsuBit,UINT kind);
WPARAM CheckGetTriple(UINT kouBit,UINT otsuBit,UINT yakuNo);
//�^�l�E�^���E�J�X�̊l��������Ԃ�
WPARAM CheckGetTaneTanKasu(BOOL myFlg,UINT kind);
//�܌��n�̔���
UINT JudgeGoko(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//�Ԍ������̔���
UINT JudgeBake(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//�ԒZ�̔���
UINT JudgeAkatan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//�Z�̔���
UINT JudgeAotan(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//�������̔���
UINT JudgeInoshika(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit);
//�^�l�̔���
UINT JudgeTane(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//�^���̔���
UINT JudgeTan(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//�J�X�̔���
UINT JudgeKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg);
//3����(�ԒZ�E�Z�E������)�̔���
//UINT JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT kind);
UINT JudgeTriple(YakuList* lpYLKou,YakuList* lpYLOtsu,UINT kouBit,UINT otsuBit,UINT yakuNo);
//�^�l�E�^���E�J�X�̔���
UINT JudgeTaneTanKasu(YakuList* lpYLKou,YakuList* lpYLOtsu,BOOL myFlg,UINT yakuNo);
//�w��D���Ƀr�b�g�𗧂Ă�
void SetupBit(UINT* lpFudaBit,UINT* lpYakuBit,UINT id,UINT kind);
public:
//�I������D����Ԃ�
UINT SelectCF(void);
//�ǂ��炩����̏�D��I������
UINT AlternativeBF(UINT id1,UINT kind1,UINT id2,UINT kind2);
//�����������邩�H
BOOL IsKoiKoi(void);
//�l���D���Ƀr�b�g�𗧂Ă�
void SetupGFudaBit(BOOL myFlg,UINT id,UINT kind);
//�𔻒�
UINT Judge(BOOL myFlg);
//�����D�������Ă��邩�H
BOOL IsBakeFuda(BOOL myFlg);
//���Z�b�g
void Reset(void);
};
