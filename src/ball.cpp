#include "../include/ball.h"

Ball::Ball() : dt(1 / FPS),  vx(0.f), vy(0.f),
        is_moving(false), side(1), ball_speed(BALL_SPEED) {
    qreal dir = side == 1 ? -1.f : 1.f;
    x = dir * ((float)BOARD_W / 2.f) - dir *  (2 * PADDLE_SPACING);
    y = 0.f;
}


Ball::~Ball() {}

double Ball::get_x() {
    return x;
}

double Ball::get_y() {
    return y;
}

void Ball::update() {
    if (!is_moving) return;

    const qreal dt = 1.f / FPS;

    x += vx * dt;
    y += vy * dt;
}

int Ball::collision(Player& p1, Player& p2) {
    if (this->collides_with(QRectF(p1.get_x(), p1.get_y(), PADDLE_W, PADDLE_H)))
        generate_new_angle(p1);
    else if (this->collides_with(QRectF(p2.get_x(), p2.get_y(), PADDLE_W, PADDLE_H)))
        generate_new_angle(p2);

    if (x + BALL_SIZE >= BOARD_W / 2) 
        return 1;
    else if (x <= -BOARD_W / 2)
        return 2;

    if (y + BALL_SIZE >= BOARD_H / 2 || y <= -BOARD_H / 2)
        vy = -vy;

    return 0;
}

bool Ball::collides_with(QRectF other) {
    if (this->x > other.x() + other.width() || this->x + BALL_SIZE < other.x())
        return false;

    if (this->y > other.y() + other.height() || this->y + BALL_SIZE < other.y())
        return false;

    return true;
}

void Ball::generate_new_angle(Player& p) {
    ball_speed += (ball_speed / 100) * BALL_SPEED_UP;

    qreal dy = (p.get_y() + PADDLE_H / 2) - (this->y + BALL_SIZE / 2); 

    qreal norm_dy = dy / (PADDLE_H / 2); 

    qreal new_angle = norm_dy * MAX_BOUNCE_ANGLE;

    if (new_angle > MAX_BOUNCE_ANGLE)
        new_angle = MAX_BOUNCE_ANGLE;
    else if (new_angle < -MAX_BOUNCE_ANGLE)
        new_angle = -MAX_BOUNCE_ANGLE;

    new_angle = new_angle * (M_PI / 180);

    qreal direction = p.get_x() > 0 ? -1 : 1;
    vx = direction * ball_speed * qCos(new_angle);
    vy = ball_speed * -qSin(new_angle);
}

void Ball::launch() {
    ball_speed = BALL_SPEED;

    qreal random_angle = QRandomGenerator::global()->bounded(0, 2 * MAX_BOUNCE_ANGLE) - MAX_BOUNCE_ANGLE;
    random_angle = random_angle * (M_PI / 180);

    qint8 dir = side == 1 ? 1 : -1;

    vx = dir * ball_speed * qCos(random_angle);
    vy = ball_speed * -qSin(random_angle);

    is_moving = true;
}

void Ball::point_scored(int pos) {
    side = pos;
    qreal dir = side == 1 ? -1.f : 1.f;
    x = dir * ((float)BOARD_W / 2.f) - dir *  (2 * PADDLE_SPACING);
    y = 0.f;
    is_moving = false;
}
