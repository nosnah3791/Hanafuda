#pragma once
#include "Character.h"
#include "MouseListner.h"

class Fuda:public Character,public MouseListner
{
public:
	Fuda(void);
	~Fuda(void);
private:
	// ID
	UINT m_uiID;
	//���
	UINT m_uiKind;//0:�܌� 1:�^�l 2:�^�� 3:�J�X
	//�I���\�t���O
	BOOL m_bSelectableFlg;
	//�s���t���O
	BOOL m_bInvisibleFlg;
protected:
	// ���[���I�[�o�[���̓���
	void OnRollOver(double x, double y);
	// ���[���A�E�g���̓���
	void OnRollOut(double x, double y);
	// �}�E�X�N���b�N���̓���
	void OnMouseClick(double x, double y);
public:
	void SetProperty(UINT id,UINT kind,UINT pResourceID,HMODULE hInstance);
	// �`��
	void Paint(Graphics* g);
	// ���W�ݒ�
	void SetPos(Point pt);
	// ���W���T�C�Y�ݒ�
	void SetPos(Point pt,double scale);
	//������
	void Init(void);
	//������
	void Init(UINT _x,UINT _y);
	//������
	void Init(Point pt);
	// ���Z�b�g
	void Reset(void);
	// ���ʒu�ɒu��������
	void Trace(Character* lpch);
	//�����ɔz�u
	void Centering(UINT _x,UINT _y);
	// �}�E�X�ړ����̏���
	BOOL MouseMove(double x, double y);
	// �}�E�X�N���b�N���̏���
	BOOL MouseClick(double x, double y);
	//�I����Ԃ��w��
	void SetupSelected(BOOL onFlg);
	//�I���\��Ԃ��w��
	void SetupSelectable(BOOL onFlg);
	//��ԃt���O�����Z�b�g
	void ResetSelect(void);
	//ID�擾
	UINT GetID(void);
	//��ʎ擾
	UINT GetKind(void);
};
