#include "liblog.hpp"
#include <string>
#include <thread>

using namespace LibLog;
// using namespace std::chrono;

void general() {
  auto L = Logger::getInstance();
  L.h1("GENERAL");
  auto CHLD = Logger(fmt::color::aqua, "CHLD");
  CHLD.setParent(&L);
  auto sign = Logger(fmt::color::purple, "@");
  sign.setParent(&CHLD);
  L.start("test");
  L.info("INFO");
  CHLD.info("child info");
  L.warn("TEST");
  CHLD.warn("child warn");
  L.start("errors");
  L.error("ERROR");
  CHLD.start("colored offsets");
  CHLD.warn("ERROR");
  auto at_label = utils::color(fmt::color::purple, "@");
  sign.start(at_label);
  sign.info("deep inside");
  sign.stop(at_label);
  CHLD.stop("colored offsets");
  L.stop("errors");
  L.debug("debug message");
  L.stop("test");
}

void formatting() {
  auto L = Logger::getInstance();
  L.h1("FORMATTING");
  fmt::print("        {}\n", utils::bold("bold string"));
  fmt::print("        {}\n", utils::italic("italic string"));
  fmt::print("        {}\n", utils::underline("underline string"));
  fmt::print("        {}\n", utils::strikethrough("strikethrough string"));

  fmt::print("\n");
  fmt::print("colored rgb: {}, terminal: {}\n",
             utils::color(fmt::rgb(0, 255, 0), "green"), utils::green("green"));
  fmt::print("nested       {}\n",
             utils::underline("{}+{}={}", utils::red("{}", 2),
                              utils::blue("{}", 2), utils::green("{}", 2 + 2)));

  fmt::print(
      "nested (fmt) {}",
      fmt::format(
          fmt::emphasis::underline, "{}+{}={}",
          fmt::format(fmt::fg(fmt::terminal_color::red), "{}", 2),
          fmt::format(fmt::fg(fmt::terminal_color::blue), "{}", 2),
          fmt::format(fmt::fg(fmt::terminal_color::green), "{}", 2 + 2)));
  fmt::print(
      utils::gray("  //same styling bug in original fmtlib implementation\n"));

  fmt::print("\n");
  L.info("formatted info: {} {}", utils::red("should work"), 1);
  L.warn("formatted warn: {} {}", utils::green("should work"), 2);
  L.error("formatted error: {} {}", utils::blue("should work"), 3);
  L.debug("formatted debug: {} {}", utils::yellow("should work"), 4);
}

void mute() {
  auto L = Logger::getInstance();
  L.h1("MUTE");
  L.mute();
  L.error("muted logger");
  L.unmute();
  L.error("unmuted logger");
  L.setMuted(false);
}

void advanced() {

  auto L = Logger::getInstance();
  L.h1("ADVANCED");
  auto OFF = Logger(fmt::color::gray, "OFFSET");
  OFF.setOffset(3);
  L.debug("line from root logger");
  OFF.debug("line with static offset");

  fmt::print("\n");
  L.print(utils::greenBg(utils::red("CUSTOM")),
          fmt::format("2+2={}", utils::blue("{}", 4)));

  fmt::print("\n");
  L.setColor(fmt::color::gold);
  auto colored_label = utils::color(fmt::color::violet, "colored label");
  L.setLabelColor(colored_label, fmt::color::violet);
  L.start("global logger color");
  L.debug("L.setColor(fmt::color::gold);");
  L.start(colored_label);
  L.debug("L.setLabelColor(\"colored label\", fmt::color::violet);");
  L.stop(colored_label);
  L.stop("global logger color", "stop label suppressed by message");
  L.resetColor();

  fmt::print("\n");
  auto label = "longer than 50ms (no border & default threshold)";
  L.start(label);
  L.debug("when threshold is default, start message prints as usually, but "
          "time at stop message is highlighted");
  std::this_thread::sleep_for(std::chrono::milliseconds(60));
  L.stop(label);

  label = "not longer than 30ms (custom border & threshold)";
  L.start(label, true); // start message suppressed by second argument
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  L.stop(label, 30);

  fmt::print("\n");
  L.setThreshold(10);   // highlight time over 10
  L.start(label, true); // start message suppressed by second argument
  L.debug("start message suppressed, and stop message prints only if threshold "
          "exeeded");
  L.mark("20ms mark (hidden by border)", 20);
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  L.mark("20ms mark");
  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  L.stop(label, 30);
}

void rules() {
  auto L = Logger::getInstance();
  L.h1("RULES");
  fmt::print(L.rule(57));
  auto l = 50;
  for (int n = 0; n < 6; n++) {
    fmt::print("{:>5}% ", n * 20);
    fmt::print(L.stacked(n * 10, l - n * 10, fmt::color::orange));
  }
  fmt::print(L.rule(57));
}

void progress() {
  auto L = Logger::getInstance();
  auto CHLD = Logger(fmt::color::aqua, "CHLD");
  CHLD.setParent(&L);

  L.h1("PROGRESS");
  auto v = 37;
  v = 0;
  auto delay = 800ms;
  L.setColor(fmt::color::lime);

  L.h2("Spinner");
  L.info("log before spinner");
  L.busy(utils::blue("Spinner demo"), fmt::color::light_blue, v);
  std::this_thread::sleep_for(delay);
  L.mark(utils::green("MARK"));
  std::this_thread::sleep_for(delay);
  CHLD.debug("child log during spinner");
  std::this_thread::sleep_for(delay);
  L.release();
  L.info("log after spinner");

  L.h2("Progressbar");
  L.progress(utils::blue("Progress demo"), fmt::color::light_coral);
  std::this_thread::sleep_for(delay);
  L.mark(utils::green("MARK"));
  CHLD.debug("child log during progress");
  for (auto i = 0; i <= 20; i++) {
    L.update(i * 0.05f);
    std::this_thread::sleep_for(delay / 4);
  }

  L.h2("Alt Progressbar");
  L.progressbar("Fullsize Progressbar", fmt::color::plum);
  for (auto i = 0; i <= 20; i++) {
    L.update(i * 0.05f);
    std::this_thread::sleep_for(delay / 4);
  }
  L.progressbar("Interrupted Progressbar", fmt::color::dark_red);
  std::this_thread::sleep_for(delay / 4);
  L.update(0.45f);
  std::this_thread::sleep_for(delay / 4);
  L.release();

  L.h2("Other");
  auto frames = IndicatorFrames[36];
  for (auto frame : frames) {
    L.print(frame, "or just use indicator frame as label");
  }
}

void markup() {
  auto L = Logger::getInstance();
  L.h1("MARKUP");
  L.h2("Style");
  fmt::print(utils::parse(
      "<b>bold</b> <u>underline</u> <i>italic</i> <s>strike</s>\n"));
  L.h2("Terminal colors");
  fmt::print(utils::parse(
      "<red>red</red> <black>black</black> "
      "<green>green</green> <blue>blue</blue> <yellow>yellow</yellow> "
      "<cyan>cyan</cyan> <gray>gray</gray>\n"));
  fmt::print("<b><red>red</red> <black>black</black> "
             "<green>green</green> <blue>blue</blue> <yellow>yellow</yellow> "
             "<cyan>cyan</cyan> <gray>gray</gray></b>\n"_p);

  L.h2("Hex colors");
  fmt::print(
      "<b><color=#11ff11>#11ff11</color> "
      "<color=#ff1111>#ff1111</color></b> <color=#aa11bb>#aa11bb</color>\n"_p);
  fmt::print("<b><bgcolor=#11ff11>#11ff11</bgcolor> "
             "<bgcolor=#ff1111>#ff1111</bgcolor></b> "
             "<bgcolor=#aa11bb>#aa11bb</bgcolor>\n"_p);

  fmt::print("<b><bgcolor=#eeeeee>"
             "<color=#222222> black on white </color></bgcolor></b>"
             " <b><bgcolor=#11ff11>"
             "<color=#ff1111> red on green </color></bgcolor></b>\n"_p);

  L.h2("Other");
  fmt::print("functions (no nesting):   ");
  fmt::print(utils::bold("{} {}\n", utils::green("green_bold"),
                         utils::red("red_bold")));
  fmt::print("fmt (no nesting):         ");
  fmt::print(fmt::format(
      fmt::emphasis::bold,
      fmt::format(
          "{} {}\n",
          fmt::format(fmt::fg(fmt::terminal_color::green), "green_bold"),
          fmt::format(fmt::fg(fmt::terminal_color::red), "red_bold"))));
  fmt::print("markup (nesting support): <b><green>green_bold</green> "
             "<red>red_bold</red></b>\n"_p);
  fmt::print("\n");
};

int main() {
  auto L = Logger::getInstance();
  general();
  formatting();
  advanced();
  rules();
  // progress();
  markup();
}
