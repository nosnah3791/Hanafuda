#pragma once

class Character:public XImage
{
public:
	Character(void):  m_bExistFlg(FALSE)
					, m_bInvisibleFlg(FALSE)
					, _x(0)
					, _y(0)
					, _width(0)
					, _height(0)
					, m_dScale(1.0)
    {
		this->m_uiCID = ++Character::uiCID;

	}
	Character(int width,int height);
	~Character(void);
private:
	// ���݃t���O
	BOOL m_bExistFlg;
	//�s���t���O
	BOOL m_bInvisibleFlg;
	//�g��k���w��
	double m_dScale;
	//��
	UINT _width;
	//����
	UINT _height;
public:
	static UINT uiCID;//�V���A���L�����N�^�[ID
	//���W
	UINT _x;
	UINT _y;
protected:
	//�L�����N�^�[ID
	UINT m_uiCID;
	//RGBA�l
	float m_fRGBA[4];
	//���݃t���O
	BOOL GetExistFlg(void);
	//����`��
	void ExtraPaint(Graphics* g,float red,float green,float blue,float alpha);
	//�u�����h�`��
	void BlendPaint(void);
public:
	//���\�[�X����摜�����[�h
	bool Load(UINT pResourceID, LPCTSTR pResourseType= RT_BITMAP, HMODULE hInstance = NULL);
	//�摜��`
	RECT returnRect(void);
	// ���W�ݒ�
	void SetPos(Point pt);
	// ���W���T�C�Y�ݒ�
	void SetPos(Point pt,double scale);
	// ���W�擾
	Point GetPos(void);
	//��
	int GetWidth(void);
	//����
	int GetHeight(void);	
	// ������
	virtual void Init(void);
	// ������
	virtual void Init(UINT _x, UINT _y);
	// ������
	virtual void Init(Point pt);
	// �`��
	void Paint(Graphics* g);
	//�����ɔz�u
	virtual void Centering(UINT _x,UINT _y,UINT width=0,UINT height=0);
	// ���Z�b�g
	virtual void Reset(void);
	// ���ʒu�ɒu��������
	virtual void Trace(Character* lpch);
	//����Ԃ�؂�ւ�
	void SetupInvisible(BOOL flag);
	//�g��k���w��
	void SetScale(double scale);
	//RGBA�l��ݒ�
	void SetRGBA(float red,float green,float blue,float alpha);
};
