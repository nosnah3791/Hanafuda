#include "StdAfx.h"
#include "XImage.h"

XImage::XImage(void)
{
}

XImage::XImage(int width, int height)
{
	this->m_bitmap = new Bitmap(width,height);
}

XImage::XImage(LPCWSTR pFileName)
{
}

XImage::XImage(UINT pResourceID, LPCTSTR pResourceType, HMODULE hInstance)
{
	this->m_bitmap = NULL;
	this->Load(pResourceID,pResourceType,hInstance);

}
XImage::~XImage(void)
{
	if(this->m_hResourceBuffer){
		GlobalUnlock(this->m_hResourceBuffer);
		GlobalFree(this->m_hResourceBuffer);
	}
}

Gdiplus::Bitmap* XImage::GetBitmap(void)
{
	return this->m_bitmap;
}

bool XImage::Load(LPCWSTR pFile)
{
	return false;
}

//���\�[�X����摜�����[�h
bool XImage::Load(UINT pResourceID, LPCTSTR pResourceType, HMODULE hInstance)
{
	
	bool rtnFlg = false;
	LPVOID lpMsgBuf;//�G���[���b�Z�[�W�o�b�t�@

	try
	{
		SetLastError(NO_ERROR);//�G���[�����N���A����

		Gdiplus::Bitmap* pBitmap;
		//Bitmap(����)
		if(pResourceType==RT_BITMAP){
			if(hInstance==NULL)
			{
				hInstance = GetModuleHandle(NULL);
			}
			const WCHAR* pResourceName = (const WCHAR *)MAKEINTRESOURCE(pResourceID);
			pBitmap = Gdiplus::Bitmap::FromResource(hInstance, (const WCHAR *)MAKEINTRESOURCE(pResourceID));
			if(pBitmap->GetLastStatus()==Gdiplus::Ok){
				this->m_bitmap = pBitmap;
				return true;
			}else{
				delete pBitmap;
			}
			throw L"error:FromResource";
		}

		//Bitmap�ȊO
		//���\�[�X�n���h�����擾
		LPCTSTR pResourceName = MAKEINTRESOURCE(pResourceID);
		HRSRC hResource = FindResource(hInstance,pResourceName,pResourceType);
		if(!hResource)
		{
			throw L"error:FindResource";
		}
		//�T�C�Y���擾
		DWORD dwResourceSize = SizeofResource(hInstance, hResource);
		if(!dwResourceSize)
		{
			throw L"error:SizeofResource";
		}
		//���\�[�X�ǂݏo���ă��b�N
		const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
		if(!pResourceData)
		{
			throw L"error:LoadResource,LockResource";
		}
		//���������m��
		this->m_hResourceBuffer = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
		if(!this->m_hResourceBuffer)
		{
			//GlobalFree(m_hResourceBuffer);
			throw L"error:GloabalAlloc";
		}
		//�����������b�N
		void* pResourceBuffer = GlobalLock(this->m_hResourceBuffer);
		if(!pResourceBuffer)
		{
		    //GlobalUnlock(m_hResourceBuffer);
		    GlobalFree(this->m_hResourceBuffer);
			throw L"error:GlobalLock";
		}
		//���\�[�X���������ɃR�s�[
		CopyMemory(pResourceBuffer, pResourceData, dwResourceSize);
		//�X�g���[�����쐬
		IStream* pIStream = NULL;
		if(CreateStreamOnHGlobal(this->m_hResourceBuffer, FALSE, &pIStream)==S_OK)
		{
			//�r�b�g�}�b�v���i�[
			pBitmap = Gdiplus::Bitmap::FromStream(pIStream);
			Gdiplus::Status result = pBitmap->GetLastStatus();
			if(result==Gdiplus::Ok)
			{
				this->m_bitmap = pBitmap;
				rtnFlg = true;
			}else{
				delete pBitmap;
			}
			//�X�g���[�������
			//pIStream->Release();
		}

		if(!rtnFlg){
			//�������̃A�����b�N�Ɖ��
			GlobalUnlock(this->m_hResourceBuffer);
			GlobalFree(this->m_hResourceBuffer);
			throw L"error:CreateStream";
		}
	}
	catch(LPCTSTR errorContent)
	{
		FormatMessage(//�G���[�\��������쐬
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					 (LPTSTR) &lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCWSTR)lpMsgBuf, errorContent, MB_OK);//���b�Z�[�W�\��
		LocalFree(lpMsgBuf);
		PostQuitMessage(1);
	}

	return rtnFlg;
}
XImage::operator Gdiplus::Bitmap*() const
{
	return m_bitmap;
}

int XImage::GetWidth(void)
{
	return this->m_bitmap->GetWidth();
}

int XImage::GetHeight(void)
{
	return this->m_bitmap->GetHeight();
}
