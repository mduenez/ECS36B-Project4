#pragma once
#include "DataSource.h"
#include <string>
#include <vector>

class CStringDataSource : public CDataSource {
private:
    std::string m_string; // internal buffer
    size_t m_index;       // current read index

public:
    // Constructor
    CStringDataSource(const std::string &str)
        : m_string(str), m_index(0) {}

    // Check if end of string is reached
    bool End() const noexcept override {
        return m_index >= m_string.size();
    }

    // Read the next character
    bool Get(char &ch) noexcept override {
        if (m_index < m_string.size()) {
            ch = m_string[m_index++];
            return true;
        }
        return false;
    }

    // Peek at the next character without advancing
    bool Peek(char &ch) noexcept override {
        if (m_index < m_string.size()) {
            ch = m_string[m_index];
            return true;
        }
        return false;
    }

    // Read up to 'count' characters into buffer
    bool Read(std::vector<char> &buf, std::size_t count) noexcept override {
        buf.clear();
        std::size_t remaining = m_string.size() - m_index;
        std::size_t n = (count < remaining) ? count : remaining;

        if (n == 0) return false;

        buf.insert(buf.end(), m_string.begin() + m_index, m_string.begin() + m_index + n);
        m_index += n;
        return true;
    }

    // Optional: reset reading position
    void Reset() noexcept { m_index = 0; }
};