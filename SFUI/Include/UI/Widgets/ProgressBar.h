#ifndef SFUI_PROGRESSBAR_H
#define SFUI_PROGRESSBAR_H

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

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class ProgressBar : public Widget
  {
  public:
    WIDGET_DERIVED(ProgressBar, Widget);
  
    ProgressBar(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());
    virtual ~ProgressBar() override = default;

    static shared_ptr Create(optional<Theme*> theme = optional<Theme*>(), optional<Widget*> parent = optional<Widget*>());

    void SetProgress(float Prog);
    void SetBufferProgress(float Prog);
    float GetProgress() const;
    float GetBufferProgress() const;
    virtual void SetProgressColor(const Color &c);

    bool IsCompleted() const;
    void OnCompleted(boost::function<void()> func);
    void Reset();

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;

    virtual void ShowPercentIndicator(bool show);
    virtual void SetBuffered(bool buffered);
  
  protected:
    Signal<void()> m_CompletedSignal;
    bool m_ShowPercText = false;
    bool m_IsBuffered = false;
    virtual void Moved() override;
    virtual void Resized() override;

    virtual void ProgressChanged();
    virtual void BufferChanged();

    static constexpr float m_BGOpacity = 0.3f;
    static constexpr float m_BufferOpacity = 0.6f;

  private:

    bool m_IsCompleted = false;
    sf::RectangleShape m_BufferRect;
    sf::RectangleShape m_FillRect;
    Color m_ProgressColor = Color(0, 92, 242);
    float m_Progress = 0.f;
    float m_BufferProgress = 0.f;
  };
  
}  

#endif // SFUI_PROGRESSBAR_H
