#ifndef SFUI_CONTEXTMENU_H
#define SFUI_CONTEXTMENU_H

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
#include <SFUI/Include/UI/Widgets/GenericContainer.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{
  
  class ContextMenu : public GenericContainer
  {
  public:
    WIDGET_DERIVED(ContextMenu, GenericContainer);
    ContextMenu(optional<Theme*> theme = {}, optional<Widget*> parent = {});
    virtual ~ContextMenu() override;
    static shared_ptr Create(optional<Theme*> theme = { }, optional<Widget*> parent = { });

    virtual bool HandleEvent(const sf::Event &event) override;
    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void Move(const Vec2i &Delta) override;

    virtual void Add(Widget::shared_ptr widget) override;
    virtual void AddMenuItem(const std::string &Text);

    virtual void Open();
    virtual void Close();

    void OnOpened(boost::function<void()> func);
    void OnClosed(boost::function<void()> func);

  protected:

    Signal<void()> m_OpenedSignal;
    Signal<void()> m_ClosedSignal;

    virtual void Opened();
    virtual void Closed();

    bool m_IsOpen = false;
    int m_ItemSpacing = 1;
    Vec2i m_ItemPadding = { 2, 2 };

  };

}

#endif // SFUI_CONTEXTMENU_H
