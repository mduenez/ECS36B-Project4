#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

// -----------------------
// Abstract Base Class
// -----------------------
class DataSink {
public:
    virtual ~DataSink() = default;

    // Write string data to sink
    virtual void Write(const std::string &data) = 0;

    // Optional: write a char buffer
    virtual void Write(const std::vector<char> &buf) {
        Write(std::string(buf.data(), buf.size()));
    }
};

// -----------------------
// FileDataSink
// -----------------------
class FileDataSink : public DataSink {
    std::ofstream file;
public:
    explicit FileDataSink(const std::string &filename) : file(filename) {
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << "\n";
        }
    }

    ~FileDataSink() {
        if (file.is_open()) file.close();
    }

    void Write(const std::string &data) override {
        if (file.is_open()) {
            file << data;
            file.flush();  // ensure data is immediately written
        }
    }

    void Write(const std::vector<char> &buf) override {
        if (file.is_open()) {
            file.write(buf.data(), buf.size());
            file.flush();
        }
    }
};

// -----------------------
// StringDataSink (for testing)
// -----------------------
class StringDataSink : public DataSink {
    std::string storage;
public:
    StringDataSink() = default;

    const std::string& Str() const { return storage; }

    void Write(const std::string &data) override {
        storage += data;
    }

    void Write(const std::vector<char> &buf) override {
        storage.append(buf.data(), buf.size());
    }
};