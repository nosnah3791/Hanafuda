#pragma once

class MouseListner
{
public:
	MouseListner(void);
	~MouseListner(void);
protected:
	RECT rect;//��`�̈�
	//���[���I�[�o�[�t���O
	BOOL m_bRolloverFlg;
private:
	// �w����W�̉摜��`���L���𔻒�
	BOOL InRect(double x, double y);
protected:
	// ���[���I�[�o�[���̓���
	virtual void OnRollOver(double x, double y) = 0;
	// ���[���A�E�g���̓���
	virtual void OnRollOut(double x, double y) = 0;
	// �}�E�X�N���b�N���̓���
	virtual void OnMouseClick(double x, double y) = 0;
public:
	// �}�E�X�ړ����̏���
	BOOL MouseMove(double x, double y);
	//���[���I�[�o�[���̏���
	BOOL RollOver(double x, double y);
	//���[���A�E�g���̏���
	BOOL RollOut(double x, double y);
	// �}�E�X�N���b�N���̏���
	BOOL MouseClick(double x, double y);
};
