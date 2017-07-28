#ifndef SFUI_MENUBAR_H
#define SFUI_MENUBAR_H

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
#include <SFUI/Include/UI/Widgets/Popup.h>
#include <SFUI/Include/UI/Widgets/Selectable.h>
#include <SFUI/Include/UI/Widgets/Menu.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  

  class MenuBar : public Widget
  {
  public:
    WIDGET_DERIVED(MenuBar, Widget);
    MenuBar(WidgetWindow *WWindow, optional<Theme*> theme = optional<Theme*>());
    virtual ~MenuBar() override;

    static shared_ptr Create(WidgetWindow *WWindow, optional<Theme*> theme = optional<Theme*>());

    virtual Menu::shared_ptr AddMenu(const std::string &Text);
    virtual void AddMenuItem(const std::string &MenuText, const std::string &ItemText);
    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    bool OpenMenu(const std::string &Txt);
    bool CloseMenu(const std::string &Txt);
    bool SelectMenuItem(const std::string &MenuTxt, const std::string &ItemTxt);

    bool HandleEvent(const sf::Event &event) override;

    void SetHeight(uint32 Height);
    
    void OnMenuItemSelected(boost::function<void(const std::string &, const std::string &)> func);
    void OnMenuOpened(boost::function<void(const std::string &)> func);
    void OnMenuClosed(boost::function<void(const std::string &)> func);
  protected:
    
    void RepositionMenus();
    Signal<void(const std::string &, const std::string &)> m_MenuItemSelectedSignal;
    Signal<void(const std::string &)> m_MenuOpenedSignal;
    Signal<void(const std::string &)> m_MenuClosedSignal;

    virtual void Moved() override;
    virtual void Resized() override;

    void MenuOpened(Menu *mPtr);
    void MenuClosed(Menu *mPtr);
    void MenuSelected(const std::string &MenuText);
    void MenuDeselected(const std::string &MenuText);
    void MenuItemSelected(const std::string &ItemTxt);

    Menu* m_CurrentMenu = nullptr;
    Selectable *m_CurrentSelection = nullptr;
    WidgetWindow *m_WWindow = nullptr;
    std::vector<std::pair<Selectable::shared_ptr, Menu::shared_ptr>> m_Menus;
    int m_Height = 15;
  };
  
}  

#endif // SFUI_MENUBAR_H
