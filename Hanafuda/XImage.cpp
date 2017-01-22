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

//リソースから画像をロード
bool XImage::Load(UINT pResourceID, LPCTSTR pResourceType, HMODULE hInstance)
{
	
	bool rtnFlg = false;
	LPVOID lpMsgBuf;//エラーメッセージバッファ

	try
	{
		SetLastError(NO_ERROR);//エラー情報をクリアする

		Gdiplus::Bitmap* pBitmap;
		//Bitmap(既定)
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

		//Bitmap以外
		//リソースハンドルを取得
		LPCTSTR pResourceName = MAKEINTRESOURCE(pResourceID);
		HRSRC hResource = FindResource(hInstance,pResourceName,pResourceType);
		if(!hResource)
		{
			throw L"error:FindResource";
		}
		//サイズを取得
		DWORD dwResourceSize = SizeofResource(hInstance, hResource);
		if(!dwResourceSize)
		{
			throw L"error:SizeofResource";
		}
		//リソース読み出してロック
		const void* pResourceData = LockResource(LoadResource(hInstance, hResource));
		if(!pResourceData)
		{
			throw L"error:LoadResource,LockResource";
		}
		//メモリを確保
		this->m_hResourceBuffer = GlobalAlloc(GMEM_MOVEABLE, dwResourceSize);
		if(!this->m_hResourceBuffer)
		{
			//GlobalFree(m_hResourceBuffer);
			throw L"error:GloabalAlloc";
		}
		//メモリをロック
		void* pResourceBuffer = GlobalLock(this->m_hResourceBuffer);
		if(!pResourceBuffer)
		{
		    //GlobalUnlock(m_hResourceBuffer);
		    GlobalFree(this->m_hResourceBuffer);
			throw L"error:GlobalLock";
		}
		//リソースをメモリにコピー
		CopyMemory(pResourceBuffer, pResourceData, dwResourceSize);
		//ストリームを作成
		IStream* pIStream = NULL;
		if(CreateStreamOnHGlobal(this->m_hResourceBuffer, FALSE, &pIStream)==S_OK)
		{
			//ビットマップを格納
			pBitmap = Gdiplus::Bitmap::FromStream(pIStream);
			Gdiplus::Status result = pBitmap->GetLastStatus();
			if(result==Gdiplus::Ok)
			{
				this->m_bitmap = pBitmap;
				rtnFlg = true;
			}else{
				delete pBitmap;
			}
			//ストリームを解放
			//pIStream->Release();
		}

		if(!rtnFlg){
			//メモリのアンロックと解放
			GlobalUnlock(this->m_hResourceBuffer);
			GlobalFree(this->m_hResourceBuffer);
			throw L"error:CreateStream";
		}
	}
	catch(LPCTSTR errorContent)
	{
		FormatMessage(//エラー表示文字列作成
					FORMAT_MESSAGE_ALLOCATE_BUFFER | 
					FORMAT_MESSAGE_FROM_SYSTEM | 
					FORMAT_MESSAGE_IGNORE_INSERTS,
					NULL, GetLastError(),
					 MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
					 (LPTSTR) &lpMsgBuf, 0, NULL);
		MessageBox(NULL, (LPCWSTR)lpMsgBuf, errorContent, MB_OK);//メッセージ表示
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
