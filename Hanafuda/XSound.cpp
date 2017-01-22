#include "StdAfx.h"
#include "XSound.h"
#include <mmsystem.h>

#pragma comment(lib,"winmm.lib")

UINT XSound::uiID = 0;

XSound::XSound(void)
{
	this->m_uiID = ++XSound::uiID;
}

XSound::~XSound(void)
{
	this->Stop();//演奏停止
	this->Close();//クローズ
	//ファイル削除
	DeleteFile(this->m_wcPath);
}

//エラー表示
void XSound::DispError(MCIERROR error,LPCTSTR str)
{
	WCHAR lpMsgBuf[256];//エラーメッセージバッファ

	mciGetErrorString(error,lpMsgBuf,256);
	MessageBox(NULL,lpMsgBuf, str, MB_OK);//メッセージ表示
	LocalFree(lpMsgBuf);
	//PostQuitMessage(1);
	SendMessage(Mediator::GetInstance()->m_hWnd,WM_DESTROY,NULL,NULL);
}


//リソースデータから音声情報を格納する
BOOL XSound::Load(UINT pResourceID,HMODULE hInstance,LPCTSTR pResourceType,LPCTSTR pDeviceType)
{
	bool rtnFlg = false;
	LPVOID lpMsgBuf = NULL;//エラーメッセージバッファ
	HANDLE hFile;//ファイルハンドル

	try
	{
		SetLastError(NO_ERROR);//エラー情報をクリアする
		
		WCHAR szPath[_MAX_PATH*2];//フルパス
		WCHAR szDrive[_MAX_DRIVE*2];//ドライブ名
		WCHAR szDir[_MAX_DIR*2];//ディレクトリ名
		WCHAR szFile[_MAX_FNAME*2];//ファイル名

		//現在のディレクトリパスを取得
		if(!::GetModuleFileName(NULL,szPath,sizeof(szPath))){
			throw L"error:GetModuleFileName";
		}
		//ファイル名を結合
		_wsplitpath_s(szPath,szDrive,sizeof(szDrive)/sizeof(szDrive[0]),szDir,sizeof(szDir)/sizeof(szDir[0]),NULL,0,NULL,0);
		wcscpy_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szDrive);
		wcscat_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szDir);
		wsprintf(szFile,_T("temp\\snd_%d.%s"),this->m_uiID,pResourceType);
		wcscat_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szFile);

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
		//ファイル作成
		hFile = CreateFile(this->m_wcPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN,NULL);	
		if(hFile == INVALID_HANDLE_VALUE)
		{
			throw L"error:CreateFile";
		}
		//ファイル書き出し
		DWORD dwWritten;
		if(!WriteFile(hFile,pResourceData,dwResourceSize,&dwWritten,NULL)){
			CloseHandle(hFile);
			throw L"error:WriteFile";
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
	//	PostQuitMessage(1);
		SendMessage(Mediator::GetInstance()->m_hWnd,WM_DESTROY,NULL,NULL);
		return FALSE;
	}

	CloseHandle(hFile);

	//情報格納
	this->m_mop.lpstrDeviceType = pDeviceType;
	this->m_mop.lpstrElementName = this->m_wcPath;

	//オープン
	this->Open();

	return rtnFlg;
}

//オープン
void XSound::Open(void)
{
	MCIERROR error = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&this->m_mop);
	if(error) this->DispError(error,_T("OPEN"));
}


//再生
void XSound::Play(void)
{
	this->Rewind();
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("PLAY"));
}

//停止
void XSound::Stop(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_STOP, MCI_WAIT, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("STOP"));
}

//巻き戻し
void XSound::Rewind(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START | MCI_WAIT, 0);
	if(error) this->DispError(error,_T("REWIND"));
}

//一時停止
void XSound::Pause(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_PAUSE, MCI_WAIT, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("PAUSE"));
}

//一時停止解除
void XSound::Replay(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_RESUME, 0, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("REPLAY"));
}

//クローズ
void XSound::Close(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_CLOSE, 0, (DWORD_PTR)&this->m_mgp);
	if(error) this->DispError(error,_T("CLOSE"));
}

