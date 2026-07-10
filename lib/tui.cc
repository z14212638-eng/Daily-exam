#include "addition/history.h"
#include "cursed/TextBox.hh"
#include <addition/tui.h>
#include <addition/utils.h>
#include <cursed/Application.hh>
#include <addition/input.h>
#include <addition/tools.h>
#include <cwchar>
#include <ncurses.h>
#include <sstream>
/**
 * @brief All the settings are defined in the namespace cursed
 *
 */

namespace cursed {

    /**
     * @brief BoxLine is used to draw the shape like  | xxxx text xxxx |
     * 
     * @param text the centered text 
     * @param width the width of the dialogue window
     * @return std::wstring 
     */
std::wstring BoxLine(const std::wstring &text, size_t width)
{
  std::wstring content = text;
  if (content.length() > width)
    content = content.substr(0, width);

  return L"|" + content + std::wstring(width - content.length(), L' ') + L"|";
}

/**
 * @brief ResultBox is used for the presentation of the result, in some special 
 * circumstances the result would vary according to the input, such as "/history"
 * 
 * @param order referring to the exact order in the input textbox
 * @return std::wstring 
 */
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
  
  /* pre-defined output as we actually do not use a real agent */
  std::wstring notice = L"## `429` Too many Requests";
  std::wstring response = L"**Sever Busy Try Later QAQ**";

  /* history command */
  if (clean_order == "/history")
  {
    std::wstring output = border + L"\n" + BoxLine(text, inner_width) + L"\n";
    int lines = 0;

    for (auto it = history.rbegin(); it != history.rend() && lines < 4; ++it)
    {
      output += BoxLine(m_StringToWide(*it), inner_width) + L"\n";
      ++lines;
    }

    if (lines == 0)
    {
      output += BoxLine(L"(empty history)", inner_width) + L"\n";
      ++lines;
    }

    while (lines < 4)
    {
      output += BoxLine(L"", inner_width) + L"\n";
      ++lines;
    }

    return output + border;
  }
  /* General situation: commands that is not pre-defined */
  else if(!clean_order.empty())
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

/**
 * @brief Initiate the Dialogue TUI, including UI-build, logic setting
 * 
 */
void Dia_Tui::Initiate() {

  const int screen_height = this->screenHeight();
  const int screen_width = this->screenWidth();

  int y = (screen_height - window_height - 1) / 2;
  int x = (screen_width - window_width - 2) / 2;

  Window window(*this, L">>>>> [DeepSleep] <<<<<", window_height, window_width, y, x);

  /* Get the Absolute Path of Home and current working directory */
  const std::string home_path = Get_home_path();
  std::string current_path = Get_Current_Path();
  current_path = Replace_Prefix(home_path, current_path);
  const std::wstring current_path_m = m_StringToWide(current_path);

  /* Display the text */
  std::ostringstream oss;
  oss << "Result of the current dialogue:"
      << "\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
      << "State\n\n"
      << "PATH: " << "\n\n"
      << "User Input (Begin with /):";
  const std::string window_label = oss.str();
  const std::wstring window_label_m = m_StringToWide(window_label);

  Label label(window,
              window_label_m,           // text
              window.getClientHeight(), // height
              window.getClientWidth(),  // width
              0,                        // y
              0);                       // x

  Label result_box(window, ResultBox(""), 14, 54, 1, 1);

  /* Display the current diretory */
  TextBox statebox(window, current_path_m, 1, 40, 20, 9);

  /* Input textbox */
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
      /* Clear the input box before next operation*/
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
