#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <inttypes.h>
#include <xdo.h>
#include <wchar.h>
#include <locale.h>


/* mgba bindings */
#define LEFT    "Left"
#define RIGHT   "Right"
#define UP  "Up"
#define DOWN    "Down"
#define L   "a"
#define R   "s"
#define SELECT  "BackSpace"
#define START   "Return"
#define Y   ""      /* dummy */
#define B   "z"
#define A   "x"
#define X   ""      /* dummy */

int
main(int argc, char *argv[], char *environ[])
{
    (void)setlocale(LC_ALL, "");

    xdo_t          *xdo = xdo_new(":0.0");

    uint8_t     i[32];

    int     start, select, a, b, x, y, l, r, up, down, left, right;
    start = select = a = b = x = y = l = r = up = down = left =
        right = 0;

    FILE           *c = fopen("/dev/uhid_snes0", "r");
    if (c == NULL)
        exit(1);

    while (!feof(c) && !ferror(c)) {
        fread(&i, 1, 8, c);

#define up(var, key) \
    if (var == 1) \
        {xdo_send_keysequence_window_up(xdo, CURRENTWINDOW, key, 0);var=0;}

#define down(var, key) \
    if (var == 0) \
        {xdo_send_keysequence_window_down(xdo, CURRENTWINDOW, key, 0);var=1;}

        if (i[3] == 127) {
            up(left, LEFT);
            up(right, RIGHT);
        } else if (i[3] == 255) {
            up(left, LEFT);
            down(right, RIGHT);
        } else if (i[3] == 0) {
            down(left, LEFT);
            up(right, RIGHT);
        }

        if (i[4] == 127) {
            up(down, DOWN);
            up(up, UP);
        } else if (i[4] == 255) {
            down(down, DOWN);
            up(up, UP);
        } else if (i[4] == 0) {
            up(down, DOWN);
            down(up, UP);
        }

        if (i[6] & 0b1) {
            down(l, L);
        } else {
            up(l, L);
        }

        if (i[6] & 0b10) {
            down(r, R);
        } else {
            up(r, R);
        }

        if (i[6] & 0b10000) {
            down(select, SELECT);
        } else {
            up(select, SELECT);
        }

        if (i[6] & 0b100000) {
            down(start, START)
        } else {
            up(start, START);
        }

        i[5] = i[5] >> 4;

        if (i[5] & 0b1000) {
            down(y, Y);
        } else {
            up(y, Y);
        }

        if (i[5] & 0b100) {
            down(b, B);
        } else {
            up(b, B);
        }

        if (i[5] & 0b10) {
            down(a, A);
        } else {
            up(a, A);
        }

        if (i[5] & 0b1) {
            down(x, X);
        } else {
            up(x, X);
        }

#ifndef NDEBUG

    if (left == 1)
        putchar('<');
    else if (right == 1)
        putchar('>');
    else
        putchar('_');

    if (up == 1)
        fputwc(L'∧', stdout);
    else if (down == 1)
        fputwc(L'∨', stdout);
    else
        putchar('_');

#define o(x, y) \
    if (x == 1) \
        putchar(y); \
    else putchar('_');

    o(a, 'A');
    o(b, 'B');

    o(x, 'X');
    o(y, 'Y');

    o(r, 'R');
    o(l, 'L');

    o(select, 'S');

#endif

    if (start == 1)
        fputwc(L'⎆', stdout);
    else putchar('_');

    putchar('\r');

    }

    fclose(c);
}
