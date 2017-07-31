#ifndef SFUI_COMMON_H
#define SFUI_COMMON_H

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
#include <SFUI/Include/System/Sys.h>
#include <SFUI/Include/Utilities/Vec.hpp>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>
#include <SelbaWard.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////
#include <bitset>
#include <string>
#include <vector>
#include <memory>
#include <utility>
#include <map>
#include <cfloat>
#include <sstream>
#include <cstdint>
#include <functional>
#include <chrono>
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <stdexcept>

#define D_SCL_SECURE_NO_WARNINGS

#define cast_float(x) (static_cast<float>(x))
#define cast_int(x) (static_cast<int>(x))

#define ClassFunction(INST, FUNC)                      \
std::bind(&##FUNC, &##INST );

#define MemberFunction(Class, Function, Instance)              \
std::bind1st(std::mem_fun(&Class##::##Function##), &Instance);

using Mouse = sf::Mouse;
using MouseButton = sf::Mouse::Button;

using Keyboard = sf::Keyboard;
using Kb = Keyboard;
using Key = sf::Keyboard::Key;

using IntRect = sf::IntRect;
using FloatRect = sf::FloatRect;

using Font = sf::Font;
using Color = sf::Color;

using sstring = std::string;
using sstream = std::stringstream;
template<class css_t, class css_u> using pair = std::pair<css_t, css_u>;

using RenderTarget = sf::RenderTarget;
using RenderTexture = sf::RenderTexture;
using RenderWindow = sf::RenderWindow;

#if defined(ENV_64)
using cfloat_hp = double;
using cfloat_lp = float;
#else //defined(ENV_64)
using cfloat_hp = float;
using cfloat_lp = float;
#endif

/* Integral types */
using int64 = std::int64_t;
using int32 = std::int32_t;
using int16 = std::int16_t;
using int8 = std::int8_t;

using uint64 = std::uint64_t;
using uint32 = std::uint32_t;
using uint16 = std::uint16_t;
using uint8 = std::uint8_t;

#if defined(ENV_64)
constexpr int64  int64_bits = 64;
constexpr uint64 uint64_bits = 64;
#else //defined(ENV_64)
constexpr int64  int64_bits = 32;
constexpr uint64 uint64_bits = 32;
#endif //defined (ENV_64)

constexpr int32  int32_bits = 32;
constexpr int16  int16_bits = 16;
constexpr int8   int8_bits = 8;
constexpr uint32 uint32_bits = 32;
constexpr uint16 uint16_bits = 16;
constexpr uint8  uint8_bits = 8;

constexpr int64 int64_max = INT64_MAX;
constexpr int64 int64_min = INT64_MIN;
constexpr int32 int32_max = INT32_MAX;
constexpr int32 int32_min = INT32_MIN;
constexpr int16 int16_max = INT16_MAX;
constexpr int16 int16_min = INT16_MIN;
constexpr int8  int8_max = INT8_MAX;
constexpr int8  int8_min = INT8_MIN;

constexpr uint64 uint64_max = UINT64_MAX;
constexpr uint32 uint32_max = UINT32_MAX;
constexpr uint16 uint16_max = UINT16_MAX;
constexpr uint8  uint8_max = UINT8_MAX;

template<typename __css_t> using css_shared_ptr = std::shared_ptr<__css_t>;
template<typename __css_t> using css_unique_ptr = std::unique_ptr<__css_t>;
template<typename __css_t, typename __css_u> using css_pair = std::pair<__css_t, __css_u>;
template<typename __css_t, typename __css_u> using css_map = std::map<__css_t, __css_u>;

using system_clock = std::chrono::system_clock;
template<typename css_t> using time_pt = std::chrono::time_point<css_t>;
using ms = std::chrono::milliseconds;
using ns = std::chrono::nanoseconds;
using sec = std::chrono::seconds;
using mins = std::chrono::minutes;
using hrs = std::chrono::hours;
namespace ch = std::chrono;

using std::begin;
using std::end;
using std::unordered_map;
using std::unordered_set;
using std::for_each;
using std::find;
using std::find_if;

template<typename _UI_TYPE_>
using _shared_ptr = std::shared_ptr<_UI_TYPE_>;

using font_handle = _shared_ptr<sf::Font>;
using texture_handle = _shared_ptr<sf::Texture>;

#define CSS_PTR_TYPEDEF(CSS_CLASS)       \
using Ptr = css_shared_ptr<CSS_CLASS>;  \
using UPtr = css_unique_ptr<CSS_CLASS>;  \
using RPtr = CSS_CLASS*;

#define CREATE_SHARED_DEFAULT(CSS_CLASS)      \
static CSS_CLASS##::sptr create() { return std::make_shared<CSS_CLASS>(); }

#define CREATE_SHARED_WPARENT(CSS_CLASS)      \
static CSS_CLASS##::sptr create(CSS_CLASS##::rptr par) \
{                                                      \
  CSS_CLASS##::sptr wid = create();                    \
  wid->parent = par;                                   \
  return wid;                                          \
}

#define DEFAULT_FUNCTION_1ARG [](auto arg) { return; }
#define DEFAULT_FUNCTION_0ARG []() { return; }

#define PRIVATE_VAR(TYPE, VAR)   \
private: TYPE VAR##;             \
public:                          \

#define GLOBAL ::

#define SFUI_CLASS_TRAITS(C_CLASS)       \
using self_type = C_CLASS;               \
using pointer = C_CLASS*;                \
using reference = C_CLASS&;              \
using const_pointer = const C_CLASS*;    \
using const_reference = const C_CLASS&;  \
using shared_ptr = _shared_ptr<C_CLASS>; \

#define StaticSharedCreate(CLASS)     \
static CLASS::shared_ptr Create() {   \
  CLASS::shared_ptr sptr = std::make_shared<CLASS>(); \
  return sptr; } \

#define CharToAlphaAscii(_ival) ( ( _ival >= 0 && _ival < 10 ? char(_ival + 48) : char(_ival + 55) ) )

#define PrintVector(VEC)      \
std::cout << "(" << VEC.x << ", " << VEC.y << ")" << std::endl;

#define PrintRect(_RECT)      \
std::cout << "(" << _RECT.left << ", " << _RECT.top << ", " << _RECT.width << ", " << _RECT.height << ")" << std::endl;

#define ExecuteOnlyOnce \
static bool __loaded__ = false; \
if (__loaded__) return; \
__loaded__ = true;

#define CodeSectionOnce( CODE ) \
static bool __executed__ = false; \
if (!__executed__) { __executed__ = true; CODE; } ;

constexpr ms operator"" _ms(unsigned long long l) { return std::chrono::milliseconds(l); }
constexpr sf::Uint8 operator"" _ui8(unsigned long long l) { return static_cast< sf::Uint8 >( l ); }

#endif // SFUI_COMMON_H
