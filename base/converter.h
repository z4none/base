#pragma once

#include <vector>
#include <string>
#include "iconv/iconv.h"

namespace base
{

class Converter
{
public:
    Converter() : m_cd((iconv_t)-1) {}
    Converter(const char *from, const char *to, bool ignore_error=false) 
    { 
        m_cd = iconv_open(to, from); 
        m_ignore_error = ignore_error;
    }
    virtual ~Converter() { close(); }

    bool open(const char *from, const char *to)
    {
        close();
        m_cd = iconv_open(to, from);
        return is_open();
    }

    bool is_open() const
    {
        return (m_cd != (iconv_t)-1);
    }

    void close() 
    {
        if (is_open())
        {
            iconv_close(m_cd);
            m_cd = (iconv_t)-1;
        }
    }

    operator iconv_t() const
    {
        return m_cd;
    }

    template<typename InCharT, typename OutCharT> 
    inline bool convert(const std::basic_string<InCharT> input, std::basic_string<OutCharT> & output)
    {
        if (!is_open())
            return false;

        const char * in = (const char *)input.data();
        size_t inSize = input.size() * sizeof(InCharT);
        size_t bufSize = 256;
        std::vector<char> buf(bufSize);

        while (0 < inSize) 
        {
            char * bufPtr = buf.data();
            bufSize = buf.size();
            size_t ret = iconv(m_cd, &in, &inSize, &bufPtr, &bufSize);
            if (ret == (size_t)-1)
            {
                if (errno == E2BIG) 
                {
                    // ignore this error
                }
                else if (m_ignore_error) {
                    // skip character
                    ++in;
                    --inSize;
                }
                else
                {
                    return false;
                }
            }
            output.append((OutCharT *)buf.data(), (buf.size() - bufSize) / sizeof(OutCharT));
        }

        return true;
    }

    void reset()
    {
        iconv(m_cd, NULL, NULL, NULL, NULL);
    }

protected:
    iconv_t m_cd;
    bool m_ignore_error;
};


std::string WsToUtf8(const std::wstring & ws);
std::wstring Utf8ToWs(const std::string & utf8);
std::string Utf32ToUtf8(const std::u32string & u32);
std::u32string Utf8ToUtf32(const std::string & utf8);
std::string GbkToUtf8(const std::string & gbk);
std::string Utf8ToGbk(const std::string & utf8);

} // namespace base