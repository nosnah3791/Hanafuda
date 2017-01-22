#pragma once
#include "character.h"
#include <vector>

class Square :
	public Character,public MouseListner
{
public:
	Square(void){}
	Square(int width,int height);
	~Square(void);
private:
	//�X�N���[��
	Graphics* m_gScreen;
	enum{
		YFUDACNT = 6,//��B���D��
		FUNC_PAINT = 0,//�`��֐�
		FUNC_SORT//�D���ёւ��֐�
	};
	//���[�h
	UINT m_uiMode;
	//�B�����r�b�g
	int m_iAchieveBit;
	//�`��̈�
	int m_iX;
	int m_iY;
	int m_iWidth;
	int m_iHeight;
	//���W
	const Point m_ptYName;//��
	const Point m_ptYF;//���D
	const Point m_ptScore;//�X�R�A�[
	const Point m_ptSumScore;//���v�X�R�A�[
	const Point m_ptGoukei;//�u���v�v
	const Point m_ptKoikoi;//�����������܂����H
	//������
	std::vector<StrImage> m_siYname;//��
	std::vector<StrImage> m_siMon;//��
	std::vector<StrImage> m_siScore;//�X�R�A�[
	StrImage* m_siKoikoi;//��������
	StrImage* m_siGoukei;//���v
public:
	enum{
		MODE_ACHIEVE = 1,//��B�����[�h
		MODE_KOIKOI,//�����������[�h
		MODE_RESULT//���ʃ��[�h
	};
	std::vector<Fuda> m_AFuda;//�B���\���D
private:
	//�D�x�N�g�����ꊇ����
	BOOL AllFudaFunc(std::vector<Fuda> &vFuda,int func,double param=0,BOOL flag=FALSE);
	//�D����ёւ���
	void SortFuda(Fuda* fuda,int i);
	//�`��͈͂�ݒ�
	void SetArea(int x,int y,int width,int height);
protected:
	// ���[���I�[�o�[���̓���
	void OnRollOver(double x, double y);
	// ���[���A�E�g���̓���
	void OnRollOut(double x, double y);
	// �}�E�X�N���b�N���̓���
	void OnMouseClick(double x, double y);
public:
	//�D����ׂ�
	void Sort(void);
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
	//���[�h�ύX
	void ChangeMode(int mode,int bit=0);
	//�`��ʒu�𒆉��ɌŒ肩�A�̈���w��{�ɏk������
	void ReduceArea(float scale);
};
