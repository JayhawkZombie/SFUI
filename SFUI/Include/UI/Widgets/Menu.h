#ifndef SFUI_MENU_H
#define SFUI_MENU_H

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
#include <SFUI/Include/UI/Widgets/Button.h>
#include <SFUI/Include/UI/Widgets/LineItem.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Menu : public Widget
  {
  public:
    WIDGET_DERIVED(Menu, Widget);
    Menu(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());
    virtual ~Menu() override;
    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget::pointer> parent = optional<Widget*>());

    void Add(const std::string &Text);
    void Remove(const std::string &Text);

    bool SelectItem(const std::string &Text);
    void SetItemHeight(uint32 Height);

    void Open();
    void Close();

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    void SetItemSpacing(uint32 Spacing);
    void SetBoxPadding(Vec2i Padding);
    void OnItemSelected(boost::function<void(const std::string &)> func);
    void OnOpened(boost::function<void(Menu*)> func);
    void OnClosed(boost::function<void(Menu*)> func);

    bool HandleEvent(const sf::Event &event) override;

  protected:

    Signal<void(Menu*)> m_OpenedSignal;
    Signal<void(Menu*)> m_ClosedSignal;
    Signal<void(const std::string &)> m_MenuItemSelectedSignal;
    virtual void ItemSelected(const std::string &str);

    virtual void Moved() override;
    virtual void Resized() override;

    void RepositionItems();

    int m_MaxItemWidth = 0;
    int m_MinWidth = 50;
    bool m_IsOpen = false;
    uint32 m_ItemHeight = 15;
    uint32 m_ItemSpacing = 3;
    Vec2i m_Padding = { 3, 3 };
    std::vector<LineItem::shared_ptr> m_Items;
  };
  
}  

#endif // SFUI_MENU_H
