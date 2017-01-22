#pragma once

class TimeControl
{
public:
	TimeControl(DWORD dwFps = 30, BOOL bSkipFlg = TRUE);
	~TimeControl(void);
private:
	DWORD m_dwLastMinitues;//�ŏI����
	DWORD m_dwFrameRate;//�t���[�����[�g(�t���[����/1�b)
	DWORD m_dwSkipRate;//�X�L�b�v���[�g(�X�L�b�v��/1�b)
	float m_fFrameTime;//�t���[���^�C��(�}�C�N���b/1�t���[��)
	BOOL m_bDrawFlg;//�`��t���O
	BOOL m_bSkipFlg;//�X�L�b�v�t���O		
	BOOL m_bInitFlg;//�����t���O
	DWORD m_dwFrameCnt;//�t���[���v����
	DWORD m_dwSkipCnt;//�X�L�b�v�v����
public:
	BOOL Measure();//���[�g���v������
	void TimeRegular();//���Ԑ���
	void SetupFPS(DWORD fps);//FPS�ݒ�
	BOOL ResetCounter(BOOL stopFlg=FALSE);//��b���ɃJ�E���^�����Z�b�g����

	void SetSkipFlg(BOOL bSkipFlg);
	DWORD GetFrameRate();
	DWORD GetSkipRate();
	BOOL GetDrawFlg();

};
