#ifndef __LOGGER_H_
#define __LOGGER_H_
#include "indicators.hpp"
#include "peglib.h"
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
using milliseconds_t = std::chrono::duration<double, std::milli>;
using namespace peg;
using namespace peg::udl;

namespace LibLog {

class utils {
public:
  static std::string parse(std::string_view text) {
    parser parser(R"(
        ROOT      <- CONTENT
        CONTENT   <- (ELEMENT / TEXT)*
        ELEMENT   <- $(STAG CONTENT ETAG)
        STAG      <- '<' _ $tag<TAG_NAME> _ ARG? _ '>'
        ETAG      <- '</' _ $tag<TAG_NAME> _ '>'
        TAG_NAME  <- ([a-zA-Z])*
        ARG       <- '='$([^>])*
        TEXT      <- TEXT_DATA
        TEXT_DATA <- ![<] .
        ~_        <- [ \t\r\n]*
   )");
    parser.enable_ast();
    parser.log = [](std::size_t line, std::size_t col, const std::string &msg) {
      std::cerr << "Parse error at " << line << ":" << col << " => " << msg
                << "\n";
    };

    std::shared_ptr<Ast> ast;
    std::string content = "";
    if (parser.parse(text, ast)) {
      ast = parser.optimize_ast(ast);
      // fmt::print(ast_to_s(ast));
      for (auto node : ast->nodes) {
        content += process_node(node);
      }
    }

    return content;
  }

  static std::string
  process_node(std::shared_ptr<Ast> node,
               fmt::text_style parent_style = fmt::text_style{}) {
    if (node->is_token && node->name != "TAG_NAME")
      return std::string(node->token);
    std::string content = "";
    if (node->name == "TEXT_DATA") {
      content += node->token;
    } else if (node->name == "ELEMENT") {
      auto tag = node->nodes[0]->token;
      if (tag == "") {
        tag = node->nodes[0]->nodes[0]->token;
      }
      auto style = parent_style;
      if (tag == "b") {
        style |= fmt::emphasis::bold;
      } else if (tag == "u") {
        style |= fmt::emphasis::underline;
      } else if (tag == "i") {
        style |= fmt::emphasis::italic;
      } else if (tag == "s") {
        style |= fmt::emphasis::strikethrough;
      } else if (tag == "red") {
        style |= fmt::fg(fmt::terminal_color::red);
      } else if (tag == "black") {
        style |= fmt::fg(fmt::terminal_color::black);
      } else if (tag == "green") {
        style |= fmt::fg(fmt::terminal_color::green);
      } else if (tag == "yellow") {
        style |= fmt::fg(fmt::terminal_color::yellow);
      } else if (tag == "blue") {
        style |= fmt::fg(fmt::terminal_color::blue);
      } else if (tag == "magenta") {
        style |= fmt::fg(fmt::terminal_color::magenta);
      } else if (tag == "cyan") {
        style |= fmt::fg(fmt::terminal_color::cyan);
      } else if (tag == "gray") {
        style |= fmt::fg(fmt::color::gray);
      } else if (tag == "color" || tag == "bgcolor") {
        auto arg = node->nodes[0]->nodes[1]->token;
        std::stringstream str;
        std::string s1 = std::string(arg).substr(2, arg.size() - 2);
        str << s1;
        uint32_t value;
        str >> std::hex >> value;
        if (tag == "bgcolor") {
          style |= fmt::bg(fmt::rgb(value));
        } else {
          style |= fmt::fg(fmt::rgb(value));
        }
      }
      content += process_node(node->nodes[1], style);
      content = fmt::format(style, content);
      // fmt::print("dbg: <{}>{}</{}>\n", tag, content, node->nodes[2]->token);
    } else if (node->name == "CONTENT") {
      for (auto child : node->nodes) {
        content += process_node(child, parent_style);
      }
    }
    return content;
  }

  template <typename S, typename... Args>
  static std::string color(fmt::detail::color_type c, const S &fmt_string,
                           const Args &... args) {
    return fmt::format(fmt::fg(c), fmt_string,
                       std::forward<const Args &>(args)...);
  }
  template <typename S, typename... Args>
  static std::string bg(fmt::detail::color_type c, const S &fmt_string,
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
}; // namespace LibLog

class Logger {
private:
  const std::string FORMAT_ALIAS = "{: ^8}";
  const std::string FORMAT = "{} ≫ {}[{}] ⊸\t{{}}\n";
  const std::string FORMAT_PROGRESS_FULL = "{} ≫ {}[{}%] ";
  const std::string FORMAT_PROGRESS = "{} ≫ {}[";
  const std::string FORMAT_PROGRESS_END = "] ⎶\t{}";
  const std::string FORMAT_START = "{} ≫ {}[{}] ⊷\t{}\n";
  const std::string OFFSET_START = "┏";
  //⎡⎣⎢⎛⎝⎜
  // {"━━━━━━━━"},
  const std::string OFFSET = "┃ ";
  const std::string OFFSET_END = "┗";
  const std::string OFFSET_START_ASYNC = "┍";
  const std::string OFFSET_ASYNC = "╎ ";
  const std::string OFFSET_END_ASYNC = "┕";
  const std::string OFFSET_END_SILENT = "╼━";
  const std::string FORMAT_STOP = "{} ≫ {}[{}] ⊶\t{}{}\n";
  const std::string FORMAT_MARK = "{} ≫ {}[{}] 🎯\t{}{}\n";

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
  std::map<std::string, fmt::detail::color_type> label_colors;
  fmt::detail::color_type getLabelColor(std::string label) {
    auto c = color;
    if (label_colors.find(label) != label_colors.end()) {
      return label_colors.at(label);
    }
    label.erase(std::remove(label.begin(), label.end(), '#'), label.end());
    if (label_colors.find(label) != label_colors.end()) {
      return label_colors.at(label);
    }
    return c;
  }

public:
  bool async = false;
  std::string getOffset(int d = 0) {
    std::string offset = "";
    if (parent != nullptr) {
      auto pa = parent->async;
      parent->setAsync(async);
      offset = parent->getOffset();
      parent->setAsync(pa);
    }
    for (auto n = 0; n < d; n++) {
      offset += fmt::format(fmt::fg(color), async ? OFFSET_ASYNC : OFFSET);
    }
    if (_start.size() == 0) {
      return offset;
    }
    for (auto l : _labels) {
      offset +=
          fmt::format(fmt::fg(getLabelColor(l)), async ? OFFSET_ASYNC : OFFSET);
    }
    return offset;
  }

  static Logger &getInstance() {
    static Logger instance("ROOT");
    return instance;
  }

  Logger(std::string n) : name(n) {}
  Logger(fmt::detail::color_type c, std::string n) : name(n), color(c) {}
  // Logger(Logger const &) = delete;
  // void operator=(Logger const &) = delete;

public:
  bool muted = false;
  std::string name = "ROOT";
  fmt::detail::color_type color;
  Logger *parent = nullptr;
  Indicator *indicator = nullptr;

  std::string
  rule(int l = 80,
       fmt::detail::color_type rule_color = fmt::terminal_color::white,
       bool end_line = true) {
    return fmt::format(utils::color(
        rule_color, utils::bold("{:━^{}}{}", "", l, end_line ? "\n" : "")));
  }

  std::string
  stacked(int l, int r,
          fmt::detail::color_type l_color = fmt::terminal_color::white,
          fmt::detail::color_type r_color = fmt::color::gray,
          bool end_line = true) {
    return rule(l, l_color, false) + rule(r, r_color, end_line);
  }

  void h1(std::string text) {
    fmt::print("\n{:━^80}\n\n", utils::bold(" {} ", text));
  }

  void h2(std::string text) {
    fmt::print("\n━━{:━<78}\n\n", utils::bold(" {} ", text));
  }

  void busy(std::string label,
            fmt::detail::color_type ind_color = fmt::terminal_color::white,
            int v = 0) {
    if ((indicator != nullptr && indicator->is_active)) {
      return;
    }
    start(label);
    indicator = spinner(label, ind_color, v);
    indicator->start();
  }
  void release() {
    if ((indicator == nullptr || !indicator->is_active)) {
      return;
    }
    indicator->stop();
    indicator = nullptr;
    is_progress_full = false;
    stop(indicator_label);
  }

  void update(float progress) {
    if ((indicator == nullptr || !indicator->is_active)) {
      return;
    }
    indicator->progress = progress;
    auto p = utils::bold("{:>3}", (int)(progress * 100));
    if (progress >= 1) {
      p = utils::green(p);
    } else if (progress >= 0.8) {
      p = utils::yellow(p);
    }
    if (!is_progress_full) {
      indicator->set_suffix(fmt::format(
          FORMAT_PROGRESS_END, fmt::format("{} ({}%)", indicator_label, p)));
    } else {
      auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);
      auto prefix =
          fmt::format(fmt::fg(fmt::terminal_color::white), FORMAT_PROGRESS_FULL,
                      alias, getOffset(static_offset), p);
      indicator->set_prefix(prefix);
    }
    if (progress >= 1) {
      release();
    }
  }

  void progress(std::string label,
                fmt::detail::color_type ind_color = fmt::terminal_color::white,
                int v = 36) {
    if ((indicator != nullptr && indicator->is_active)) {
      return;
    }
    start(label);
    indicator = spinner(label, ind_color, v, true);
    indicator->start();
  }

  std::string indicator_label = "";
  Indicator *
  spinner(std::string label,
          fmt::detail::color_type ind_color = fmt::terminal_color::white,
          int v = 0, bool is_progress = false) {
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);
    auto prefix = fmt::format(fmt::fg(fmt::terminal_color::white),
                              FORMAT_PROGRESS, alias, getOffset(static_offset));
    indicator_label = label;
    auto suffix = fmt::format(FORMAT_PROGRESS_END, label);
    // auto done = format_msg(utils::bold(fmt::format(fmt::fg(ind_color),
    // "Done")), label, FORMAT);
    auto ind = new Indicator(std::chrono::milliseconds(200), v, "", prefix,
                             suffix, ind_color, true, is_progress);
    return ind;
  }

  bool is_progress_full = false;
  void
  progressbar(std::string label,
              fmt::detail::color_type ind_color = fmt::terminal_color::white,
              fmt::detail::color_type dim_color = fmt::color::gray,
              int size = 50) {
    if ((indicator != nullptr && indicator->is_active)) {
      return;
    }
    is_progress_full = true;
    start(label);
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);
    auto prefix =
        fmt::format(fmt::fg(fmt::terminal_color::white), FORMAT_PROGRESS_FULL,
                    alias, getOffset(static_offset), fmt::format("{:>3}", 0));
    indicator_label = label;
    // auto suffix = fmt::format(FORMAT_PROGRESS_END, label);
    auto suffix = "";
    // auto done = format_msg(utils::bold(fmt::format(fmt::fg(ind_color),
    // "Done")), label, FORMAT);
    std::vector<std::string> frames;
    for (int i = 0; i <= size; i++) {
      frames.push_back(stacked(i, size - i, ind_color, dim_color, false));
    }
    indicator = new Indicator(std::chrono::milliseconds(200), 0, "", prefix,
                              suffix, fmt::color::gray, false, true, frames);

    indicator->start();
  }

  template <typename... Args>
  std::string format_msg(std::string level, std::string msg_format,
                         std::string inner_format, const Args &... args) {
    auto alias = fmt::format(fmt::fg(color), FORMAT_ALIAS, name);

    auto fmt_string =
        fmt::format(fmt::fg(fmt::terminal_color::white), inner_format, alias,
                    getOffset(static_offset), level);
    auto msg = fmt::format(fmt::fg(fmt::terminal_color::white), msg_format,
                           std::forward<const Args &>(args)...);
    return fmt::format(fmt_string, msg);
  }

  void clearIndicator() {

    if ((indicator != nullptr && indicator->is_active)) {
      indicator->clear();
    } else if (parent != nullptr && parent->indicator != nullptr &&
               parent->indicator->is_active) {
      parent->indicator->clear();
    }
  }

  template <typename... Args>
  void print(std::string level, std::string msg_format, const Args &... args) {
    if (muted)
      return;
    clearIndicator();
    fmt::print(format_msg(level, msg_format, FORMAT,
                          std::forward<const Args &>(args)...));
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
      label = "#" + label;
    }
    _start[label] = std::chrono::system_clock::now();
    _labels.push_back(label);
    if (silent)
      return;
    if (muted)
      return;
    clearIndicator();
    offset += fmt::format(fmt::fg(getLabelColor(label)),
                          async ? OFFSET_START_ASYNC : OFFSET_START);
    fmt::print(FORMAT_START, getName(label), offset, label,
               utils::yellow("start"));
  }

  std::string formatTime(milliseconds_t ms) {
    auto time = utils::green("{}", ms.count());
    if (ms.count() > threshold) {
      time = utils::bold(utils::redBg("{}", ms.count()));
    }
    return time;
  }

  void stop(std::string label, float b = 0) { stop(label, label, b); }

  void stop(std::string label, std::string msg, float b = 0) {
    auto silent = false;
    if (_start.find("#" + label) != _start.end()) {
      silent = true;
      label = "#" + label;
    } else if (_start.find(label) == _start.end()) {
      warn("label '{}' not found", label);
      return;
    }
    auto start = _start.at(label);
    milliseconds_t ms = std::chrono::system_clock::now() - start;
    _start.erase(label);
    _labels.erase(std::remove(_labels.begin(), _labels.end(), label),
                  _labels.end());
    if (ms.count() < b) {
      return;
    }
    auto time = formatTime(ms);
    auto offset = getOffset(static_offset);
    _start.erase(label);
    if (muted)
      return;
    if (!silent) {
      offset += fmt::format(fmt::fg(getLabelColor(label)),
                            async ? OFFSET_END_ASYNC : OFFSET_END);
    } else {
      offset += fmt::format(fmt::fg(getLabelColor(label)), OFFSET_END_SILENT);
    }
    fmt::print(FORMAT_STOP, getName(label), offset, msg, time,
               utils::yellow("ms"));
  }

  void mark(std::string msg, float b = 0) {
    auto label = _labels.back();
    auto start = _start[label];
    milliseconds_t ms = std::chrono::system_clock::now() - start;
    if (ms.count() < b) {
      return;
    }
    auto time = formatTime(ms);
    auto offset = getOffset(static_offset);
    if (muted)
      return;
    clearIndicator();
    fmt::print(FORMAT_MARK, getName(label), offset, msg, time,
               utils::yellow("ms"));
  }

  void setParent(Logger *p) { parent = p; }
  void setThreshold(float t) { threshold = t; }
  void setOffset(int o) { static_offset = o; }
  void setColor(fmt::detail::color_type c) { color = c; }
  void resetColor() { color = fmt::detail::color_type{}; }

  void mute() { muted = true; }
  void unmute() { muted = false; }
  void setMuted(bool m) { muted = m; }

  void setAsync(bool m) { async = m; }

  void setLabelColor(std::string label, fmt::detail::color_type c) {
    label_colors[label] = c;
  }
}; // namespace LibLog

std::string operator""_p(const char *str, std::size_t len) {
  return utils::parse(str);
}

} // namespace LibLog
#endif // __LOGGER_H_
