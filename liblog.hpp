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
  const std::string OFFSET_START =         "┏";
  const std::string OFFSET       =         "┃ ";
  const std::string OFFSET_END   =         "┗";
  const std::string OFFSET_END_SILENT   =  "╼━";
  const std::string FORMAT_STOP  = "{} ≫ {}[{}] ⊶\t{}{}\n";
  const std::string FORMAT_MARK = "{} ≫ {}[{}] ⊙\t{}{}\n";

  float threshold = 50;
  int static_offset = 0;
  std::map<std::string, std::chrono::time_point<std::chrono::system_clock>>
      _start;

  std::string getName(std::string label) {
    auto pName = name;
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, pName);
    return alias;
  }

  std::vector<std::string> _labels;
  std::map<std::string, fmt::internal::color_type> label_colors;
    fmt::internal::color_type getLabelColor(std::string label) {
      auto c = color;
      if (label_colors.find(label) != label_colors.end()) {
        return label_colors.at(label);
      }
      return c;
    }

public:
  std::string getOffset(int d = 0) {
    std::string offset = "";
    if (parent != nullptr) {
      offset = parent->getOffset();
    }
    for (auto n = 0; n < d; n++) {
      offset += fmt::format(fmt::fg(color), OFFSET);
    }
    if (_start.size() == 0) {
      return offset;
    }
    for (auto l : _labels) {
      offset += fmt::format(fmt::fg(getLabelColor(l)), OFFSET);
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
  std::string name = "ROOT";
  fmt::internal::color_type color;
  Logger *parent = nullptr;

  template <typename... Args>
  void print(std::string level, std::string msg_format, const Args &... args) {
    if (muted)
      return;
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);

    auto fmt_string =
        fmt::format(FORMAT, alias, getOffset(static_offset), level);
    auto msg = fmt::format(msg_format, std::forward<const Args &>(args)...);
    fmt::print(fmt_string, msg);
  }

  template <typename... Args> void warn(std::string msg_format, Args... args) {
    print(utils::bold(utils::grayBg(utils::yellow("WARN"))), msg_format,
          std::forward<const Args &>(args)...);
  }

  template <typename... Args>
  void info(std::string msg_format, const Args &... args) {
    print(utils::bold(utils::blue("INFO")), msg_format,
          std::forward<const Args &>(args)...);
  }

  template <typename... Args> void error(std::string msg_format, Args... args) {
    print(utils::bold(utils::redBg("!ERR")), msg_format,
          std::forward<const Args &>(args)...);
  }

  template <typename... Args> void debug(std::string msg_format, Args... args) {
    print(utils::gray(utils::italic("_dbg")), msg_format,
          std::forward<const Args &>(args)...);
  }

  void start(std::string label, bool silent = false) {
    auto offset = getOffset(static_offset);
    if (silent) {
      label = "#"+label;
    }
    _start[label] = std::chrono::system_clock::now();
    _labels.push_back(label);
    if (silent)
      return;
    if (muted)
      return;
    offset += fmt::format(fmt::fg(getLabelColor(label)), OFFSET_START);
    fmt::print(FORMAT_START, getName(label), offset, label,
               utils::yellow("start"));
  }

  void stop(std::string label, float b = 0) { stop(label, label, b); }

  void stop(std::string label, std::string msg, float b = 0) {
    auto silent = false;
    if (_start.find("#"+label) != _start.end()) {
      silent = true;
      label = "#" + label;
    } else
    if (_start.find(label) == _start.end()) {
      warn("label '{}' not found", label);
      return;
    }
    auto start = _start.at(label);
    milliseconds ms = std::chrono::system_clock::now() - start;
    _start.erase(label);
    _labels.erase(std::remove(_labels.begin(), _labels.end(), label), _labels.end());
    if (ms.count() < b) {
      return;
    }
    auto time = utils::green("{}", ms.count());
    if (ms.count() > threshold) {
      time = utils::style(fmt::emphasis::bold, utils::redBg("{}", ms.count()));
    }
    auto offset = getOffset(static_offset);
    _start.erase(label);
    if (muted)
      return;
    if (!silent) {
      offset += fmt::format(fmt::fg(getLabelColor(label)), OFFSET_END);
    } else {
      offset += fmt::format(fmt::fg(getLabelColor(label)), OFFSET_END_SILENT);
    }
    fmt::print(FORMAT_STOP, getName(label), offset, msg, time,
               utils::yellow("ms"));
  }

  void mark(std::string msg, float b = 0) {
    auto label = _labels.back();
    auto start = _start[label];
    milliseconds ms = std::chrono::system_clock::now() - start;
    if (ms.count() < b) {
      return;
    }
    auto time = utils::green("{}", ms.count());
    if (ms.count() > threshold) {
      time = utils::style(fmt::emphasis::bold, utils::redBg("{}", ms.count()));
    }
    auto offset = getOffset(static_offset);
    if (muted)
      return;
    fmt::print(FORMAT_MARK, getName(label), offset, msg, time, utils::yellow("ms"));
  }

  void setParent(Logger *p) { parent = p; }
  void setThreshold(float t) { threshold = t; }
  void setOffset(int o) { static_offset = o; }
  void setColor(fmt::internal::color_type c) { color = c; }
  void resetColor() { color = fmt::internal::color_type{}; }

  void mute() { muted = true; }
  void unmute() { muted = false; }
  void setMuted(bool m) { muted = m; }

  void setLabelColor(std::string label, fmt::internal::color_type c) { label_colors[label] = c; }
};
} // namespace LibLog
#endif // __LOGGER_H_
