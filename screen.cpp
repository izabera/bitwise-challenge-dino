#include "screen.hpp"
#include <cstdlib>
#include <cstring>
#include <fstream>
#include <iostream>
#include <string>
#include <unistd.h>

static void writeraw(const char *msg) {
    write(1, msg, strlen(msg));
}

screen::screen() {
    struct winsize win;
    if (ioctl(0, TIOCGWINSZ, &win) == -1) {
        writeraw("not a tty\n");
        exit(1);
    }

    W = win.ws_col;
    H = win.ws_row * 2;

    writeraw("\x1b[?1049h"); // alt screen on
    writeraw("\x1b[?25l");   // cursor off

    termios raw{};
    tcgetattr(1, &raw);
}

screen::~screen() {
    system("stty sane");
    writeraw("\x1b[?25h");
    writeraw("\x1b[?1049l");
}

void screen::clear() {
    objects = {};
}

void screen::drawframe() {
    std::string tty;
}

sprite::sprite() { W = H = 0; }

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
