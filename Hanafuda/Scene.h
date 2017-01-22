#pragma once
#include "stdAfx.h"


class Scene
{
public:
	Scene(void);
	~Scene(void);
protected:
	//�ꎞ��~�w��
	UINT m_uiStopCnt;
	//���ʉ�
	XSound* m_sound;
public:
	// ������
	virtual void Init(void)=0;
	// ���Z�b�g
	virtual void Reset(void)=0;
	// �}�E�X�ړ����̏���
	virtual BOOL MouseMove(double x, double y) = 0;
	// �}�E�X�N���b�N���̏���
	virtual BOOL MouseClick(double x, double y) = 0;
	// �D�𓮂���
	virtual void Move(void)=0;
	// �`��
	virtual void Paint(void)=0;
	//���ʉ�
	void Sound(BOOL playFlg);
	//�ꎞ��~
	BOOL Stop(void);
};
