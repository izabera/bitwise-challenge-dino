#include <cstdlib>
#include <cstring>
#include <string>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include "screen.hpp"

using namespace std::string_literals;
static void writeraw(const std::string& msg) {
    write(1, msg.data(), msg.size());
}


screen::screen() {
    struct winsize win;
    if (ioctl(1, TIOCGWINSZ, &win) == -1) {
        writeraw("not a tty\n");
        exit(1);
    }

    W = win.ws_col - 1;
    H = (win.ws_row - 1) * 2; // skip the final column and line for now

    writeraw("\x1b[?1049h"); // alt screen on
    writeraw("\x1b[?25l");   // cursor off

    system("stty raw -echo");
    fb.resize(W * H);
}

screen::~screen() {
    system("stty sane");
    writeraw("\x1b[?25h");
    writeraw("\x1b[?1049l");
}

void screen::clear() {
    std::fill(fb.begin(), fb.end(), 0);
}

void screen::draw() const {
    const char *blocks[] = { " ", "▄", "▀", "█" };
    std::string tty = "\x1b[H";
    for (auto h = 0; h < H; h+=2) {
        for (auto w = 0; w < W; w++) {
            auto bits = (fb[h*W+w] << 1) | fb[(h+1)*W+w];
            tty += blocks[bits];
        }
        tty += "\r\n";
    }
    writeraw(tty);
}

void screen::debug(const char *msg) {
    auto s = "\x1b[H"s + msg + "\x1b[K";
    writeraw(s);
}

void input::get() {
    w = a = s = d = quit = 0;

    fd_set rfds{};
    FD_SET(0, &rfds);
    struct timeval tv { 0, 1'000'000/FPS };

    while (1) {
        if (select(1, &rfds, NULL, NULL, &tv) <= 0)
            break;

        char buf[100];
        ssize_t r = read(0, buf, sizeof buf);
        if (r <= 0)
            break;

        for (ssize_t b = 0; b < r; b++) {
            switch (buf[b]) {
                case ' ':
                case 'w': case 'W': w = true; break;
                case 'a': case 'A': a = true; break;
                case 's': case 'S': s = true; break;
                case 'd': case 'D': d = true; break;
                case '\3': quit = true; break; // ^C
            }
        }
    }
    tick++;
}
