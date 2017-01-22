#pragma once
#include "character.h"

class YakuList :
	public Character
{
public:
	YakuList(void);
	~YakuList(void);
private:
	UINT m_uiReachBit;//���[�`�r�b�g
	UINT m_uiAchieveBit;//�B���r�b�g
	//�萔
	enum{
		YL_WIDTH = 75,//��
		YL_HEIGHT = 20//����
	};
public:
	// �`��
	void Paint(Graphics* g);
	//���Z�b�g
	void Reset(void);
	//�w��ʒu�̃��[�`�r�b�g��ύX
	void SetReachBit(UINT no,BOOL flg);
	//�w��ʒu�̃��[�`�r�b�g���擾
	BOOL GetReachBit(UINT no);
	//�w��ʒu�̓��B�r�b�g��ύX
	void SetAchieveBit(UINT no,BOOL flg);
	//�w��ʒu�̓��B�r�b�g���擾
	BOOL GetAchieveBit(UINT no);
};
