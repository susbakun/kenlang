#include <cstddef>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <string_view>
#include <vector>

class Lox {
 public:
  static inline void runFile(std::string_view path) {
    std::ifstream fstream{static_cast<std::string>(path), std::ios::binary};

    if (!fstream.is_open()) {
      std::cerr << "Couldn't open the file\n";
      return;
    }

    // getting the file size
    fstream.seekg(0, std::fstream::end);
    auto file_size{fstream.tellg()};
    fstream.seekg(0, std::fstream::beg);

    std::vector<char> bytes(static_cast<std::size_t>(file_size));

    if (!fstream.read(reinterpret_cast<char*>(bytes.data()), file_size)) {
      std::cerr << "Couldn't read the file\n";
      return;
    }

    std::string source{std::string(bytes.begin(), bytes.end())};

    run(source);

    if (had_error) std::exit(65);
  }

  static inline void runPrompt() {
    while (true) {
      std::cout << "< ";
      std::string line{};
      std::cin >> line;
      if (line == "") break;
      run(line);
      had_error = false;
    }
  }

  static inline void run(std::string& source) {
    std::stringstream sstream{source};

    // iterate over tokens
  }

  void error(int line, std::string_view error) { report(line, "", error); }

  void report(int line, std::string_view where, std::string_view error) {
    std::println("[line {}] Error{}: {}", line, where, error);
    had_error = true;
  }

 private:
  static inline bool had_error{false};
};

int main(int argc, char* argv[]) {
  if (argc > 1) {
    std::cerr << "too many argmuents" << "\n";
    return 64;
  } else if (argc == 1) {
    Lox::runFile(argv[0]);
  } else {
    Lox::runPrompt();
  }

  return 0;
}