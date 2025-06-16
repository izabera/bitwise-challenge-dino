#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <sys/ioctl.h>
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#include <vector>

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;
using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

struct input {
    bool up;
    bool down;
    u32 tick;
};

struct gamestate {
    i16 height;
    i8 speed;
    u8 seed;
} __attribute__((packed));
static_assert(sizeof(gamestate) <= sizeof(u64));

struct screen {
    int W, H;

    struct pixel { u8 r,g,b; };
    std::vector<pixel> fb;
    termios term;

    screen() {
        struct winsize win;
        if (ioctl(0, TIOCGWINSZ, &win) == -1) {
            write("not a tty\n");
            exit(1);
        }

        W = win.ws_col;
        H = win.ws_row * 2;
        fb = std::vector<pixel>(W*H);

        write("\x1b[?1049h"); // alt screen on
        write("\x1b[?25l");   // cursor off

        tcgetattr(1, &term);
        termios raw{};
        tcgetattr(1, &raw);
    }

    ~screen() {
        tcsetattr(1, 0, &term);
        write("\x1b[?25h");
        write("\x1b[?1049l");
    }

    void write(const char *msg) {
        ::write(1, msg, strlen(msg));
    }

    void clear() { memset(fb.data(), 0, fb.size()); }
    void draw();
};

struct scene {
    std::vector<int> birds;
    std::vector<int> cactuses;
    int dinoheight;
    int dinoframe;
    int dinostate;
    void render(screen& s) {
    }
};


// takes keyboard input, game state and a (cleared) frame
// returns the new game state
u64 makeframe(const input& i, u64 state, screen& output) {
    u16 seed = state & 0xffff;
    i8 speed = (state >> 16) & 0xff;
    i16 height = (state >> 24) & 0xffff;

    if (height == 0 && i.up)
        speed = 126;

    height += speed;

    if (height <= 0)
        height = speed = 0;
    else
        speed -= 10;

    return seed | u64(u8(speed)) << 16 | u64(u16(height)) << 24;
}

input getinput();

int main() {
    constexpr static auto FPS = 60;

    uint64_t state = 0;

    screen scr;
    input in;

    while (1) {
        in.up = false;

        fd_set rfds;
        FD_ZERO(&rfds);
        FD_SET(0, &rfds);
        struct timeval tv { 0, 1'000'000/FPS };
        if (select(1, &rfds, nullptr, nullptr, &tv) > 0) {
            ssize_t r;
            char buf[100];

            while ((r = read(0, buf, sizeof buf)) > 0) {
                for (ssize_t b = 0; b < r; b++) {
                    switch (buf[b]) {
                        case ' ': in.up = true; break;
                        case '\3': return 0; // ^C
                    }
                }
            }
        }

        scr.clear();
        state = makeframe(in, state, scr);
        scr.draw();
        in.tick++;
    }
}
