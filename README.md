# RAII_File_Work_Library

[![Typing SVG](https://readme-typing-svg.herokuapp.com?color=%2336BCF7&lines=Computer+science+student)](https://git.io/typing-svg)

**1)Создание объекта, берущего управление файлом**
>Реализовано в конструкторе FileRAII(const std::string& path, Mode mode)

```cpp
// Открытие файла на запись (создаст или очистит file.txt)
FileRAII writer("test.txt", FileRAII::Mode::Write);
```
**2)Автоматическое закрытие по окончанию жизненного цикла**
>Реализовано в деструкторе ~FileRAII()

```cpp
{
    FileRAII w("a.txt", FileRAII::Mode::Write);
    w.writeLine("Hello");
} // здесь файл закрывается автоматически

```

**3)Методы *readLine* и *writeLine***
>Объявлены в *include/FileRAII.hpp* и определены в *src/FileRAII.cpp*

```cpp
writer.writeLine("Первая строка");
std::string line = reader.readLine();
```

**4)Обработка ошибок через исключения**
>Класс исключения: *fsutil::FileError*, наследник *std::runtime_error*
>Бросается при любых ошибках открытия, чтения или записи

```cpp
try {
    FileRAII bad("/no/such", FileRAII::Mode::Read);
} catch (const FileError& ex) {
    std::cerr << ex.what() << "\n";
}
```

**5)Демо‑программа (*demo/main.cpp*)**
>Показывает:
>- ✅ Запись (Write)
>- ✅ Чтение (Read)
>- ✅ Добавление (Append)
>- ✅ Демонстрацию ошибки при открытии несуществующей папки

```cpp
// запись
{
    FileRAII w("test.txt", FileRAII::Mode::Write);
    w.writeLine("Pervaya stroka");
    w.writeLine("Vtoraya stroka");
    std::cout << "Zapis uspeshna\n";
}
// чтение
{
    FileRAII r("test.txt", FileRAII::Mode::Read);
    while (auto l = r.readLine(), !l.empty())
        std::cout << l << "\n";
}
// append
{
    FileRAII a("test.txt", FileRAII::Mode::Append);
    a.writeLine("Appended line.");
    std::cout << "Append uspeshny\n";
}
// ошибка
try {
    FileRAII bad("nonexistent_dir/x.txt", FileRAII::Mode::Write);
} catch (const FileError& ex) {
    std::cerr << "Oshibka: " << ex.what() << "\n";
}

```




