#pragma once
#include "Scene.h"
#include "Fuda.h"
#include "StrImage.h"

class ChooseScene:public Scene
{
private:
	ChooseScene(void);
	ChooseScene(const ChooseScene& chooseScene) {}
	ChooseScene& operator=(const ChooseScene& chooseScene) {}
public:
	static ChooseScene* GetInstance(void){
		static ChooseScene chooseScene;
		return &chooseScene;
	}
	~ChooseScene(void);
private:
	enum{
		STR_ANAOYA = 0,
		STR_ERA,
		STR_WATOYA,
		X_LFUDA = 250,//���D���Wx
		X_RFUDA = 530,//�E�D���Wx
		Y_MSG = 114//���b�Z�[�W���Wy
	};
	Fuda m_LFuda;//���D
	Fuda m_RFuda;//�E�D
	StrImage* m_siMsg;//���b�Z�[�W
	StrImage* m_siLMON;//��(��)
	StrImage* m_siRMON;//��(�E)
	StrImage* m_siLWho;//�N(��)
	StrImage* m_siRWho;//�N(�E)
	//���W
	const Point m_ptLU,m_ptRD;
private:
	//�e���ߒ��I
	Point Oyagime(void);
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
};

