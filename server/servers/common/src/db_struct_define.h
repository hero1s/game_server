
#pragma once

// ��һ�������
struct stPlayerBaseInfo
{
  string name;         // ����
  uint8_t  sex;          // �Ա�
  uint32_t offlinetime;  // ����¼ʱ��
  string safepwd;      // ����������
  uint8_t  safeboxState; // ������״̬
  uint32_t clogin;       // ������½����
  uint8_t  weekLogin;    // �����ۼƵ�½
  uint8_t  bankrupt;     // �Ʋ�����
  uint32_t loginIP;      // ��½IP
  uint32_t allLoginDays; // �ۼƵ�½����
  int64_t  roomCard;     // ����
  int64_t  coin;         // ���
  int32_t  vip;          // vipֵ
  int64_t  safecoin;     // �������ʲ�
  int64_t  clubID;       // ���ھ��ֲ�ID
  double lon;           // ��γ��
  double lat;           // ��γ��

  stPlayerBaseInfo()
  {
	  clear();
  }

  void clear()
  {
	  name         = "";
	  sex          = 0;
	  offlinetime  = 0;
	  safepwd      = "";
	  safeboxState = 0;
	  clogin       = 0;
	  weekLogin    = 0;
	  bankrupt     = 0;
	  loginIP      = 0;
	  allLoginDays = 0;
	  roomCard     = 0;
	  coin         = 0;
	  vip          = 0;
	  safecoin     = 0;
	  clubID       = 0;
	  lon          = 0;
	  lat          = 0;

  }
};

enum emACC_VALUE_TYPE
{
  emACC_VALUE_ROOMCARD = 1,        // ����
  emACC_VALUE_COIN     = 2,        // ���
  emACC_VALUE_SAFECOIN = 3,        // ������ֵ

  emACC_VALUE_MAX,                 // max
};

// ����������Ϣ
struct stRoomCfg
{
  uint16_t roomID;       // ����ID
  uint8_t  consume;      // ��������
  int64_t  enter_min;    // �����ż�
  int64_t  enter_max;    // ��������
  int64_t  baseScore;    // �׷�
  uint8_t  roomType;     // ��������(0��ͨ��,1˽�˷�)
  uint16_t tableNum;     // ����������
  uint8_t  marry;        // ƥ�䷽ʽ
  uint8_t  limitEnter;   // ��������(ͨ����СЯ��)
  uint32_t showonline;   // ��ʾ����
  int64_t  sitdown;      // ��������
  uint8_t  feeType;      // ̨������
  int32_t  feeValue;     // ̨��ֵ
  uint16_t seatNum;      // ��λ��
  uint16_t showType;     // ��ʾ����
  uint16_t showPic;      // ��ʾͼƬ

  stRoomCfg()
  {
	  memset(this, 0, sizeof(stRoomCfg));
  }
};

//����������
struct stMissionPrizeCfg
{
  uint32_t poid;   //����id
  uint32_t qty;    //����
  stMissionPrizeCfg()
  {
	  memset(this, 0, sizeof(stMissionPrizeCfg));
  }
};

//����������Ϣ
struct stMissionCfg
{
  uint32_t                    msid;        //����ID
  uint16_t                    type;        //��������
  uint8_t                     autoprize;   //�Զ��콱
  uint32_t                    cate1;       //����
  uint32_t                    cate2;       //����
  uint32_t                    cate3;       //����
  uint32_t                    cate4;       //����
  uint32_t                    mtimes;      //�ﵽ����
  uint8_t                     straight;     //�Ƿ�����
  uint8_t                     cycle;        //����
  uint32_t                    cycletimes;  //����ɴ���
  uint8_t                     status;       //����״̬
  vector<stMissionPrizeCfg> missionprize; //������
};

//���������Ϣ
struct stUserMission
{
  uint32_t msid;    // ID
  uint32_t rtimes;  // �������
  uint32_t ctimes;  // ������ɴ���
  uint32_t ptime;   // ����ʱ��
  uint8_t  update;  // �������ݿ�����
  uint32_t cptime;  // ���ʱ��
  stUserMission()
  {
	  memset(this, 0, sizeof(stUserMission));
  }
};

// ��������
enum emACCDATA_TYPE
{
  emACCDATA_TYPE_BASE = 0,    // ������Ϣ
  emACCDATA_TYPE_MISS,        // ��������

  emACCDATA_TYPE_MAX,
};

enum emDBACTION
{
  emDB_ACTION_NONE = 0,           //������
  emDB_ACTION_UPDATE,             //�޸�
  emDB_ACTION_INSERT,             //����
  emDB_ACTION_DELETE,             //ɾ��
};

// ˽�˷���Ϣ
struct stPrivateTable
{
  int64_t  tableID;     // ����ID
  string passwd;      // ����
  uint32_t hostID;      // ����ID
  uint16_t dueround;    // ��������
  uint32_t dueTime;     // ����ʱ��
  uint32_t startTime;   // ��ʼʱ��
  int64_t  baseScore;   // �׷�
  uint16_t seatNum;     // ��λ��
  uint16_t gameType;    // ��Ϸ����
  uint8_t  playType;    // �淨
  string addParam;    // �����ַ�������
  string matchParam;  // ��������
  int32_t  enterMin;    // ��С����
  uint8_t  consume;     // ��������
  uint8_t  rentType;    // ����֧����ʽ
  uint32_t fee;         // ����
  int64_t  clubID;      // ���ֲ�ID
  int64_t  clubFloor;   // ���ֲ�¥��
  uint16_t clubPos;     // ���ֲ�����λ��
  uint8_t  idxRound;    // ����ѡ��

  uint32_t createTime;  // ����ʱ��
  uint8_t  createType;  // ������ʽ
  string gameInfo;    // ��Ϸ��Ϣ

  stPrivateTable()
  {
	  tableID    = 0;
	  hostID     = 0;
	  passwd     = "";
	  hostID     = 0;
	  baseScore  = 0;
	  seatNum    = 0;
	  dueTime    = 0;
	  startTime  = 0;
	  gameType   = 0;
	  playType   = 0;
	  addParam   = ""; // ���Ӳ���
	  matchParam = "";// ��������
	  enterMin   = 0;
	  consume    = 0;
	  rentType   = 0;
	  fee        = 0;
	  clubID     = 0;
	  clubFloor  = 0;
	  clubPos    = 0;
	  idxRound   = 0;

	  createType = 0;
	  createTime = 0;
	  gameInfo   = "";
  }
};

// ˽�˷�������
struct stRoomCardCfg
{
  uint8_t  idx;         // ���
  uint16_t gameType;    // ��Ϸ����
  uint8_t  playType;    // �淨
  uint8_t  round;       // ����
  uint8_t  card;        // ����
  uint8_t  cardAA;      // ֧����ʽ
  stRoomCardCfg()
  {
	  memset(this, 0, sizeof(stRoomCardCfg));
  }
};
// ��Ϸ��Ϣ
struct stGameListCfg
{
  uint32_t gameID;    // ��ϷID
  uint16_t gameType;  // ��Ϸ����
  uint16_t playType;  // �淨
  uint8_t  contest;   // �Ƿ񿪷ű���
  stGameListCfg()
  {
	  memset(this, 0, sizeof(stGameListCfg));
  }
};

// �Ʒ���Ϣ
struct stCalcScoreInfo
{
  uint32_t uid;
  int64_t  score;
  int64_t  fee;
  int64_t  buyIn;
  string param;
  int64_t  roundWin;//��ǰС����Ӯ

  stCalcScoreInfo()
  {
	  Reset();
  }
  void Reset()
  {
	  uid      = 0;
	  score    = 0;
	  fee      = 0;
	  buyIn    = 0;
	  param    = "";
	  roundWin = 0;
  }
  int64_t WinScore()
  {
	  return score - buyIn;
  }

};








