#include <addition/tools.h>
#include <cursed/Window.hh>

std::wstring CurrentTime() {
  const std::time_t now = std::time(nullptr);
  std::tm local_time;
  localtime_r(&now, &local_time);

  wchar_t buffer[9];
  if (std::wcsftime(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%H:%M:%S",
                    &local_time) == 0)
    return L"--:--:--";

  return buffer;
}

const std::string Get_home_path()
{
    const char *home_env = std::getenv("HOME");
    if (home_env == nullptr)
        return "";

    return std::string(home_env);
}
