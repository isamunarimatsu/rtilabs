// RGISSmallZyouti.cpp: RGISSmallZyouti クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "RGISSmallZyouti.h"

//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallZyouti::RGISSmallZyouti()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallZyouti::~RGISSmallZyouti()
{
}


void RGISSmallZyouti::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//線/点の描画
void RGISSmallZyouti::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallZyoutiDataOptimizMainData mainD;
	SmallZyoutiDataOptimizUniqData d;

	mainD.LoopCount = 0;

	ParticularLevel level = inDraw->getLevel();

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , 1 , 8 );
		case LEVEL_NOT_MIDDLELEVEL:
			break;
		}
	}
}

//文字列の描画
void RGISSmallZyouti::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallZyoutiDataOptimizMainData mainD;
	SmallZyoutiDataOptimizUniqData d;

	mainD.LoopCount = 0;

	ParticularLevel level = inDraw->getLevel();
	RGISSmallDupKill dupKill;

	RStdioMemory memory;
	memory.Open(Data );

	for(int i = 0 ; i < this->DataCount ; i++ )
	{
		if (mainD.LoopCount == 0)
		{
			readDataMain(&memory , &mainD);
			readDataUniq(&memory , &d);
		}
		else
		{
			readDataUniq(&memory , &d);
			mainD.LoopCount --;
		}

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , inName->getPoint(mainD.NameIndex) , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
		case LEVEL_NOT_LOWLEVEL:
		case LEVEL_NOT_MIDDLELEVEL:
			break;
		}

	}
}



//データの読み込み.
void RGISSmallZyouti::readDataMain(RStdioMemory * ioFile, SmallZyoutiDataOptimizMainData * outData) const
{
	//名前
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}

//データの読み込み.
void RGISSmallZyouti::readDataUniq(RStdioMemory * ioFile, SmallZyoutiDataOptimizUniqData * outData) const
{

	//CVデータ(点)の取得
	outData->CVCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//点データの読み飛ばし
	outData->CV = ioFile->ptr();
	ioFile->fseek(  sizeof(unsigned short) * 2, SEEK_CUR );
	ioFile->fseek(  (outData->CVCount - 1) *  sizeof(unsigned char) * 2, SEEK_CUR );
	ASSERT(ioFile->ftell() <= DataSize);
}
