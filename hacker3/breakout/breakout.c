// Akis Tsakiroglou
// tsa.akis@gmail.com

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
#include <math.h>

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
#define COLS 4

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

// god mode
#define GOD_STRING "GOD"

// Maximum velocity
#define MAX_VEL 2.5 * VEL_CONST

// Laser
#define LASER_WIDTH 3
#define LASER_HEIGHT PADDLE_WIDTH

// colors
#define NUM_COLORS 10
char * colors[NUM_COLORS] = {"BLUE", "RED", "GREEN", "PINK", "ORANGE", 
                             "MAGENTA", "GRAY", "YELLOW", "BLACK", "CYAN"};

// prototypes
void initBricks(GWindow window);
GOval initBall(GWindow window);
GRect initPaddle(GWindow window);
void initVelocity(double * vx, double * vy);
GLabel initScoreboard(GWindow window);
GLabel initLifeLabel(GWindow window);
void updateScoreboard(GWindow window, GLabel label, int points, int lives, int bricks);
void updateLifeLabel(GWindow window, GLabel label, int lives);
void movePaddle(GWindow window, GObject paddle, double px);
void ballBounceWall(GWindow window, GObject ball, double * vx, double * vy);
void ballBounceBrick(GWindow window, GObject ball, GObject brick, 
                     double * vx, double * vy);
void ballBouncePaddle(GWindow window, GObject ball, GObject paddle, 
                      double * vx, double * vy);

// hacker version
void shrinkPaddle(GObject paddle, GObject ball, 
                  const int total_bricks, int bricks);
int getPoints(GObject brick, char* colors[]);
void increaseSpeed(double * vy);
GObject shootLaser(GWindow window, GObject paddle);
GObject detectLaserCollision(GWindow window, GRect laser);
GObject laserHitWall(GWindow window, GObject laser);
void destroyBrick(GWindow window, GObject obj, GObject paddle, GOval ball, 
                  int * bricks, int total_bricks, int lives, 
                  GLabel label, double * vy, int * points, char * colors[]);
GObject destroyLaser(GWindow window, GObject laser);
int ballLost(GWindow window, GLabel label, int points, int * lives, 
             int bricks, GLabel life_label, GOval ball, GObject paddle, 
             double * vx, double * vy);

GObject detectCollision(GWindow window, GOval ball);
void resetGame(GWindow window, GObject ball, GObject paddle, 
               double * vx, double * vy);

int main(int argc, char* argv[]) {
    int god_mode = 0;

    if (argc > 1) {
        if (strcmp(argv[1], GOD_STRING) != 0) {
            printf("Usage for god mode: %s %s\n", argv[0], GOD_STRING);
            return -1;
        }
        else
            god_mode = 1;
    }

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
    const int total_bricks = bricks;

    // number of lives initially
    int lives = LIVES;

    // number of points initially
    int points = 0;

    waitForClick();
    
    // velocity
    double vx, vy;
    initVelocity(&vx, &vy);
    
    double new_pos;
    
    GObject laser = NULL;
    int playing = 0;
    
    // keep playing until game over
    while (lives > 0 && bricks > 0)
    {   
        if (god_mode) {
            new_pos = getX(ball) + getWidth(ball) / 2 - getWidth(paddle) / 2;
        }
        else {
            // move paddle with mouse
            GEvent event = getNextEvent(MOUSE_EVENT);
            if (event != NULL) {
                if (getEventType(event) == MOUSE_MOVED) {
                    new_pos = getX(event) - getWidth(paddle) / 2;
                    playing = 1;
                }
                else if (getEventType(event) == MOUSE_CLICKED && playing) {
                    if (laser == NULL)
                        laser = shootLaser(window, paddle);
                }
            }
        }
        
        movePaddle(window, paddle, new_pos); 
        
        // move laser shot
        if (laser != NULL)
            move(laser, 0, -2 * VEL_CONST);
        
        // move ball to its new position
        move(ball, vx, vy);

        // bounce off walls
        ballBounceWall(window, ball, &vx, &vy);

        // ball lost
        if (getY(ball) + getHeight(ball) >= getHeight(window)) {
            playing = ballLost(window, label, points, &lives, bricks, 
                               life_label, ball, paddle, &vx, &vy);
            if (laser != NULL)
                laser = destroyLaser(window, laser);
            waitForClick();
        }

        // detect ball collision with laser, paddle, or brick
        GObject obj = detectCollision(window, ball);
        if (obj != NULL)
            if (!strcmp(getType(obj), "GRect")) {
                if (getHeight(obj) == LASER_HEIGHT) {
                    // laser
                    playing = ballLost(window, label, points, &lives, 
                                       bricks, life_label, ball, paddle, 
                                       &vx, &vy);
                    laser = destroyLaser(window, laser);
                    waitForClick();
                }
                else if (getY(ball) > getY(label)) {
                    // paddle
                    ballBouncePaddle(window, ball, paddle, &vx, &vy);
                }
                else {
                    // brick
                    ballBounceBrick(window, ball, obj, &vx, &vy);
                    destroyBrick(window, obj, paddle, ball, &bricks, 
                                 total_bricks, lives, label, &vy, 
                                 &points, colors);
                }
            }
        
        // laser interactions
        if (laser != NULL) {
            obj = detectLaserCollision(window, laser);
            if (obj != NULL) {
                if (!strcmp(getType(obj), "GRect")) {
                    // it's a brick (ball is GOval)
                    destroyBrick(window, obj, paddle, ball, &bricks, 
                                 total_bricks, lives, label, &vy, 
                                 &points, colors);
                    laser = destroyLaser(window, laser);
                }
            }
            else
                laser = laserHitWall(window, laser);
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
    GRect rect = newGRect(getWidth(window) / 2 - PADDLE_WIDTH / 2, 
                          (1 - MARGIN_RATIO) * getHeight(window), 
                          PADDLE_WIDTH, PADDLE_HEIGHT);
    setFilled(rect, true);
    setColor(rect, "BLACK");
    add(window, rect);

    return rect;
}

/**
 * Initialize random velocity.
 */
void initVelocity(double * vx, double * vy) {
    *vx = (drand48() + VEL_CONST) * (drand48() > 0.5 ? 1 : -1);
    *vy = drand48() + VEL_CONST;
    return;
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
void updateScoreboard(GWindow window, GLabel label, int points, 
                      int lives, int bricks) {
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
void movePaddle(GWindow window, GObject paddle, double px) {    
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
void ballBounceBrick(GWindow window, GObject ball, GObject brick, 
                     double * vx, double * vy) {
    
    *vy = -(*vy);
    return;
}

/**
 * Bounces ball against the paddle.
 * In simple case: vy inversion.
 * Hacker edition: vy inversion plus
 *    vx increase if ball hits "far" from center && invert vx direction
 * OR vx decrease if ball hits close to center && keep vx direction
 */
void ballBouncePaddle(GWindow window, GObject ball, GObject paddle, 
                      double * vx, double * vy) {
    const double max_ratio = 1.0;
    const double threshold_ratio = 0.75;
    const double coeff = 0.10;

    // avoid funny bouncing on the paddle
    if (*vy > 0) {
        int ball_center = getX(ball) + RADIUS;
        int paddle_center = getX(paddle) + getWidth(paddle) / 2;
        int distance = ball_center - paddle_center;
        double far_ratio = (double) (abs(distance)) / (getWidth(paddle) / 2); // [0, 1.0]
        if (far_ratio > max_ratio)
            far_ratio = max_ratio; // [0, max_ratio]
        
        if (far_ratio > threshold_ratio) {
            *vx = -(1 + coeff) * (*vx); 
        }
        else {
            *vx = (1 - coeff) * (*vx);
        }
        *vy = -(*vy);
    }
    return;
}

/**
 * Shrinks the paddle until it is the width of the ball.
**/
void shrinkPaddle(GObject paddle, GObject ball, 
                  const int total_bricks, int bricks) {
    
    if (getWidth(paddle) > getWidth(ball)) {
        double per_cent = (double) (bricks) / total_bricks;
        setSize(paddle, trunc(per_cent * PADDLE_WIDTH), getHeight(paddle));
    }
    return;
}

/**
 * Variable number of points per brick depending on color.
**/
int getPoints(GObject brick, char* colors[]) {
    int points = 0;
    char * color = getColorGObject(brick);
    
    for (int i = 0; i < ROWS; i++) {
        if (!strcmp(color, colors[i])) {
            points = pow(2, ROWS - i);
            break;
        }
    }
    return points;
}

/**
 * Increasing vertical speed until a threshold.
**/
void increaseSpeed(double * vy) {
   if (*vy < MAX_VEL)
       *vy = (1.20 * (*vy) < MAX_VEL) ? 1.20 * (*vy) : MAX_VEL;
   return;
}

/**
 * Shoot laser beam!
**/
GObject shootLaser(GWindow window, GObject paddle) {
    int lx = getX(paddle) + getWidth(paddle) / 2 - LASER_WIDTH;
    int ly = getY(paddle) - LASER_HEIGHT;

    GRect laser = newGRect(lx, ly, LASER_WIDTH, LASER_HEIGHT);
    setFilled(laser, true);
    setColor(laser, "CYAN");
    add(window, laser);
    return laser;
}

/**
 * Detects whether laser beam has collided with a brick or the
 * upper wall. Only upper corners need be checked.
 * Returns object if so, else NULL.
 */
GObject detectLaserCollision(GWindow window, GRect laser) {
    // ball's location
    double x = getX(laser);
    double y = getY(laser);

    // for checking for collisions
    GObject object;

    // check for collision at laser's top-left corner
    object = getGObjectAt(window, x, y - 1);
    if (object != NULL)
        return object;

    // check for collision at ball's top-right corner
    object = getGObjectAt(window, x + getWidth(laser), y - 1);
    if (object != NULL)
        return object;

    return NULL;
}

/**
 * Destroy laser if it hit upper wall.
 * returns NULL if it hit, else laser object itself 
**/
GObject laserHitWall(GWindow window, GObject laser) {
    if (getY(laser) <= 0) {
        return destroyLaser(window, laser);
    }
    else {
        return laser;
    }
}

/**
 * Destroy brick, increase points & speed, shrink paddle, update Scoreboard.
**/
void destroyBrick(GWindow window, GObject obj, GObject paddle, GOval ball,
                  int * bricks, int total_bricks, int lives, GLabel label,
                  double * vy, int * points, char * colors[]) {
    
    (*bricks)--;
    *points += getPoints(obj, colors);
    shrinkPaddle(paddle, ball, total_bricks, *bricks);
    increaseSpeed(vy);
    updateScoreboard(window, label, *points, lives, *bricks);
    removeGWindow(window, obj);
    return;
}

/**
 * Destroy laser shot.
**/
GObject destroyLaser(GWindow window, GObject laser) {
    removeGWindow(window, laser);
    return NULL;
}

/**
 * When ball lost, decrement lives, update Scoreboard & LifeLabel, reset game.
**/
int ballLost(GWindow window, GLabel label, int points, int * lives,
             int bricks, GLabel life_label, GOval ball, GObject paddle,
             double * vx, double * vy) {
    (*lives)--;
    updateScoreboard(window, label, points, *lives, bricks);
    updateLifeLabel(window, life_label, *lives);
    if (*lives > 0)
        resetGame(window, ball, paddle, vx, vy);
    return 0;
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
 * Resets paddle, ball, velocity for a new round.
 */
void resetGame(GWindow window, GObject ball, GObject paddle, 
               double * vx, double * vy) {
    initVelocity(vx, vy);
    setLocation(ball, getWidth(window) / 2 - RADIUS, 
                getHeight(window) / 2 - RADIUS);
    setLocation(paddle, getWidth(window) / 2 - getWidth(paddle) / 2, 
                getY(paddle));
    return;
}
