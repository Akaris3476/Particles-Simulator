#include <iostream>
#include <vector>
#include "raylib.h"
#include "vmath.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 650;


class Particle {
    private:

        float x;
        float y;

        float vx;
        float vy;

        float vx_limit = 15;
        float vy_limit = 15;


        enum class BorderDirection {
            LeftRight,
            UpDown
        };

        void checkBorder(BorderDirection direction) {

            float maxCoord, minCoord;
            float* coord,* v;

            if (direction == BorderDirection::LeftRight) {
                maxCoord = SCREEN_WIDTH - radius;
                minCoord = radius;

                coord = &x;
                v = &vx;
            }
            else {
                maxCoord = SCREEN_HEIGHT - radius;
                minCoord = radius;

                coord = &y;
                v = &vy;
            }

            if (*coord < minCoord) {
                *v = -(*v);
                float delta = minCoord - *coord;
                *coord = minCoord + delta;

                *v = *v * bounceModifier;
            }
            if (*coord > maxCoord) {
                *v = -(*v);
                float delta = *coord - maxCoord;
                *coord = maxCoord - delta;

                *v = *v * bounceModifier;
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

            // if (abs(value) < v_deadzone ) {
            //     vx = 0.f;
            //     return;
            // }

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
            // if (abs(value) < v_deadzone && y > SCREEN_HEIGHT - radius-1 ) {
            //     vy = 0.f;
            //     return;
            // }
            vy = value;
        }
        void set_v(Vector2 value) {
            set_vx(value.x);
            set_vy(value.y);
        }


        Color color;
        float radius;

        float g = 3.f;
        float airResistanceModifier = 0.97f;
        float bounceModifier = 0.8f;
        // float v_deadzone = 0.3f;



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

            if (!collision) return;

            Vector2 posA = get_coord();
            Vector2 posB = particle.get_coord();

            Vector2 vA = get_v();
            Vector2 vB = particle.get_v();

            float distance = vdistance(posA, posB);
            if (distance == 0 )
                return;

            float minDistance = radius+particle.radius;
            float overlap = minDistance - distance;

            Vector2 vector_outwards = vsub(posA, posB);
            Vector2 normal = vnormalize(vector_outwards);

            // make acceptable some overlap
            if (overlap > 2.f)
            {
                // in case of one particle stuck in another - get direction outwards and apply repulsion force
                Vector2 repulsion = vmult(normal, overlap/5);

                set_v(vadd(vA, repulsion));
                particle.set_v(vsub(vB, repulsion));
            }


            Vector2 p2vrelative = vsub(vB, vA);
            // if it's positive - particles are getting closer
            float relVelNormalDot = vdot(p2vrelative, normal);

            // if particles still stuck, but second particle flies away - don't apply impulse
            if (relVelNormalDot > 0) {
                Vector2 impulse = vmult(normal, vdot(p2vrelative, normal)*bounceModifier);

                set_v(vadd(vA, impulse));
                particle.set_v(vsub(vB, impulse));
            }

        }


        void applyAirResistance() {
            float slowing_force = 1 - airResistanceModifier;
            Vector2 new_v = vmult(get_v(), 1-slowing_force*GetFrameTime());
            set_v(new_v);
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
        int system_v = 0;

        Vector2 MousePos = GetMousePosition();

        //spawn new particles
        if (IsKeyDown(KEY_W)) {
            for (int i = 0; i < 2; i++) {
                Vector2 v(rand()%26-16, rand()%21-16);
                Particle p(MousePos, v, PARTICLE_RADIUS, randomColor());
                particles.push_back(p);
            }
        }

        for (auto& p : particles) {
            p.applyGravity();
            p.applyAirResistance();
        }

        for (int i = 0; i < particles.size(); i++) {
            Particle& p = particles[i];

            for (int j = i+1; j < particles.size(); j++) {
                p.collision(particles[j]);
            }
        }

        for (auto& p : particles) {
            p.applyVelocityToCoords();
            system_v += vmagnitude(p.get_v());
        }

        // std::vector<Particle> newParticles;
        // for (Particle &p : particles) {
        //
        //     if (rand() % 300 == 0 ) {
        //         Particle new_one = p;
        //         new_one.set_vx(rand());
        //         new_one.set_vy(rand());
        //         newParticles.push_back(new_one);
        //
        //         p.set_vx(rand() % 5);
        //         p.set_vy(rand() % 5);
        //     }
        // }
        // particles.insert(particles.end(), newParticles.begin(), newParticles.end());

        std::cout << std::endl << std::endl;
        std::cout << "----------------------------------" << std::endl;
        std::cout << "Number of particles:" << particles.size() << std::endl;
        std::cout << "Current FPS: " << GetFPS() << std::endl;
        std::cout << "Sum of all velocities:" << system_v << std::endl;


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

