//
// Created by toney on 2018/3/1.
//
#pragma once

#include "svrlib.h"

class CGameTable;
class CGamePrivateTable;
class CGameCoinTable;
class CGameRoom;

class CGameLogicMgr : public AutoDeleteSingleton<CGameLogicMgr>
{
public:
	CGameLogicMgr();

	~CGameLogicMgr();

	bool Init();

	void ShutDown();

	// ¹¤³§º¯Êý
	CGamePrivateTable* CreatePrivateTable(CGameRoom* pRoom,int64_t tableID,uint16_t playType);
	CGameCoinTable* CreateCoinTable(CGameRoom* pRoom,int64_t tableID);


protected:

};



