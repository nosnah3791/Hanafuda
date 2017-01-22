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
	WCHAR m_wcPath[(_MAX_PATH+_MAX_FNAME)*2];//ファイルパス
	UINT m_uiID;//ID
public:
	static UINT uiID;//シリアルID
private:
	//エラー表示
	void DispError(MCIERROR error,LPCTSTR str);
public:
	//リソースデータから音声情報を格納する
	BOOL Load(UINT pResourceID, HMODULE hInstance,LPCTSTR pResourceType,LPCTSTR pDeviceType);
	//オープン
	void Open(void);
	//再生
	void Play(void);
	//停止
	void Stop(void);
	//巻き戻し
	void Rewind(void);
	//一時停止
	void Pause(void);
	//一時停止解除
	void Replay(void);
	//クローズ
	void Close(void);
};
