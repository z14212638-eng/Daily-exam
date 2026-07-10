#ifndef ADDITION_UTILS_H
#define ADDITION_UTILS_H

#include <string>

std::string Get_Current_Path();

std::wstring m_StringToWide(const std::string& str);

std::string m_WideToString(const std::wstring& str);

std::string Replace_Prefix(const std::string& pre, std::string entire);

#endif
