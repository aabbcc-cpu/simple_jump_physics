#include <SFML/Graphics.hpp>

using namespace sf;

class Player {
    struct Point {
        float X;
        float Y;
    };

private:
    Point _size{};
    Point _position{};
    Point _velocity{};
    Point _acceleration{};
    float _gravity_scale;
    bool _on_ground;
    // TODO: временно
    RectangleShape shape;

public:
    Player() {
//        _size.X = 64.0f;
//        _size.Y = 64.0f;
//        _position.X = 100.0f;
//        _position.Y = 694.0f;
//        _velocity.X = 0.5f;
//        _velocity.Y = 0.75f;
//        _acceleration.X = 2.5e-4f;
//        _acceleration.Y = 2.5e-4f;
//        _gravity_scale = 6.0f;
//        _on_ground = true;

        _size = {0.0f, 0.0f};
        _position = {0.0f, 0.0f};
        _velocity = {0.0f, 0.0f};
        _acceleration = {0.0f, 0.0f};
        _gravity_scale = 0.0f;
        _on_ground = false;
    }

    Player(float coord_x, float coord_y,
           float position_x, float position_y,
           float vel_x, float vel_y,
           float accel_x, float accel_y,
           float gravity_scale,
           bool on_ground) :
           _size({coord_x, coord_y}),
           _position({position_x, position_y}),
           _velocity({vel_x, vel_y}),
           _acceleration({accel_x, accel_y}),
           _gravity_scale(gravity_scale),
           _on_ground(on_ground) {
        shape.setSize(Vector2f(_size.X, _size.Y));
        shape.setFillColor(Color::Black);
        shape.setPosition(_position.X, _position.Y);
    }

    void start() {
        shape.setSize(Vector2f(_size.X, _size.Y));
        shape.setFillColor(Color::Black);
        shape.setPosition(_position.X, _position.Y);
    }

    void set_size(float x, float y) {
        _size = {x, y};
    }

    void set_position(float x, float y) {
        _position = {x, y};
    }

    void set_velocity(float x, float y) {
        _velocity = {x, y};
    }

    void set_acceleration(float x, float y) {
        _acceleration = {x, y};
    }

    void set_gravity_scale(float g) {
        _gravity_scale = g;
    }

    void set_on_ground(bool v) {
        _on_ground = v;
    }

    RectangleShape get() const {
        return shape;
    }

    void set_default_velocityX() {
        _velocity.X = 0.5f;
    }

    void update(float time) {
        if (Keyboard::isKeyPressed(Keyboard::D)) {
            _position.X += _velocity.X * time;
            _velocity.X += _acceleration.X * _gravity_scale * time;
            shape.setPosition(_position.X, _position.Y);
            if (_velocity.X >= 0.75f) _velocity.X = 0.75f;
            info();
        }
        if (Keyboard::isKeyPressed(Keyboard::A)) {
            _position.X += -_velocity.X * time;
            _velocity.X += _acceleration.X * _gravity_scale * time;
            shape.setPosition(_position.X, _position.Y);
            if (_velocity.X >= 0.75) _velocity.X = 0.75f;
            info();
        }
        // TODO: доработать, т.к. во время прыжка можно менять направление
        if (!_on_ground) {
            _velocity.X = 0.5;
            _position.Y += -_velocity.Y * time;
            _velocity.Y += _acceleration.Y * (-_gravity_scale) * time;
            shape.setPosition(_position.X, _position.Y);
            if (_position.Y + _size.Y >= 470) { // TODO: нужно получать границу земли
                _position.Y = 406.0f;
                _velocity.Y = 0.75f;
                shape.setPosition(_position.X, _position.Y);
                _on_ground = true;
            }
            info();
        }
    }

    void info() const {
        printf("Coord: [%.2f; %.2f] "
               "Velocity: [%.2f; %.2f] "
               "ON_GROUND: %d\n",
               _position.X, _position.Y,
               _velocity.X, _velocity.Y,
               _on_ground);
    }
};


int main() {
    RenderWindow window(VideoMode(640, 480), "Physics");
    // FIXME: временно
    window.setVerticalSyncEnabled(true);

    RectangleShape ground;
    ground.setSize(Vector2f(640.0f, 10.0f));
    ground.setFillColor(Color::Green);
    ground.setPosition(0.0f, 470.0f);

    Player player;
    player.set_size(64.0f, 64.0f);
    player.set_position(100.0f, 406.0f);
    player.set_velocity(0.5f, 0.75f);
    player.set_acceleration(2.5e-4f, 2.5e-4f);
    player.set_gravity_scale(6.0f);
    player.set_on_ground(true);
    player.start();

    Clock clock;
    while (window.isOpen()) {
        Event event{};
        float elapsed_time = (float) clock.getElapsedTime().asMilliseconds();
        clock.restart();
        //elapsed_time /= 800.0f;
        //printf("Elapsed time: %f\n", elapsed_time);

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyReleased &&
                (event.key.code == Keyboard::D ||
                 event.key.code == Keyboard::A)) {
                player.set_default_velocityX();
            }
            if (event.type == Event::KeyReleased &&
                event.key.code == Keyboard::Space) {
                player.set_on_ground(false);
            }
        }

        player.update(elapsed_time);
        //player.info();

        window.clear(Color::White);
        window.draw(ground);
        window.draw(player.get());
        window.display();
    }
}

