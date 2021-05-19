#ifndef BALL_H
#define BALL_H

#include <QtMath>
#include <QRandomGenerator>
#include "player.h"
#include "constants.h"

class Ball {
    public:
        Ball();
        ~Ball();
        double get_x();
        double get_y();
        void update();
        int collision(Player& p1, Player& p2);
        void launch();
        bool collides_with(QRectF other);
        void generate_new_angle(Player& p);
        void point_scored(int pos);

    private:
        quint8 side;
        bool is_moving;
        double x, y;
        double vx, vy;
        double dt;
        double ball_speed;
};

#endif // BALL_H
