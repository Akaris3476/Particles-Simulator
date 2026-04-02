#include <iostream>
#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PARTICLE_RADIUS = 3;
const int DISTANCE_FROM_BORDER = PARTICLE_RADIUS*2;

class  Particle {
    private:
        float x;
        float y;

        float vx;
        float vy;

        float vx_limit = 10;
        float vy_limit = 10;
        float v_deadzone = 0.1;

        void checkUpDownBorder() {
            float ceil = DISTANCE_FROM_BORDER;
            float floor = SCREEN_HEIGHT - DISTANCE_FROM_BORDER;

            if (y < ceil) {
                vy = -vy;
                float delta = ceil - y;
                y = ceil + delta;
            }

            if (y > floor) {
                vy = -vy;
                float delta = y - floor;
                y = floor - delta;
            }
        }

        void checkLeftRightBorder() {
            float left = DISTANCE_FROM_BORDER;
            float right = SCREEN_WIDTH - DISTANCE_FROM_BORDER;

            if (x < left) {
                vx = -vx;
                float delta = left - x;
                x = left + delta;
            }
            if (x > right) {
                vx = -vx;
                float delta = x - right ;
                x = right - delta ;
            }
        }

    public:

        float get_x() { return x; }
        float get_y() { return y; }

        void set_x(float value) {
            x = value;
            checkLeftRightBorder();
        }
        void set_y(float value) {
            y = value;
            checkUpDownBorder();

        }


        float get_vx() { return vx; }
        float get_vy() { return vy; }

        void set_vx(float value) {


            if (value >= vx_limit ) {
                vx = vx_limit;
                return;
            }

            if (value <= -vx_limit) {
                vx = -vx_limit;
                return;
            }

            vx = value;
        }
        void set_vy(float value) {


            if (value >= vy_limit ) {
                vy = vy_limit;
                return;
            }

            if (value <= -vy_limit) {
                vy = -vy_limit;
                return;
            }
            if (abs(value) < v_deadzone && y > SCREEN_HEIGHT - DISTANCE_FROM_BORDER-2 ) {
                vy = 0;
                return;
            }
            vy = value;
        }


        Color color;

        float g = 5;


        Particle() = default;
        Particle(float x, float y, float vx, float vy, Color color)
                : x(x), y(y), vx(vx), vy(vy), color(color) {}




        void applyAirResistance() {
            set_vx(vx * 0.99);
            set_vy(vy * 0.99);
        }

        void applyGravity() {
            float delta_t = GetFrameTime();
            set_vy(vy + g * delta_t);
        }

        void applyVelocity() {
            set_x(x += vx);
            set_y(y += vy);
        }
};

int main(int argc, char** argv) {



    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulator");
    SetTargetFPS(144);

    int numParticles = 10;
    std::vector<Particle> particles(numParticles);

    for (auto &p : particles) {

        p.set_x(rand() % SCREEN_WIDTH);
        p.set_y(rand() % SCREEN_HEIGHT);
        p.set_vx(0);
        p.set_vy(0);
        p.color = GREEN;
    }

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        std::vector<Particle> newParticles;
        for (Particle &p : particles) {
            //
            // if (rand() % 300 == 0 ) {
            //     Particle nowaya = p;
            //     nowaya.set_vx(rand());
            //     nowaya.set_vy(rand());
            //     newParticles.push_back(nowaya);
            //
            //     p.set_vx(rand() % 5);
            //     p.set_vy(rand() % 5);
            // }




            // if (rand() % 2 == 0) {
            //     p.set_vx(-p.get_vx() * 0.1);
            //     p.set_vy(-p.get_vy() * 0.1);
            // } else {
            //     p.set_vx(p.get_vx() * 0.1);
            //     p.set_vy(p.get_vy() * 0.1);
            // }



            std::cout << p.get_vx() << ' ' << p.get_vy() << std::endl;

            p.applyGravity();
            p.applyAirResistance();
            p.applyVelocity();


        }
        particles.insert(particles.end(), newParticles.begin(), newParticles.end());



        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);

        for (auto &p : particles)
            DrawCircle(p.get_x(), p.get_y(), PARTICLE_RADIUS, p.color);

        DrawText("", 190, 200, 40, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}