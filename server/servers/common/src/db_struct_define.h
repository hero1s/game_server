
#pragma once

// ��һ�������
struct stPlayerBaseInfo
{
  string name;           // ����
  uint8_t  sex;          // �Ա�
  uint32_t offlinetime;  // ����¼ʱ��
  uint32_t clogin;       // ������½����
  uint8_t  weekLogin;    // �����ۼƵ�½
  uint8_t  bankrupt;     // �Ʋ�����
  uint32_t loginIP;      // ��½IP
  uint32_t allLoginDays; // �ۼƵ�½����
  int64_t  coin;         // ���
  int32_t  vip;          // vipֵ
  double lon;            // ��γ��
  double lat;            // ��γ��

  stPlayerBaseInfo()
  {
	  clear();
  }

  void clear()
  {
	  name         = "";
	  sex          = 0;
	  offlinetime  = 0;
	  clogin       = 0;
	  weekLogin    = 0;
	  bankrupt     = 0;
	  loginIP      = 0;
	  allLoginDays = 0;
	  coin         = 0;
	  vip          = 0;
	  lon          = 0;
	  lat          = 0;
  }
};

// ��������
enum emACCDATA_TYPE
{
  emACCDATA_TYPE_BASE = 0,    // ������Ϣ
  emACCDATA_TYPE_MISS,        // ��������

  emACCDATA_TYPE_MAX,
};










