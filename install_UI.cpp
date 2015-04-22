#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define DC2VO_IOC_MAGIC		'd'
#define DC2VO_SET_BG_SWAP	_IO(DC2VO_IOC_MAGIC, 19)

#define UPDATE_SCREEN(screen, x,y,w,h)	SDL_Flip(screen)

enum {
	FONT_SIZE = 28,
	// hint message position
	HINT_Y = 416,
	HINT_H = 26,

	// update percentage position
	RATIO_Y = 490,
	RATIO_H = 26,

	// progress bar position
	ICON_X = 532,
	ICON_Y = 454,
	ICON_SPACE = 8,
	ICON_NUM = 14,

	SCREEN_WIDTH = 1280,
	SCREEN_HEIGHT = 720
};

inline void setRect(SDL_Rect *rect, int x, int y, int w, int h)
{
	rect->x = x;
	rect->y = y;
	rect->w = w;
	rect->h = h;
}

int graphicInit()
{
	printf("set ARGB and do pan display in graphicInit()\n");
	int framebuffer_handler;
	framebuffer_handler = open("/dev/fb0", O_RDWR);
	if(framebuffer_handler == -1) {
		printf("[%s, %d] open /dev/fb0 failed!\n", __func__, __LINE__);
		return -1;
	}

	int bg_swap = 0;
	bg_swap = ioctl(framebuffer_handler, DC2VO_SET_BG_SWAP, &bg_swap);

	struct fb_var_screeninfo variable_info;
	ioctl (framebuffer_handler,FBIOGET_VSCREENINFO, &variable_info);
	variable_info.yoffset = 0;
	for(int i=0;i<10;i++)
		ioctl (framebuffer_handler,FBIOPAN_DISPLAY, &variable_info);

	close(framebuffer_handler);

	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
		printf("[%s, %d] failed: %s\n", __func__, __LINE__, SDL_GetError());
		return -1;
	}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("[%s, %d] failed: %s\n", __func__, __LINE__, IMG_GetError());
		return -1;
	}

	if(TTF_Init() != 0) {
		printf("[%s, %d] failed: %s\n", __func__, __LINE__, TTF_GetError());
		return -1;
	}

	return 0;
}

void graphicQuit()
{
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void* uiThread(void *arg)
{

	SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
	if( screen == NULL ) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		return 0;
	}
	SDL_ShowCursor(SDL_DISABLE);

	SDL_Rect rectDst;
	int iconW, iconH;

	SDL_Surface *image = IMG_Load("/screen/image/splash.png");
	iconW = image->w;
        iconH = image->h;
	//setRect( &rectDst, 290, 120,  iconW, iconH);
	setRect( &rectDst, 0, 0,  iconW, iconH);
	SDL_BlitSurface(image, NULL, screen, &rectDst);
	UPDATE_SCREEN(screen, 0, 0, 0, 0);
	SDL_Delay(2000);

	SDL_FreeSurface(image);

	//SDL_Quit();
	//graphicQuit();
	return 0;
}

int main(int argc, char* argv[])
{
	pthread_t threadID;
	if( graphicInit() !=0 ) {
		return -1;
	}

	if(pthread_create(&threadID, NULL, uiThread, 0) != 0)
	{
		printf("[%s:%d] can't create thread.\n", __FILE__, __LINE__);
		return -1;
	}
	pthread_join(threadID,NULL);   

	for(;access("/tmp/android_ready",F_OK) != 0;){

	}
	graphicQuit();
	return 0;
}
