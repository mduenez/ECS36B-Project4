#include "StringDataSource.h"

CStringDataSource::CStringDataSource(const std::string &str) : DString(str), DIndex(0) {}

bool CStringDataSource::End() const noexcept {
    return DIndex >= DString.length();
}

bool CStringDataSource::Get(char &ch) noexcept {
    if (DIndex < DString.length()) {
        ch = DString[DIndex++];
        return true;
    }
    return false;
}

bool CStringDataSource::Peek(char &ch) noexcept {
    if (DIndex < DString.length()) {
        ch = DString[DIndex];
        return true;
    }
    return false;
}

bool CStringDataSource::Read(std::vector<char> &buf, std::size_t count) noexcept {
    buf.clear();
    std::size_t remaining = DString.length() - DIndex;
    std::size_t toRead = std::min(count, remaining);
    if (toRead == 0) return false;

    buf.reserve(toRead);
    for (std::size_t i = 0; i < toRead; ++i) {
        buf.push_back(DString[DIndex++]);
    }
    return true;
}