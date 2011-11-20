// RMediaPlayer.h: RMediaPlayer クラスのインターフェイス
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_)
#define AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "comm.h"
#include "RMediaViewPlayer.h"
#include "AtlActiveXBrige.h"
#include "RNotSupportException.h"
#include "RActiveXWindow.h"
#import "C:\WINNT\System32\msdxm.ocx" named_guids raw_interfaces_only exclude("IServiceProvider") include("DISPPARAMS")


class RMediaPlayer  :
 public RMediaViewPlayer ,	//インターフェース
 public RActiveXWindow
{
public:
	RMediaPlayer();
	virtual ~RMediaPlayer();
	//実態の作成
	void Create(HINSTANCE inINstance,HWND isParentWindow) throw();

	//ファイルを開く
	virtual void Open(const string inFilename )	throw() ;

	//再生する.
	virtual void Play() throw() ;

	//停止する.
	virtual void Stop() throw() ;

	//一時停止 / 解除
	virtual void Pause(bool inON)	throw() ;

	//ファイルクローズ
	virtual void Close()	throw() ;

	//ボリューム
	virtual int getVolum() const throw() ;

	virtual void setVolum(int inVolum) throw() ;

	//再生位置
	virtual int  getPos() const throw() ;

	virtual void setPos(int inPos) throw() ;

	//再生中?
	virtual bool IsPlay() const ;

	//一時停止中?
	virtual bool IsPause() const ;
	//サポートしてます?
	static bool IsSupport(const string inFilename);

	virtual const RWindow*	getWindow() const
	{
		return (const RWindow*)this;
	}
	//タイトルの取得
	string getTitle() const;
	
private:

	CComPtr<MediaPlayer::IMediaPlayer2>	MediaPlayer;
};

#endif // !defined(AFX_RMEDIAPLAYER_H__33B302F5_FDAC_4841_B66F_72B7E83B040F__INCLUDED_)
