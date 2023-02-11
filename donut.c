#include <stdio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>

#define K 1
#define RADIUS 1
#define CENTRE 3
#define VIEW_DISTANCE 5
#define WIDTH 150
#define HEIGHT 50
#define PI 3.14159265359
#define A_SPACING 0.0700
#define B_SPACING 0.0350
#define THETA_SPACING 0.07
#define PHI_SPACING 0.02
#define DELAY_TIME 25000
#define DONUT_SIZE_X 30
#define DONUT_SIZE_Y 15

char buffer[HEIGHT][WIDTH] ;
float zbuffer[HEIGHT][WIDTH] ;
const char *colors[] =  { "\033[1;31m", "\033[1;32m", "\033[1;33m", "\033[1;34m", "\033[1;35m", "\033[1;36m" } ;

void display_color()
{
    printf("\x1b[H") ;
    for (int i = 0; i < HEIGHT; i++){
        for( int j = 0 ; j < WIDTH ; j++ )
            printf("%s%c\033[0m", colors[i%6], buffer[i][j]) ;
        putchar('\n') ;
    }
    usleep(DELAY_TIME);
}
void display()
{
    printf("\x1b[H") ;
    for (int i = 0; i < HEIGHT; i++){
        for( int j = 0 ; j < WIDTH ; j++ )
            putchar(buffer[i][j]) ;
        putchar('\n') ;
    }
    usleep(DELAY_TIME);
}

void render( float A, float B )
{
    float sinA = sin(A) ;
    float sinB = sin(B) ;
    float cosA = cos(A) ;
    float cosB  = cos(B) ;
    for ( float theta = 0 ; theta < 2*PI ; theta += THETA_SPACING ){
        float sinTheta = sin(theta) ;
        float cosTheta = cos(theta) ;
        for ( float phi = 0 ; phi < 2*PI; phi += PHI_SPACING ){
            float sinPhi = sin(phi) ;
            float cosPhi = cos(phi) ;
            float z = 1 / (sinPhi * (RADIUS*(cosTheta+CENTRE)) * sinA + sinTheta * cosA + VIEW_DISTANCE) ;
            int x = (WIDTH/2) + DONUT_SIZE_X * z * K * (cosPhi * (RADIUS*(cosTheta+CENTRE)) * cosB - (sinPhi * (RADIUS*(cosTheta+CENTRE)) * cosA - sinTheta * sinA) * sinB) ;
            int y = (HEIGHT/2) + DONUT_SIZE_Y * z * K * (cosPhi * (RADIUS*(cosTheta+CENTRE)) * sinB + (sinPhi * (RADIUS*(cosTheta+CENTRE)) * cosA - sinTheta * sinA) * cosB) ;
            int lum_index = 8 * ((sinTheta * sinA - sinPhi * cosTheta * cosA) * cosB - sinPhi * cosTheta * sinA - sinTheta * cosA - cosPhi * cosTheta * sinB) ;
            if( (y>0) && (y<HEIGHT) && (x>0) && (x<WIDTH) && (cosTheta > zbuffer[y][x]) ){
                zbuffer[y][x] = z ;
                buffer[y][x] = ".,-~:;=!*#$@"[(lum_index>0)?lum_index:0] ;
            }
        }
    }
}

int main( int argc, char ** argv )
{
    float A = 0, B = 0 ;
    printf("\x1b[2J") ;
    for(;;){
        memset(buffer, ' ', HEIGHT*WIDTH) ;
        memset(zbuffer, 0, sizeof(float)*WIDTH*HEIGHT) ;
        render(A,B) ;
        display() ;
        A += A_SPACING;
        B += B_SPACING;
    }
    
    return 0;
}
