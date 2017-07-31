#ifndef SFUI_APPENTRY_H
#define SFUI_APPENTRY_H

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

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

//Application main entry point

int main(int argc, const char **argv)
{

  //Create initialization data
  sfui::AppMainInitData initData;
  initData.argc = argc;
  initData.argv = argv;
  initData.WindowTitle = "SFUI";
  initData.WindowSize = Vec2i(1400, 800);
  initData.WindowStyle = sf::Style::Default;

  APP_LOG("Initializing application data");

  //Try to initialize, but initiate a shutdown if it fails
  auto initStatus = sfui::AppMainWindow::Init(initData);
  if (initStatus != 0)
    return sfui::AppMainWindow::FailedStartup();

  APP_LOG("Beginning main loop");

  while (sfui::AppMainWindow::IsRunning()) {

    //A non-zero return is only used for unrecoverable errors.
    // Errors handled internally (successfully handled) will still return 0
    try
    {
      if (sfui::AppMainWindow::ProcessEvents() != 0)
        throw std::runtime_error("Event Processing Error");
      if (sfui::AppMainWindow::Update() != 0)
        throw std::runtime_error("Update Error");
      if (sfui::AppMainWindow::Render() != 0)
        throw std::runtime_error("Rendering Error");
    }
    catch (const std::exception &exc)
    {
      APP_LOG_EXCEPTION("Uncaught runtime error", exc);

      return sfui::AppMainWindow::Abort();
    }

  }

  APP_LOG("Beginning application shutdown");
  try { return sfui::AppMainWindow::Shutdown(); }
  catch (const std::exception&) { return -1; }
}

#endif // SFUI_APPENTRY_H
