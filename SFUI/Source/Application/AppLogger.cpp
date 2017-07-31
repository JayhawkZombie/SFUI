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
#include <SFUI/Include/Application/AppLogger.h>

////////////////////////////////////////////////////////////
// Dependency Headers
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// Standard Library Headers
////////////////////////////////////////////////////////////

namespace sfui
{  
  
  FileLogPolicy::FileLogPolicy()
    : m_OutStream(new std::ofstream)
  {

  }

  FileLogPolicy::~FileLogPolicy()
  {
    if (m_OutStream)
    {
      CloseStream();
      m_OutStream.reset();
    }
  }

  void FileLogPolicy::OpenStream(const std::string &Name)
  {
    m_OutStream->open(Name, std::ios::binary | std::ios::out);
    if (!m_OutStream) {
      std::cerr << "Could not open file \"" << Name << "\" for logging\n";
      throw std::runtime_error("AppLogger: Failed to open output stream");
    }
  }

  void FileLogPolicy::CloseStream()
  {
    if (m_OutStream) {
      m_OutStream->close();
    }
  }

  void FileLogPolicy::Write(const std::string &Message)
  {
    m_OutStream->write(Message.c_str(), Message.size());
    m_OutStream->put('\n');
  }

}  
