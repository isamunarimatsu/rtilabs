// RGISSmallTetudo.cpp: RGISSmallTetudo クラスのインプリメンテーション
//
//////////////////////////////////////////////////////////////////////

#include "RGISSmallTetudo.h"


//////////////////////////////////////////////////////////////////////
// 構築/消滅
//////////////////////////////////////////////////////////////////////

RGISSmallTetudo::RGISSmallTetudo()
{
	Data = NULL;
	DataCount = 0;
}

RGISSmallTetudo::~RGISSmallTetudo()
{
}


void RGISSmallTetudo::Create(unsigned long inBlock , unsigned long inDataSize ,unsigned long inDataCount ,const char* inBuffer) throw(RException)
{
	this->DataSize = inDataSize;
	this->DataCount = inDataCount;
	this->Data = inBuffer;
}


//線/点の描画
void RGISSmallTetudo::Draw1(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw)
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

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
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount ,selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_KOUKYOU:
			//駅名だけは表示してあげる.
			if (mainD.EkiNameIndex == 0xffff)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			else
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_LOWLEVEL:
			inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//普通鉄道JRと普通鉄道
			if (0x43 == mainD.SB || 0x44 == mainD.SB)
				inDraw->Polygon(inBlockStart, d.CV , d.CVCount , selectSize(mainD.SB , mainD.EkiNameIndex) , selectColor(mainD.SB , mainD.EkiNameIndex) );
			break;
		}
	}
}

//文字列の描画
void RGISSmallTetudo::Draw3(const LPOINT *inBlockStart , const RGISSmallDraw *inDraw, const RGISSmallNameDatabase * inName)
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

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

		const char * name ;
		if (mainD.EkiNameIndex != 0xffff)	name = inName->getPoint(mainD.EkiNameIndex);
		else							name = inName->getPoint(mainD.NameIndex);

		switch(level)
		{
		case LEVEL_FULL:
		case LEVEL_NOT_BANCHI:
		case LEVEL_NOT_CHIMEI:
			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , name , &dupKill);
			break;
		case LEVEL_NOT_KOUKYOU:
			//駅名だけは表示してあげる.
			if (mainD.EkiNameIndex != 0xffff)
				inDraw->TextOut(inBlockStart, d.CV , d.CVCount ,  name , &dupKill);
//			else
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		case LEVEL_NOT_LOWLEVEL:
//			inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		case LEVEL_NOT_MIDDLELEVEL:
			//普通鉄道JRと普通鉄道
//			if (0x43 == mainD.SB || 0x44 == mainD.SB)
//				inDraw->TextOut(inBlockStart, d.CV , d.CVCount , "" , &dupKill);
			break;
		}

		
	}
}

//駅検索.
void RGISSmallTetudo::FindEki(const LPOINT *inBlockStart , FindPool * outFindData , 
							  const string &inFindName , unsigned short inFindNameHash,
							  const RGISSmallNameDatabase * inNameDB ) const
{
	SmallTetudoDataOptimizMainData mainD;
	SmallTetudoDataOptimizUniqData d;

	mainD.LoopCount = 0;

	RStdioMemory memory;
	memory.Open(Data );

	for(int n = 0; n < this->DataCount ; n++)
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

		//名前の index は等しいですか?
		if (mainD.EkiNameIndex != inFindNameHash)
		{
			continue;	//一致しない
		}
		//名前が先頭一致しますか?
		const char * fullname = inNameDB->getPoint(mainD.EkiNameIndex);
		if ( strncmp( fullname , inFindName.c_str() , inFindName.size() ) != 0)
		{
			continue;	//一致しない
		}

		LPOINT avgPoint = RGISSmallType::AvgCV(inBlockStart , d.CV , d.CVCount ); 
		//これが探していたノードだ!
		const char * lineName = inNameDB->getPoint(mainD.NameIndex);
		if (lineName != NULL && lineName[0] != '\0')
		{
			outFindData->push_back(new RGISSmallFindObject(&avgPoint ,  string(fullname) + "(" + lineName +")") );
		}
		else
		{
			outFindData->push_back(new RGISSmallFindObject(&avgPoint ,  fullname ) );
		}
	}
}



COLORREF16 RGISSmallTetudo::selectColor(unsigned char inShubetsu , unsigned short inER) const
{
	if (inER != 0xffff)				return BBananaConfig::getInstance()->getColor(COLOR_EKI);		//駅

	return BBananaConfig::getInstance()->getColor(inShubetsu - 0x43 + COLOR_TETUDO_JR);
/*
	if (inER != 0xffff)				return COLOR_6;		//駅

	if (0x43 == inShubetsu)				return COLOR_6;		//普通鉄道JR
	if (0x44 == inShubetsu)				return COLOR_6;		//普通鉄道
	if (0x45 == inShubetsu)				return COLOR_7;		//路面電車
	if (0x46 == inShubetsu)				return COLOR_7;		//地下式鉄道
	if (0x47 == inShubetsu)				return COLOR_6;		//その他

	ASSERT(0);
	return 0;
*/
}

COLORREF RGISSmallTetudo::selectSize(unsigned char inShubetsu , unsigned short inER) const
{
	if (inER != 0xffff)				return 8;		//駅

	if (0x43 == inShubetsu)				return 4;		//普通鉄道JR
	if (0x44 == inShubetsu)				return 4;		//普通鉄道
	if (0x45 == inShubetsu)				return 1;		//路面電車
	if (0x46 == inShubetsu)				return 1;		//地下式鉄道
	if (0x47 == inShubetsu)				return 1;		//その他

	ASSERT(0);
	return 0;
}

//データの読み込み.
void RGISSmallTetudo::readDataMain(RStdioMemory * ioFile, SmallTetudoDataOptimizMainData * outData) const
{
	outData->SB = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);

	//名前
	outData->NameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	//駅の名前
	outData->EkiNameIndex = ioFile->readGetUS( );
	ASSERT(ioFile->ftell() <= DataSize);

	outData->LoopCount = ioFile->readGetUC( );
	ASSERT(ioFile->ftell() <= DataSize);
}

//データの読み込み.
void RGISSmallTetudo::readDataUniq(RStdioMemory * ioFile, SmallTetudoDataOptimizUniqData * outData) const
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
