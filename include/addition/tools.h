#ifndef TOOLS_H
#define TOOL_H

#include <ctime>
#include <cursed/Window.hh>
#include <string>

std::wstring CurrentTime();

void show_real_time(cursed::Window);

const std::string Get_home_path();

#endif