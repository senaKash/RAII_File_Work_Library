// src/FileRAII.cpp
#include "FileRAII.hpp"

namespace fsutil {

    FileRAII::FileRAII(const std::string& path, Mode mode) {
        std::ios_base::openmode om = std::ios::binary;
        switch (mode) {
        case Mode::Read:      om |= std::ios::in;                      break;
        case Mode::Write:     om |= std::ios::out | std::ios::trunc;   break;
        case Mode::Append:    om |= std::ios::out | std::ios::app;     break;
        case Mode::ReadWrite: om |= std::ios::in | std::ios::out;     break;
        }
        _stream.open(path, om);
        if (!_stream.is_open() || _stream.fail()) {
            throw FileError("cannot open file '" + path + "'");
        }
    }

    FileRAII::~FileRAII() {
        if (_stream.is_open())
            _stream.close();
    }

    FileRAII::FileRAII(FileRAII&& other) noexcept
        : _stream(std::move(other._stream)) {
    }

    FileRAII& FileRAII::operator=(FileRAII&& other) noexcept {
        if (this != &other) {
            if (_stream.is_open())
                _stream.close();
            _stream = std::move(other._stream);
        }
        return *this;
    }

    std::string FileRAII::readLine() {
        if (!_stream.is_open() || !_stream.good())
            throw FileError("stream not ready for reading");
        std::string line;
        if (!std::getline(_stream, line)) {
            if (_stream.eof())
                return std::string();
            throw FileError("error reading line");
        }
        return line;
    }

    void FileRAII::writeLine(const std::string& line) {
        if (!_stream.is_open() || !_stream.good())
            throw FileError("stream not ready for writing");
        _stream << line;
        if (_stream.fail())
            throw FileError("error writing line");
        _stream << '\n';
    }

} // namespace fsutil
