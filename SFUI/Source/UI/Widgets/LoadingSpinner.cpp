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
#include <SFUI/Include/UI/Widgets/LoadingSpinner.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  LoadingSpinner::LoadingSpinner(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = nullptr */)
    : Widget(theme, parent)
  {
    m_Speed.setFromMilliseconds(10);
    m_AnimationTimer.setTime(m_Speed);
    m_AnimationTimer.start();
  }

  LoadingSpinner::~LoadingSpinner()
  {

  }

  LoadingSpinner::shared_ptr LoadingSpinner::Create(optional<Theme*> theme /* = { } */, optional<Widget*> parent /* = nullptr */)
  {
    return std::make_shared<LoadingSpinner>(theme, parent);
  }

  bool LoadingSpinner::HandleEvent(const sf::Event &event)
  {
    return false;
  }

  void LoadingSpinner::Render(sf::RenderTarget &Target)
  {
    Target.draw(m_BackgroundRect);
  }

  void LoadingSpinner::Update()
  {
    auto rem = m_AnimationTimer.getTime();

    if (m_AnimationTimer.isDone()) {
      AdvanceFrame();
      m_AnimationTimer.setTime(m_Speed);
      m_AnimationTimer.start();
    }
  }

  void LoadingSpinner::SetPosition(const Vec2i &Position)
  {
    super::SetPosition(Position);
  }

  void LoadingSpinner::SetSize(const Vec2i &Size)
  {
    super::SetSize(Size);
  }

  void LoadingSpinner::SetSpeed(int rate)
  {
    m_Speed.setFromMilliseconds(rate);
  }

  void LoadingSpinner::SetFrameCount(uint32 count)
  {
    m_FrameCount = count;
    GenerateFrames();
  }

  void LoadingSpinner::LoadTexture(const std::string &File)
  {
    m_SpinnerTexture = std::make_shared<sf::Texture>();
    m_SpinnerTexture->loadFromFile(File);
    GenerateFrames();
  }

  void LoadingSpinner::AdvanceFrame()
  {
    m_CurrentFrame++;
    if (m_CurrentFrame > m_FrameCount - 1)
      m_CurrentFrame = 0;
    m_BackgroundRect.setTextureRect(m_Frames[m_CurrentFrame]);
  }

  void LoadingSpinner::GenerateFrames()
  {
    auto tSize = m_SpinnerTexture->getSize();
    int frameWidth = cast_int(floor(tSize.x / (float)m_FrameCount));
    int frameHeight = cast_int(tSize.y);
    int left = 0; int top = 0;
    m_BackgroundRect.setSize({ cast_float(frameWidth), cast_float(frameHeight) });
    m_Frames.clear();
    m_Frames.resize(m_FrameCount, {});

    IntRect rect = { 0, 0, frameWidth, frameHeight };

    for (size_t i = 0; i < m_FrameCount; ++i) {
      m_Frames[i] = rect;
      rect.left += frameWidth;
    }

    m_AnimationTimer.setTime(m_Speed);
    m_AnimationTimer.start();
    m_CurrentFrame = 0;
    m_BackgroundRect.setTexture(m_SpinnerTexture.get());
    m_BackgroundRect.setTextureRect(m_Frames[m_CurrentFrame]);
  }

}  
