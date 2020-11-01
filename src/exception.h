#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_
#include <iostream>
#include <exception>
#include <string>

const int EXCEPTION_NOT_ENOUGH_INPUT =    0x1;
const int EXCEPTION_OPEN_DESIGN_ERROR =   0x2;

//use this by throw
//BaseException(__FILE__, __LINE__, __PRETTY_FUNCTION__, "comments")
class BaseException : public std::exception
{
public:
    BaseException(const std::string& fileName, int lineNum,
            const std::string& funcName, const std::string& message)
        : m_fileName(fileName)
        , m_lineNum(lineNum)
        , m_funcName(funcName)
        , m_message(message)
    {
    }

    ~BaseException() throw() {}

    void printLog() const
    {
        std::cout << "[Exception] " << m_fileName << ":" << m_lineNum
            << ": " << m_funcName << ": " << m_message << std::endl;
    }

private:
    std::string m_fileName;
    int m_lineNum;
    std::string m_funcName;
    std::string m_message;
};

#endif

