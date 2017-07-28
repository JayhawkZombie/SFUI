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
#include <SFUI/Include/UI/Widgets/ComboBox.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  ComboBox::ComboBox(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Selectable(theme, parent)
  {
    m_IconRect.setTexture(m_Theme->IconTexture.get());
    m_IconRect.setTextureRect(m_CollapseRect);
    m_SelectionColor = m_BGColor;

    m_Options = m_Theme->MakeListView();

    m_Options->OnItemSelected([this](auto str) { ItemSelected(str); });
  }

  ComboBox::~ComboBox()
  {

  }

  sfui::ComboBox::shared_ptr ComboBox::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<ComboBox>(theme, parent);
  }

  bool ComboBox::HandleEvent(const sf::Event &event)
  {
    if (!Selectable::HandleEvent(event) && !m_IsOpen) return false;

    if (!m_IsOpen) return false;

    if (m_Options->HandleEvent(event)) return true;
    return false;
  }

  void ComboBox::Update()
  {
    Selectable::Update();

    if (m_IsOpen) {
      m_Options->BaseUpdate();
    }
  }

  void ComboBox::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    m_TextView.value()->Render(Target, {});
    Target.draw(m_IconRect);

    if (m_IsOpen) {
      m_Options->Render(Target);
    }
  }

  void ComboBox::AddItem(const std::string &Text)
  {
    m_Options->AddItem(Text);
  }

  void ComboBox::Open()
  {
    m_IconRect.setTextureRect(m_ExpandedIconRect);
    m_BackgroundRect.setFillColor(m_BGColor);
    m_IsOpen = true;
  }

  void ComboBox::Close()
  {
    m_IconRect.setTextureRect(m_CollapseRect);
    m_IsOpen = false;
  }

  bool ComboBox::Select(const std::string &text)
  {
    return ( m_Options && m_Options->SelectItem(text) );
  }

  void ComboBox::SetBoxSize(const Vec2i &Size)
  {
    m_Options->SetSize(Size);
  }

  void ComboBox::OnItemSelected(boost::function<void(const std::string &)> func)
  {
    m_ItemSelectedSignal.connect(func);
  }

  std::string ComboBox::GetSelectedItem() const
  {
    return GetText();
  }

  bool ComboBox::IsItemSelected() const
  {
    return ( m_IsOpen && m_Options->GetCurrentSelection() != "" );
  }

  void ComboBox::ItemSelected(const std::string &str)
  {
    SetText(str);
    Deselect();
    m_ItemSelectedSignal(str);
  }

  void ComboBox::Selected()
  {
    Open();
    m_BackgroundRect.setFillColor(m_BGColor);
  }

  void ComboBox::Deselected()
  {
    Close();
    m_BackgroundRect.setFillColor(m_BGColor);
  }

  void ComboBox::MouseLeft()
  {
    super::MouseLeft();
    m_BackgroundRect.setFillColor(m_BGColor);
  }

  void ComboBox::MouseEntered()
  {
    super::MouseEntered();
    m_BackgroundRect.setFillColor(m_BGColor + sf::Color(m_BrightFactor, m_BrightFactor, m_BrightFactor));
  }

  void ComboBox::Moved()
  {
    super::Moved();
    m_IconRect.setPosition(m_Position + 0.25f * Vec2f(m_Size.y, m_Size.y));
    m_Options->SetPosition({ m_Position.x, m_Position.y + m_Size.y });

    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_TextView.value()->SetSize({ m_Size.x - m_Size.y, m_Size.y });
  }

  void ComboBox::Resized()
  {
    m_IconRect.setSize(Vec2f(0.5f * m_Size.y, 0.5f * m_Size.y));
    m_IconRect.setPosition(m_Position + 0.25f * Vec2f(m_Size.y, m_Size.y));

    auto size = m_Options->GetSize();
    size.x = m_Size.x;
    m_Options->SetSize(size);
    m_Options->SetPosition({ m_Position.x, m_Position.y + m_Size.y });
    m_TextView.value()->SetPosition({ m_Position.x + m_Size.y, m_Position.y });
    m_TextView.value()->SetSize({ m_Size.x - m_Size.y, m_Size.y });
  }

}  
