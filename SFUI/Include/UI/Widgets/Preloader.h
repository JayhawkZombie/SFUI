#ifndef SFUI_PRELOADER_H
#define SFUI_PRELOADER_H

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
#include <SFUI/Include/UI/Widgets/PreloaderBox.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  class Preloader : public Widget
  {
  public:
    WIDGET_DERIVED(Preloader, Widget);
    Preloader(optional<Theme*> theme = { }, optional<Widget*> parent = { });
    virtual ~Preloader() override;

    static shared_ptr Create(optional<Theme*> theme = { }, optional<Widget*> parent = { });

    virtual void Update() override;
    virtual void Render(sf::RenderTarget &Target) override;
    virtual void SetPosition(const Vec2i &Position) override;
    virtual void SetSize(const Vec2i &Size) override;
    virtual void Move(const Vec2i &Delta) override;

    virtual void Start(uint32 SpawnFallDuration, uint32 SitDuration, uint32 DespawnFallDuration);
    virtual void Pause();
    virtual void Stop();
    virtual void Restart();

    virtual void SetColor(const Color &c);
  protected:

    void PositionBoxes();
    void BlockFinished();
    SequenceTimer m_RestartTimer;
    std::vector<PreloaderBox::shared_ptr> m_Boxes;
    Vec2f m_BoxVelocities;
    int m_BoxHeight = 0;
    int m_BoxSpacing = 0;
    int m_FinishedCount = 0;
    uint32 m_SpawnDur = 0;
    uint32 m_SitDur = 0;
    uint32 m_DespDur = 0;

    kairos::Timer m_AnimTimer;
    kairos::Duration m_AnimDuration;
    bool m_ShouldRestart = true;
    bool m_IsPlaying = false;
  };
  
}  

#endif // SFUI_PRELOADER_H
