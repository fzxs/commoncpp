
#ifndef __GTL_StringBuilder_EX_H_
#define __GTL_StringBuilder_EX_H_

#include <string.h>
#include <string>

#include "buf.h"

/*
高效的StringBuilder
缺点：需要依赖buf.h
*/

namespace gtl 
{
    class StringBuilderEx
    {
    public:
        StringBuilderEx & Append(const std::string &str)
        {
            this->append(str);
            return *this;
        }

        StringBuilderEx & Revoke(unsigned int size) 
        {
            if (size < this->_buffer.readableBytes())
            {
                this->revoke(size);
            }
            return *this;
        }

        StringBuilderEx & AppendLine(const std::string &str) 
        {
            static char lineFeed[]{ 10, 0 }; // C++ 11

            this->append(str);
            this->append(lineFeed);

            return *this;
        }

        StringBuilderEx & AppendLine() 
        {
            static char lineFeed[]{ 10, 0 };

            this->append(lineFeed);
            return *this;
        }

        void Clear()
        {
            this->_buffer.retrieveAll();
        }

        std::string ToString() const 
        {
            return std::string((const char *)this->_buffer.peek(), this->_buffer.readableBytes());
        }

        unsigned int Length()
        {
            return this->_buffer.readableBytes();
        }

    private:
        void append(const std::string &str)
        {
            this->_buffer.append(str.c_str(), str.length());
        }

        void revoke(unsigned int size)
        {
            this->_buffer.unwrite(size);
        }

    private:
        Buffer _buffer;

    };
}

#endif
