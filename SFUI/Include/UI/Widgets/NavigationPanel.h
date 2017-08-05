#ifndef SFUI_NAVIGATIONPANEL_H
#define SFUI_NAVIGATIONPANEL_H

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
#include <SFUI/Include/UI/Widgets/Panel.h>
#include <SFUI/Include/UI/Widgets/Button.h>
#include <SFUI/Include/UI/Widgets/Selectable.h>
#include <SFUI/Include/UI/Widgets/ScrollBar.h>
#include <SFUI/Include/UI/Animation/WidgetAnimator.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class NavigationPanel : public Panel
  {
  public:
    WIDGET_DERIVED(NavigationPanel, Panel);
    NavigationPanel(optional<Theme*> theme = {}, optional<Widget*> parent = {});
    virtual ~NavigationPanel() override;
    
    static shared_ptr Create(optional<Theme*> theme = {}, optional<Widget*> parent = {});

    virtual bool HandleEvent(const sf::Event &event) override;
    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetDefaultSize(const Vec2i &Size) override;
    virtual void Move(const Vec2i &Delta) override;

    bool IsOpen() const;
    std::string GetSelectedPanel() const;
    void AddItem(const std::string &Text);

    void Open();
    void Close();

    void SetWidth(int Width);
    void SetHeight(int Height);
    void SetClosedWidth(int Width);
    void OnPanelSelected(boost::function<void(const std::string &)> func);
    bool SelectItem(const std::string &Text);

  protected:
    Signal<void(const std::string &)> m_PanelSelectedSignal;

    virtual void Opened();
    virtual void Closed();
    virtual void ItemSelected(const std::string &Text);

    virtual void Shown() override;
    virtual void Hidden() override;
    virtual void Moved() override;
    virtual void Resized() override;

    std::unordered_map<std::string, Selectable *> m_StringItemMap;
    Selectable *m_CurrentPage = nullptr;

    std::string m_SelectedPanelText = "";
    int m_TotalItemsHeight = 0;
    Button* m_OpenCloseButton;
    bool m_IsOpen = false;
    int m_ExpandedWidth = 100;
    int m_CollapsedWidth = 50;

    Vec2i m_CollapsedSize = { 0, 0 };
    Vec2i m_ExpandedSize = { 0, 0 };
  };
  
}  

#endif // SFUI_NAVIGATIONPANEL_H
