#ifndef SFUI_WIDGETTREE_H
#define SFUI_WIDGETTREE_H

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
#include <SFUI/Include/UI/Widget.h>
#include <SFUI/Include/UI/Widgets/TreeRoot.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class WidgetTree : public Widget
  {
  public:
    WIDGET_DERIVED(WidgetTree, Widget);
    WidgetTree(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~WidgetTree() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual bool HandleEvent(const sf::Event &event) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;

    virtual void ExpandRoot(uint32 index);
    virtual void CollapseRoot(uint32 index);
    size_t Count() const;

    std::shared_ptr<TreeRoot> NewRoot();
    virtual void SetTopWindow(WidgetWindow *TopWindow) override;
    
  protected:
    Signal<void()> m_ExpandedSignal;

    void RootExpanded(TreeRoot* rootPtr);
    void RootCollapsed(TreeRoot* rootPtr);
    void RootResized(TreeRoot* rootPtr);
    
    std::vector<std::shared_ptr<TreeRoot>> m_Roots;

  };
  
}  

#endif // SFUI_WIDGETTREE_H
