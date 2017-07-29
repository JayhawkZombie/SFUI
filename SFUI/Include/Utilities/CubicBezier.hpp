#ifndef SFUI_CUBICBEZIER_H
#define SFUI_CUBICBEZIER_H

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
#include <SFUI/Include/Utilities/Vec.hpp>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

class CubicBezier {
public:
  CubicBezier( ) = default;
  ~CubicBezier( ) = default;
  CubicBezier(float x1, float y1, float x2, float y2)
    : m_X1(x1), m_Y1(y1), m_X2(x2), m_Y2(y2) { }

  void Set(float x1, float y1, float x2, float y2)
  {
    m_X1 = x1;
    m_X2 = x2;
    m_Y1 = y1;
    m_Y2 = y2;
  }

  Vec2f Compute(float t)
  {
    static Vec2f first = { 0.f, 0.f };
    static Vec2f last = { 1.f, 1.f };
    Vec2f p1 = { m_X1, m_Y1 };
    Vec2f p2 = { m_X2, m_Y2 };

    Vec2f point = first;
    float a = 1 - t;
    float b = a * a;
    float y = t * t;

    /* B(t) = (1 - t)^3P0 + 3(1 - t)^2 t*P1 + 3(1 - t) t^2*P2 + t^3*P3 */

    point += 3 * b * t * p1;
    point += 3 * a * y * p2;
    point += y * t * last;

    return point;
  }

private:
  float m_X1 = 0.f;
  float m_Y1 = 0.f;
  float m_X2 = 0.f;
  float m_Y2 = 0.f;
};

#endif // SFUI_CUBICBEZIER_H
