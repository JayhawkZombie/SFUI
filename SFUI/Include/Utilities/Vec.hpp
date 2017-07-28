#ifndef SFUI_VEC_H
#define SFUI_VEC_H

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

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////
#include <SFML/Graphics.hpp>

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

#include <algorithm>

/* Simple 2D vector class - friendly with the sf::Vector2 template and
*  its specializations */

template<class Vec_t>
class Vec2 {
public:
  Vec2( ) = default;

  template<typename other_t>
  Vec2(const Vec2<other_t> &v) : x(static_cast<Vec_t>( v.x )), y(static_cast<Vec_t>( v.y ))
  { }

  template<typename sf_t, typename std::enable_if<std::is_convertible<sf_t, Vec_t>::value>* = nullptr>
  Vec2(const sf::Vector2<sf_t> &v) : x(static_cast<Vec_t>( v.x )), y(static_cast<Vec_t>( v.y )) { }

  Vec2(const Vec_t &a, const Vec_t &b) : x(a), y(b) { }
  ~Vec2( ) = default;

  template<typename sf_t>
  operator sf::Vector2<sf_t>( ) const { return sf::Vector2<sf_t>(static_cast<sf_t>( x ), static_cast<sf_t>( y )); }

  template<typename other_t>
  Vec2<Vec_t>& operator=(const sf::Vector2<other_t> &vec) { x = static_cast<Vec_t>( vec.x ); y = static_cast<Vec_t>( vec.y ); return *this; }

  float mag( ) const
  {
    static float s_mag = std::sqrt(( x * x ) + ( y * y ));
    static float s_x = static_cast<float>( x );
    static float s_y = static_cast<float>( y );

    if (std::abs(s_x - x) < 0.01 && std::abs(s_y - y) < 0.01)
      return s_mag;

    s_mag = std::sqrt(( x * x ) + ( y * y ));
    return s_mag;
  }

  template<typename other_t, typename std::enable_if<std::is_convertible<Vec_t, other_t>::value>::type* = nullptr>
  void operator += (const Vec2<other_t> &other) { x += static_cast<Vec_t>( other.x ); y += static_cast<Vec_t>( other.y ); }

  template<typename other_t, typename std::enable_if<std::is_convertible<Vec_t, other_t>::value>::type* = nullptr>
  void operator -= (const Vec2<other_t> &other) { x -= static_cast<Vec_t>( other.x ); y -= static_cast<Vec_t>( other.y ); }
  template<typename fact_t>
  void operator *= (const fact_t &factor) { x = static_cast<Vec_t>( x * factor ); y = static_cast<Vec_t>( y * factor ); }
  template<typename fact_t>
  void operator /= (const fact_t &factor) { x = static_cast<Vec_t>( x / factor ); y = static_cast<Vec_t>( y / factor ); }

  template<typename other_t, typename std::enable_if<std::is_integral<other_t>::value>::type* = nullptr>
  bool operator==(const Vec2<other_t> &other)
  {
    return ( ( static_cast<other_t>( x ) == other.x ) && ( static_cast<other_t>( y ) == other.y ) );
  }

  template<typename other_t, typename std::enable_if<std::is_floating_point<other_t>::value>::type* = nullptr>
  bool operator==(const Vec2<other_t> &other)
  {
    return ( std::abs(x - other.x) < std::numeric_limits<Vec_t>::epsilon( ) &&
            std::abs(y - other.y) < std::numeric_limits<Vec_t>::epsilon( ) );
  }

  template<typename fact_t>
  friend Vec2<Vec_t> operator *(const fact_t &factor, const Vec2<Vec_t> &vec);

  template<typename fact_t>
  friend Vec2<Vec_t> operator *(const Vec2<Vec_t> &vec, const fact_t &factor);

  template<typename fact_t>
  friend Vec2<Vec_t> operator /(const fact_t &factor, const Vec2<Vec_t> &vec);

  template<typename fact_t>
  friend Vec2<Vec_t> operator /(const Vec2<Vec_t> &vec, const fact_t &factor);

  template<typename other_t>
  friend Vec2<Vec_t> operator+ (const Vec2<Vec_t> &v1, const Vec2<other_t> &v2);
  template<typename other_t>
  friend Vec2<Vec_t> operator- (const Vec2<Vec_t> &v1, const Vec2<other_t> &v2);

  Vec_t x = static_cast<Vec_t>( 0 );
  Vec_t y = static_cast<Vec_t>( 0 );
};

template<class Vec_t, typename fact_t>
inline Vec2<Vec_t> operator*(const fact_t & factor, const Vec2<Vec_t>& vec)
{
  return Vec2<Vec_t>(static_cast< Vec_t >( vec.x * factor ), static_cast< Vec_t >( vec.y * factor ));
}

template<class Vec_t, typename fact_t>
inline Vec2<Vec_t> operator*(const Vec2<Vec_t>& vec, const fact_t & factor)
{
  return Vec2<Vec_t>( vec.x * factor, vec.y * factor );
}

template<class Vec_t, typename fact_t>
inline Vec2<Vec_t> operator/(const fact_t & factor, const Vec2<Vec_t>& vec)
{
  return Vec2<Vec_t>( vec.x / factor, vec.y / factor );
}

template<class Vec_t, typename fact_t>
inline Vec2<Vec_t> operator/(const Vec2<Vec_t>& vec, const fact_t & factor)
{
  return Vec2<Vec_t>( vec.x / factor, vec.y / factor );
}

template<class Vec_t, typename other_t>
inline Vec2<Vec_t> operator+(const Vec2<Vec_t>& v1, const Vec2<other_t>& v2)
{
  return Vec2<Vec_t>( v1.x + v2.x, v1.y + v2.y );
}

template<class Vec_t, typename other_t>
inline Vec2<Vec_t> operator-(const Vec2<Vec_t>& v1, const Vec2<other_t>& v2)
{
  return Vec2<Vec_t>( v1.x - v2.x, v1.y - v2.y );
}

using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;
using Vec2d = Vec2<double>;
using Vec2u = Vec2<unsigned>;
using Vec2l = Vec2<long>;
using Vec2ul = Vec2<unsigned long>;

#define cast_vector2f(x) (static_cast<sf::Vector2f>(x))

#endif // SFUI_VEC_H
