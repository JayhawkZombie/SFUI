#ifndef SFUI_TABLIST_H
#define SFUI_TABLIST_H

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
#include <SFUI/Include/UI/Widgets/Tab.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class TabList : public Widget
  {
  public:
    WIDGET_DERIVED(TabList, Widget);
    TabList(optional<Widget*> parent = optional<Widget*>(), optional<Theme*> theme = optional<Theme*>());
    virtual ~TabList() override;

    static shared_ptr Create(optional<Widget*> parent = optional<Widget*>(), optional<Theme*> theme = optional<Theme*>());

    virtual Panel::shared_ptr AddTab(const std::string &Text);
    Tab::shared_ptr GetSelectedTab() const;
    Panel *GetCurrentPanel() const;
    virtual void SelectTab(const std::string &Text);
    virtual void SelectTab(Tab *tPtr);

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    void SetTabHeight(uint32 Height);
    void SetPanelHeight(uint32 Height);
    void SetPanelWidth(uint32 Width);
    void SetMinimumTabWidth(uint32 Width);
    virtual void OnTabSelected(boost::function<void(Tab*)> func);
    virtual void SetSize(const Vec2i &Size) override;
    virtual bool HandleEvent(const sf::Event &event) override;

  protected:
    Signal<void(Tab*)> m_TabSelectedSignal;

    optional<Tab::shared_ptr> GetTabUnderMouse();
    void RepositionTabs();
    void TabSelected(Tab* tPtr);

    virtual void Moved() override;
    virtual void Resized() override;

    Vec2i m_PanelPosition;
    Vec2i m_PanelSize;

    uint32 m_MinTabWidth = 50;
    uint32 m_TabHeight = 15;
    uint32 m_PanelHeight = 150;
    uint32 m_PanelWidth = 400;
    Tab *m_CurrentTab = nullptr;
    Panel *m_CurrentPanel;
    std::vector<std::pair<Tab::shared_ptr, Panel::shared_ptr>> m_Tabs;
  };
  
}  

#endif // SFUI_TABLIST_H
