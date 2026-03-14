#pragma once
#include <vector>
#include <cstddef> // for std::size_t

// -----------------------
// Abstract Data Source
// -----------------------
class CDataSource {
public:
    virtual ~CDataSource() = default;

    // Returns true if the source has reached its end
    virtual bool End() const noexcept = 0;

    // Reads one character and advances the read position
    virtual bool Get(char &ch) noexcept = 0;

    // Peeks at the next character without advancing
    virtual bool Peek(char &ch) noexcept = 0;

    // Reads up to 'count' characters into buf; clears buf first
    virtual bool Read(std::vector<char> &buf, std::size_t count) noexcept = 0;
};