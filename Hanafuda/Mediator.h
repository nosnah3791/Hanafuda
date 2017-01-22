#pragma once
#include "stdAfx.h"
#include "Character.h"
#include "Fuda.h"
#include "StrImage.h"
#include "SelectImage.h"
#include "YakuList.h"
#include "XSound.h"
#include "Scene.h"
#include <vector>
#include "TimeControl.h"

class Mediator
{
private:
	Mediator(void) {}
	Mediator(const Mediator& mediator) {}
	Mediator& operator=(const Mediator& mediator) {}
public:
	static Mediator* GetInstance(void){
		static Mediator mediator;
		return &mediator;
	}
	~Mediator(void);
public:
	//���f�o�b�O�p�t���O
	BOOL m_bDebugFlg;
	//�萔
	enum{
		ORG_X = 0,//��ʌ��_X
		ORG_Y = 0,//��ʌ��_Y
		WND_WIDTH = 840,//�E�B���h�E��
		WND_HEIGHT = 600,//�E�B���h�E����
		FUDACNT = 4,//�����̎D��
		YAKUCNT = 12,//��
		MONCNT = 12,//����
		ALLFUDACNT = FUDACNT*MONCNT,//�D����
		KIND_GOKO = 0,//�D���(�܌�)
		KIND_TANE = 1,//�D���(�^�l)
		KIND_TAN = 2,//�D���(�^��)
		KIND_KASU = 3,//�D���(�J�X)
		S_CHSCNT = 3,//�e���߃V�[��������
		S_PLSCNT = 2,//�΋ǃV�[��������
		S_LASCNT = 3,//�ŏI�V�[��������
		SCORE_GOKO = 15,//�܌�(15��)
		SCORE_SHIKO = 10,//�l��(10��)
		SCORE_AMESHIKO = 8,//�J�l��(8��)
		SCORE_SANKO = 6,//�O��(6��)
		SCORE_HANAMI = 3,//�Ԍ�(3��)
		SCORE_TSUKIMI = 3,//����(3��)
		SCORE_AKATAN = 6,//�ԒZ(6��)
		SCORE_AOTAN = 6,//�Z(6��)
		SCORE_INOSHIKA = 5,//������(5��)
		SCORE_TANE = 1,//�^�l(1��)
		SCORE_TAN = 1,//�^��(1��)
		SCORE_KASU = 1,//�J�X(1��)
		SCORE_OYAKEN = 3//�e��(3��)
	};
	//�E�B���h�E�n���h��
	HWND m_hWnd;
	//�I�t�X�N���[��
	Graphics* m_gOffScreen;
	// �D
	Fuda m_Fuda[ALLFUDACNT];
	//���D
	Fuda m_UraFuda;
	//***������***
	StrImage m_siCom;//COM
	StrImage m_siYou;//���Ȃ�
	StrImage m_siMonth[MONCNT];//��
	StrImage m_siChS[S_CHSCNT];//�e���߃V�[��
	StrImage m_siPlS[S_PLSCNT];//�΋ǃV�[��
	StrImage m_siLaS[S_LASCNT];//�ŏI�V�[��
	StrImage m_siYName[YAKUCNT];//��
	StrImage m_siOyaken;//�e��
	StrImage m_siNumberB[10];//��(��)
	StrImage m_siMonB;//��(��)
	StrImage m_siGoukeiB;//���v(��)
	StrImage m_siNumberW[10];//��(��)
	StrImage m_siMonW;//��(��)
	StrImage m_siGoukeiW;//���v(��)
	//�I���摜
	SelectImage m_slHai;//�͂�
	SelectImage m_slIie;//������
	//���ꗗ�\
	YakuList m_siYakuList;
	//BGM
//	XSound m_sndPut1;//�D���d�˂�
//	XSound m_sndPut2;//�D��u��
	XSound m_sndAcheive;//��B��
	XSound m_sndWin;//����
	XSound m_sndEnding;//�G���f�B���O
	//���_�ꗗ
	static const UINT m_uiScore[YAKUCNT];
	//���_�\
	UINT m_uiScoreTable[MONCNT][2];
private:
	// �w�i�摜
	XImage m_imgBG;
	// �V�[��ID
	UINT sceneID;
	// �O���t�B�b�N�X
	XGraphics m_XG;
	//�ĕ`��̈�
	RECT m_rctRepaint;
	//�V�[���I�u�W�F�N�g
	Scene* m_lpScene;
	//�X���b�h�n���h��
	HANDLE m_thread;
	//Event
	HANDLE m_hEvent;
	//�X���b�h�^�C�}�[
	TimeControl m_tc;
	//�C�x���g��
	static const LPCWSTR EVENT_NAME;
	//�e�t���O
	BOOL m_bOyaFlg;
	//��
	UINT m_uiKyoku;
	//���[���I�[�o�[ID
	UINT m_uiROID;
	//�X���b�h�t���O
	BOOL m_bThreadFlg;
	//�X���b�h�֐�
	static unsigned __stdcall Run(LPVOID param);
	//�D���
	static const UINT m_uiFKind[ALLFUDACNT];
public:
	//�I�t�X�N���[���Ƀu�����h�`��
	void BlendImages(int x,int y,Bitmap* srcBitmap);
	// ������
	void Init(HWND hWnd);
	//���Z�b�g
	void Reset(void);
	//�X�R�A�[���Z�b�g
	void ResetScore(void);
	// ���C���X�N���[���`��
	void Paint(void);
	//�ĕ`��
	void Repaint(void);
	//�ĕ`��̈���Z�b�g
	void SetupRepaint(int left,int top,int right,int bottom);
	//�ĕ`��̈���Z�b�g
	void SetupRepaint(RECT rect);
	// �}�E�X�ړ����̏���
	void MouseMove(double x, double y);
	// �}�E�X�N���b�N���̏���
	void MouseClick(double x, double y);
	//���ʉ�
	void Sound(BOOL playFlg);
	//�X���b�h�����s
	void RunThread(BOOL onFlg);
	//�V�[���ύX
	void ChangeScene(Scene* scene);
	//�e�t���O
	BOOL GetOyaFlg(void);
	void SetOyaFlg(BOOL oyaFlg);
	//���ǂɐi�߂�
	UINT NxtKyoku(BOOL nxtFlg);
	//���[���I�[�o�[ID���擾
	UINT GetROID(void);
	//���[���I�[�o�[ID��ύX
	void ChangeROID(UINT roID,BOOL roFlg);
	//���_�����Z
	void AddScore(int score,BOOL myFlg);
	//�݌v���_���擾
	Point GetScore(void);
	//�w��r�b�g����w��̏��Ԃ��擾
	int ExtBitOrder(int bit,int total,int i=0);
	//�X�R�A�[���l�摜���i�[
	void HouseScore(UINT score,std::vector<StrImage> &vSI,Point rPos,BOOL whiteFlg=FALSE,double scale=1.0);
	//������摜���ꊇ�`��
	void PaintStrImage(std::vector<StrImage> &vSI,Graphics* g);
};
