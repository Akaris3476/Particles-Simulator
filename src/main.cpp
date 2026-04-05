#include <iostream>
#include <vector>
#include "raylib.h"
#include "vmath.h"

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;



class Particle {
    private:

        float x;
        float y;

        float vx;
        float vy;

        float vx_limit = 10;
        float vy_limit = 10;
        float v_deadzone = 0.2f;


        enum class BorderDirection {
            LeftRight,
            UpDown
        };

        void checkBorder(BorderDirection direction) {

            float maxCoord, minCoord;
            float* coord,* vCoord;

            if (direction == BorderDirection::LeftRight) {
                maxCoord = SCREEN_WIDTH - radius;
                minCoord = radius;

                coord = &x;
                vCoord = &vx;
            }
            else {
                maxCoord = SCREEN_HEIGHT - radius;
                minCoord = radius;

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

        Vector2 get_coord() { return Vector2(x, y); }
        void set_coord(Vector2 value) {
            set_x(value.x);
            set_y(value.y);
        }

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
        Vector2 get_v() {
            return Vector2(get_vx(), get_vy() );
        }


        void set_vx(float value) {


            if (value >= vx_limit ) {
                vx = vx_limit;
                return;
            }

            if (value <= -vx_limit) {
                vx = -vx_limit;
                return;
            }

            if (abs(value) < v_deadzone ) {
                vx = 0.f;
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
            if (abs(value) < v_deadzone && y > SCREEN_HEIGHT - radius-1 ) {
                vy = 0.f;
                return;
            }
            vy = value;
        }
        void set_v(Vector2 value) {
            set_vx(value.x);
            set_vy(value.y);
        }


        Color color;
        float radius;

        float g = 5.f;
        float airResistanceModifier = 0.98f;
        float bounceModifier = 0.6f;



        Particle() = default;
        Particle(float x, float y, float vx, float vy, Color color)
                : x(x), y(y), vx(vx), vy(vy), color(color), radius(radius) {}
        Particle(Vector2 cord, float radius, Color color) {
            set_coord(cord);
            this->radius = radius;
            this->color = color;
        }
        Particle(Vector2 cord, Vector2 v, float radius, Color color) {
            set_coord(cord);
            this->radius = radius;
            this->color = color;
            set_v(v);
        }

        void collision(Particle &particle) {
            if (this == &particle)
                return;

            bool collision = CheckCollisionCircles(get_coord(), radius,
                particle.get_coord(), particle.radius);


            if (collision) {


                float distance = vdistance(get_coord(), particle.get_coord());
                if (distance == 0 ) {
                    return;
                }
                float minDistance = radius+particle.radius;

                Vector2 vector_outwards = vsub(get_coord(), particle.get_coord());
                Vector2 normal = vnormalize(vector_outwards);


                // in case of one particle stuck in another - get direction outwards and apply repulsion force
                Vector2 repulsion = vmult(normal, minDistance - distance);
                set_x(x + repulsion.x);
                set_y(y + repulsion.y);

                Vector2 p2vrelative = vsub(particle.get_v(), get_v());
                Vector2 impulse = vmult(normal, vdot(p2vrelative, normal)*bounceModifier*0.5f);

                set_v(vadd(get_v(), impulse));


            }

        }


        void applyAirResistance() {
            set_vx(vx * airResistanceModifier);
            set_vy(vy * airResistanceModifier);
        }

        void applyGravity() {
            float delta_t = GetFrameTime();
            set_vy(vy + g * delta_t);
        }

        void applyVelocityToCoords() {
            set_x(x += vx);
            set_y(y += vy);
        }
};

Color randomColor() {
    return Color(rand() % 256, rand() % 256, rand() % 256, 255 );
}

int main(int argc, char** argv) {



    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Particle Simulator");
    SetTargetFPS(60);

    int numParticles = 100;
    const int PARTICLE_RADIUS = 6;
    std::vector<Particle> particles(numParticles);

    for (auto &p : particles) {

        p.set_x(rand() % SCREEN_WIDTH);
        p.set_y(rand() % SCREEN_HEIGHT);
        p.set_vx(0.f);
        p.set_vy(0.f);
        p.color = randomColor();
        p.radius = PARTICLE_RADIUS;
    }

    // HideCursor();


    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------

        Vector2 MousePos = GetMousePosition();

        //spawn new particles
        if (IsKeyDown(KEY_W)) {
            Vector2 v(rand()%10, rand()%10);
            Particle p(MousePos, v, PARTICLE_RADIUS, randomColor());
            particles.push_back(p);

            Vector2 v2(-rand()%10, -rand()%10);
            Particle p2(MousePos, v2, PARTICLE_RADIUS, randomColor());
            particles.push_back(p2);
        }

        // std::vector<Particle> newParticles;
        for (Particle &p : particles) {

            // if (rand() % 300 == 0 ) {
            //     Particle new_one = p;
            //     new_one.set_vx(rand());
            //     new_one.set_vy(rand());
            //     newParticles.push_back(new_one);
            //
            //     p.set_vx(rand() % 5);
            //     p.set_vy(rand() % 5);
            // }


            std::cout << p.get_vx() << ' ' << p.get_vy() << std::endl;

            p.applyAirResistance();
            p.applyGravity();

            for (Particle &p2 : particles) {
                p.collision(p2);
            }

            p.applyVelocityToCoords();


        }
        // particles.insert(particles.end(), newParticles.begin(), newParticles.end());

        std::cout << "----------------------------------" << std::endl;
        std::cout << "Number of particles:" << particles.size() << std::endl;
        std::cout << "Current FPS: " << GetFPS() << std::endl;


        //----------------------------------------------------------------------------------



        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        for (auto &p : particles)
            DrawCircleV(p.get_coord(), p.radius, p.color);


        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}

