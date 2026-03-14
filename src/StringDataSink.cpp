#include "StringDataSink.h"

const std::string &CStringDataSink::String() const{
    return DString;
}

bool CStringDataSink::Put(const char &ch) noexcept{
    DString += std::string(1,ch);
    return true;
}

bool CStringDataSink::Write(const std::vector<char> &buf) noexcept{
    DString += std::string(buf.data(),buf.size());
    return true;
}
#include "StringDataSink.h"

const std::string &CStringDataSink::String() const {
    return DString;
}

bool CStringDataSink::Put(const char &ch) noexcept {
    DString.push_back(ch); // more efficient than creating a temporary string
    return true;
}

bool CStringDataSink::Write(const std::vector<char> &buf) noexcept {
    DString.append(buf.data(), buf.size()); // directly append buffer
    return true;
}