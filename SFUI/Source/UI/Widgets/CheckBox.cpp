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
#include <SFUI/Include/UI/Widgets/CheckBox.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  

  
  CheckBox::CheckBox(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Button(theme, parent)
  {
    m_Texture = std::make_shared<sf::Texture>();
    m_Texture->loadFromFile("iconmap_grey.png");
    m_CheckedRect = IntRect(153, 102, 50, 50);
    m_UncheckedRect = IntRect(612, 0, 50, 50);
    
    m_CheckRect.setTexture(m_Texture.get());
    m_CheckRect.setTextureRect(m_UncheckedRect);

    m_CanAnimateExpand = false;
    m_CanAnimateContract = false;
  }

  sfui::CheckBox::shared_ptr CheckBox::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<CheckBox>(theme, parent);
  }

  bool CheckBox::IsChecked() const
  {
    return m_IsChecked;
  }

  void CheckBox::Check()
  {
    m_CheckRect.setTextureRect(m_CheckedRect);
    SetTextAlignment(TextAlignment::Left);
    m_IsChecked = true;
    m_CheckedSignal();
  }

  void CheckBox::Uncheck()
  {
    m_CheckRect.setTextureRect(m_UncheckedRect);
    SetTextAlignment(TextAlignment::Left);
    m_IsChecked = false;
  }

  void CheckBox::SetDefaultSize(const Vec2i &Size)
  {
    m_ContractSize = Size;
    m_ExpandSizeOffset = { 0, 0 };
    SetSize(Size);
  }

  void CheckBox::OnChecked(boost::function<void()> func)
  {
    m_CheckedSignal.connect(func);
  }

  void CheckBox::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_CheckRect);
    if (m_TextView)
      ( *m_TextView )->Render(Target, {});
  }

  void CheckBox::Clicked()
  {
    if (m_IsChecked) {
      Uncheck();
    }
    else {
      Check();
    }
  }

  void CheckBox::Moved()
  {
    m_CheckRect.setPosition(m_Position);
    if (m_TextView)
      ( *m_TextView )->SetPosition({ (m_Position.x + m_Size.y), (m_Position.y) });
  }

  void CheckBox::Resized()
  {
    m_CheckRect.setSize({ cast_float(m_Size.y), cast_float(m_Size.y) });
    if (m_TextView)
      ( *m_TextView )->SetPosition({ (m_Position.x + m_Size.y), (m_Position.y) });
  }

  void CheckBox::Checked()
  {

  }

}  
