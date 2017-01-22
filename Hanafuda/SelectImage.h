#pragma once
#include "Character.h"
#include "MouseListner.h"

class SelectImage :
	public Character,public MouseListner
{
public:
	SelectImage(void);
	~SelectImage(void);
protected:
	// ���[���I�[�o�[���̓���
	void OnRollOver(double x, double y);
	// ���[���A�E�g���̓���
	void OnRollOut(double x, double y);
	// �}�E�X�N���b�N���̓���
	void OnMouseClick(double x, double y);
public:
	// �`��
	void Paint(Graphics* g);
	// �}�E�X�ړ����̏���
	BOOL MouseMove(double x, double y);
	// �}�E�X�N���b�N���̏���
	BOOL MouseClick(double x, double y);
	//������
	void Init(void);
	//������
	void Init(UINT _x,UINT _y);
	//������
	void Init(Point pt);
	// ���Z�b�g
	void Reset(void);
};
