#ifndef SFUI_COMBOBOX_H
#define SFUI_COMBOBOX_H

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
#include <SFUI/Include/UI/Widgets/Selectable.h>
#include <SFUI/Include/UI/Widgets/ListView.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ComboBox : public Selectable
  {
  public:
    WIDGET_DERIVED(ComboBox, Selectable);
    ComboBox(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ComboBox() override;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    bool HandleEvent(const sf::Event &event) override;

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void AddItem(const std::string &Text);
    virtual void Open();
    virtual void Close();
    virtual bool Select(const std::string &text);
    virtual void SetBoxSize(const Vec2i &Size);
    virtual void OnItemSelected(boost::function<void(const std::string &)> func);

    std::string GetSelectedItem() const;
    bool IsItemSelected() const;

  protected:
    Signal<void(const std::string &)> m_ItemSelectedSignal;

    void ItemSelected(const std::string &str);
    virtual void Selected() override;
    virtual void Deselected() override;
    virtual void MouseLeft() override;
    virtual void MouseEntered() override;

    virtual void Moved() override;
    virtual void Resized() override;

    bool m_IsOpen = false;
    Vec2i m_BoxSize = { 100, 100 };
    sf::RectangleShape m_IconRect;
    sf::IntRect m_ExpandedIconRect = IntRect(306, 153, 50, 50);
    sf::IntRect m_CollapseRect = IntRect(408, 102, 50, 50);
    ListView::shared_ptr m_Options;
  };
  
}  

#endif // SFUI_COMBOBOX_H
