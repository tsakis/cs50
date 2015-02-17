/**
 * label.c
 *
 * David J. Malan
 * malan@harvard.edu
 *
 * Draws a label in a window.
 */

// standard libraries
#include <ctype.h>

// Stanford Portable Library
#include <spl/gobjects.h>
#include <spl/gwindow.h>

int main(void)
{
    // instantiate window
    GWindow window = newGWindow(320, 240);

    // instantiate label
    GLabel label = newGLabel("");
    setFont(label, "SansSerif-36");
    add(window, label);

    GLabel label2 = newGLabel("");
    setFont(label2, "SansSerif-36");
    add(window, label2);

    char * strings[4] = {"You", "are", "getting", "dizzy!"};

    // count down from 50 to 0
    for (int i = 50; i >= 0; i--)
    {
        // to store 50 through 0 (with '\0'), we need <= 3 chars
        char s[3];
        char s2[8];
        
        // convert i from int to string
        sprintf(s, "%i", i);
        sprintf(s2, "%s", strings[(50 - i) % 4]);

        // update label
        setLabel(label, s);
        setLabel(label2, s2);

        // center label
        double x = (getWidth(window) - getWidth(label)) / 2;
        double y = (getHeight(window) - getHeight(label)) / 2;
        setLocation(label, x, y);

        double x2 = (getWidth(window) - getWidth(label2)) / 2;
        double y2 = 2 * y;
        setLocation(label2, x2, y2);

        // linger for 100 milliseconds
        pause(100);
    }
 
    // that's all folks
    closeGWindow(window);
    return 0;
}
