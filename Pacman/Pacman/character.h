#define MOVE 1
#define QUIET 2

struct Character {
    float x, y;
    int r, c;
    float vx,vy; //-- Velocity vector
    int state = QUIET;
    long time_remaining;

    Character();
    Character(float x, float y, int r, int c);

    public:
        void set_position(float x, float y);
        void init_movement(float destination_x, float destination_y, float duration);
        void integrate(long t);
        void draw();
};


