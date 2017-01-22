#pragma once

class XGraphics
{
public:
	XGraphics(void);
	XGraphics(HWND hWnd);
	XGraphics(HWND hWnd, int width, int height);
	~XGraphics(void);
private:
	//�E�B���h�E��
	UINT m_uiWidth;
	//�E�B���h�E����
	UINT m_uiHeight;
	//�I���X�N���[��
	Graphics* m_gOnScreen;
	//�I�t�X�N���[���r�b�g�}�b�v
	Bitmap* m_offScreenBmp;
public:
	enum{
		BLEND_ALPHA = 1//�u�����h���(�A���t�@)
	};
	//�E�B���h�E�n���h��
	HWND m_hWnd;
	//�I�t�X�N���[��
	Graphics* m_gOffScreen;
private:
	//�u�����h��`�֐�
	byte (XGraphics::*m_funcBlend)(UINT,UINT,UINT);
	//�A���t�@�u�����h����
	byte BlendAlpha(UINT dstByte, UINT srcByte, UINT alpha);
public:
	//��ʍX�V
	Status Update(HDC hdc);
	//�ڍאݒ�
	void SetProperty(HWND hWnd);
	//��̉摜���u�����h����
	void BlendImages(INT x, INT y, int blendFunc,Bitmap* srcBitmap, Bitmap* dstBitmap=NULL);

};
