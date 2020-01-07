#include "liblog.hpp"
#include <string>
#include <thread>

using namespace LibLog;

int main() {
    fmt::print("\n{:*^80}\n\n", utils::bold("GENERAL"));
    auto L = Logger::getInstance();
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

    fmt::print("\n{:*^80}\n\n", utils::bold("FORMATTING"));
    fmt::print("        {}\n", utils::bold("bold string"));
    fmt::print("        {}\n", utils::italic("italic string"));
    fmt::print("        {}\n", utils::underline("underline string"));
    fmt::print("        {}\n", utils::strikethrough("strikethrough string"));

    fmt::print("\n");
    fmt::print("colored rgb: {}, terminal: {}\n", utils::color(fmt::rgb(0,255,0), "green"), utils::green("green"));
    fmt::print("nested       {}\n",
               utils::underline("{}+{}={}",
                           utils::red("{}", 2),
                           utils::blue("{}", 2),
                           utils::green("{}", 2+2)));

    fmt::print("nested (fmt) {}",
               fmt::format(fmt::emphasis::underline, "{}+{}={}",
                           fmt::format(fmt::fg(fmt::terminal_color::red), "{}", 2),
                           fmt::format(fmt::fg(fmt::terminal_color::blue), "{}", 2),
                           fmt::format(fmt::fg(fmt::terminal_color::green), "{}", 2+2)));
    fmt::print(utils::gray("  //same styling bug in original fmtlib implementation\n"));

    fmt::print("\n");
    L.info("formatted info: {} {}", utils::red("should work"), 1);
    L.warn("formatted warn: {} {}", utils::green("should work"), 2);
    L.error("formatted error: {} {}", utils::blue("should work"), 3);
    L.debug("formatted debug: {} {}", utils::yellow("should work"), 4);

    fmt::print("\n{:*^80}\n\n", utils::bold("MUTE"));
    L.mute();
    L.error("muted logger");
    L.unmute();
    L.error("unmuted logger");
    L.setMuted(false);

    fmt::print("\n{:*^80}\n\n", utils::bold("ADVANCED"));
    auto OFF = Logger(fmt::color::gray, "OFFSET");
    OFF.setOffset(3);
    L.debug("line from root logger");
    OFF.debug("line with static offset");

    fmt::print("\n");
    L.print(utils::greenBg(utils::red("CUSTOM")), fmt::format("2+2={}", utils::blue("{}", 4)));

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
        L.debug("when threshold is default, start message prints as usually, but time at stop message is highlighted");
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    L.stop(label);

    label = "not longer than 30ms (custom border & threshold)";
    L.start(label, true); //start message suppressed by second argument
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    L.stop(label, 30);

    fmt::print("\n");
    L.setThreshold(10); //highlight time over 10
    L.start(label, true); //start message suppressed by second argument
        L.debug("start message suppressed, and stop message prints only if threshold exeeded");
        L.mark("20ms mark (hidden by border)", 20);
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
        L.mark("20ms mark");
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    L.stop(label, 30);

};
