#ifndef SFUI_COLORS_H
#define SFUI_COLORS_H

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
#include <SFUI/Include/Common.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

constexpr uint32 TEXT_OPACITY_HIGH_CONTRAST_LARGE_TEXT = 138;
constexpr uint32 TEXT_OPACITY_HIGH_CONTRAST_SMALL_TEXT = 221;
constexpr uint32 TEXT_OPACITY_TEXT_DISABLED = 80;

const sf::Color WIDGET_STYLE_COLOR_NEAR_BLACK = sf::Color(15, 15, 15);
const sf::Color WIDGET_STYLE_COLOR_VERY_DARK = sf::Color(35, 35, 35);
const sf::Color WIDGET_STYLE_COLOR_DARK = sf::Color(61, 61, 61);
const sf::Color WIDGET_STYLE_COLOR_MEDIUM_DARK = sf::Color(76, 76, 76);
const sf::Color WIDGET_STYLE_COLOR_MEDIUM_LIGHT_DARK = sf::Color(92, 92, 92);
const sf::Color WIDGET_STYLE_COLOR_LIGHT_DARK = sf::Color(102, 102, 102);
const sf::Color WIDGET_STYLE_COLOR_MEDIUM_GREY = sf::Color(155, 155, 155);
const sf::Color WIDGET_STYLE_COLOR_LIGHT_GREY = sf::Color(191, 191, 191);

#endif // SFUI_COLORS_H
