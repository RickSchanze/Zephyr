/**
 * @file Guid.cpp
 * @author Echo 
 * @Date 2024/2/27
 * @brief 
 */

#include "Guid.h"

Guid::Guid() {
    CoCreateGuid(&m_guid);
    // TODO: Log if failed
}

std::string Guid::ToString() const {
    char buffer[GUID_LENGTH] = {0};
    sprintf_s(buffer, "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X",
            m_guid.Data1, m_guid.Data2, m_guid.Data3,
            m_guid.Data4[0], m_guid.Data4[1], m_guid.Data4[2], m_guid.Data4[3],
            m_guid.Data4[4], m_guid.Data4[5], m_guid.Data4[6], m_guid.Data4[7]);
    return {buffer};
}
