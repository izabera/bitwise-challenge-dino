#include "types.hpp"
#include <sys/ioctl.h>
#include <termios.h>
#include <vector>

// we only need netbpm P1
struct sprite {
    int W, H;
    std::vector<bool> data;
    sprite(const char *filename);
    sprite();
};

struct entity {
    sprite s;
    int W, H;
};

struct screen {
    int W, H;
    std::vector<entity> objects;

    screen();
    ~screen();

    void clear();
    void drawframe();
};
