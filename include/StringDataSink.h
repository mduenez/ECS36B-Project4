#pragma once
#include "DataSink.h"
#include <string>
#include <vector>

// -----------------------
// String-based Data Sink
// -----------------------
class CStringDataSink : public CDataSink {
private:
    std::string m_string;  // internal buffer

public:
    // Returns the current string contents
    const std::string& String() const { return m_string; }

    // Add a single character to the buffer
    bool Put(const char &ch) noexcept override {
        m_string += ch;
        return true;
    }

    // Add a vector of chars to the buffer
    bool Write(const std::vector<char> &buf) noexcept override {
        m_string.append(buf.data(), buf.size());
        return true;
    }

    // Add a std::string directly
    bool Write(const std::string &str) noexcept {
        m_string += str;
        return true;
    }
};