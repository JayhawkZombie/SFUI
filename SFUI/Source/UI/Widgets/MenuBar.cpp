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
#include <SFUI/Include/UI/Widgets/MenuBar.h>
#include <SFUI/Include/UI/Widgets/WidgetWindow.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  MenuBar::MenuBar(WidgetWindow *WWindow, optional<Theme*> theme /*= optional<Theme*>()*/)
    : Widget(theme, WWindow)
  {
    
  }

  MenuBar::~MenuBar()
  {

  }

  sfui::MenuBar::shared_ptr MenuBar::Create(WidgetWindow *WWindow, optional<Theme*> theme /*= optional<Theme*>()*/)
  {
    MenuBar::shared_ptr mPtr = std::make_shared<MenuBar>(WWindow, theme);
    return mPtr;
  }

  Menu::shared_ptr MenuBar::AddMenu(const std::string &Text)
  {
    Selectable::shared_ptr sBle = m_Theme->MakeSelectableButton(Text, this);
    Menu::shared_ptr mPtr = m_Theme->MakeMenu(this);

    sBle->OnSelected([this](auto str) { MenuSelected(str); });
    sBle->OnDeselected([this](auto str) { MenuDeselected(str); });
    mPtr->OnOpened([this](auto mptr) { MenuOpened(mptr); });
    mPtr->OnClosed([this](auto mptr) { MenuClosed(mptr); });
    mPtr->OnItemSelected([this](auto it) { MenuItemSelected(it); });

    mPtr->SetBoxPadding({ 0, 0 });

    m_Menus.emplace_back( sBle, mPtr );

    RepositionMenus();
    return mPtr;
  }

  void MenuBar::AddMenuItem(const std::string &MenuText, const std::string &ItemText)
  {
    auto it = find_if(begin(m_Menus), end(m_Menus),
                      [&MenuText](auto menuptr) { return ( menuptr.first->GetText() == MenuText ); });

    if (it != m_Menus.end()) {
      ( *it ).second->Add(ItemText);
    }
  }

  void MenuBar::Update()
  {
    for (auto & mptr : m_Menus) {
      mptr.first->BaseUpdate();
    }

    if (m_CurrentMenu)
      m_CurrentMenu->BaseUpdate();
  }

  void MenuBar::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);

    for (auto & mptr : m_Menus) {
      mptr.first->Render(Target);
    }

    if (m_CurrentMenu)
      m_CurrentMenu->Render(Target);
  }

  bool MenuBar::OpenMenu(const std::string &Txt)
  {
    auto it = find_if(begin(m_Menus), end(m_Menus),
                      [&Txt](auto mptr) { return ( mptr.first->GetText() == Txt ); });
    if (it != m_Menus.end()) {
      ( *it ).first->Select();
      return true;
    }

    return false;
  }

  bool MenuBar::CloseMenu(const std::string &Txt)
  {
    if (m_CurrentMenu && m_CurrentMenu->GetText() == Txt) {
      m_CurrentMenu->Close();
      return true;
    }

    return false;
  }

  bool MenuBar::SelectMenuItem(const std::string &MenuTxt, const std::string &ItemTxt)
  {
    auto it = find_if(begin(m_Menus), end(m_Menus),
                      [&MenuTxt](auto mptr) { return ( mptr.first->GetText() == MenuTxt ); });
    if (it != m_Menus.end()) {
      return ( ( *it ).second->SelectItem(ItemTxt) );
    }

    return false;
  }

  bool MenuBar::HandleEvent(const sf::Event &event)
  {
    //No open menu & no mouse over us
    if (!m_CurrentMenu && !Widget::HandleEvent(event)) return false;

    //See if any menu tabs were interacted with
    for (auto & mptr : m_Menus) {
      if (mptr.first->HandleEvent(event)) return true;
    }

    //See if current menu (if one is open) consumed it
    if (m_CurrentMenu && m_CurrentMenu->HandleEvent(event)) return true;

    //If we have an open menu AND the user clicked outside of it, close it
    if (m_CurrentMenu && event.type == sf::Event::MouseButtonPressed) {
      //m_CurrentMenu->Close();
      m_CurrentSelection->Deselect();
      return true;
    }

    return false;
  }

  void MenuBar::SetHeight(uint32 Height)
  {
    SetSize({ m_Size.x, ( int )Height });
  }

  void MenuBar::OnMenuItemSelected(boost::function<void(const std::string &, const std::string &)> func)
  {
    m_MenuItemSelectedSignal.connect(func);
  }

  void MenuBar::OnMenuOpened(boost::function<void(const std::string &)> func)
  {
    m_MenuOpenedSignal.connect(func);
  }

  void MenuBar::OnMenuClosed(boost::function<void(const std::string &)> func)
  {
    m_MenuClosedSignal.connect(func);
  }

  void MenuBar::RepositionMenus()
  {
    if (m_Menus.empty()) return;

    Vec2i mpos = { 0, 0 };
    int iHeight = ( int )m_Height;

    Vec2i itemPos = { 0, 0 };
    Vec2i menuPos = itemPos;

    Vec2i itemSize = { 0, 0 };
    Vec2i menuSize = { 0, 0 };
    
    for (auto & mptr : m_Menus) {
      //Selectable btn
      mptr.first->SetPosition(itemPos);
      itemSize = mptr.first->GetTextSize();
      itemSize.y = iHeight;
      itemSize.x = cast_int(itemSize.x * 1.2f);
      mptr.first->SetSize(itemSize);

      //Dropdown menu
      menuPos = itemPos;
      menuPos.y += iHeight;
      mptr.second->SetPosition(menuPos);

      menuSize = mptr.second->GetSize();
      if (menuSize.x < 50)
        menuSize.x = 50;
      mptr.second->SetSize(menuSize);

      mptr.first->SetTextAlignment(TextAlignment::Centered);

      itemPos.x += itemSize.x + 2;
    }
  }

  void MenuBar::Moved()
  {
    RepositionMenus();
  }

  void MenuBar::Resized()
  {
    RepositionMenus();
  }

  void MenuBar::MenuOpened(Menu *mPtr)
  {
    m_CurrentMenu = mPtr;
    //TakeGlobalMouseFocus(this);
    StealMouseFocus(this);
    m_MenuOpenedSignal(mPtr->GetText());
  }

  void MenuBar::MenuClosed(Menu *mPtr)
  {
    if (m_CurrentMenu == mPtr)
      m_CurrentMenu = nullptr;
    //ReleaseGlobalMouseFocus();
    ReturnMouseFocus(this);
    m_MenuClosedSignal(mPtr->GetText());
  }

  void MenuBar::MenuSelected(const std::string &MenuText)
  {
    auto it = find_if(begin(m_Menus), end(m_Menus),
                      [&MenuText](auto mpair) { return ( mpair.first->GetText() == MenuText ); });

    if (it != m_Menus.end()) {
      if (m_CurrentSelection)
        m_CurrentSelection->Deselect();

      m_CurrentSelection = ( *it ).first.get();
      ( *it ).second->Open();
    }
  }

  void MenuBar::MenuDeselected(const std::string &MenuText)
  {
    // The selected one SHOULD be the current menu, unless something has gone wrong
    if (m_CurrentMenu) {
      m_CurrentMenu->Close();
      m_CurrentMenu = nullptr;
    }

    m_CurrentMenu = nullptr;
    m_CurrentSelection = nullptr;
  }

  void MenuBar::MenuItemSelected(const std::string &ItemTxt)
  {
    //Workaround to prevent infinitely-calling back on ourselves
    auto csTmp = m_CurrentSelection;
    auto cmTmp = m_CurrentMenu;

    m_CurrentSelection = nullptr;
    m_CurrentMenu = nullptr;

    std::cout << "Menu: " << csTmp->GetText() << "\n";
    std::cout << "Item selected: " << ItemTxt << "\n";

    m_MenuItemSelectedSignal(csTmp->GetText(), ItemTxt);

    cmTmp->Close();
    csTmp->Deselect();
  }

}  
