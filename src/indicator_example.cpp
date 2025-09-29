#include <chrono>
#include <string>
#include <thread>
#include <vector>

#include <indicators/block_progress_bar.hpp>
#include <indicators/cursor_control.hpp>
#include <indicators/progress_bar.hpp>

namespace opt = indicators::option;

void ProgressBar1() {
    indicators::show_console_cursor(false);

    indicators::ProgressBar bar{
        indicators::option::BarWidth{50},
        opt::Start{"["},
        opt::Fill{"="},
        opt::Lead{">"},
        opt::Remainder{" "},
        opt::End{"]"},
        opt::PrefixText{"Progress "},
        opt::PostfixText{"Loading dependency 1/4"}
    };

    bar.set_progress(10); // 10% done

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    bar.set_option(opt::PostfixText{"Loading dependency 2/4"});
    bar.set_progress(30); // 30% done

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    bar.set_option(opt::PostfixText{"Loading dependency 3/4"});
    bar.set_progress(65); // 65% done

    std::this_thread::sleep_for(std::chrono::milliseconds(300));

    bar.set_option(opt::PostfixText{"Loaded dependencies!"});
    bar.set_progress(100); // all done

    indicators::show_console_cursor(true);
}

void ProgressBar2() {
    indicators::show_console_cursor(false);

    constexpr auto kSize = 500;

    std::string s = "Progress ";
    indicators::ProgressBar bar{
        opt::BarWidth{50},
        opt::Start{"["},
        opt::Fill{"="},
        opt::Lead{">"},
        opt::Remainder{" "},
        opt::End{"]"},
        opt::PrefixText{s},
        opt::ForegroundColor{indicators::Color::cyan},
        opt::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
        opt::MaxProgress{kSize}
    };

    for (size_t i = 0; i < kSize; ++i) {
        std::this_thread::sleep_for(std::chrono::milliseconds(2));
        bar.set_option(opt::PostfixText{std::to_string(i + 1) + "/" + std::to_string(kSize)});
        bar.tick();
    }

    indicators::show_console_cursor(true);
}

void ProgressBar3() {
    indicators::show_console_cursor(false);

    constexpr auto kSize = 1;

    std::string s = "Progress ";
    indicators::ProgressBar bar{
        opt::BarWidth{50},
        opt::Start{"["},
        opt::Fill{"="},
        opt::Lead{">"},
        opt::Remainder{" "},
        opt::End{"]"},
        opt::PrefixText{s},
        opt::ForegroundColor{indicators::Color::cyan},
        opt::FontStyles{std::vector<indicators::FontStyle>{indicators::FontStyle::bold}},
        opt::MaxProgress{kSize}
    };

    bar.set_progress(0); // 10% done
    for (size_t i = 0; i < kSize; ++i) {
        bar.set_option(opt::PostfixText{std::to_string(i + 1) + "/" + std::to_string(kSize)});
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        bar.tick();
    }

    indicators::show_console_cursor(true);
}

int main() {
    ProgressBar1();
    ProgressBar2();
    ProgressBar3();
    return 0;
}
