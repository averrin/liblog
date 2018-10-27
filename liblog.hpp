#ifndef __LOGGER_H_
#define __LOGGER_H_
#include <chrono>
#include <fmt/format.h>
#include <limits>
#include <map>
#include <regex>
#include <string>

using namespace std::string_literals;
using milliseconds = std::chrono::duration<double, std::milli>;

namespace LibLog {

namespace rang {

enum class style {
  reset = 0,
  bold = 1,
  dim = 2,
  italic = 3,
  underline = 4,
  blink = 5,
  rblink = 6,
  reversed = 7,
  conceal = 8,
  crossed = 9
};

enum class fg {
  black = 30,
  red = 31,
  green = 32,
  yellow = 33,
  blue = 34,
  magenta = 35,
  cyan = 36,
  gray = 37,
  reset = 39
};

enum class bg {
  black = 40,
  red = 41,
  green = 42,
  yellow = 43,
  blue = 44,
  magenta = 45,
  cyan = 46,
  gray = 47,
  reset = 49
};

enum class fgB {
  black = 90,
  red = 91,
  green = 92,
  yellow = 93,
  blue = 94,
  magenta = 95,
  cyan = 96,
  gray = 97
};

enum class bgB {
  black = 100,
  red = 101,
  green = 102,
  yellow = 103,
  blue = 104,
  magenta = 105,
  cyan = 106,
  gray = 107
};
}; // namespace rang

class utils {
public:
  static std::string color(rang::fg c, std::string m) {
    return fmt::format("\033[{}m{}\033[0m", static_cast<int>(c), m);
  }
  static std::string style(rang::style c, std::string m) {
    return fmt::format("\033[{}m{}\033[0m", static_cast<int>(c), m);
  }
  static std::string bg(rang::bg c, std::string m) {
    return fmt::format("\033[{}m{}\033[0m", static_cast<int>(c), m);
  }
  static std::string black(std::string m) { return color(rang::fg::black, m); }
  static std::string red(std::string m) { return color(rang::fg::red, m); }
  static std::string green(std::string m) { return color(rang::fg::green, m); }
  static std::string yellow(std::string m) {
    return color(rang::fg::yellow, m);
  }
  static std::string blue(std::string m) { return color(rang::fg::blue, m); }
  static std::string magenta(std::string m) {
    return color(rang::fg::magenta, m);
  }
  static std::string cyan(std::string m) { return color(rang::fg::cyan, m); }
  static std::string gray(std::string m) { return color(rang::fg::gray, m); }
  static std::string bold(std::string m) { return style(rang::style::bold, m); }

  static std::string blackBg(std::string m) { return bg(rang::bg::black, m); }
  static std::string redBg(std::string m) { return bg(rang::bg::red, m); }
  static std::string greenBg(std::string m) { return bg(rang::bg::green, m); }
  static std::string yellowBg(std::string m) { return bg(rang::bg::yellow, m); }
  static std::string blueBg(std::string m) { return bg(rang::bg::blue, m); }
  static std::string magentaBg(std::string m) {
    return bg(rang::bg::magenta, m);
  }
  static std::string cyanBg(std::string m) { return bg(rang::bg::cyan, m); }
  static std::string grayBg(std::string m) { return bg(rang::bg::gray, m); }

  template <typename T>
  static std::string join(const T &array, const std::string &delimiter) {
    std::string res;
    for (auto &element : array) {
      if (!res.empty()) {
        res += delimiter;
      }

      res += element;
    }

    return res;
  }

  template <typename rT, typename iT>
  static std::vector<std::shared_ptr<rT>>
  castObjects(std::vector<std::shared_ptr<iT>> input, bool unique = false) {
    std::vector<std::shared_ptr<rT>> result;
    for (auto input_object : input) {
      if (auto casted_object = std::dynamic_pointer_cast<rT>(input_object)) {
        result.push_back(casted_object);
      }
    }
    if (unique) {
      auto it = std::unique(result.begin(), result.end());
      result.resize(std::distance(result.begin(), it));
    }
    return result;
  }

  static std::vector<std::string> split(std::string strToSplit,
                                        char delimeter) {
    std::stringstream ss(strToSplit);
    std::string item;
    std::vector<std::string> splittedStrings;
    while (std::getline(ss, item, delimeter)) {
      splittedStrings.push_back(item);
    }
    return splittedStrings;
  }
};

class Logger {
  std::map<std::string, std::chrono::time_point<std::chrono::system_clock>>
      _start;
  std::map<std::string, std::string> _aliases;

  std::string getOffset(int d = 0) {
    std::string offset = "";
    for (auto n = 0; n < d; n++) {
      offset += "| ";
    }
    if (_start.size() == 0) {
      return offset;
    }
    for (auto n = 0; n < _start.size() - 1; n++) {
      offset += "| ";
    }
    return offset;
  }
  std::string getName(std::string label) {

    auto pName = name;
    if (_aliases.find(label) != _aliases.end()) {
      pName = _aliases[label];
    }
    return fill(pName, 6);
  }

  std::string fill(std::string m, int c) {
    std::regex esc_re("\033\\[\\d{1,2}m", std::regex::ECMAScript);
    std::string stripped = std::regex_replace(m, esc_re, "");
    int d = int(c) - int(stripped.size());
    if (d <= 0) {
      return m;
    }
    std::string offset = "";
    for (auto n = 0; n < d; n++) {
      offset += " ";
    }
    return m + offset;
  }

public:
  static Logger &getInstance() {
    static Logger instance("ROOT");
    return instance;
  }

private:
  Logger(std::string n) : name(n) {}

public:
  Logger(Logger const &) = delete;
  void operator=(Logger const &) = delete;

public:
  std::string name;
  void warn(std::string msg) { warn(name, msg); }
  void warn(std::string alias, std::string msg) {
    fmt::print(" {} ≫ {}[{}] ⊸  {}\n", fill(alias, 6), getOffset(1),
               utils::yellowBg(utils::black("WARN")), msg);
  }
  void info(std::string msg) { info(name, msg); }
  void info(std::string alias, std::string msg) {
    fmt::print(" {} ≫ {}[{}] ⊸  {}\n", fill(alias, 6), getOffset(1),
               utils::blueBg(utils::black("INFO")), msg);
  }
  void error(std::string msg) { error(name, msg); }
  void error(std::string alias, std::string msg) {
    fmt::print(" {} ≫ {}[{}] ⊸  {}\n", fill(alias, 6), getOffset(1),
               utils::redBg("!ERR"), msg);
  }
  void start(std::string alias, std::string label, bool silent = false) {
    _aliases[label] = alias;
    start(label, silent);
  }
  void start(std::string label, bool silent = false) {
    _start[label] = std::chrono::system_clock::now();
    if (silent)
      return;
    fmt::print(" {} ≫ {}[{}] ⊷  {}\n", getName(label), getOffset(), label,
               utils::yellow("start"));
  }
  void stop(std::string label, float b = 0) { stop(label, label, b); }

  void stop(std::string label, std::string msg, float b) {
    auto start = _start.at(label);
    milliseconds ms = std::chrono::system_clock::now() - start;
    if (ms.count() < b) {
      _start.erase(label);
      return;
    }
    auto time = utils::green(fmt::format("{}", ms.count()));
    if (ms.count() > 50) {
      time = utils::style(rang::style::bold,
                          utils::redBg(fmt::format("{}", ms.count())));
    }
    fmt::print(" {} ≫ {}[{}] ⊶  {}{}\n", getName(label), getOffset(), msg, time,
               utils::yellow("ms"));
    _start.erase(label);
    _aliases.erase(label);
  }
};
} // namespace LibLog
#endif // __LOGGER_H_
