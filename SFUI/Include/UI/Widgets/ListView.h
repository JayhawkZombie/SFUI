#ifndef SFUI_LISTVIEW_H
#define SFUI_LISTVIEW_H

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
#include <SFUI/Include/UI/Widgets/ScrollBar.h>
#include <SFUI/Include/UI/Widgets/Selectable.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ListView : public Widget
  {
  public:
    WIDGET_DERIVED(ListView, Widget);
    ListView(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ListView() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual void AddItem(const std::string &Text);

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    bool HandleEvent(const sf::Event &event) override;

    std::string GetCurrentSelection() const;
    bool SelectItem(const std::string &Text);
    void OnItemSelected(boost::function<void(const std::string &)> func);

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void Move(const Vec2i &Delta) override;
    virtual void ScrollTo(int ListTop);

  protected:
    Signal<void(const std::string &)> m_ItemSelectedSignal;

    void AdjustScrollbar();
    void AdjustList();
    void ScrollItems(double delta);
    void ItemSelected(const std::string &str);

    int m_MaxItemHeight = 10;
    int m_ListTop = 0;

    bool m_ScrollbarEnabled = false;
    ScrollBar::shared_ptr m_ScrollBar = nullptr;
    Vec2i m_CumulativeSize = { 0, 0 };
    Selectable *m_CurrentSelection = nullptr;
    std::vector<Selectable::shared_ptr> m_Items;
  };
  
}  

#endif // SFUI_LISTVIEW_H
