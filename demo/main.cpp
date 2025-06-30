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

        // ---------- 4) Демонстрация обработки ошибки ----------
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
