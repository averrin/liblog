#ifndef __LOGGER_H_
#define __LOGGER_H_
#include <chrono>
#include <fmt/color.h>
#include <fmt/format.h>
#include <functional>
#include <iostream>
#include <limits>
#include <map>
#include <regex>
#include <sstream>
#include <string>

using namespace std::string_literals;
using milliseconds = std::chrono::duration<double, std::milli>;

namespace LibLog {

class utils {
public:
  template <typename S, typename... Args>
  static std::string color(fmt::internal::color_type c, const S &fmt_string,
                           const Args &... args) {
    return fmt::format(fmt::fg(c), fmt_string,
                       std::forward<const Args &>(args)...);
  }
  template <typename S, typename... Args>
  static std::string bg(fmt::internal::color_type c, const S &fmt_string,
                        const Args &... args) {
    return fmt::format(fmt::bg(c), fmt_string,
                       std::forward<const Args &>(args)...);
  }
  template <typename S, typename... Args>
  static std::string style(fmt::text_style c, const S &fmt_string,
                           const Args &... args) {
    return fmt::format(c, fmt_string, std::forward<const Args &>(args)...);
  }

  template <typename S, typename... Args>
  static std::string red(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::red, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string black(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::black, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string green(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::green, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string yellow(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::yellow, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string blue(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::blue, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string magenta(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::magenta, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string cyan(const S &fmt_string, Args... args) {
    return color(fmt::terminal_color::cyan, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string gray(const S &fmt_string, Args... args) {
    return color(fmt::color::gray, fmt_string, std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string bold(const S &fmt_string, Args... args) {
    return style(fmt::emphasis::bold, fmt_string, std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string italic(const S &fmt_string, Args... args) {
    return style(fmt::emphasis::italic, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string underline(const S &fmt_string, Args... args) {
    return style(fmt::emphasis::underline, fmt_string,
                 std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string strikethrough(const S &fmt_string, Args... args) {
    return style(fmt::emphasis::strikethrough, fmt_string,
                 std::forward<Args>(args)...);
  }

  template <typename S, typename... Args>
  static std::string redBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::red, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string blackBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::black, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string greenBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::green, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string yellowBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::yellow, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string blueBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::blue, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string magentaBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::magenta, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string cyanBg(const S &fmt_string, Args... args) {
    return bg(fmt::terminal_color::cyan, fmt_string,
              std::forward<Args>(args)...);
  }
  template <typename S, typename... Args>
  static std::string grayBg(const S &fmt_string, Args... args) {
    return bg(fmt::color::gray, fmt_string, std::forward<Args>(args)...);
  }

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
private:
  const std::string FORMAT_ALIAS = "{: ^8}";
  const std::string FORMAT = "{} ≫ {}[{}] ⊸\t{{}}\n";
  const std::string FORMAT_START = "{} ≫ {}[{}] ⊷\t{}\n";
  const std::string FORMAT_STOP = "{} ≫ {}[{}] ⊶\t{}{}\n";

  std::map<std::string, std::string> _aliases;
  float threshold = 50;
  int static_offset = 0;
  std::map<std::string, std::chrono::time_point<std::chrono::system_clock>>
      _start;

  std::string getName(std::string label) {
    auto pName = name;
    if (_aliases.find(label) != _aliases.end()) {
      pName = _aliases[label];
    }
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, pName);
    return alias;
  }

public:
  std::string getOffset(int d = 0) {
    std::string offset = "";
    if (parent != nullptr) {
      offset = parent->getOffset();
    }
    for (auto n = 0; n < d; n++) {
      offset += fmt::format(fmt::fg(color), "│ ");
    }
    if (_start.size() == 0) {
      return offset;
    }
    for (auto n = 0; n < _start.size(); n++) {
      offset += fmt::format(fmt::fg(color), "│ ");
    }
    return offset;
  }

  static Logger &getInstance() {
    static Logger instance("ROOT");
    return instance;
  }

  Logger(std::string n) : name(n) {}
  Logger(fmt::internal::color_type c, std::string n) : name(n), color(c) {}
  // Logger(Logger const &) = delete;
  // void operator=(Logger const &) = delete;

public:
  bool muted = false;
  std::string name;
  fmt::internal::color_type color;
  Logger *parent = nullptr;

  template <typename... Args>
  void print(std::string level, const Args &... args) {
    if (muted)
      return;
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);

    auto fmt_string =
        fmt::format(FORMAT, alias, getOffset(static_offset), level);
    fmt::print(fmt_string, std::forward<const Args &>(args)...);
  }

  template <typename... Args> void warn(Args... args) {
    print(utils::bold(utils::grayBg(utils::yellow("WARN"))),
          std::forward<const Args &>(args)...);
  }
  template <typename... Args> void info(Args... args) {
    print(utils::bold(utils::blue("INFO")),
          std::forward<const Args &>(args)...);
  }
  template <typename... Args> void error(Args... args) {
    print(utils::bold(utils::redBg("!ERR")), std::forward<const Args &>(args)...);
  }
  template <typename... Args> void debug(Args... args) {
    print(utils::gray(utils::italic("_dbg")),
          std::forward<const Args &>(args)...);
  }
  void start(std::string label, bool silent = false) {
    auto offset = getOffset(static_offset);
    _start[label] = std::chrono::system_clock::now();
    if (silent)
      return;
    if (muted)
      return;
    fmt::print(FORMAT_START, getName(label), offset, label,
               utils::yellow("start"));
  }
  void stop(std::string label, float b = 0) { stop(label, label, b); }

  void stop(std::string label, std::string msg, float b = 0) {
    if (_start.find(label) == _start.end()) {
      warn("label '{}' not found", label);
      return;
    }
    auto start = _start.at(label);
    milliseconds ms = std::chrono::system_clock::now() - start;
    _start.erase(label);
    if (ms.count() < b) {
      return;
    }
    auto time = utils::green("{}", ms.count());
    if (ms.count() > threshold) {
      time = utils::style(fmt::emphasis::bold,
                          utils::redBg("{}", ms.count()));
    }
    auto offset = getOffset(static_offset);
    _start.erase(label);
    _aliases.erase(label);
    if (muted)
      return;
    fmt::print(FORMAT_STOP, getName(label), offset, msg, time,
               utils::yellow("ms"));
  }

  void setParent(Logger *p) { parent = p; }
  void setThreshold(float t) { threshold = t; }
  void setOffset(int o) { static_offset = o; }

  void mute(){muted = true;}
  void unmute(){muted = false;}
  void setMuted(bool m){muted = m;}
};
} // namespace LibLog
#endif // __LOGGER_H_
