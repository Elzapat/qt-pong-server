#ifndef BALL_H
#define BALL_H

class Ball {
    public:
        Ball(double FPS);
        ~Ball();
        double get_x();
        double get_y();

    private:
        double x, y;
        double dt;
        const double SPEED = 500.f;
};

#endif // BALL_H
