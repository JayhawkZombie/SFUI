#ifndef SFUI_CONFIRMDIALOG_H
#define SFUI_CONFIRMDIALOG_H

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
#include <SFUI/Include/Application/AppWindow.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ConfirmDialog : public AppWindow
  {
  public:
    friend class AppMainWindow;

    ConfirmDialog(Vec2i Size, const std::string &Title, sf::Uint32 style,
                  const std::string &Message, const std::string &CnclTxt, const std::string &OKTxt, 
                  boost::function<void()> cancelled, boost::function<void()> accepted, std::shared_ptr<Theme> themePtr);
    virtual ~ConfirmDialog() override;

  protected:
    virtual void PostLaunch() override;
    virtual void Cancelled();
    virtual void Accepted();

    virtual void PositionWidgets();

    boost::function<void()> m_CancelledCallback = [ ]() { };
    boost::function<void()> m_AcceptedCallback = [ ]() { };
    Theme::shared_ptr m_ThemePtr;

    std::string m_MessageText;
    MultilineTextView::shared_ptr m_Message;
    Button::shared_ptr m_CancelBtn;
    Button::shared_ptr m_AcceptBtn;

  };
  
}  

#endif // SFUI_CONFIRMDIALOG_H
