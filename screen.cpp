#include <cstdlib>
#include <cstring>
#include <fstream>
#include <string>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>
#include "screen.hpp"

static void writeraw(const char *msg) {
    write(1, msg, strlen(msg));
}


screen::screen() {
    struct winsize win;
    if (ioctl(1, TIOCGWINSZ, &win) == -1) {
        writeraw("not a tty\n");
        exit(1);
    }

    W = win.ws_col;
    H = win.ws_row * 2;

    writeraw("\x1b[?1049h"); // alt screen on
    writeraw("\x1b[?25l");   // cursor off

    system("stty raw -echo");
}

screen::~screen() {
    system("stty sane");
    writeraw("\x1b[?25h");
    writeraw("\x1b[?1049l");
}

void screen::clear() {
    objects = {};
}

void screen::draw() const {
    std::string tty;
}

void screen::debug(const char *msg) {
    std::string s = msg;
    s += "\x1b[K\r";
    writeraw(s.data());
}

sprite::sprite() {}

sprite::sprite(const char *filename) {
    std::ifstream file(filename);
    if (file.bad())
        return;

    char header[3];
    file >> header;
    if (header[0] != 'P' || header[1] != '1')
        return;

    file >> W >> H;
    data.resize(W*H);
    char c;
    for (int i = 0; i < W * H; i++) {
        file >> c;
        data[i] = c == '1';
    }
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
