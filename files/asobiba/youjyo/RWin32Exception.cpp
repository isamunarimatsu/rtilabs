// RWin32Exception.cpp: RWin32Exception クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "comm.h"
#include "RWin32Exception.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////
RWin32Exception::RWin32Exception(const string inFormat)
{
	DWORD errorCode = ::GetLastError();

	setErrorCode(errorCode);
	setError(inFormat);

	//エラーを文字列かして先頭に加えます。
	AppendWin32Error(errorCode);
}

RWin32Exception::RWin32Exception(char* Format,...)
{
	DWORD errorCode = ::GetLastError();

	setErrorCode(errorCode);
	setError(Format,(char*)(&Format+1));

	//エラーを文字列かして先頭に加えます。
	AppendWin32Error(errorCode);
}

RWin32Exception::~RWin32Exception()
{
}

//エラーを文字列かして先頭に加えます。
void RWin32Exception::AppendWin32Error(DWORD inErrorCode)
{
	LPVOID msgBuf;
	::FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		inErrorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // デフォルト言語
		(LPTSTR) &msgBuf,
		0,
		NULL 
	);

	setError( string( (char*)msgBuf) + "\n" + getMessage() );

	::LocalFree( msgBuf );
}
