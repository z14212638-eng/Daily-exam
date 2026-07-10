#include <cursed/Application.hh>
#include <ncurses.h>
#include <iostream>
#include <cursed/cursed.hh>
#include <string>

static const int window_height = 35;
static const int window_width = 60;

namespace cursed
{

/**
 * @brief The official entrance of the Tui
 * 
 */

class Dia_Tui : public Application
{
    public:
    Dia_Tui() = default;
    ~Dia_Tui() = default;
    void Initiate();
    private:
    const std::wstring window_text = L"DeepSleep";
    int y = (screenHeight() - window_height) / 2;
    int x = (screenWidth() - window_width) / 2;

};

std::wstring BoxLine(const std::wstring &text, size_t width);

std::wstring ResultBox(const std::string &order);

}