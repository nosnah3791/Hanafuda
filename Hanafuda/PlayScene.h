#pragma once
#include "Scene.h"
#include "Fuda.h"
#include "Square.h"
#include <vector>

class PlayScene:public Scene
{
private:
	PlayScene(void);
	PlayScene(const PlayScene& playScene) {}
	PlayScene& operator=(const PlayScene& playScene) {}
public:
	static PlayScene* GetInstance(void){
		static PlayScene playScene;
		return &playScene;
	}
	~PlayScene(void);
public:
	enum{
		FUDACNT = 4,//�z��D��
		FUDAKIND = 4,//�D���
		STR_KOIKOI1 = 0,//������(�����������܂����H)
		STR_KOIKOI2//������(�����������܂�)
	};
	std::vector<Fuda*> m_ComFuda;//����D
	std::vector<Fuda*> m_BaFuda;//��D
	std::vector<Fuda*> m_GMyFuda[FUDAKIND];//�l���ώ�D
	std::vector<Fuda*> m_GComFuda[FUDAKIND];//�l���ϑ���D
	//���ꗗ
	YakuList m_ylComYaku;
	YakuList m_ylMyYaku;
private:
	enum{
		FADECNT = 10,//�t�F�[�h�A�E�g�t���[����
		TEFUDACNT = FUDACNT*2,//��D��
		BAFUDACNT = 18,//��D��
		DAIFUDACNT = TEFUDACNT*3,//��D��
		UFUDACNT = 21,//���D��
		GFUDACNT = 10,//�l���ώD��
		YFUDACNT = 6,//��B���D��
		MOVESTEPCNT = 8,//�X�e�b�v��
		BAFUDADIF = 6,//��D�̌���
		FUNC_PAINT = 0,//�`��֐�
		FUNC_SELECTABLE,//�I���\�t���O�؂�ւ��֐�
		FUNC_SELECTED,//�I�𒆃t���O�؂�ւ��֐�
		FUNC_RESET,//�I�����Z�b�g�֐�
		FUNC_INVISIBLE,//�s����Ԑ؂�ւ��֐�
		FUNC_INSERT,//��D�}���֐�
		FUNC_SORT,//�D���ёւ��֐�
		FUNC_ERASE,//�폜�֐�
		FUNC_COPY,//�R�s�[�֐�
		SCENE_DEAL = 0,//�z��V�[��
		SCENE_PLAY,//�ΐ�V�[��
		CUT_TSELECT = 0,//��D�I���J�b�g
		CUT_BSELECT1,//��D�I���J�b�g1
		CUT_TPUT,//��D���킹�J�b�g
		CUT_DOPEN,//��D�߂���J�b�g
		CUT_BSELECT2,//��D�I���J�b�g2
		CUT_DPUT,//��D���킹�J�b�g
		CUT_TGET,//��D�l���J�b�g
		CUT_B1GET,//��D1�l���J�b�g
		CUT_DGET,//��D�l���J�b�g
		CUT_B2GET,//��D2�l���J�b�g
		CUT_SORT,//�D���ёւ��J�b�g
		CUT_ACHIEVE,//��B���J�b�g
		CUT_KOIKOI,//���������J�b�g
		CUT_RESULT,//���ʃJ�b�g
		SORT_TEFUDA=0,//��D�\�[�g
		SORT_COMFUDA,//����D�\�[�g(���f�o�b�O�p)
		SORT_GETFUDA,//�l���ώD�\�[�g
		WIN_OFF = 0,//�����z���}�[�N(��)
		WIN_COM,//�����z���}�[�N(COM)
		WIN_ME//�����z���}�[�N(����)
	};
	//�D
	Fuda m_UFuda[UFUDACNT];//���D
	std::vector<Fuda*> m_MyFuda;//��D
	std::vector<Fuda*> m_DaiFuda;//��D
	Fuda* m_GetTFuda;//�l����D
	Fuda* m_GetDFuda;//�l����D
	Fuda* m_GetBFuda1[FUDACNT-1];//�l����D1
	Fuda* m_GetBFuda2[FUDACNT-1];//�l����D2
	//�r�b�g
	int m_iBFbit1;//��D�l���r�b�g1
	int m_iBFbit2;//��D�l���r�b�g2
	int m_iBFbitBlank;//��D�󔒃r�b�g
	int m_iAchieveBit;//�B�����r�b�g
	int m_iMyAchieveBit;//�B�����r�b�g(����)
	int m_iComAchieveBit;//�B�����r�b�g(COM)
	//�����z���}�[�N
	UINT m_uiWin;//0:�� 1:COM 2:����
	//�D�ԍ�
	std::vector<int> m_vNo;
	//�D�ړ����W�z��
	std::vector<std::vector<Point>> m_ptMove;
	//������摜
	StrImage* m_siMonth;//��
	StrImage* m_siCom;//COM
	StrImage* m_siYou;//���Ȃ�
	StrImage m_siMon[2];//��
	std::vector<StrImage> m_siScore;//�X�R�A�[
	//���w�i
	Square* m_square;
	//�I���摜
	SelectImage* m_slHai;//�͂�
	SelectImage* m_slIie;//������
	//���W
	const Point m_ptCF,m_ptCF2;//����D
	const Point m_ptBFt,m_ptBFt2;//��D��i
	const Point m_ptDF;//��D
	const Point m_ptMonth;//��
	const Point m_ptBFb,m_ptBFb2;//��D���i
	const Point m_ptMF,m_ptMF2;//��D
	const Point m_ptGMF;//�l����D
	const Point m_ptGCF;//�l������D
	const Point m_ptCom,m_ptYou;//����
	const Point m_ptCY,m_ptMY;//���ꗗ
	const Point m_ptYSize;//��\���g�T�C�Y
	const Point m_ptCScore,m_ptMScore;//�X�R�A�[
	const Point m_ptSquare,m_ptSqSize;//���w�i
	const Point m_ptHai,m_ptIie;//�I���摜
	//�V�[��No.
	UINT m_uiSceneNo;//0:�z��,1:�ΐ�
	//�J�b�gNo.
	UINT m_uiCutNo;
	//�t���[��No.
	UINT m_uiFrameNo;
	//�^�[��No.
	UINT m_uiTurnNo;
	//���C���[
	std::vector<int> m_vLayer;
	//���\�[�X�t�@�C��
	HMODULE m_hModule;
private:
	//�D���V���b�t������
	void Shuffle(void);
	//�D���Z�b�g����
	void SetupFuda(void);
	//�ړ��o�H���m�肷��
	void SetupRoute(Fuda* lpFuda,Point* ePt,UINT cnt,UINT step=MOVESTEPCNT);
	//�ړ��o�H���m�肷��
	void SetupRoute(Fuda** lplpFuda,Point* ePt,UINT cnt,UINT step=MOVESTEPCNT);
	//���킹�D�̈ړ��o�H���m�肷��
	void SetupAFRoute(BOOL teFlg,UINT step=MOVESTEPCNT);
	//�l���D�̈ړ��o�H���m�肷��
	void SetupGFRoute(Fuda** lplpFuda,UINT cnt=1,UINT step=MOVESTEPCNT,UINT ratio=2);
	//�D���w�薇�����A1�t���[�����k�����Ȃ��炸�炷
	BOOL SlideReduce(Fuda** lplpFuda,UINT cnt=1,UINT step=MOVESTEPCNT,UINT ratio=2);
	//���D���w�薇�����A1�t���[�����炷
	BOOL Slide(Fuda* lpUFuda,UINT cnt,UINT step=MOVESTEPCNT);
	//�D��z��
	void Deal(void);
	//�ΐ�
	BOOL Play(void);
	//��D�I��
	BOOL PlayTSelect(void);
	//��D�I��
	BOOL PlayBSelect(BOOL teFlg);
	//�D���킹
	BOOL PlayPut(BOOL teFlg);
	//��D�߂���
	BOOL PlayDOpen(void);
	//�D�l��
	BOOL PlayGet(Fuda** lplpFuda,UINT cnt=1);
	//���ёւ�
	BOOL PlaySort(void);
	//��B��
	BOOL PlayAcheive(void);
	//��������
	BOOL PlayKoikoi(void);
	//����
	BOOL PlayResult(void);
	//�D�x�N�g�����ꊇ����
	BOOL AllFudaFunc(std::vector<Fuda*> &vFuda,int func,double param = 0,BOOL flag = FALSE);
	//�I���\�ȎD�Ƀt���O�𗧂Ă�
	void SetupSelectable(void);
	//���킹���D���i�[����
	BOOL HouseBF(BOOL teFlg,UINT mon);
	//�I�����Ȃ����̏�D�����ɖ߂�
	BOOL ReturnBF(BOOL teFlg,Fuda* lpBaFuda,UINT i);
	//�I��s�̎D����D�ɉ�����
	void AddBF(BOOL teFlg);
	//�l���ώD���i�[����
	void HouseGF(Fuda** lplpFuda,UINT cnt=1);
	//�D����ёւ���
	void SortFuda(Fuda* fuda,int fudakind,int i=0);
	//�w��̃��C���[����ԏ�ɂ���
	void SortLayer(int layer);
	//�B���������D��\������
	void DispAcheiveFuda(void);
	//�B��������r�b�g�����Z����
	void AddAcheiveBit(int bit);
public:
	//������
	void Init(void);
	//���Z�b�g
	void Reset(void);
	// �}�E�X�ړ����̏���
	BOOL MouseMove(double x, double y);
	// �}�E�X�N���b�N���̏���
	BOOL MouseClick(double x, double y);
	// �D�𓮂���
	void Move(void);
	// �`��
	void Paint(void);
	//�B���������̕������擾
	UINT GetAcheiveScore(int bit);
	//�B�����̍��v�X�R�A�[��ǉ�
	void AddScore(int score);
	//�����̔Ԃ��ǂ���
	BOOL IsMyTurn(void);
	//�������̎D���H
	BOOL IsSameMonth(Fuda* fuda,UINT mon);
	//�^�[��No
	UINT GetTurnNo(void);
};
