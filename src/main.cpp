#include <iostream>
#include "raylib.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int PARTICLE_RADIUS = 5;
const int DISTANCE_FROM_BORDER = PARTICLE_RADIUS;

class  Particle {
    private:

        float x;
        float y;

        float vx;
        float vy;

        float vx_limit = 10;
        float vy_limit = 10;
        float v_deadzone = 0.1f;


        enum class BorderDirection {
            LeftRight,
            UpDown
        };

        void checkBorder(BorderDirection direction) {

            int maxCoord, minCoord;
            float* coord,* vCoord;

            if (direction == BorderDirection::LeftRight) {
                maxCoord = SCREEN_WIDTH - DISTANCE_FROM_BORDER;
                minCoord = DISTANCE_FROM_BORDER;

                coord = &x;
                vCoord = &vx;
            }
            else {
                maxCoord = SCREEN_HEIGHT - DISTANCE_FROM_BORDER;
                minCoord = DISTANCE_FROM_BORDER;

                coord = &y;
                vCoord = &vy;
            }

            if (*coord < minCoord) {
                *vCoord = -(*vCoord);
                float delta = minCoord - *coord;
                *coord = minCoord + delta;

                *vCoord = *vCoord * bounceModifier;
            }
            if (*coord > maxCoord) {
                *vCoord = -(*vCoord);
                float delta = *coord - maxCoord;
                *coord = maxCoord - delta;

                *vCoord = *vCoord * bounceModifier;
            }

        }

    public:

        float get_x() { return x; }
        float get_y() { return y; }

        void set_x(float value) {
            x = value;
            checkBorder(BorderDirection::LeftRight);
        }
        void set_y(float value) {
            y = value;
            checkBorder(BorderDirection::UpDown);

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
            if (abs(value) < v_deadzone && y > SCREEN_HEIGHT - DISTANCE_FROM_BORDER-1 ) {
                vy = 0.f;
                return;
            }
            vy = value;
        }


        Color color;

        float g = 5.f;
        float airResistanceModifier = 0.99f;
        float bounceModifier = 0.7f;



        Particle() = default;
        Particle(float x, float y, float vx, float vy, Color color)
                : x(x), y(y), vx(vx), vy(vy), color(color) {}




        void applyAirResistance() {
            set_vx(vx * airResistanceModifier);
            set_vy(vy * airResistanceModifier);
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

Color randomColor() {
    return Color(rand() % 256, rand() % 256, rand() % 256, 255 );
}

int main(int argc, char** argv) {



    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulator");
    SetTargetFPS(144);

    int numParticles = 3000;
    std::vector<Particle> particles(numParticles);

    for (auto &p : particles) {

        p.set_x(rand() % SCREEN_WIDTH);
        p.set_y(rand() % SCREEN_HEIGHT);
        p.set_vx(0.f);
        p.set_vy(0.f);
        p.color = randomColor();
    }



    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        std::vector<Particle> newParticles;
        for (Particle &p : particles) {

            // if (rand() % 300 == 0 ) {
            //     Particle nowaya = p;
            //     nowaya.set_vx(rand());
            //     nowaya.set_vy(rand());
            //     newParticles.push_back(nowaya);
            //
            //     p.set_vx(rand() % 5);
            //     p.set_vy(rand() % 5);
            // }




            std::cout << p.get_vx() << ' ' << p.get_vy() << std::endl;

            p.applyAirResistance();
            p.applyGravity();

            p.applyVelocity();


        }
        particles.insert(particles.end(), newParticles.begin(), newParticles.end());

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Number of particles:" << particles.size() << std::endl;
        std::cout << "Current FPS: " << GetFPS() << std::endl;

        //----------------------------------------------------------------------------------



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        for (auto &p : particles)
            DrawCircle(p.get_x(), p.get_y(), PARTICLE_RADIUS, p.color);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

