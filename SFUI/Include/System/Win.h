#ifndef SFUI_WIN_H
#define SFUI_WIN_H

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

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

#if defined ( _MSC_VER )

#if ( _MSC_VER >= 1900 )

#include <string_view>
#include <any>
#include <filesystem>
#include <variant>
#include <optional>

template<typename opt_t>
using optional = std::optional<opt_t>;

#endif

#pragma warning ( default : 4061 ) // enumerator 'identifier' in a switch of enum 'enumeration' is not explicitly handled by a case label
#pragma warning ( default : 4062 ) // enumerator 'identifier' in a switch of enum 'enumeration' is not handled 
#pragma warning ( default : 4191 ) // 'operator/operation': unsafe conversion from 'type of expression' to 'type required'
#pragma warning ( default : 4242 ) // 'identifier': conversion from 'type1' to 'type2', possible loss of data
#pragma warning ( default : 4254 ) // 'operator': conversion from 'type1' to 'type2', possible loss of data
#pragma warning ( default : 4296 ) // 'operator': expression is always false 
#pragma warning ( default : 4365 ) // 'action': conversion from 'type_1' to 'type_2', signed/unsigned mismatch
#pragma warning ( default : 4388 ) // signed/unsigned mismatch
#pragma warning ( default : 4464 ) // relative include path contains '..'
#pragma warning ( default : 4514 ) // 'function': unreferenced inline function has been removed
#pragma warning ( default : 4640 ) // 'instance': construction of local static object is not thread-safe
#pragma warning ( default : 4625 ) // 'derived class': copy constructor could not be generated because a base class copy constructor is inaccessible
#pragma warning ( default : 4623 ) // 'derived class': default constructor could not be generated because a base class default constructor is inaccessible
#pragma warning ( default : 4626 ) // 'derived class': assignment operator could not be generated because a base class assignment operator is inaccessible

#endif

#endif // SFUI_WIN_H
