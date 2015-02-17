//
// breakout.c
//
// Computer Science 50
// Problem Set 3
//

// standard libraries
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Stanford Portable Library
#include <spl/gevents.h>
#include <spl/gobjects.h>
#include <spl/gwindow.h>

// height and width of game's window in pixels
#define HEIGHT 600
#define WIDTH 400

// margin up and down
#define MARGIN_RATIO 0.1

// number of rows of bricks
#define ROWS 3

// number of columns of bricks
#define COLS 3

// radius of ball in pixels
#define RADIUS 10

// bricks height
#define BRICK_HEIGHT ((int) (HEIGHT / 50))

// bricks width
#define BRICK_WIDTH ((int) (WIDTH / (COLS + 1)))

// paddle height
#define PADDLE_HEIGHT ((int) (HEIGHT / 55))

// paddle width 
#define PADDLE_WIDTH ((int) (WIDTH / 8))

// lives
#define LIVES 3

// Velocity constant
#define VEL_CONST 2

// label length
#define MAX_LABEL_LENGTH 12

// colors
char * colors[10] = {"BLUE", "RED", "GREEN", "PINK", "ORANGE", "MAGENTA", "GRAY", "YELLOW", "BLACK", "CYAN"};

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
GLabel initScoreboard(GWindow window);
GLabel initLifeLabel(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points, int lives, int bricks);
void updateLifeLabel(GWindow window, GLabel label, int lives);
void movePaddle(GWindow window, GObject paddle, GEvent event);
void ballBounceWall(GWindow window, GObject ball, double * vx, double * vy);
void ballBounceBrick(GWindow window, GObject ball, GObject brick, double * vx, double * vy);
void ballBouncePaddle(GWindow window, GObject ball, GObject paddle, double * vx, double * vy);
GObject detectCollision(GWindow window, GOval ball);
void resetGame(GWindow window, GObject ball, GObject paddle, double * vx, double * vy);

int main(void) {
    // seed pseudorandom number generator
    srand48(time(NULL));

    // instantiate window
    GWindow window = newGWindow(WIDTH, HEIGHT);

    // instantiate bricks
    initBricks(window);

    // instantiate ball, centered in middle of window
    GOval ball = initBall(window);

    // instantiate paddle, centered at bottom of window
    GRect paddle = initPaddle(window);

    // instantiate scoreboard, centered in middle of window, just above ball
    GLabel label = initScoreboard(window);
    GLabel life_label = initLifeLabel(window);

    // number of bricks initially
    int bricks = COLS * ROWS;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    waitForClick();
    
    // velocity
    double vx = drand48() + VEL_CONST;
    double vy = drand48() + VEL_CONST;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {   
        // move paddle with mouse
        GEvent event = getNextEvent(MOUSE_EVENT);
        if (event != NULL)
            if (getEventType(event) == MOUSE_MOVED)
               movePaddle(window, paddle, event); 

        // move ball to its new position
        move(ball, vx, vy);

        // bounce off walls
        ballBounceWall(window, ball, &vx, &vy);

        // ball lost
        if (getY(ball) + getHeight(ball) >= getHeight(window)) {
            lives--;
            updateScoreboard(window, label, points, lives, bricks);
            updateLifeLabel(window, life_label, lives);
            if (lives > 0) {
                resetGame(window, ball, paddle, &vx, &vy);
                waitForClick();
            }
        }
        
        // detect collision with paddle or brick
        GObject obj = detectCollision(window, ball);
        if (obj != NULL)
            if (!strcmp(getType(obj), "GRect")) {
                if (getY(ball) > getY(label)) {
                    // paddle
                    ballBouncePaddle(window, ball, paddle, &vx, &vy);
                }
                else {
                    // bricks
                    ballBounceBrick(window, ball, obj, &vx, &vy);
                    bricks--;
                    points++;
                    updateScoreboard(window, label, points, lives, bricks);
                    removeGWindow(window, obj);
                }
            }
        pause(10); //ms
    }
    // wait for click before exiting
    waitForClick();

    // game over
    closeGWindow(window);
    return 0;
}

/**
 * Initializes window with a grid of bricks.
 */
void initBricks(GWindow window) {
    /*
    side_margin | brick_width | inter_brick_margin | brick_width ... | side_margin
    Adapted for variable number of ROWS and COLS
    */
    int total_margin = getWidth(window) - COLS * BRICK_WIDTH;
    int inter_brick_total_margin = total_margin - 2; //at least 1 px each side
    
    // COLS - 1 inter_brick_margins
    while (inter_brick_total_margin % (COLS - 1) != 0)
       inter_brick_total_margin--;
    
    int inter_brick_margin = inter_brick_total_margin / (COLS - 1);
    int side_margin = (total_margin - inter_brick_total_margin) / 2;
   
    for (int i = 0; i < COLS; i++) {
        int bx = side_margin + i * BRICK_WIDTH + i * inter_brick_margin;
        for (int j = 0; j < ROWS; j++) {
            int by = MARGIN_RATIO * getHeight(window) + j * BRICK_HEIGHT + j * inter_brick_margin;
            
            if (getGObjectAt(window, bx, by) != NULL)
                continue;
            
            GRect rect = newGRect(bx, by, BRICK_WIDTH, BRICK_HEIGHT);
            setFilled(rect, true);
            setColor(rect, colors[j % 10]);
            add(window, rect);
        }
    }
    return;

}

/**
 * Instantiates ball in center of window.  Returns ball.
 */
GOval initBall(GWindow window) {
    int cx = getWidth(window) / 2 - RADIUS;
    int cy = getHeight(window) / 2 - RADIUS;
    GOval ball = newGOval(cx, cy, 2 * RADIUS, 2 * RADIUS);
    
    setFilled(ball, true);
    setColor(ball, "BLACK");
    add(window, ball);

    return ball;
}

/**
 * Instantiates paddle in bottom-middle of window.
 */
GRect initPaddle(GWindow window) {
    GRect rect = newGRect(getWidth(window) / 2 - PADDLE_WIDTH / 2, (1 - MARGIN_RATIO) * getHeight(window), PADDLE_WIDTH, PADDLE_HEIGHT);

    setFilled(rect, true);
    setColor(rect, "BLACK");
    add(window, rect);

    return rect;
}

/**
 * Instantiates, configures, and returns label for scoreboard.
 */
GLabel initScoreboard(GWindow window) {
    GLabel label = newGLabel("0");
    setFont(label, "SansSerif-18");
    setColor(label, "GRAY");

    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    add(window, label);
    
    return label;
}

/**
 * Instantiates, configures, and returns label for lives.
 */
GLabel initLifeLabel(GWindow window) {
    char s[MAX_LABEL_LENGTH];

    sprintf(s, "Lives: %d", LIVES); 
    GLabel label = newGLabel(s);
    setFont(label, "SansSerif-16");
    setColor(label, "GRAY");

    double x = 0.7 * getWidth(window);
    double y = 0.95 * getHeight(window);
    setLocation(label, x, y);
    add(window, label);
    
    return label;
}

/**
 * Updates scoreboard's label, keeping it centered in window
 */
void updateScoreboard(GWindow window, GLabel label, int points, int lives, int bricks) {
    // update label
    char s[MAX_LABEL_LENGTH];
    
    if (lives == 0)
        sprintf(s, "You lost!");
    else if (bricks == 0)
        sprintf(s, "You won!?");
    else {
        sprintf(s, "%d", points);
    }
    setLabel(label, s);

    // center label in window
    double x = (getWidth(window) - getWidth(label)) / 2;
    double y = (getHeight(window) - getHeight(label)) / 2;
    setLocation(label, x, y);
    return;
}

/**
 * Updates lives label.
 */
void updateLifeLabel(GWindow window, GLabel label, int lives) {
    char s[MAX_LABEL_LENGTH];
    
    sprintf(s, "Lives: %d", lives);
    setLabel(label, s);
    return;
}

/**
 * Moves paddle according to mouse event.
 */
void movePaddle(GWindow window, GObject paddle, GEvent event) {    
    double px = getX(event) - getWidth(paddle) / 2;
    if (px < 0)
        px = 0;
    else if (px > getWidth(window) - getWidth(paddle))
        px = getWidth(window) - getWidth(paddle);
    setLocation(paddle, px, getY(paddle));
    return;
}

/**
 * Bounces ball against the walls.
 */
void ballBounceWall(GWindow window, GObject ball, double * vx, double * vy) {
    if (getX(ball) + getWidth(ball) >= getWidth(window))
        *vx = -(*vx);
    else if (getX(ball) <= 0)
        *vx = -(*vx);
    else if (getY(ball) <= 0)
        *vy = -(*vy);
    else if (getY(ball) + getHeight(ball) >= getHeight(window))
        *vy = -(*vy);
    return;
}

/**
 * Bounces ball against the bricks.
 * For now, stupid y-velocity inversion, which is not always the case.
 */
void ballBounceBrick(GWindow window, GObject ball, GObject brick, double * vx, double * vy) {
    
    *vy = -(*vy);
    return;
}

/**
 * Bounces ball against the paddle.
 * For now, stupid y-velocity inversion, which is not always the case.
 */
void ballBouncePaddle(GWindow window, GObject ball, GObject paddle, double * vx, double * vy) {
    if (*vy > 0)
        *vy = -(*vy);
    return;
}

/**
 * Detects whether ball has collided with some object in window
 * by checking the four corners of its bounding box (which are
 * outside the ball's GOval, and so the ball can't collide with
 * itself).  Returns object if so, else NULL.
 */
GObject detectCollision(GWindow window, GOval ball) {
    // ball's location
    double x = getX(ball);
    double y = getY(ball);

    // for checking for collisions
    GObject object;

    // check for collision at ball's top-left corner
    object = getGObjectAt(window, x, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-left corner
    object = getGObjectAt(window, x, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // check for collision at ball's bottom-right corner
    object = getGObjectAt(window, x + 2 * RADIUS, y + 2 * RADIUS);
    if (object != NULL)
    {
        return object;
    }

    // no collision
    return NULL;
}

/**
 * Resets paddle, ball, velocity, bricks for a new round.
 */
void resetGame(GWindow window, GObject ball, GObject paddle, double * vx, double * vy) {
    *vx = drand48() + VEL_CONST;
    *vy = drand48() + VEL_CONST;
    setLocation(ball, getWidth(window) / 2 - RADIUS, getHeight(window) / 2 - RADIUS);
    setLocation(paddle, getWidth(window) / 2 - getWidth(paddle) / 2, getY(paddle));
    return;
}
