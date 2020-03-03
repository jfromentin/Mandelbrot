#include <iostream>
#include <SDL/SDL.h>



using namespace std;

#define WIDTH 800
#define HEIGHT 600

const double r=2.5/WIDTH;
const double dx=-2.5;
const double dy=-r/2*float(HEIGHT);

double cx=0;
double cy=0;
int step=1;
double zoom=1;
double zoom_factor=1.2;
int iterations=64;
float iterations_factor=1.5;
Uint32 (*coloration)(int);

SDL_Surface *screen;

inline Uint32 HSVtoRGB(unsigned char h,unsigned char s,unsigned char v){
  unsigned char r,g,b;
  
  unsigned char region, remainder, p, q, t;
  if(s == 0){
    r = v;
    g = v;
    b = v;
  }
  else{
    region = h / 43;
    remainder = (h - (region * 43)) * 6; 
    p = (v * (255 - s)) >> 8;
    q = (v * (255 - ((s * remainder) >> 8))) >> 8;
    t = (v * (255 - ((s * (255 - remainder)) >> 8))) >> 8;
    switch (region){
    case 0:
      r = v; g = t; b = p;
      break;
    case 1:
      r = q; g = v; b = p;
      break;
    case 2:
      r = p; g = v; b = t;
      break;
    case 3:
      r = p; g = q; b = v;
      break;
    case 4:
      r = t; g = p; b = v;
      break;
    default:
      r = v; g = p; b = q;
      break;
    }
  }
  return  SDL_MapRGB(screen->format, r, g, b);
}

inline Uint32 grey(int i){
  return 0x010101*((255*(iterations-i))/iterations);
}

inline Uint32 hsv1(int i){
  if(i==iterations) return 0;
  return HSVtoRGB(i%256,255,255);
}

inline void seahorse_tail(){
  step=1;
  cx=-0.7435669;
  cy=0.1314023;
  iterations=100;
  zoom=2500;
}

inline void refresh(){
  SDL_Flip(screen);  
}

inline void draw_pixel(int x,int y,int v){
  Uint32 c=coloration(v);
  SDL_Rect rect={x,y,step,step};
  SDL_FillRect(screen,&rect,c);
}

void draw_Mandelbrot(){
  cout<<"------- Draw Mandelbrot --------"<<endl;
  cout<<"> center = "<<cx;
  if(cy>=0) cout<<"+"<<cy;
  else cout<<cy;
  cout<<"i"<<endl;
  cout<<"> zoom = "<<zoom<<endl; 
  cout<<"> step = "<<step<<endl;
  cout<<"> iterations = "<<iterations<<endl; 
  int X,Y;
  double x0,y0,x,y,t;
  int i;
  for(X=0;X<WIDTH;X+=step){
    for(Y=0;Y<=HEIGHT;Y+=step){
      x0=((X-WIDTH/2)*r)/zoom+cx;
      y0=((Y-HEIGHT/2)*r)/zoom+cy;
      i=0;
      x=0;
      y=0;
      while(x*x+y*y<4 and i<iterations){
	t=x;
	x=x*x-y*y+x0;
	y=2*t*y+y0;
	++i;
      }
      draw_pixel(X,Y,i);
    }
  }
  refresh();
  cout<<"--------------------------------"<<endl;
}


int main(){
  SDL_Init(SDL_INIT_VIDEO);
  screen=SDL_SetVideoMode(WIDTH,HEIGHT,32,SDL_HWSURFACE | SDL_DOUBLEBUF);
  if(screen==NULL){
    cerr << "SDL_SetVideoMode() Failed: " << SDL_GetError() << endl;
    SDL_Quit();
    exit(1);
  }
  refresh();
  SDL_Event event;
  //coloration=grey;
  coloration=hsv1;
  //seahorse_tail();
  draw_Mandelbrot();
  bool stop=false;
  while(not stop){
    SDL_WaitEvent(&event); //Wait for event
    switch(event.type){
    case SDL_QUIT:
      stop=true;
      break;
    case SDL_KEYDOWN: //Keyboard
      switch(event.key.keysym.sym){
      case SDLK_a:
	if(step>1) --step;
	draw_Mandelbrot();
	break;
      case SDLK_z:
	++step;
	draw_Mandelbrot();
	break;
      case SDLK_q:
	iterations/=iterations_factor;
	if(iterations<1) iterations=1;
	draw_Mandelbrot();
	break;
      case SDLK_s:
	iterations*=iterations_factor;
	draw_Mandelbrot();
	break;
      case SDLK_KP_PLUS:
      case SDLK_PLUS:
	zoom*=zoom_factor;
	draw_Mandelbrot();
	break;
      case SDLK_KP_MINUS:
      case SDLK_MINUS:
	zoom/=zoom_factor;
	if(zoom<0.5) zoom=0.5;
	draw_Mandelbrot();
	break;
      default:
	break;
      }
      break;
    case SDL_MOUSEBUTTONDOWN:
      cx=((event.motion.x-WIDTH/2)*r)/zoom+cx;
      cy=((event.motion.y-HEIGHT/2)*r)/zoom+cy;
  
      draw_Mandelbrot();
      break;
    default:
      break;
    }
    while(SDL_PollEvent(&event)) { }
  }
  SDL_Quit();
  return 0;
}
