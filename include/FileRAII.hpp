// include/FileRAII.hpp
#ifndef FILERAII_HPP
#define FILERAII_HPP

#include <fstream>
#include <string>
#include <stdexcept>

namespace fsutil {

    /// ���������� ��� ������ �������� ��������
    class FileError : public std::runtime_error {
    public:
        explicit FileError(const std::string& msg)
            : std::runtime_error("FileError: " + msg) {}
    };

    /// �����-������ ��� std::fstream � RAII
    class FileRAII {
    public:
        enum class Mode { Read, Write, Append, ReadWrite };

        /// ��������� ���� � �������� ������ ��� ������� FileError
        FileRAII(const std::string& path, Mode mode);
        /// ��������� ����
        ~FileRAII();

        // ������� �����������
        FileRAII(const FileRAII&) = delete;
        FileRAII& operator=(const FileRAII&) = delete;
        // ��������� �����������
        FileRAII(FileRAII&& other) noexcept;
        FileRAII& operator=(FileRAII&& other) noexcept;

        /// ������ ���� ������ (��� ������� '\\n'). 
        /// ���������� ������ ������ �� EOF.
        /// ������� FileError ��� �������.
        std::string readLine();

        /// ���������� ������ � ����������� �������� '\\n'.
        /// ������� FileError ��� �������.
        void writeLine(const std::string& line);

    private:
        std::fstream _stream;
    };

} // namespace fsutil

#endif // FILERAII_HPP
