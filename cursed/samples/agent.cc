#include <cursed/TextBox.hh>
#include <cursed/cursed.hh>
#include <addition/agent.h>
#include <exception>
#include <iostream>
#include <memory>


using namespace cursed;


int main()
{
	// Application app;

	// const int height = 35;
	// const int width  = 60;
	// int y = (app.screenHeight() - height) / 2;
	// int x = (app.screenWidth() - width) / 2;

	// Window window(app, L"DeepSleep Agent", height, width, y, x);

	// Label label(window,
	// 	L"Result of the current dialogue:\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n"
	// 	"State\n\n"
	// 	"State:\n\n"
	// 	"> Input: ",                  // text
	// 	window.getClientHeight(),   // height
	// 	window.getClientWidth(),    // width
	// 	0,                          // y
	// 	0 );                        // x

	// Button button(window,
	// 	L"Close",  // text
	// 	20,         // y
	// 	9,         // x
	// 	[](Button &button) -> void { button.getParent().close(); } );

	// TextBox statebox(window,
	// 	L"state:",
	// 	1,
	// 	30,
	// 	22,
	// 	9);

	// TextBox textbox(window,
	// 	L"Write something nice",  // text
	// 	3,                        // height
	// 	30,                       // width
	// 	24,                        // y
	// 	9 );                      // x

	// window.setActive(button);
	// window.showModal();
	try
	{
		std::shared_ptr<Dia_Tui> tui = std::make_shared<Dia_Tui>();
		tui->Initiate();
	}
	catch (const std::exception &error)
	{
		std::cerr << "agent: " << error.what() << std::endl;
		return 1;
	}

	return 0;
}
