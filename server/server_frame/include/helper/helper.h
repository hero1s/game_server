
#pragma once

#include <vector>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/types.h>
#include <asm/unistd.h>
#include <string>
#include "fundamental/common.h"
#include "utility/basic_types.h"

namespace svrlib {
class CHelper {
    typedef std::vector<unsigned char> BUF;
public:
    static size_t GetDirectoryFiles(std::string const& oPathDir, std::vector<std::string>& oFileNames);

    static size_t GetAllDirectories(std::string const& oPathDir, std::vector<std::string>& oDirNames);

    static std::string GetExeDir();

    static std::string GetExeFileName();

    static std::string GetLanIP();

    static std::string GetNetIP();

    static bool IsHaveNetIP();

    static bool IsLanIP(uint32_t uiIP);

    static size_t GetAllHostIPs(std::vector<uint32_t>& oIPs);

    // ip�����ַ���ת��
    static uint32_t IPToValue(const std::string& strIP);

    static std::string ValueToIP(uint32_t ulAddr);
    // ѹ����ѹ
    static int32_t Uncompress(const char* pdata, uint32_t uLen, uint8_t** pLocalBuff);

    static int32_t Compress(const char* pData, uint32_t size, uint8_t** pLocalBuff);

};
}


