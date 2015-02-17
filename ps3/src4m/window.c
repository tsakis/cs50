/**
 * window.c
 *
 * David J. Malan
 * malan@harvard.edu
 *
 * Draws a window.
 */

// Stanford Portable Library
#include <spl/gwindow.h>

int main(void)
{
    // instantiate window
    GWindow window = newGWindow(640, 480);
 
    // let's look at it for a while
    pause(1000);

    // that's all folks
    closeGWindow(window);
    return 0;
}
