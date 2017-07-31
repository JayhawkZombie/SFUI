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
#include <SFUI/Include/UI/Widgets/NavigationPanel.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
   NavigationPanel::NavigationPanel(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
     : Panel(theme, parent)
  {
     auto btn = m_Theme->MakeIconButton(m_Theme->IconTexture, IntRect(51, 408, 50, 50), this);
     m_OpenCloseButton = btn.get();
     m_OpenCloseButton->SetSize({ 25, 25 });
     m_OpenCloseButton->SetPosition(m_Position);
     m_OpenCloseButton->SetBackgroundColor(sf::Color::Transparent);
     m_OpenCloseButton->SetBorderWidth(0);
     m_OpenCloseButton->DisableAnimations();
     m_IsTopLevel = true;
     m_OpenCloseButton->OnClicked(
       [this]()
     {
       if (m_IsOpen)
         Close();
       else
         Open();
     }
     );

     m_TotalItemsHeight = m_OpenCloseButton->GetSize().y + 10;
     Add(btn);
  }

  NavigationPanel::~NavigationPanel()
  {
    
  }

  NavigationPanel::shared_ptr NavigationPanel::Create(optional<Theme*> theme /* = {} */, optional<Widget*> parent /* = {} */)
  {
    return std::make_shared<NavigationPanel>(theme, parent);
  }

  bool NavigationPanel::HandleEvent(const sf::Event &event)
  {
    if (m_IsOpen) {

      //If we are open and we DIDN'T handle the event, we should close if the event was a mouse press
      if (!super::HandleEvent(event)) {
        if (event.type == sf::Event::MouseButtonPressed) {
          Close();
          return true;
        }
        return true;
      }

      return false;
    }
    
    if (m_Bounds.contains(currentMousePosition)) {

      m_OpenCloseButton->HandleEvent(event);

      return true;
    }

    return false;
  }

  void NavigationPanel::Update()
  {
    if (m_IsOpen) {
      super::Update();
    }
    else {
      m_OpenCloseButton->Update();
    }
  }

  void NavigationPanel::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
    if (m_IsOpen) {
      std::for_each(begin(m_StringItemMap), end(m_StringItemMap),
                    [this, &Target](auto &selPair) 
                    { selPair.second->Render(Target); }
      );
    }
    
    m_OpenCloseButton->Render(Target);
  }

  void NavigationPanel::SetPosition(const Vec2i &Position)
  {
    //Our position is ALWAYS (0, 0)
    Widget::SetPosition({ 0, Position.y });
    m_OpenCloseButton->SetPosition({ 3, m_Position.y + 3 });
  }

  void NavigationPanel::SetDefaultSize(const Vec2i &Size)
  {
    super::SetDefaultSize(Size);
  }

  void NavigationPanel::Move(const Vec2i &Delta)
  {
    super::Move(Delta);
  }

  bool NavigationPanel::IsOpen() const
  {
    return m_IsOpen;
  }

  std::string NavigationPanel::GetSelectedPanel() const
  {
    return m_SelectedPanelText;
  }

  void NavigationPanel::AddItem(const std::string &Text)
  {
    auto newItem = m_Theme->MakeSelectableButton(Text, this);
    newItem->SetBackgroundColor(sf::Color::Transparent);
    newItem->SetBorderWidth(0);
    newItem->SetSize({ m_Size.x - 6, 25 });
    newItem->SetPosition({ m_Position.x + 3, m_TotalItemsHeight });

    Add(newItem);
    m_TotalItemsHeight += newItem->GetSize().y + 5;
    m_StringItemMap[Text] = newItem.get();
  }

  void NavigationPanel::Open()
  {
    m_IsOpen = true;

    //Play the opening animation
    m_Animator.Animate(WidgetAnimation::Expand, m_CollapsedSize, m_ExpandedSize, Easing::Decelerate, 150);

    std::for_each(begin(m_StringItemMap), end(m_StringItemMap),
                  [this](auto &selPair)
                  {
                    auto itemPos = selPair.second->GetPosition();
                    auto itemSize = selPair.second->GetSize();
                    selPair.second->Animate(WidgetAnimation::SlideIn, Vec2i(-30 - itemSize.x, itemPos.y), Vec2i(m_Position.x + 3, itemPos.y), Easing::Accelerate, 225);
                  } );
    StealMouseFocus(this);
  }

  void NavigationPanel::Close()
  {
    m_IsOpen = false;

    //Play the closing animation
    m_Animator.Animate(WidgetAnimation::Contract, m_ExpandedSize, m_CollapsedSize, Easing::Decelerate, 150);

    std::for_each(begin(m_StringItemMap), end(m_StringItemMap),
              [this](auto &selPair)
              {
                auto itemPos = selPair.second->GetPosition();
                auto itemSize = selPair.second->GetSize();
                selPair.second->Animate(WidgetAnimation::SlideOut, itemPos, Vec2i(-itemSize.x, itemPos.y), Easing::Accelerate, 225);
              } );
    ReturnMouseFocus(this);
  }

  void NavigationPanel::SetWidth(int Width)
  {
    m_ExpandedSize.x = Width;
    
    if (m_IsOpen) {
      SetSize(m_ExpandedSize);
    }

    std::for_each(
      begin(m_StringItemMap), end(m_StringItemMap),
      [this, &Width](auto &selPair)
    {
      selPair.second->SetSize({ Width - 6, 25 });
    }
    );
  }

  void NavigationPanel::SetHeight(int Height)
  {
    //m_Size.y = Height;
    //m_BackgroundRect.setSize(sf::Vector2f(cast_float(m_Size.x), cast_float(Height)));
    //m_Bounds.height = Height;
    m_ExpandedSize.y = Height;
    m_CollapsedSize.y = Height;

    if (m_IsOpen) {
      SetSize(m_ExpandedSize);
    }
    else {
      SetSize(m_CollapsedSize);
    }
  }

  void NavigationPanel::SetClosedWidth(int Width)
  {
    m_CollapsedSize.x = Width;
    if (!m_IsOpen) {
      SetSize(m_CollapsedSize);
    }
  }

  void NavigationPanel::OnPanelSelected(boost::function<void(const std::string &)> func)
  {
    m_PanelSelectedSignal.connect(func);
  }

  bool NavigationPanel::SelectItem(const std::string &Text)
  {
    auto it = m_StringItemMap.find(Text);

    if (it != m_StringItemMap.end()) {
      it->second->Select();
      return true;
    }

    return false;
  }

  void NavigationPanel::Shown()
  {

  }

  void NavigationPanel::Hidden()
  {

  }

  void NavigationPanel::Opened()
  {

  }

  void NavigationPanel::Closed()
  {

  }

  void NavigationPanel::ItemSelected(const std::string &Text)
  {
    m_SelectedPanelText = Text;
    m_PanelSelectedSignal(Text);
  }

  void NavigationPanel::Moved()
  {
    m_OpenCloseButton->SetPosition(m_Position);
  }

  void NavigationPanel::Resized()
  {

  }

}  
