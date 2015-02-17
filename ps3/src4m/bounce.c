/**
 * bounce.c
 *
 * David J. Malan
 * malan@harvard.edu
 *
 * Bounces a circle back and forth in a window.
 */

// standard libraries
#include <stdio.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

int main(void)
{
    // instantiate window
    GWindow window = newGWindow(320, 240);

    // instantiate circle
    GOval circle = newGOval(0, 110, 20, 20);
    setColor(circle, "BLACK");
    setFilled(circle, true);
    add(window, circle);
    
    // initial velocity
    double vx = 2.0;
    double vy = vx / 2;
    
    // bounce forever
    while (true)
    {
        // move circle along x-axis
        move(circle, vx, vy);

        // bounce off right edge of window
        if (getX(circle) + getWidth(circle) >= getWidth(window))
        {
            vx = -vx;
        }

        // bounce off left edge of window
        else if (getX(circle) <= 0)
        {
            vx = -vx;
        }

        else if (getY(circle) <= 0)
            vy = -vy;
        else if (getY(circle) + getHeight(circle) >= getHeight(window))
            vy = -vy;

        // linger before moving again
        pause(10);
    }
}
