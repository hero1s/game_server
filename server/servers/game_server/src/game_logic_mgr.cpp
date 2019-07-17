//
// Created by toney on 2018/3/1.
//

#include "game_logic_mgr.h"
#include <data_cfg_mgr.h>
#include "game_table.h"
#include "game_table_coin.h"
#include "game_table_private.h"
#include "game_room.h"

// ��������
CGamePrivateTable* CGameRoom::CreatePrivateTable(int64_t tableID, uint16_t playType)
{
	return CGameLogicMgr::Instance().CreatePrivateTable(this, tableID, playType);
}
CGameCoinTable* CGameRoom::CreateCoinTable(int64_t tableID)
{
	return CGameLogicMgr::Instance().CreateCoinTable(this, tableID);
}

CGameLogicMgr::CGameLogicMgr()
{

}

CGameLogicMgr::~CGameLogicMgr()
{

}

bool CGameLogicMgr::Init()
{
	return true;
}

void CGameLogicMgr::ShutDown()
{

}

// ��������
CGamePrivateTable* CGameLogicMgr::CreatePrivateTable(CGameRoom* pRoom, int64_t tableID, uint16_t playType)
{
	CGamePrivateTable* pTable = NULL;
	uint16_t gameType = pRoom->GetGameType();
	if (pRoom->GetRoomType() == emROOM_TYPE_PRIVATE)
	{//˽�˷�
		switch (gameType)
		{
		case net::GAME_CATE_LAND:
		{
			//pTable = new game_land::CGameLandTable(pRoom, tableID);                  // ������
			break;
		}
		case net::GAME_CATE_BEARD:
		{
			//pTable = new game_beard::CGameBeardTable(pRoom, tableID);                // �ܺ���
			break;
		}
		case net::GAME_CATE_BEARD_PENG:
		{
			//pTable = new game_penghu::CGamePenghuTable(pRoom, tableID);              // ����
			break;
		}
		case net::GAME_CATE_NIUNIU:
		{
			//pTable = new game_niuniu::CGameNiuniuTable(pRoom, tableID);              // ţţ
			break;
		}
		case net::GAME_CATE_FANSANPI:
		{
			//pTable = new game_fansanpi::CGameFanSanPiTable(pRoom,tableID);           // ����Ƥ
			break;
		}
		case net::GAME_CATE_SANGONG:
		{
			//pTable = new game_sangong::CGameSanGongTable(pRoom, tableID);            // ����
			break;
		}
		case net::GAME_CATE_THIRTEEN:
		{
			//pTable = new game_thirteen::CGameThirteenTable(pRoom, tableID);          // ʮ��ˮ
			break;
		}
		case net::GAME_CATE_KUAIPAO:
		{
			//pTable = new game_kuaipao::CGameKuaipaoTable(pRoom, tableID);            // �ܵÿ�
			break;
		}
		case net::GAME_CATE_MAJIANG:
		{
//			if (playType == net::emMAJIANG_TYPE::MAJIANG_TYPE_SUINING)
//			{//�����齫
//				pTable = new game_majiang::CGameMaJiangSuiNingTable(pRoom, tableID);
//			}
//			else if (playType == net::emMAJIANG_TYPE::MAJIANG_TYPE_HONGZHONG_CZ)
//			{//���ݺ���
//				pTable = new game_majiang::CGameMaJiangHongZhongCZTable(pRoom, tableID);
//			}
//            else if (playType == net::emMAJIANG_TYPE::MAJIANG_TYPE_GZ_TUIDAOHU)
//            {//�����Ƶ���
//                pTable = new game_majiang::CGameMaJiangGzTuidaohuTable(pRoom, tableID);
//            }
//			else if (playType == net::emMAJIANG_TYPE::MAJIANG_TYPE_CHANGSHA
//					|| playType == net::emMAJIANG_TYPE::MAJIANG_TYPE_NINGXIANG)
//			{//��ɳ����
//				pTable = new game_majiang::CGameMaJiangChangShaTable(pRoom, tableID);
//			}
//			else
//			{
//				pTable = new game_majiang::CGameMaJiangTable(pRoom, tableID);            // �齫
//			}
			break;
		}
		case net::GAME_CATE_MAJIANG_XB:
		{
			//pTable = new game_majiang_xb::CGameMaJiangXBTable(pRoom, tableID);       // �����齫
			break;
		}
		default:
			ASSERT_LOG(false, "�������Ϸ����:{}", gameType);
			break;
		}
	}

	return pTable;
}
CGameCoinTable* CGameLogicMgr::CreateCoinTable(CGameRoom* pRoom, int64_t tableID)
{
	CGameCoinTable* pTable = NULL;
	uint16_t gameType = pRoom->GetGameType();
	if (pRoom->GetRoomType() == emROOM_TYPE_COMMON)
	{//���ɳ�
		switch (gameType)
		{

		default:
			ASSERT_LOG(false, "�������Ϸ����:{}", gameType);
			break;
		}

	}
	else if (pRoom->GetRoomType() == emROOM_TYPE_MATCH)
	{//������


	}

	return pTable;
}





























