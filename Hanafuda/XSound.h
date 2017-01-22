#pragma once

class XSound
{
public:
	XSound(void);
	~XSound(void);
private:
	MCI_OPEN_PARMS m_mop;
	MCI_PLAY_PARMS m_mpp;
	MCI_GENERIC_PARMS m_mgp;
	WCHAR m_wcPath[(_MAX_PATH+_MAX_FNAME)*2];//�t�@�C���p�X
	UINT m_uiID;//ID
public:
	static UINT uiID;//�V���A��ID
private:
	//�G���[�\��
	void DispError(MCIERROR error,LPCTSTR str);
public:
	//���\�[�X�f�[�^���特�������i�[����
	BOOL Load(UINT pResourceID, HMODULE hInstance,LPCTSTR pResourceType,LPCTSTR pDeviceType);
	//�I�[�v��
	void Open(void);
	//�Đ�
	void Play(void);
	//��~
	void Stop(void);
	//�����߂�
	void Rewind(void);
	//�ꎞ��~
	void Pause(void);
	//�ꎞ��~����
	void Replay(void);
	//�N���[�Y
	void Close(void);
};
