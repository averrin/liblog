#include "liblog.hpp"
#include <string>

int main() {
    Logger& L = Logger::getInstance();
    L.start("test");
    L.info("INFO");
    L.info("CHLD", "child info");
    L.warn("TEST");
    L.warn("CHLD", "child warn");
    L.start("errors");
        L.error("ERROR");
        L.error("CHLD", "child error");
    L.stop("errors");
    L.stop("test");
}
