#ifndef SFUI_APPMAIN_H
#define SFUI_APPMAIN_H

////////////////////////////////////////////////////////////
//
// MIT License
//
// Copyright(c) 2017 Kurt Slagle - kurt_slagle@yahoo.com
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files(the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and / or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions :
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//
// The origin of this software must not be misrepresented; you must not claim
// that you wrote the original software.If you use this software in a product,
// an acknowledgment of the software used is required.
//
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Internal Headers
////////////////////////////////////////////////////////////
#include <SFUI/Include/Common.h>
#include <SFUI/Include/Application/Concurrent.h>
#include <SFUI/Include/UI/UI.h>
#include <SFUI/Include/Application/AppWindow.h>
#include <SFUI/Include/UI/Theme.h>
#include <SFUI/Include/Application/AppLogger.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

extern int ExternCreateUI(sfui::Theme *uiTheme, std::function<void(sfui::Widget::shared_ptr)> addFunc);

#define START_PERFORMANCE_TIMING(START_TIME, FREQ) \
QueryPerformanceFrequency(&FREQ); \
QueryPerformanceCounter(&START_TIME); 

#define END_PERFORMANCE_TIMING(END_TIME, START_TIME, FREQ, ELAPSED) \
QueryPerformanceCounter(&END_TIME); \
ELAPSED.QuadPart = END_TIME.QuadPart - START_TIME.QuadPart; \
ELAPSED.QuadPart *= 1000000; \
ELAPSED.QuadPart /= FREQ.QuadPart;

namespace sfui
{

  struct AppMainInitData
  {
    int argc;
    const char **argv;
    std::string WindowTitle;
    sf::Uint32 WindowStyle;
    Vec2i WindowSize;
  };

  class AppWindow;
  class ConfirmDialog;

  extern Vec2i WindowSize;

  class AppMainWindow
  {
  public:
    // For last-second "Oh shit"'s
    static void TerminateHandler();

    static int Allocate();

    static int FailedStartup();
    static int UncaughtRuntimeException(std::exception &exc);

    static int Init(const AppMainInitData &InitData);
    static int CreateTheme();
    static int CreateUI();

    //Query our own events
    static int ProcessEvents();

    static int BeginFrame();
    static int EndFrame();

    static bool IsRunning();

    static void SetDefaultTheme(std::shared_ptr<Theme> themePtr);

    //Render us
    static int Render();

    //Begin shutdown (proper)
    static int Shutdown();

    //Update application
    static int Update();

    //Enter an error state - will loop in here indefinitely
    // until it can find a resolution
    static void Error();

    //Begin to close the app
    static int Close();

    //Close asap, try to clean up but just let 
    // something go if it doesn't quickly cooperate
    static int Abort();

    static void AddWidget(Widget::shared_ptr widget);

    static std::shared_ptr<WidgetWindow> AppMainWidgets;

    static AppWindowHandle* LaunchPopup(Vec2i size, const std::string &title, sf::Uint32 style);
    static AppWindowHandle* Confirm(Vec2i Size, const std::string &Title, sf::Uint32 style,
                                    const std::string &Message, const std::string &CnclTxt, const std::string &OKTxt,
                                    boost::function<void()> cancelled, boost::function<void()> accepted);

  private:
    AppMainWindow() = delete;
    ~AppMainWindow() = delete;

    static std::shared_ptr<Theme> m_DefaultTheme;
    static std::vector<std::shared_ptr<AppWindow>> m_Windows;
    static std::vector<AppWindowHandle*> WindowHandles;
    static bool MarkForShutdown;
    static std::shared_ptr<sf::RenderWindow> AppMainRenderWindow;
    static LARGE_INTEGER AppStartTime;
    static LARGE_INTEGER AppEndTime;
    static LARGE_INTEGER AppFrequency;
    static LARGE_INTEGER AppElapsedTimeRun;

    static LARGE_INTEGER FrameStart;
    static LARGE_INTEGER FrameEnd;
    static LARGE_INTEGER FrameElapsed;

  };

}

#endif // SFUI_APPMAIN_H
