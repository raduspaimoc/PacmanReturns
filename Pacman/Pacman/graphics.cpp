#include <GL/glut.h>
#include <iostream>
#include <algorithm>    // std::shuffle
#include <random>       // std::default_random_engine
#include <chrono>       // std::chrono::system_clock
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "./jpeg-6b/jpeglib.h"
#include "graphics.h"
//#include "./jpeg-6b"
#include "ShareDefines.h"
#include "Utils.h"

long last_t = 0;
int anglealpha = -90;
int anglebeta = 0;

void Graphics::display()
{
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


    std::vector<std::vector<Cell> > grid = s_map.grid;

    float cell_width = (float)WIDTH / (float)s_columns;
    float cell_height = (float)HEIGHT / (float)s_rows;
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;


    for (int i = 0; i < s_rows; i++)
    {
        for (int j = 0; j < s_columns; j++)
        {
            int real_i = s_rows - i - 1;

            Cell* cell = &grid[real_i][j];

            if (cell->isWall())
            {
                glColor3f(0.0, 0.0, 255); // Superior part
                glBegin(GL_QUADS);

                glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH,  i * cell_height - HEIGHT_2);
                glEnd();

                glColor3f(0, 0, 0);

                glBegin(GL_QUADS);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);

                glEnd();

                glBegin(GL_QUADS);

                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);

                glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);

                glEnd();


                glBegin(GL_QUADS);
                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);

                glEnd();


                glBegin(GL_QUADS);

                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glEnd();
            }
            else
            {
                // Ground
                glColor3f(0.15, 0.15, 0.15);
                glBegin(GL_QUADS);
                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);
                glVertex3i(j * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, (i + 1) * cell_height - HEIGHT_2);
                glVertex3i((j + 1) * cell_width - WIDTH_2, -DEPTH, i * cell_height - HEIGHT_2);

                glEnd();

                if (!cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
                    continue;

                // Superior part
                glColor3f(0, 1, 0);
                glBegin(GL_QUADS);

                glVertex3i(j * cell_width - WIDTH_2 + cell_width4, DEPTH_4_3,  i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + ( 2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + ( 2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j * cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);

                glEnd();

                glBegin(GL_QUADS);
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i * cell_height - HEIGHT_2 + (2 * cell_height4));
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i * cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j * cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i * cell_height - HEIGHT_2 + cell_height4);

                glEnd();

                glBegin(GL_QUADS);
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);

                glEnd();


                glBegin(GL_QUADS);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + cell_height4);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i * cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));

                glEnd();

                glBegin(GL_QUADS);
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + cell_width4, DEPTH_4, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4, i* cell_height - HEIGHT_2 + (2 * cell_width4));
                glVertex3i(j* cell_width - WIDTH_2 + (2 * cell_width4), DEPTH_4_3, i* cell_height - HEIGHT_2 + (2 * cell_width4));

                glEnd();
            }

            /*

            if (cell->hasFlag(CellFlags::CELL_FLAG_EMPTY))
                glColor3f(0.0, 0.0, 0.0);
            if (cell->hasFlag(CellFlags::CELL_FLAG_FOOD))
                glColor3f(0.0, 1.0, 1.0);
            else
                continue;
*/
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
    glTexEnvi(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_REPLACE);
    glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,dim,dim,0,GL_RGB,GL_UNSIGNED_BYTE,buffer2);

    free(buffer);
    free(buffer2);
}

void Graphics::idle()
{
  long t;

  t=glutGet(GLUT_ELAPSED_TIME);

  if(last_t==0)
    last_t=t;
  else
  {
    s_map.pacman.integrate(t-last_t);
    s_map.ghost.integrate(t-last_t);

    int pacman_x = (int) s_map.pacman.x;
    int pacman_y = (int) s_map.pacman.y;
    int ghost_x = (int) s_map.ghost.x;
    int ghost_y = (int) s_map.ghost.y;

    if (pacman_x == ghost_x && pacman_y == ghost_y)
        std::exit(0);

    for (auto & auto_ghost : s_map.auto_ghosts)
    {
        ghost_x = (int) s_map.ghost.x;
        ghost_y = (int) s_map.ghost.y;

        if (pacman_x == ghost_x && pacman_y == ghost_y)
            std::exit(0);

        auto_ghost.integrate(t-last_t);
    }

    last_t = t;
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

                ghost->grid_x = cell->x;
                ghost->grid_y = cell->y;

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

void Graphics::moveCharacters(int x){
    movePacman(x);
    moveAutoGhosts();
    glutTimerFunc(1000, moveCharacters, 0);
}

void Graphics::movePacman(int t){

  Cell* pacman = &s_map.grid[(int)s_map.pacman.grid_x][(int)s_map.pacman.grid_y];;
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

        /*
         j * cell_width - WIDTH_2, DEPTH, i * cell_height - HEIGHT_2
        */
        //s_map.pacman.initMovement(cell->x * cell_height, cell->y * cell_width, 1000);
        s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
        s_map.pacman.setCell(cell);

        break;
    }
    itr++;
  }
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
    float cell_width4 = cell_width / 4;
    float cell_height4 = cell_height / 4;
    //s_map.pacman.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);
    //int au = s_map.ghost.grid_x;
    //int v = s_map.ghost.grid_y;
    //Cell aux = s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y];

    if (s_map.ghost.state == QUIET){
        if(toupper(c) == Directions::UP && s_map.ghost.grid_x - 1 >= 0  && !s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y].isWall()){

            Cell* cell = &s_map.grid[s_map.ghost.grid_x - 1][s_map.ghost.grid_y];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

        }
        if(toupper(c) == Directions::DOWN && s_map.ghost.grid_x + 1 < s_map.grid.size() && !s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x + 1][s_map.ghost.grid_y];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

        }
        if(toupper(c) == Directions::LEFT && s_map.ghost.grid_y + 1 >= 0 && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y + 1];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

        }
        if(toupper(c) == Directions::RIGHT && s_map.ghost.grid_y - 1 < s_map.grid[0].size() && !s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1].isWall()) {

            Cell* cell = &s_map.grid[s_map.ghost.grid_x][s_map.ghost.grid_y - 1];
            s_map.ghost.grid_x = cell->x;
            s_map.ghost.grid_y = cell->y;
            s_map.ghost.initMovement(cell->x * cell_width - WIDTH_2, cell->y * cell_height - HEIGHT_2, 1000);

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
