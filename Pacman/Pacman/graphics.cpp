#include <GL/glut.h>
#include <iostream>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "graphics.h"
#include <thread>
// C library headers
#include <stdio.h>
#include <string.h>

// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

#ifdef __cplusplus
extern "C" {
    #endif
    #include <jpeglib.h>
    #ifdef __cplusplus
}
#endif
#include "Utils.h"
#include "Agents.h"

long last_t = 0;
int anglealpha = -90;
int anglebeta = 0;
int first_time = 0;
std::thread t1;

enum eTextures
{
    Wall = 0,
    Ground,
};

void Graphics::readTextures(){
    glBindTexture(GL_TEXTURE_2D,eTextures::Wall);
    LoadTexture("../pared3.jpg", 64);
    glBindTexture(GL_TEXTURE_2D,eTextures::Ground);
    LoadTexture("../pared.jpg", 64);
}

void Graphics::display()
{
    GLint position[4];
    GLfloat color[4];
    GLfloat material[4];

    glClearColor(255, 255, 255, 0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    PositionObserver(anglealpha,anglebeta,450);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-WIDTH*0.6,WIDTH*0.6,-HEIGHT*0.6,HEIGHT*0.6,10,2000);

    glMatrixMode(GL_MODELVIEW);

    glPolygonMode(GL_FRONT,GL_FILL);
    glPolygonMode(GL_BACK,GL_LINE);


    position[0]=0; position[1]=0; position[2]=0; position[3]=1;
    glLightiv(GL_LIGHT0,GL_POSITION,position);

    color[0]=0.1; color[1]=0.1; color[2]=0.1; color[3]=1;
    glLightfv(GL_LIGHT0,GL_DIFFUSE,color);
    glEnable(GL_LIGHT0);

    // spot pacman
    position[0]=s_map.pacman.y; position[1]=DEPTH/2; position[2]=-s_map.pacman.x; position[3]=1;
    glLightiv(GL_LIGHT1,GL_POSITION,position);

    color[0]=1; color[1]=1; color[2]=1; color[3]=1;
    glLightfv(GL_LIGHT1,GL_DIFFUSE,color);
    glLightf (GL_LIGHT1, GL_SPOT_CUTOFF,45.0f);
    glLightf (GL_LIGHT1, GL_SPOT_EXPONENT, 3.0f);

    glLightf(GL_LIGHT1,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT1,GL_LINEAR_ATTENUATION,0.0);
    glLightf(GL_LIGHT1,GL_QUADRATIC_ATTENUATION,0.0);

    glLightfv(GL_LIGHT1,GL_SPOT_DIRECTION, s_map.pacman.dir);

    glEnable(GL_LIGHT1);

    // spot main ghost
    position[0]=s_map.ghost.y; position[1]=DEPTH*2; position[2]=-s_map.ghost.x; position[3]=1;
    glLightiv(GL_LIGHT2,GL_POSITION,position);

    color[0]=1; color[1]=1; color[2]=1; color[3]=1;
    glLightfv(GL_LIGHT2,GL_DIFFUSE,color);
    glLightf (GL_LIGHT2, GL_SPOT_CUTOFF,45.0f);
    glLightf (GL_LIGHT2, GL_SPOT_EXPONENT, 3.0f);

    glLightf(GL_LIGHT2,GL_CONSTANT_ATTENUATION,1.0);
    glLightf(GL_LIGHT2,GL_LINEAR_ATTENUATION,0.0);
    glLightf(GL_LIGHT2,GL_QUADRATIC_ATTENUATION,0.0);

    glLightfv(GL_LIGHT2,GL_SPOT_DIRECTION, s_map.ghost.dir);

    glEnable(GL_LIGHT2);

    std::vector<std::vector<Cell> > grid = s_map.grid;

    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;


    readTextures();

    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            Cell* cell = &grid[real_i][j];

            material[0]=1.0; material[1]=1.0; material[2]=1.0; material[3]=1.0;
            glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);

            if (cell->isWall())
            {
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, eTextures::Wall);

                glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glTexCoord2f(0.0,0.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH,  i * cell_height - HEIGHT_2);
                glEnd();
                glDisable(GL_TEXTURE_2D);

                material[0]=0.0; material[1]=0.0; material[2]=0.0; material[3]=1.0;
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);

                glBegin(GL_QUADS);
                glNormal3f(1,0,0);
                glTexCoord2f(0.0,0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glEnd();


                glBegin(GL_QUADS);
                glNormal3f(0,0,1);
                glTexCoord2f(0.0,0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glEnd();

                glBegin(GL_QUADS);
                glNormal3f(0,0,-1);
                glTexCoord2f(0.0,0.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glEnd();

                glBegin(GL_QUADS);
                glNormal3f(-1,0,0);
                glTexCoord2f(0.0,0.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glEnd();

            }
            else
            {
                // Ground
                glEnable(GL_TEXTURE_2D);
                glBindTexture(GL_TEXTURE_2D, eTextures::Ground);
                glBegin(GL_QUADS);

                glNormal3f(0,1,0);
                glTexCoord2f(0.0,0.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glTexCoord2f(0.0,1.0);glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0,1.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glTexCoord2f(1.0, 0.0);glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);

                glEnd();
                glDisable(GL_TEXTURE_2D);

                if (!cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
                    continue;
                // Superior part
                material[0]=0.0; material[1]=1.0; material[2]=0.0; material[3]=1.0;
                glMaterialfv(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE,material);

                glBegin(GL_QUADS);
                glNormal3f(0,1,0);
                glVertex3i(j * cell_width - WIDTH_2 + cell_width4, DEPTH_4_3,  i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + ( 2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + ( 2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j * cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);

                glEnd();

                glBegin(GL_QUADS);
                glNormal3f(1,0,0);
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i * cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);

                glEnd();

                glBegin(GL_QUADS);
                glNormal3f(0,0,-1);
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);

                glEnd();


                glBegin(GL_QUADS);
                glNormal3f(-1,0,0);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i * cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));

                glEnd();

                glBegin(GL_QUADS);
                glNormal3f(0,0,1);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));

                glEnd();
            }
        }
    }

   s_map.pacman.draw();
   s_map.ghost.draw();

    for (auto & auto_ghost : s_map.auto_ghosts)
        auto_ghost.draw();

    glutSwapBuffers();
}

void Graphics::readJPEG(char *filename,unsigned char **image,int *width, int *height)
{
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;
    FILE * infile;
    unsigned char **buffer;
    int i,j;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);


    if ((infile = fopen(filename, "rb")) == NULL) {
        printf("Unable to open file %s\n",filename);
        exit(1);
    }

    jpeg_stdio_src(&cinfo, infile);
    jpeg_read_header(&cinfo, TRUE);
    jpeg_calc_output_dimensions(&cinfo);
    jpeg_start_decompress(&cinfo);

    *width = cinfo.output_width;
    *height  = cinfo.output_height;


    *image=(unsigned char*)malloc(cinfo.output_width*cinfo.output_height*cinfo.output_components);

    buffer=(unsigned char **)malloc(1*sizeof(unsigned char **));
    buffer[0]=(unsigned char *)malloc(cinfo.output_width*cinfo.output_components);


    i=0;
    while (cinfo.output_scanline < cinfo.output_height) {
        jpeg_read_scanlines(&cinfo, buffer, 1);

        for(j=0;j<cinfo.output_width*cinfo.output_components;j++)
        {
            (*image)[i]=buffer[0][j];
            i++;
        }

    }

    free(buffer);
    jpeg_finish_decompress(&cinfo);
}

void Graphics::LoadTexture(char *filename,int dim)
{
    unsigned char *buffer;
    unsigned char *buffer2;
    int width,height;
    long i,j;
    long k,h;

    readJPEG(filename,&buffer,&width,&height);

    buffer2=(unsigned char*)malloc(dim*dim*3);

    //-- The texture pattern is subsampled so that its dimensions become dim x dim --
    for(i=0;i<dim;i++)
        for(j=0;j<dim;j++)
        {
            k=i*height/dim;
            h=j*width/dim;

            buffer2[3*(i*dim+j)]=buffer[3*(k*width +h)];
            buffer2[3*(i*dim+j)+1]=buffer[3*(k*width +h)+1];
            buffer2[3*(i*dim+j)+2]=buffer[3*(k*width +h)+2];

        }

    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_MODULATE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim,dim,0,GL_RGB,GL_UNSIGNED_BYTE,buffer2);

    free(buffer);
    free(buffer2);
}

void Graphics::readPort() {
    /*enum Directions : unsigned char
    {
        UP = 'W',
        DOWN = 'S',
        LEFT = 'A',
        RIGHT = 'D'
    };*/
    // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
    int serial_port = open("/dev/ttyACM0", O_RDWR);

    // Create new termios struc, we call it 'tty' for convention
    struct termios tty;
    memset(&tty, 0, sizeof tty);

    // Read in existing settings, and handle any error
    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
    }

    tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    tty.c_cflag |= CS8; // 8 bits per byte (most common)
    tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
    // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
    // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

    tty.c_cc[VTIME] = 10;    // Wait for up to 1s (10 deciseconds), returning as soon as any data is received.
    tty.c_cc[VMIN] = 0;

    // Set in/out baud rate to be 9600
    cfsetispeed(&tty, B9600);
    cfsetospeed(&tty, B9600);

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
    }
    while(!s_map.pacmanLoses() && !s_map.pacmanWins()){
        char read_buf [256];
        memset(&read_buf, '\0', sizeof(read_buf));

        // Read bytes. The behaviour of read() (e.g. does it block?,
        // how long does it block for?) depends on the configuration
        // settings above, specifically VMIN and VTIME
        int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

        // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
        if (num_bytes < 0) {
            printf("Error reading: %s", strerror(errno));
        }

        // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
        // print it to the screen like this!)
        //printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
        unsigned char direction;
        if(read_buf[0] == 'L'){
            direction = Directions::LEFT;
            GhostMovement(direction);
        } else if(read_buf[0] == 'R'){
            direction = Directions::RIGHT;
            GhostMovement(direction);
        } else if(read_buf[0] == 'U') {
            direction = Directions::UP;
            GhostMovement(direction);
        } else if (read_buf[0] == 'D'){
            direction = Directions::DOWN;
            GhostMovement(direction);
        }
    }
    char read_buf [256];
    memset(&read_buf, '\0', sizeof(read_buf));

    // Read bytes. The behaviour of read() (e.g. does it block?,
    // how long does it block for?) depends on the configuration
    // settings above, specifically VMIN and VTIME
    int num_bytes = read(serial_port, &read_buf, sizeof(read_buf));

    // n is the number of bytes read. n may be 0 if no bytes were received, and can also be -1 to signal an error.
    if (num_bytes < 0) {
        printf("Error reading: %s", strerror(errno));
    }

    // Here we assume we received ASCII data, but you might be sending raw bytes (in that case, don't try and
    // print it to the screen like this!)
    //printf("Read %i bytes. Received message: %s", num_bytes, read_buf);
    unsigned char direction;
    if(read_buf[0] == 'L'){
        direction = Directions::LEFT;
        GhostMovement(direction);
    } else if(read_buf[0] == 'R'){
        direction = Directions::RIGHT;
        GhostMovement(direction);
    } else if(read_buf[0] == 'U') {
        direction = Directions::UP;
        GhostMovement(direction);
    } else if (read_buf[0] == 'D'){
        direction = Directions::DOWN;
        GhostMovement(direction);
    }

    /*if(strcmp("LEFT", read_buf) == 0)
        direction = Directions::LEFT;

    if(strcmp("RIGHT", read_buf) == 0)
        direction = Directions::RIGHT;

    if(strcmp("UP", read_buf) == 0)
        direction = Directions::UP;

    if(strcmp("DOWN", read_buf) == 0)
        direction = Directions::DOWN;*/


    close(serial_port);
}

void Graphics::idle()
{
  long t;
  bool joystick = true;

  t=glutGet(GLUT_ELAPSED_TIME);

  if(last_t==0)
    last_t=t;
  else
  {
    s_map.checkGameState(t, last_t);

    last_t = t;
  }
  if(joystick && first_time == 0){
      //std::thread t1(readPort);
      //t1(readPort);
      //t1.joinable(false);
      //readPort();
      first_time++;
  }

  glutPostRedisplay();
}

void Graphics::moveAutoGhosts(){
    for(int i=0; i < s_map.auto_ghosts.size(); i++){
        Character* ghost = &s_map.auto_ghosts[i];
        static std::vector<std::vector<int>> movements = direct;
        shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

        for (auto itr = movements.begin(); itr != movements.end();) {
            int i_offset = (*itr)[0];
            int j_offset = (*itr)[1];
            if ((i_offset + ghost->grid_x < 0) || (j_offset + ghost->grid_y < 0) || (i_offset + ghost->grid_x >= s_map.grid.size()) ||
                (j_offset + ghost->grid_y >= s_map.grid[0].size())) {
                itr++;
                continue;
            }

            Cell *cell = &s_map.grid[(int) ghost->grid_x + i_offset][(int) ghost->grid_y + j_offset];

            if (!cell->hasFlag(CellFlags::CELL_FLAG_WALL)) {

                ghost->grid_x = (int) cell->x;
                ghost->grid_y = (int) cell->y;

                float cell_width = (float) WIDTH / (float) s_columns;
                float cell_height = (float) HEIGHT / (float) s_rows;

                //ghost->initMovement(cell->x * cell_height, cell->y * cell_width, 1000);
                //ghost.setCell(cell);
                //s_map.pacman.initMovement(cell->x * cell_height, cell->y * cell_width, 1000);
                ghost->initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
                //s_map.pacman.setCell(cell);

                break;
            }
            itr++;
        }
    }
}

std::vector<int> Graphics::getPacmanNextPos(){
    Map map = s_map;
    Agents agents  = Agents();
    return agents.getAction(map, 3);
}

void Graphics::moveCharacters(int x){
    std::vector<int> random_action = getPacmanNextPos();
    movePacman(random_action);
    moveAutoGhosts();
    glutTimerFunc(1000, moveCharacters, 0);
}

void Graphics::movePacman(std::vector<int> random_action){

    /*std::vector<Character> all_characters = s_map.auto_ghosts;
    all_characters.push_back(s_map.ghost);
    all_characters.push_back(s_map.pacman);*/
    /*Map map = s_map;

    Agents agents  = Agents();
    std::vector<int> random_action = agents.getAction(map, 2);*/

    float cell_height = (float)HEIGHT / (float)s_rows;
    float cell_width = (float)WIDTH / (float)s_columns;

    //std::shuffle( final_actions.begin(), final_actions.end(), final_actions);
    //shuffle(final_actions.begin(), final_actions.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));
    //std::vector<int> random_action = final_actions[0];


    Cell* cell = &s_map.grid[(int)s_map.pacman.grid_x + random_action[0]][(int)s_map.pacman.grid_y + random_action[1]];
    s_map.pacman.grid_x = cell->x;
    s_map.pacman.grid_y = cell->y;
    s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
    s_map.pacman.setCell(cell);

    //s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
    //s_map.pacman.setCell(cell);

    if(s_map.total_food == 0)
      std::exit(0);
  /*Cell* pacman = &s_map.grid[(int)s_map.pacman.grid_x][(int)s_map.pacman.grid_y];;
  static std::vector<std::vector<int>> movements = direct;
      shuffle(movements.begin(), movements.end(), std::default_random_engine(std::chrono::system_clock::now().time_since_epoch().count()));

  for (auto itr = movements.begin(); itr != movements.end();)
  {
    int i_offset = (*itr)[0];
    int j_offset = (*itr)[1];
    if ((i_offset + pacman->x < 0) || (j_offset + pacman->y < 0) || (i_offset + pacman->x >= s_map.grid.size()) || (j_offset + pacman->y >= s_map.grid[pacman->x].size()))
    {
        itr++;
        continue;
    }

    Cell* cell = &s_map.grid[(int)s_map.pacman.grid_x + i_offset][(int)s_map.pacman.grid_y + j_offset];

    if(!cell->hasFlag(CellFlags::CELL_FLAG_WALL))
    {
        s_map.pacman.grid_x = cell->x;
        s_map.pacman.grid_y = cell->y;

        float cell_width = (float)WIDTH / (float)s_columns;
        float cell_height = (float)HEIGHT / (float)s_rows;

        if(s_map.pacman.hasFlag(CELL_FLAG_FOOD))
            s_map.total_food--;

        s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
        s_map.pacman.setCell(cell);

        if(s_map.total_food == 0)
            std::exit(0);

        break;
    }
    itr++;
  }*/
}

void Graphics::keyboard(unsigned char c, int x, int y)
{
    if (c == ' ')
    {
        s_map.reset();
        s_map.setWalls();

        glutPostRedisplay();
    }
    else if (c=='i' && anglebeta<=(90-4))
        anglebeta=(anglebeta+3);
    else if (c=='k' && anglebeta>=(-90+4))
        anglebeta=anglebeta-3;
    else if (c=='j')
        anglealpha=(anglealpha+3)%360;
    else if (c=='l')
        anglealpha=(anglealpha-3+360)%360;
    else
        GhostMovement(c);

    glutPostRedisplay();

}

void Graphics::GhostMovement(unsigned char c){
    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;

    /* To play with pacman
    if (s_map.pacman.state == QUIET){
        if(toupper(c) == Directions::UP && s_map.pacman.grid_x - 1 >= 0  && !s_map.grid[s_map.pacman.grid_x - 1][s_map.pacman.grid_y].isWall()){

            Cell* cell = &s_map.grid[s_map.pacman.grid_x - 1][s_map.pacman.grid_y];
            s_map.pacman.grid_x = cell->x;
            s_map.pacman.grid_y = cell->y;
            s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
            s_map.pacman.setCell(cell);

            s_map.pacman.dir[0] = 0;
            s_map.pacman.dir[1] = 0;
            s_map.pacman.dir[2] = 1;
        }
        if(toupper(c) == Directions::DOWN && s_map.pacman.grid_x + 1 < s_map.grid.size() && !s_map.grid[s_map.pacman.grid_x + 1][s_map.pacman.grid_y].isWall()) {

            Cell* cell = &s_map.grid[s_map.pacman.grid_x + 1][s_map.pacman.grid_y];
            s_map.pacman.grid_x = cell->x;
            s_map.pacman.grid_y = cell->y;
            s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
            s_map.pacman.setCell(cell);

            s_map.pacman.dir[0] = 0;
            s_map.pacman.dir[1] = 0;
            s_map.pacman.dir[2] = -1;
        }
        if(toupper(c) == Directions::LEFT && s_map.pacman.grid_y + 1 >= 0 && !s_map.grid[s_map.pacman.grid_x][s_map.pacman.grid_y + 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.pacman.grid_x][s_map.pacman.grid_y + 1];
            s_map.pacman.grid_x = cell->x;
            s_map.pacman.grid_y = cell->y;
            s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
            s_map.pacman.setCell(cell);

            s_map.pacman.dir[0] = 1;
            s_map.pacman.dir[1] = 0;
            s_map.pacman.dir[2] = 0;
        }
        if(toupper(c) == Directions::RIGHT && s_map.pacman.grid_y - 1 < s_map.grid[0].size() && !s_map.grid[s_map.pacman.grid_x][s_map.pacman.grid_y - 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.pacman.grid_x][s_map.pacman.grid_y - 1];
            s_map.pacman.grid_x = cell->x;
            s_map.pacman.grid_y = cell->y;
            s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
            s_map.pacman.setCell(cell);

            s_map.pacman.dir[0] = -1;
            s_map.pacman.dir[1] = 0;
            s_map.pacman.dir[2] = 0;
        }
    }*/

    if (s_map.ghost.state == QUIET){
        if(toupper(c) == Directions::UP && s_map.ghost.grid_x - 1 >= 0  && !s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y].isWall()){

            Cell* cell = &s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

            s_map.ghost.dir[0] = 0;
            s_map.ghost.dir[1] = 0;
            s_map.ghost.dir[2] = 1;
        }
        if(toupper(c) == Directions::DOWN && s_map.ghost.grid_x + 1 < s_map.grid.size() && !s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

            s_map.ghost.dir[0] = 0;
            s_map.ghost.dir[1] = 0;
            s_map.ghost.dir[2] = -1;
        }
        if(toupper(c) == Directions::LEFT && s_map.ghost.grid_y + 1 >= 0 && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

            s_map.ghost.dir[0] = 1;
            s_map.ghost.dir[1] = 0;
            s_map.ghost.dir[2] = 0;
        }
        if(toupper(c) == Directions::RIGHT && s_map.ghost.grid_y - 1 < s_map.grid[0].size() && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

            s_map.ghost.dir[0] = -1;
            s_map.ghost.dir[1] = 0;
            s_map.ghost.dir[2] = 0;
        }
    }
}

void Graphics::PositionObserver(float alpha,float beta,int radi)
{
    float x,y,z;
    float upx,upy,upz;
    float modul;

    x = (float)radi*cos(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);
    y = (float)radi*sin(beta*2*PI/360.0);
    z = (float)radi*sin(alpha*2*PI/360.0)*cos(beta*2*PI/360.0);

    if (beta>0)
    {
        upx=-x;
        upz=-z;
        upy=(x*x+z*z)/y;
    }
    else if(beta==0)
    {
        upx=0;
        upy=1;
        upz=0;
    }
    else
    {
        upx=x;
        upz=z;
        upy=-(x*x+z*z)/y;
    }


    modul=sqrt(upx*upx+upy*upy+upz*upz);

    upx=upx/modul;
    upy=upy/modul;
    upz=upz/modul;

    gluLookAt(x,y,z,    0.0, 0.0, 0.0,     upx,upy,upz);
}
