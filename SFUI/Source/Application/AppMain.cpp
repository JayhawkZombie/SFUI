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
#include <SFUI/Include/Application/AppMain.h>
#include <SFUI/Include/Application/AppWindow.h>
#include <SFUI/Include/Application/ConfirmDialog.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <ppltasks.h>

namespace sfui
{

  Vec2i WindowSize = Vec2i(0, 0);

  void AppMainWindow::TerminateHandler()
  {
    APP_LOG_FAT("Application terminated by call to std::terminate");

    exit(-10);
  }

  int AppMainWindow::Allocate()
  {
    APP_LOG("Allocating OS render window");
    AppMainRenderWindow = std::make_shared<sf::RenderWindow>();

    APP_LOG("Allocating main UI widget handler");
    AppMainWidgets = std::make_shared<sfui::WidgetWindow>(*AppMainRenderWindow);
    
    return 0;
  }

  int AppMainWindow::FailedStartup()
  {
    //Uh oh

    try
    {
      return Shutdown();
    }
    catch (const std::exception &exc)
    {
      APP_LOG_EXCEPTION("Main thread - Shutdown failure. Terminating", exc);

      terminate();
    }
  }

  int AppMainWindow::UncaughtRuntimeException(std::exception &exc)
  {
    APP_LOG_EXCEPTION("Main thread - uncaught runtime exception", exc);

    return 0;
  }

  int AppMainWindow::Init(const AppMainInitData &InitData)
  {
    try
    {
      //Start timing for the app's total time open
      START_PERFORMANCE_TIMING(AppStartTime, AppFrequency);

      LARGE_INTEGER InitStart, InitEnd, ElspasedMicroseconds;
      LARGE_INTEGER Frequency;
      //Start timing for window creation
      START_PERFORMANCE_TIMING(InitStart, Frequency);

      int alloc_ret = Allocate();
      sf::ContextSettings set;
      set.majorVersion = 3;
      set.minorVersion = 3;
      WindowSize = Vec2i(InitData.WindowSize);
      AppMainRenderWindow->create(sf::VideoMode(InitData.WindowSize.x, InitData.WindowSize.y), InitData.WindowTitle, InitData.WindowStyle, set);

      //End timing for window creation and log the time
      END_PERFORMANCE_TIMING(InitEnd, InitStart, Frequency, ElspasedMicroseconds);
      APP_LOG_TASK_PERFORMANCE("Initial Window Allocation & Creation", ElspasedMicroseconds);

      AppMainRenderWindow->setVerticalSyncEnabled(false);
      AppMainWidgets->SetSize(InitData.WindowSize);

      int uiStatus = 0;
      int themeStatus = 0;

      //Start timing for concurrent ui creation
      START_PERFORMANCE_TIMING(InitStart, Frequency);

      auto CreatorTask = concurrency::create_task(
        []()
      {
        try { return CreateTheme(); }
        catch (std::exception &exc) { APP_LOG_EXCEPTION("Failed to create ui theme", exc); return -1; }
      }
      ).then([&InitStart, &Frequency](int themeRet)
      {
        if (themeRet != 0)
          return -1;

        START_PERFORMANCE_TIMING(InitStart, Frequency);
        try { return CreateUI(); }
        catch (const std::exception &exc) { APP_LOG_EXCEPTION("Failed to create ui", exc); return -1; }
      }
      );

      CreatorTask.wait();

      //End timing for ui creation and log the time
      END_PERFORMANCE_TIMING(InitEnd, InitStart, Frequency, ElspasedMicroseconds);
      APP_LOG_TASK_PERFORMANCE("UI Creation (concurrent)", ElspasedMicroseconds);

      if (CreatorTask.get() != 0)
        return -3;

      APP_LOG("Loading UI Shaders");
      sfui::DropShadow::LoadShaders();

      return 0;
    }
    catch (const std::exception &exc)
    {
      APP_LOG_EXCEPTION("Main thread - initialization failure", exc);

      return -1;
    }

    return 0;
  }

  int AppMainWindow::CreateTheme()
  {
    try
    {
      m_DefaultTheme = std::make_shared<sfui::Theme>();

      texture_handle iconTexture = std::make_shared<sf::Texture>();
      iconTexture->loadFromFile("iconmap_grey.png");

      font_handle hFont = std::make_shared<sf::Font>();
      if (!hFont->loadFromFile("segoeui.ttf")) {
        APP_LOG_LOAD_FAILURE("segoeui.ttf");
        return -1;
      }

      m_DefaultTheme->DefaultFont = hFont;
      m_DefaultTheme->IconTexture = iconTexture;

      //Load bitmap fonts
      m_DefaultTheme->BitmapFonts.LoadFont("opensans", "Fonts/BitmapFonts/Images/osans14.png", "Fonts/BitmapFonts/Data/osans14.fnt", 14);
      m_DefaultTheme->BitmapFonts.LoadFont("sugo", "Fonts/BitmapFonts/Images/sugo16_light.png", "Fonts/BitmapFonts/Data/sugo16_light.fnt", 16);
      m_DefaultTheme->DefaultBitmapFont = m_DefaultTheme->BitmapFonts.GetFont("sugo", 16).value_or(nullptr);

      return 0;
    }
    catch (const std::exception &exc)
    {
      APP_LOG_EXCEPTION("Failed to create theme", exc);
      return -1;
    }
  }

  int AppMainWindow::CreateUI()
  {
    try { return ExternCreateUI(m_DefaultTheme.get(), [ ](Widget::shared_ptr wptr) { sfui::AppMainWindow::AddWidget(wptr); }); }
    catch (const std::exception &exc) 
    { 
      APP_LOG_EXCEPTION("Failed to create ui", exc);
      return -1; 
    }
  }

  int AppMainWindow::ProcessEvents()
  {
    static sf::Event event;

    while (AppMainRenderWindow->pollEvent(event)) {
      if (event.type == sf::Event::Closed)
        MarkForShutdown = true;
      else if (event.type == sf::Event::Resized) {
        sf::View newview;
        auto newsize = AppMainRenderWindow->getSize();
        newview.reset(FloatRect(0.f, 0.f, cast_float(newsize.x), cast_float(newsize.y)));
        newview.setViewport(FloatRect(0.f, 0.f, 1.f, 1.f));
        AppMainRenderWindow->setView(newview);
      }

      AppMainWidgets->HandleEvent(event);
    }

    return 0;
  }

  bool AppMainWindow::IsRunning()
  {
    return ( !MarkForShutdown );
  }

  void AppMainWindow::SetDefaultTheme(std::shared_ptr<Theme> themePtr)
  {
    m_DefaultTheme = themePtr;
  }

  int AppMainWindow::Render()
  {
    //AppMainRenderWindow->setActive(true);
    AppMainRenderWindow->clear(sf::Color(122, 122, 122));
    AppMainWidgets->Render();
    AppMainRenderWindow->display();
    //AppMainRenderWindow->setActive(false);
    return 0;
  }

  int AppMainWindow::Shutdown()
  {
    try
    {
      // Signal windows to close down
      m_DefaultTheme = nullptr;

      for (auto & handle : WindowHandles)
        delete handle;
      WindowHandles.clear();
      m_Windows.clear();

      sfui::DropShadow::CleanupShaders();
      // After all windows are closed, shut down ourselves
      if (AppMainWidgets) {
        AppMainWidgets->Cleanup();
        AppMainWidgets.reset();
      }

      if (AppMainRenderWindow) {
        AppMainRenderWindow->close();
        AppMainRenderWindow.reset();
      }

      //Log how long the application was run for
      END_PERFORMANCE_TIMING(AppEndTime, AppStartTime, AppFrequency, AppElapsedTimeRun);
      std::stringstream appTimeStr; appTimeStr.str("");
      appTimeStr << "Application was run for " << static_cast< uint32 >( floor(AppElapsedTimeRun.QuadPart * 0.001) ) << " ms";
      APP_LOG(appTimeStr.str());
    }
    catch (const std::exception &exc)
    {
      APP_LOG_EXCEPTION("Failed propert shutdown", exc);
      throw;
    }

    return 0;
  }

  int AppMainWindow::Update()
  {
    try
    {
      AppMainWidgets->Update();

      // Poll if any of our child windows were closed
      auto it = m_Windows.begin();
      while (it != m_Windows.end()) {
        if (( *it )->IsDone()) {
          ( *it )->m_Thread.join();
          break;
        }
        ++it;
      }

      if (it != m_Windows.end()) {
        m_Windows.erase(it);
      }

    }
    catch (const std::exception &e)
    {
      APP_LOG_EXCEPTION("Exception while updating", e);
      return -1;
    }

    return 0;
  }

  void AppMainWindow::Error()
  {

  }

  int AppMainWindow::Close()
  {
    //Synchronize();
    return 0;
  }

  int AppMainWindow::Abort()
  {
    return 0;
  }

  void AppMainWindow::AddWidget(Widget::shared_ptr widget)
  {
    AppMainWidgets->Add(widget);
  }
  
  std::shared_ptr<sfui::Theme> AppMainWindow::m_DefaultTheme;

  std::vector<std::shared_ptr<sfui::AppWindow>> AppMainWindow::m_Windows;

  std::vector<AppWindowHandle*> AppMainWindow::WindowHandles;

  bool AppMainWindow::MarkForShutdown = false;

  std::shared_ptr<sf::RenderWindow> AppMainWindow::AppMainRenderWindow;


  LARGE_INTEGER AppMainWindow::AppStartTime;

  LARGE_INTEGER AppMainWindow::AppEndTime;

  LARGE_INTEGER AppMainWindow::AppFrequency;

  LARGE_INTEGER AppMainWindow::AppElapsedTimeRun;

  std::shared_ptr<sfui::WidgetWindow> AppMainWindow::AppMainWidgets;

  AppWindowHandle* AppMainWindow::LaunchPopup(Vec2i size, const std::string &title, sf::Uint32 style)
  {
    std::shared_ptr<AppWindow> win = std::make_shared<AppWindow>(size, title, style);
    m_Windows.push_back(win);

    WindowHandles.push_back(new AppWindowHandle(win.get()));
    return WindowHandles.back();
  }

  sfui::AppWindowHandle* AppMainWindow::Confirm(Vec2i Size, const std::string &Title, sf::Uint32 style, const std::string &Message, const std::string &CnclTxt, const std::string &OKTxt, boost::function<void()> cancelled, boost::function<void()> accepted)
  {
    std::shared_ptr<ConfirmDialog> win = std::make_shared<ConfirmDialog>(Size, Title, style, Message, CnclTxt, OKTxt, cancelled, accepted, m_DefaultTheme);
    m_Windows.push_back(win);
    win->PositionWidgets();

    WindowHandles.push_back(new AppWindowHandle(win.get()));
    return WindowHandles.back();
  }

}
