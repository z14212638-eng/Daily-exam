#include "addition/history.h"
#include "cursed/TextBox.hh"
#include <addition/tui.h>
#include <addition/utils.h>
#include <cursed/Application.hh>
#include <addition/input.h>
#include <ctime>
#include <cwchar>
#include <ncurses.h>
#include <sstream>
#include <stdexcept>
/**
 * @brief This function is used to force stop the Tui thread
 *
 */

namespace cursed {

namespace {

std::wstring CurrentTime()
{
  const std::time_t now = std::time(nullptr);
  std::tm local_time;
  localtime_r(&now, &local_time);

  wchar_t buffer[9];
  if (std::wcsftime(buffer, sizeof(buffer) / sizeof(buffer[0]), L"%H:%M:%S",
                    &local_time) == 0)
    return L"--:--:--";

  return buffer;
}

std::wstring BoxLine(const std::wstring &text, size_t width)
{
  std::wstring content = text;
  if (content.length() > width)
    content = content.substr(0, width);

  return L"|" + content + std::wstring(width - content.length(), L' ') + L"|";
}

std::wstring ResultBox(const std::string &order)
{
  static const size_t inner_width = 52;
  const std::wstring border = L"+" + std::wstring(inner_width, L'-') + L"+";

  std::string clean_order = order;
  for (char &ch : clean_order)
  {
    if (ch == '\n' || ch == '\r')
      ch = ' ';
  }

  std::wstring text = L"User Input: ";
  if (!clean_order.empty())
    text += m_StringToWide(clean_order);
  
  std::wstring notice = L"## `429` Too many Requests";
  std::wstring response = L"**Sever Busy Try Later QAQ**";

  if(!clean_order.empty())
  {
      return border + L"\n" +
         BoxLine(text, inner_width) + L"\n" +
         BoxLine(notice, inner_width) + L"\n" +
         BoxLine(response, inner_width) + L"\n" +

         BoxLine(L"", inner_width) + L"\n" +
         BoxLine(L"", inner_width) + L"\n" +
         border;
  }
  else {
  return border + L"\n" +
         BoxLine(L"", inner_width) + L"\n" +
         BoxLine(L"", inner_width) + L"\n" +
         border;
  }

}

} // namespace

void Dia_Tui::Initiate() {

  const int screen_height = this->screenHeight();
  const int screen_width = this->screenWidth();

  int y = (screen_height - window_height - 1) / 2;
  int x = (screen_width - window_width - 2) / 2;

  Window window(*this, L"DeepSleep Agent", window_height, window_width, y, x);

  const char *home_env = std::getenv("HOME");
  const std::string home_path = home_env == nullptr ? "" : home_env;
  std::string current_path = Get_Current_Path();
  std::ostringstream oss;
  oss << "Result of the current dialogue:"
      << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
      << "State\n\n"
      << "PATH: " << "\n\n"
      << "User Input (Begin with /):";

  const std::string window_label = oss.str();
  const std::wstring window_label_m = m_StringToWide(window_label);

  current_path = Replace_Prefix(home_path, current_path);

  const std::wstring current_path_m = m_StringToWide(current_path);
  //   Label label(
  //       window,
  //       L"Result of the current
  //       dialogue:\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n" "State\n\n"
  //       "State:" current_path "\n\n"
  //       "> Input: ",              // text
  //       window.getClientHeight(), // height
  //       window.getClientWidth(),  // width
  //       0,                        // y
  //       0);                       // x
  Label label(window,
              window_label_m,           // text
              window.getClientHeight(), // height
              window.getClientWidth(),  // width
              0,                        // y
              0);                       // x

  Label result_box(window, ResultBox(""), 7, 54, 1, 1);

  TextBox statebox(window, current_path_m, 1, 40, 20, 9);

  TextBox textbox(window,
                  L">", // text
                  5,                       // height
                  40,                      // width
                  23,                      // y
                  9);                      // x
  textbox.setOnSubmit([&textbox,&result_box, &window](const std::string &order) {
      std::string display_order = order;

      if (!display_order.empty() && display_order[0] == '>')
          display_order.erase(0, 1);

      if (display_order == "/exit") {
          window.close();
          return;
      }

      if (display_order == "/read") {
          result_box.setText(ResultBox("Read command received"));
          textbox.clear();
          return;
      }

      result_box.setText(ResultBox(display_order));
      textbox.clear();
  });


  bool show_time = false;
  Label time_label(window, L"", 1, 8, 18, 23);
  time_label.setTheme(THEME_LABEL_TIME);

  Button button(window, L"Show Time", 18, 9,
                [&textbox, &time_label, &show_time](Button &button) {
    show_time = true;
    time_label.setText(CurrentTime());
    button.getParent().setActive(textbox);
    // textbox.Waiting_for_input();
  });

  window.setTimer(1000, [&time_label, &show_time]() {
    if (!show_time)
      return false;

    time_label.setText(CurrentTime());
    return true;
  });



  window.setActive(button);
  window.showModal();
}
} // namespace cursed
