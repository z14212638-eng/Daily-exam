#include <addition/utils.h>

#include <cerrno>
#include <codecvt>
#include <filesystem>
#include <locale>
#include <stdexcept>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

std::string Get_Current_Path() {
#ifdef _WIN32
  DWORD size = GetCurrentDirectoryA(0, nullptr);
  if (size == 0)
    return "";

  std::vector<char> buffer(size);
  if (GetCurrentDirectoryA(size, buffer.data()) == 0)
    return "";
  return std::string(buffer.data());
#else
  std::vector<char> buffer(256);

  while (getcwd(buffer.data(), buffer.size()) == nullptr) {
    if (errno != ERANGE)
      return "";
    buffer.resize(buffer.size() * 2);
  }

  return std::string(buffer.data());
#endif
}

std::wstring m_StringToWide(const std::string &str) {
  if (str.empty())
    return L"";

#ifdef _WIN32
  int input_size = static_cast<int>(str.size());
  int size = MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.data(),
                                 input_size, nullptr, 0);
  if (size == 0)
    return L"";

  std::wstring wstr(size, 0);
  if (MultiByteToWideChar(CP_UTF8, MB_ERR_INVALID_CHARS, str.data(), input_size,
                          &wstr[0], size) == 0)
    return L"";

  return wstr;
#else
  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.from_bytes(str);
  } catch (const std::range_error &) {
    std::wstring fallback;
    fallback.reserve(str.size());
    for (unsigned char byte : str)
      fallback.push_back(static_cast<wchar_t>(byte));
    return fallback;
  }
#endif
}

std::string m_WideToString(const std::wstring &str) {
  if (str.empty())
    return "";

#ifdef _WIN32
  int input_size = static_cast<int>(str.size());
  int size = WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(),
                                 input_size, nullptr, 0, nullptr, nullptr);
  if (size == 0)
    return "";

  std::string result(size, 0);
  if (WideCharToMultiByte(CP_UTF8, WC_ERR_INVALID_CHARS, str.data(), input_size,
                          &result[0], size, nullptr, nullptr) == 0)
    return "";

  return result;
#else
  try {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    return converter.to_bytes(str);
  } catch (const std::range_error &) {
    std::string fallback;
    fallback.reserve(str.size());
    for (wchar_t ch : str)
      fallback.push_back(ch >= 0 && ch <= 0x7F ? static_cast<char>(ch) : '?');
    return fallback;
  }
#endif
}

std::string Replace_Prefix(const std::string &pre, std::string entire) {
  if (!pre.empty() && entire.compare(0, pre.size(), pre) == 0) {
    entire.replace(0, pre.size(), "~");
  }

  return entire;
}
