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
#include <SFUI/Include/Application/ConfirmDialog.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ConfirmDialog::ConfirmDialog(Vec2i Size, const std::string &Title, sf::Uint32 style,
                               const std::string &Message, const std::string &CnclTxt, const std::string &OKTxt,
                               boost::function<void()> cancelled, boost::function<void()> accepted, std::shared_ptr<Theme> themePtr)
    : AppWindow(Size, Title, sf::Style::None)
    , m_ThemePtr(themePtr), m_MessageText(Message), m_AcceptedCallback(accepted), m_CancelledCallback(cancelled)
  {
    m_Message = MultilineTextView::Create(m_ThemePtr.get());
    m_CancelBtn = m_ThemePtr->MakeButton(CnclTxt);
    m_AcceptBtn = m_ThemePtr->MakeButton(OKTxt);
    m_IsModal = true;
  }

  ConfirmDialog::~ConfirmDialog()
  {

  }

  void ConfirmDialog::PostLaunch()
  {
    auto handle = m_Window->getSystemHandle();

    //D:
    SetWindowPos(handle, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
  }

  void ConfirmDialog::Cancelled()
  {
    m_CancelledCallback();
    Close();
  }

  void ConfirmDialog::Accepted()
  {
    m_AcceptedCallback();
    Close();
  }

  void ConfirmDialog::PositionWidgets()
  {
    //btns are 35% of the window width, and a fixed height of 20px

    m_CancelBtn->SetSize({ cast_int(floor(0.35 * m_WindowSize.x)), 20 });
    m_AcceptBtn->SetSize({ cast_int(floor(0.35 * m_WindowSize.x)), 20 });
    m_CancelBtn->OnClicked([this]() {Cancelled(); });
    m_AcceptBtn->OnClicked([this]() {Accepted(); });

    m_CancelBtn->SetPosition({ cast_int(floor(m_WindowSize.x * 0.10)), cast_int(floor(0.9 * m_WindowSize.y)) - 20 });
    m_AcceptBtn->SetPosition({ cast_int(floor(0.55 * m_WindowSize.x)), cast_int(floor(0.9 * m_WindowSize.y)) - 20 });

    AddWidget(m_AcceptBtn);
    AddWidget(m_CancelBtn);


    //Text fills up 75% of width, and as much height as needed
    m_Message->LoadFontTexture("Fonts/BitmapFonts/Images/osans14_ns.png");
    m_Message->LoadFontData("Fonts/BitmapFonts/Data/osans14_ns.fnt");
    m_Message->SetPosition({ cast_int(floor(m_WindowSize.x * 0.2)), cast_int(floor(m_WindowSize.x * 0.2)) });
    m_Message->SetColor(sf::Color::White);
    m_Message->SetText(m_MessageText);
    AddWidget(m_Message);
  }

}  
