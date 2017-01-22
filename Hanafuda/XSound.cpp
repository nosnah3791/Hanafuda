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
	this->Stop();//���t��~
	this->Close();//�N���[�Y
	//�t�@�C���폜
	DeleteFile(this->m_wcPath);
}

//�G���[�\��
void XSound::DispError(MCIERROR error,LPCTSTR str)
{
	WCHAR lpMsgBuf[256];//�G���[���b�Z�[�W�o�b�t�@

	mciGetErrorString(error,lpMsgBuf,256);
	MessageBox(NULL,lpMsgBuf, str, MB_OK);//���b�Z�[�W�\��
	LocalFree(lpMsgBuf);
	//PostQuitMessage(1);
	SendMessage(Mediator::GetInstance()->m_hWnd,WM_DESTROY,NULL,NULL);
}


//���\�[�X�f�[�^���特�������i�[����
BOOL XSound::Load(UINT pResourceID,HMODULE hInstance,LPCTSTR pResourceType,LPCTSTR pDeviceType)
{
	bool rtnFlg = false;
	LPVOID lpMsgBuf = NULL;//�G���[���b�Z�[�W�o�b�t�@
	HANDLE hFile;//�t�@�C���n���h��

	try
	{
		SetLastError(NO_ERROR);//�G���[�����N���A����
		
		WCHAR szPath[_MAX_PATH*2];//�t���p�X
		WCHAR szDrive[_MAX_DRIVE*2];//�h���C�u��
		WCHAR szDir[_MAX_DIR*2];//�f�B���N�g����
		WCHAR szFile[_MAX_FNAME*2];//�t�@�C����

		//���݂̃f�B���N�g���p�X���擾
		if(!::GetModuleFileName(NULL,szPath,sizeof(szPath))){
			throw L"error:GetModuleFileName";
		}
		//�t�@�C����������
		_wsplitpath_s(szPath,szDrive,sizeof(szDrive)/sizeof(szDrive[0]),szDir,sizeof(szDir)/sizeof(szDir[0]),NULL,0,NULL,0);
		wcscpy_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szDrive);
		wcscat_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szDir);
		wsprintf(szFile,_T("temp\\snd_%d.%s"),this->m_uiID,pResourceType);
		wcscat_s(this->m_wcPath,sizeof(this->m_wcPath)/sizeof(this->m_wcPath[0]),szFile);

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
		//�t�@�C���쐬
		hFile = CreateFile(this->m_wcPath,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,FILE_ATTRIBUTE_HIDDEN,NULL);	
		if(hFile == INVALID_HANDLE_VALUE)
		{
			throw L"error:CreateFile";
		}
		//�t�@�C�������o��
		DWORD dwWritten;
		if(!WriteFile(hFile,pResourceData,dwResourceSize,&dwWritten,NULL)){
			CloseHandle(hFile);
			throw L"error:WriteFile";
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
	//	PostQuitMessage(1);
		SendMessage(Mediator::GetInstance()->m_hWnd,WM_DESTROY,NULL,NULL);
		return FALSE;
	}

	CloseHandle(hFile);

	//���i�[
	this->m_mop.lpstrDeviceType = pDeviceType;
	this->m_mop.lpstrElementName = this->m_wcPath;

	//�I�[�v��
	this->Open();

	return rtnFlg;
}

//�I�[�v��
void XSound::Open(void)
{
	MCIERROR error = mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD_PTR)&this->m_mop);
	if(error) this->DispError(error,_T("OPEN"));
}


//�Đ�
void XSound::Play(void)
{
	this->Rewind();
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_PLAY, 0, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("PLAY"));
}

//��~
void XSound::Stop(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_STOP, MCI_WAIT, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("STOP"));
}

//�����߂�
void XSound::Rewind(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_SEEK, MCI_SEEK_TO_START | MCI_WAIT, 0);
	if(error) this->DispError(error,_T("REWIND"));
}

//�ꎞ��~
void XSound::Pause(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_PAUSE, MCI_WAIT, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("PAUSE"));
}

//�ꎞ��~����
void XSound::Replay(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_RESUME, 0, (DWORD_PTR)&this->m_mpp);
	if(error) this->DispError(error,_T("REPLAY"));
}

//�N���[�Y
void XSound::Close(void)
{
	MCIERROR error = mciSendCommand(this->m_mop.wDeviceID, MCI_CLOSE, 0, (DWORD_PTR)&this->m_mgp);
	if(error) this->DispError(error,_T("CLOSE"));
}

