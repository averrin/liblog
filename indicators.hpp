/*
 *   cpp-indicators - iAmir (Amir Ahmady) (C) 2020
 *       file name: indicators.hpp
 */

#ifndef __CPP_INDICATORS_
#define __CPP_INDICATORS_

#include <chrono>
#include <cmath>
#include <fmt/color.h>
#include <fmt/format.h>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>
#include <vector>

namespace LibLog {

using namespace std::chrono;

std::vector<std::vector<std::string>> IndicatorFrames = {
    {" o    ", "  o   ", "   o  ", "    o ", "     o", "    o ", "   o  ",
     "  o   ", " o    "},
    {"←", "↖", "↑", "↗", "→", "↘", "↓", "↙"},
    {"▁", "▃", "▄", "▅", "▆", "▇", "█", "▇", "▆", "▅", "▄", "▃", "▁"},
    {"▖", "▘", "▝", "▗"},
    {"┤", "┘", "┴", "└", "├", "┌", "┬", "┐"},
    {"◢", "◣", "◤", "◥"},
    {"◰", "◳", "◲", "◱"},
    {"◴", "◷", "◶", "◵"},
    {"◐", "◓", "◑", "◒"},
    {".", "o", "O", "@", "*"},
    {"|", "/", "-", "\\"},
    {"◡◡", "⊙⊙", "◠◠"},
    {"⣾", "⣽", "⣻", "⢿", "⡿", "⣟", "⣯", "⣷"},
    {">))'>", " >))'>", "  >))'>", "   >))'>", "    >))'>", "   <'((<",
     "  <'((<", " <'((<"},
    {"⠁", "⠂", "⠄", "⡀", "⢀", "⠠", "⠐", "⠈"},
    {"⠋", "⠙", "⠹", "⠸", "⠼", "⠴", "⠦", "⠧", "⠇", "⠏"},
    {"a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m",
     "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z"},
    {"▉", "▊", "▋", "▌", "▍", "▎", "▏", "▎", "▍", "▌", "▋", "▊", "▉"},
    {"■", "□", "▪", "▫"},
    {"←", "↑", "→", "↓"},
    {"╫", "╪"},
    {"⇐", "⇖", "⇑", "⇗", "⇒", "⇘", "⇓", "⇙"},
    {"⠁", "⠁", "⠉", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠤", "⠄", "⠄", "⠤",
     "⠠", "⠠", "⠤", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒", "⠓", "⠋", "⠉", "⠈", "⠈"},
    {"⠈", "⠉", "⠋", "⠓", "⠒", "⠐", "⠐", "⠒", "⠖", "⠦", "⠤", "⠠",
     "⠠", "⠤", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒", "⠓", "⠋", "⠉", "⠈"},
    {"⠁", "⠉", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠤", "⠄",
     "⠄", "⠤", "⠴", "⠲", "⠒", "⠂", "⠂", "⠒", "⠚", "⠙", "⠉", "⠁"},
    {"⠋", "⠙", "⠚", "⠒", "⠂", "⠂", "⠒", "⠲", "⠴", "⠦", "⠖", "⠒", "⠐", "⠐", "⠒",
     "⠓", "⠋"},
    {"ｦ", "ｧ", "ｨ", "ｩ", "ｪ", "ｫ", "ｬ", "ｭ", "ｮ", "ｯ", "ｱ", "ｲ", "ｳ", "ｴ",
     "ｵ", "ｶ", "ｷ", "ｸ", "ｹ", "ｺ", "ｻ", "ｼ", "ｽ", "ｾ", "ｿ", "ﾀ", "ﾁ", "ﾂ",
     "ﾃ", "ﾄ", "ﾅ", "ﾆ", "ﾇ", "ﾈ", "ﾉ", "ﾊ", "ﾋ", "ﾌ", "ﾍ", "ﾎ", "ﾏ", "ﾐ",
     "ﾑ", "ﾒ", "ﾓ", "ﾔ", "ﾕ", "ﾖ", "ﾗ", "ﾘ", "ﾙ", "ﾚ", "ﾛ", "ﾜ", "ﾝ"},
    {".", "..", "..."},
    {"▁", "▂", "▃", "▄", "▅", "▆", "▇", "█", "▉", "▊", "▋", "▌", "▍", "▎", "▏",
     "▏", "▎", "▍", "▌", "▋", "▊", "▉", "█", "▇", "▆", "▅", "▄", "▃", "▂", "▁"},
    {".", "o", "O", "°", "O", "o", "."},
    {"|", "/", "-", "\\"},
    {"+", "x"},
    {"v", "<", "^", ">"},
    {"|       ", "||      ", "|||     ", "||||    ", "|||||   ", "||||||| ",
     "||||||||", "||||||| ", "||||||  ", "|||||   ", "||||    ", "|||     ",
     "||      ", "|       "},
    {"[          ]", "[=         ]", "[==        ]", "[===       ]",
     "[====      ]", "[=====     ]", "[======    ]", "[=======   ]",
     "[========  ]", "[========= ]", "[==========]"},
    {"(*---------)", "(-*--------)", "(--*-------)", "(---*------)",
     "(----*-----)", "(-----*----)", "(------*---)", "(-------*--)",
     "(--------*-)", "(---------*)"},
    {"█▒▒▒▒▒▒▒▒▒", "███▒▒▒▒▒▒▒", "█████▒▒▒▒▒", "███████▒▒▒", "██████████"},
    {"🌍", "🌎", "🌏"},
    {"◜", "◝", "◞", "◟"},
    {"⬒", "⬔", "⬓", "⬕"},
    {"⬖", "⬘", "⬗", "⬙"},
    {"♠", "♣", "♥", "♦"},
    {"➞", "➟", "➠", "➡", "➠", "➟"},
    {"  |  ", " \\   ", "_    ", " \\   ", "  |  ", "   / ", "    _", "   / "},
    {"  . . . .", ".   . . .", ". .   . .", ". . .   .", ". . . .  ",
     ". . . . ."},
    {" |     ", "  /    ", "   _   ", "    \\  ", "     | ", "    \\  ",
     "   _   ", "  /    "},
    {"⎺", "⎻", "⎼", "⎽", "⎼", "⎻"},
    {"▹▹▹▹▹", "▸▹▹▹▹", "▹▸▹▹▹", "▹▹▸▹▹", "▹▹▹▸▹", "▹▹▹▹▸"},
    {"[    ]", "[   =]", "[  ==]", "[ ===]", "[====]", "[=== ]", "[==  ]",
     "[=   ]"},
    {"( ●    )", "(  ●   )", "(   ●  )", "(    ● )", "(     ●)", "(    ● )",
     "(   ●  )", "(  ●   )", "( ●    )"},
    {"✶", "✸", "✹", "✺", "✹", "✷"},
    {"?", "¿"},
    {"⢹", "⢺", "⢼", "⣸", "⣇", "⡧", "⡗", "⡏"},
    {"⢄", "⢂", "⢁", "⡁", "⡈", "⡐", "⡠"},
    {".  ", ".. ", "...", " ..", "  .", "   "},
    {".", "o", "O", "°", "O", "o", "."},
    {"▓", "▒", "░"},
    {"▌", "▀", "▐", "▄"},
    {"⊶", "⊷"},
    {"▪", "▫"},
    {"□", "■"},
    {"▮", "▯"},
    {"-", "=", "≡"},
    {"d", "q", "p", "b"},
    {"∙∙∙", "●∙∙", "∙●∙", "∙∙●", "∙∙∙"},
    {"🌑 ", "🌒 ", "🌓 ", "🌔 ", "🌕 ", "🌖 ", "🌗 ", "🌘 "},
    {"☗", "☖"},
    {"⧇", "⧆"},
    {"◉", "◎"},
    {"㊂", "㊀", "㊁"},
    {"⦾", "⦿"},
    {"ဝ", "၀"},
    {"▌", "▀", "▐▄"}};

class Indicator {
public:
  Indicator(const duration<int, std::milli> &delay = 200ms, int type = 0,
            const std::string &endmsg = "", const std::string &prefix = "",
            const std::string &suffix = "",
            fmt::detail::color_type color = fmt::terminal_color::white,
            bool hide_on_end = false, bool is_progress = false,
            const std::vector<std::string> &custom_frames = {})
      : frame_delay(delay), type(type), endmsg(endmsg), prefix(prefix),
        suffix(suffix), hide_on_end(hide_on_end), custom_frames(custom_frames),
        is_active(false), color(color), is_progress(is_progress) {}

  fmt::detail::color_type color;
  bool is_progress = false;
  float progress = 0;
  ~Indicator() { stop(); }

  void start() {
    is_active = true;
    std::cout.flush();
    last_text_size = 0;
    if (type >= IndicatorFrames.size())
      type = 0;

    thread = std::thread([&]() {
      const std::vector<std::string> &frames =
          (custom_frames.size() > 0) ? custom_frames : IndicatorFrames[type];
      int l = frames.size();
      progress = 0;

      while (is_active) {
        unsigned int n = 0;
        if (is_progress) {
          n = round((l - 1) * progress);
        } else {
          n = (int)progress % l;
        }
        const std::string &frame = fmt::format(fmt::fg(color), frames[n]);
        if (!is_progress) {
          progress++;
        }
        std::cout << "\r" << prefix << frame << suffix;
        last_text_size = prefix.length() + frame.length() + suffix.length();
        std::cout.flush();
        std::this_thread::sleep_for(frame_delay);
      }
    });
  }

  void clear() {
    std::cout << "\r" << std::string(last_text_size, ' ');
    std::cout << "\r";
  }

  void stop() {
    if (is_active) {
      is_active = false;
      thread.join();
      if (endmsg.size() > 0) {
        if (hide_on_end) {
          std::cout << "\r" << std::string(last_text_size, ' ');
          std::cout << "\r" << endmsg;
        } else {
          std::cout << "\n" << endmsg;
        }
      }
      std::cout.flush();
    }
  }

  void set_frame_delay(const duration<int, std::milli> &delay) {
    frame_delay = delay;
  }

  void set_prefix(const std::string &string) { prefix = string; }

  void set_suffix(const std::string &string) { suffix = string; }

  void set_end_msg(const std::string &string) { endmsg = string; }

  bool is_active;

private:
  duration<int, std::milli> frame_delay;
  std::string prefix;
  std::string suffix;
  std::vector<std::string> custom_frames;
  unsigned char type;
  std::string endmsg;
  unsigned int last_text_size;
  bool hide_on_end;
  std::thread thread;
};
} // namespace LibLog

#endif
