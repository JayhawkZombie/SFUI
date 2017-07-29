#ifndef SFUI_LOADINGSPINNER_H
#define SFUI_LOADINGSPINNER_H

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
#include <Kairos.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class LoadingSpinner : public Widget
  {
  public:
    WIDGET_DERIVED(LoadingSpinner, Widget);
    LoadingSpinner(optional<Theme*> theme = {}, optional<Widget*> parent = nullptr);
    virtual ~LoadingSpinner() override;

    static shared_ptr Create(optional<Theme*> theme = { }, optional<Widget*> parent = nullptr);

    virtual bool HandleEvent(const sf::Event &event) override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void Update() override;

    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void SetSpeed(int rate);
    void SetFrameCount(uint32 count);
    void LoadTexture(const std::string &File);

  protected:
    void AdvanceFrame();
    void GenerateFrames();

    uint32 m_FrameCount = 10;
    kairos::Duration m_Speed = 10;
    kairos::Timer m_AnimationTimer;
    std::vector<IntRect> m_Frames;
    size_t m_CurrentFrame = 0;
    _shared_ptr<sf::Texture> m_SpinnerTexture = nullptr;
  };
  
}  

#endif // SFUI_LOADINGSPINNER_H
