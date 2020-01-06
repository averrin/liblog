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

    fmt::print("\n{:*^80}\n\n", utils::bold("MUTE"));
    L.mute();
    L.error("muted logger");
    L.unmute();
    L.error("unmuted logger");
    L.setMuted(false);

    fmt::print("\n{:*^80}\n\n", utils::bold("ADVANCED"));
    auto OFF = Logger(fmt::color::gray, "OFFSET");
    OFF.setOffset(3);
    L.info("line from root logger");
    OFF.info("line with static offset");

    L.print(utils::greenBg(utils::red("CUSTOM")), fmt::format("2+2={}", utils::blue("{}", 4)));

    L.start("first");
    L.info("1st started");
    L.start("second");
    L.info("2nd started");
    L.stop("second");
    L.stop("first", "stop label suppresed by message");

    auto label = "longer than 50ms (no border & default threshold)";
    L.start(label);
        L.info("when threshold is default, start message prints as usually, but time at stop message is highlighted");
        std::this_thread::sleep_for(std::chrono::milliseconds(60));
    L.stop(label);

    label = "not longer than 30ms (custom border & threshold)";
    L.start(label, true); //start message suppressed by second argument
        std::this_thread::sleep_for(std::chrono::milliseconds(20));
    L.stop(label, 30);

    L.setThreshold(10); //highlight time over 10
    L.start(label, true); //start message suppressed by second argument
        L.info("start message suppressed, and stop message prints only if threshold exeeded");
        std::this_thread::sleep_for(std::chrono::milliseconds(40));
    L.stop(label, 30);
};
