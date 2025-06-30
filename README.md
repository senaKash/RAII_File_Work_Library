# RAII_File_Work_Library

[![Typing SVG](https://readme-typing-svg.herokuapp.com?color=%2336BCF7&lines=Computer+science+student)](https://git.io/typing-svg)

**1)Создание объекта, берущего управление файлом**
>Реализовано в конструкторе FileRAII(const std::string& path, Mode mode)

```cpp
// FileRAII.hpp
class FileRAII {
public:
    enum class Mode { Read, Write, Append, ReadWrite };
    FileRAII(const std::string& path, Mode mode);
    // …
};
```

```cpp
// FileRAII.cpp
FileRAII::FileRAII(const std::string& path, Mode mode) {
    std::ios_base::openmode om = std::ios::binary;
    switch (mode) {
        case Mode::Read:      om |= std::ios::in;                    break;
        case Mode::Write:     om |= std::ios::out | std::ios::trunc; break;
        case Mode::Append:    om |= std::ios::out | std::ios::app;   break;
        case Mode::ReadWrite: om |= std::ios::in  | std::ios::out;   break;
    }
    _stream.open(path, om);
    if (!_stream.is_open() || _stream.fail())
        throw FileError("cannot open file '" + path + "'");
}

```
**2)Автоматическое закрытие по окончанию жизненного цикла**
>Реализовано в деструкторе ~FileRAII()

```cpp
// FileRAII.cpp
FileRAII::~FileRAII() {
    if (_stream.is_open())
        _stream.close();
}
```
```cpp
{
    // Запись в test.txt и автоматическое закрытие по выходу из блока
    FileRAII writer("test.txt", FileRAII::Mode::Write);
    writer.writeLine("Pervaya stroka");
    writer.writeLine("Vtoraya stroka");
} // <-- здесь срабатывает деструктор FileRAII и файл закрывается
```

**3)Методы *readLine* и *writeLine***
>Объявлены в *include/FileRAII.hpp* и определены в *src/FileRAII.cpp*

```cpp
// FileRAII.hpp
std::string readLine();
void writeLine(const std::string& line);
```

```cpp
// FileRAII.cpp
std::string FileRAII::readLine() {
    if (!_stream.is_open() || !_stream.good())
        throw FileError("stream not ready for reading");
    std::string line;
    if (!std::getline(_stream, line)) {
        if (_stream.eof()) return {};
        throw FileError("error reading line");
    }
    return line;
}

void FileRAII::writeLine(const std::string& line) {
    if (!_stream.is_open() || !_stream.good())
        throw FileError("stream not ready for writing");
    _stream << line;
    if (_stream.fail()) throw FileError("error writing line");
    _stream << '\n';
}

```

**4)Обработка ошибок через исключения**
>Класс исключения: *fsutil::FileError*, наследник *std::runtime_error*
>Бросается при любых ошибках открытия, чтения или записи

```cpp
// FileRAII.hpp
class FileError : public std::runtime_error {
public:
    explicit FileError(const std::string& msg)
        : std::runtime_error("FileError: " + msg) {}
};
```

**5)Демо‑программа (*demo/main.cpp*)**
>Показывает:
>- ✅ Запись (Write)
>- ✅ Чтение (Read)
>- ✅ Добавление (Append)
>- ✅ Демонстрацию ошибки при открытии несуществующей папки

```cpp
#include "FileRAII.hpp"
#include <iostream>
#include <filesystem>

int main() {
    using fsutil::FileRAII;
    using fsutil::FileError;

    const std::string path = "test.txt";

    try {
        // ---------- 1) Write and close immediately ----------
        {
            FileRAII writer(path, FileRAII::Mode::Write);
            writer.writeLine("Pervaya stroka");
            writer.writeLine("Vtoraya stroka");
            std::cout << "Recording is successful\n";
        }   

        
        std::cout << "Current working directory: "
            << std::filesystem::current_path() << "\n\n";

        // ---------- 2) Read content ----------
        {
            FileRAII reader(path, FileRAII::Mode::Read);
            std::cout << "File content:\n";
            while (true) {
                auto line = reader.readLine();
                if (line.empty()) break;   // EOF
                std::cout << line << "\n";
            }
        }

        // ---------- 3) Append ----------
        {
            FileRAII appender(path, FileRAII::Mode::Append);
            appender.writeLine("Appended line.");
            std::cout << "\nAppend successful\n";
        }

        // ---------- 4) ERROR----------
        try {
            FileRAII bad("nonexistent_dir/file.txt", FileRAII::Mode::Write);
        }
        catch (const FileError& ex) {
            std::cerr << "\nFile operation error: " << ex.what() << "\n";
        }

    }
    catch (const FileError& ex) {
        std::cerr << "Fatal FileError: " << ex.what() << "\n";
        return 1;
    }
    catch (const std::exception& ex) {
        std::cerr << "General error: " << ex.what() << "\n";
        return 2;
    }

    return 0;
}

```




