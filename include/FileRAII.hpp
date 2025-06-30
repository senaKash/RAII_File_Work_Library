// include/FileRAII.hpp
#ifndef FILERAII_HPP
#define FILERAII_HPP

#include <fstream>
#include <string>
#include <stdexcept>

namespace fsutil {

    /// Исключение для ошибок файловых операций
    class FileError : public std::runtime_error {
    public:
        explicit FileError(const std::string& msg)
            : std::runtime_error("FileError: " + msg) {}
    };

    /// Класс-обёртка над std::fstream с RAII
    class FileRAII {
    public:
        enum class Mode { Read, Write, Append, ReadWrite };

        /// Открывает файл в заданном режиме или бросает FileError
        FileRAII(const std::string& path, Mode mode);
        /// Закрывает файл
        ~FileRAII();

        // Убираем копирование
        FileRAII(const FileRAII&) = delete;
        FileRAII& operator=(const FileRAII&) = delete;
        // Разрешаем перемещение
        FileRAII(FileRAII&& other) noexcept;
        FileRAII& operator=(FileRAII&& other) noexcept;

        /// Читает одну строку (без символа '\\n'). 
        /// Возвращает пустую строку на EOF.
        /// Бросает FileError при ошибках.
        std::string readLine();

        /// Записывает строку с последующим символом '\\n'.
        /// Бросает FileError при ошибках.
        void writeLine(const std::string& line);

    private:
        std::fstream _stream;
    };

} // namespace fsutil

#endif // FILERAII_HPP
