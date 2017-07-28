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
#include <SFUI/Include/UI/Widgets/WidgetTree.h>
#include <SFUI/Include/UI/Theme.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  WidgetTree::WidgetTree(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
    : Widget(theme, parent)
  {

  }

  WidgetTree::~WidgetTree()
  {

  }

  sfui::WidgetTree::shared_ptr WidgetTree::Create(optional<Theme*> theme /*= optional<Theme*>()*/, optional<Widget*> parent /*= optional<Widget*>()*/)
  {
    return std::make_shared<WidgetTree>(theme, parent);
  }

  void WidgetTree::Update()
  {
    for (auto & root : m_Roots)
      root->BaseUpdate();
  }

  void WidgetTree::Render(sf::RenderTarget &Target)
  {
    super::Render(Target);
    for (auto & root : m_Roots)
      root->Render(Target);
  }

  bool WidgetTree::HandleEvent(const sf::Event &event)
  {
    if (!Widget::HandleEvent(event)) return false;

    for (auto & root : m_Roots)
      if (root->HandleEvent(event)) return true;
    
    return false;
  }

  void WidgetTree::SetPosition(const Vec2i &Position)
  {
    auto delta = Position - m_Position;
    super::SetPosition(Position);

    for (auto & root : m_Roots) root->Move(delta);
  }

  void WidgetTree::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
  }

  void WidgetTree::ExpandRoot(uint32 index)
  {
    if (index > m_Roots.size())
      throw std::out_of_range("root index out of range");

    m_Roots[index]->Expand();
  }

  void WidgetTree::CollapseRoot(uint32 index)
  {
    if (index > m_Roots.size())
      throw std::out_of_range("root index out of range");

    m_Roots[index]->Collapse();
  }

  size_t WidgetTree::Count() const
  {
    return m_Roots.size();
  }

  std::shared_ptr<sfui::TreeRoot> WidgetTree::NewRoot()
  {
    Vec2i pos = ( m_Roots.empty() ? m_Position : m_Roots.back()->GetPosition() + Vec2i(0, m_Roots.back()->GetSize().y) );
    auto rootPtr = TreeRoot::Create(this, m_Theme);
    m_Roots.push_back(rootPtr);
    rootPtr->SetSize({ m_Size.x, 25 });
    rootPtr->SetPosition(pos);
    rootPtr->SetBitmapFont(m_Theme->DefaultBitmapFont);
    rootPtr->OnExpanded([this, rp = rootPtr.get()]() { RootExpanded(rp); });
    rootPtr->OnCollapsed([this, rp = rootPtr.get()]() {RootCollapsed(rp); });
    return rootPtr;
  }

  void WidgetTree::SetTopWindow(WidgetWindow *TopWindow)
  {
    super::SetTopWindow(TopWindow);
    for (auto & root : m_Roots)
      root->SetTopWindow(TopWindow);
  }

  void WidgetTree::RootExpanded(TreeRoot* rootPtr)
  {
    auto it = std::find_if(begin(m_Roots), end(m_Roots), [&rootPtr](auto rPtr) { return ( rPtr.get() == rootPtr ); });

    std::cout << "Root expanded\n";

    if (it != m_Roots.end()) {

      //get the new size of the root and tell all the roots below it to move down
      auto newsize = ( *it )->GetExpansionSize();

      std::cout << "Moving others down by: (" << newsize.x << ", " << newsize.y << ")\n";
      it++;
      while (it != m_Roots.end()) {
        ( *it )->Move(Vec2i(0, newsize.y));
        it++;
      }
    }

  }

  void WidgetTree::RootCollapsed(TreeRoot* rootPtr)
  {
    auto it = std::find_if(begin(m_Roots), end(m_Roots), [&rootPtr](auto rPtr) { return ( rPtr.get() == rootPtr ); });

    std::cout << "Root collapsed\n";

    //Find all the widgets after it and tell them to move UP
    if (it != m_Roots.end()) {
      auto newsize = ( *it )->GetExpansionSize();
      it++;
      while (it != m_Roots.end()) {
        ( *it )->Move(Vec2i(0, -newsize.y));
        it++;
      }
    }
  }

  void WidgetTree::RootResized(TreeRoot* rootPtr)
  {

  }

}  
