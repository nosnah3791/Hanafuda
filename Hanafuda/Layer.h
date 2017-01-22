#pragma once
#include "character.h"

class Layer :
	public Character
{
public:
	Layer(void){}
	Layer(int width,int height);
	~Layer(void);
private:
	//�X�N���[��
	Graphics* m_gScreen;
	enum{
		MONCNT = 12,
		Y_MSG = 510,
		DIF_Y = 35,
		MSG_WAK = 0,
		MSG_ANK,
		MSG_DRAW
	};
	//���W
	const Point m_ptCom,m_ptYou;//�uCOM�v�u���Ȃ��v
	const Point m_ptMonth;//��
	const Point m_ptCScore,m_ptMScore;//�X�R�A�[
	//������摜
	StrImage* m_siCom;//COM
	StrImage* m_siYou;//���Ȃ�
	StrImage* m_siMonth[MONCNT];//��
	StrImage m_siMon[MONCNT+1][2];//��
	std::vector<StrImage> m_siScore;//�X�R�A�[
	StrImage* m_siGoukei;//���v
	StrImage* m_siMsg;//���b�Z�[�W
public:
	//������
	void Init(void);
	//������
	void Init(UINT _x,UINT _y);
	//������
	void Init(Point pt);
	//�����ɔz�u
	void Centering(UINT _x,UINT _y,UINT width=0,UINT height=0);
	// ���Z�b�g
	void Reset(void);
	// �`��
	void Paint(Graphics* g);
};
