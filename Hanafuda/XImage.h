#pragma once

class XImage
{
public:
	XImage(void);
	XImage(int width,int height);
	XImage(LPCWSTR pFileName);
	XImage(UINT pResourceID, LPCTSTR pResourceType = RT_BITMAP, HMODULE hInstance = NULL);
	~XImage(void);
private:
	//�r�b�g�}�b�v
	Gdiplus::Bitmap* m_bitmap;
	//���\�[�X�o�b�t�@
	HGLOBAL m_hResourceBuffer;
protected:
	//�r�b�g�}�b�v���擾
	Gdiplus::Bitmap* GetBitmap(void);
public:
	//�Q�ƃI�y���[�^�[
	operator Gdiplus::Bitmap*() const;
	//�t�@�C�������w�肵�ĉ摜�����[�h
	bool Load(LPCWSTR pFile);
	//���\�[�X����摜�����[�h
	virtual bool Load(UINT pResourceID, LPCTSTR pResourseType= RT_BITMAP, HMODULE hInstance = NULL);
	//��
	virtual int GetWidth(void);
	//����
	virtual int GetHeight(void);
};
