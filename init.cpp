#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <linux/fb.h>
#include <sys/ioctl.h>

#define UPDATE_IMG_BG 		0
#define UPDATE_IMG_BG_OK 	(UPDATE_IMG_BG+1)
#define UPDATE_IMG_BG_FAIL 	(UPDATE_IMG_BG+2)
#define UPDATE_IMG_FOCUS 	(UPDATE_IMG_BG+3)
#define UPDATE_IMG_UNFOCUS 	(UPDATE_IMG_BG+4)
#define UPDATE_IMG_NUM 		(UPDATE_IMG_BG+5)

#define STAND_ALONE_AP

#ifdef STAND_ALONE_AP
	#define DIR_PATH	"res/"
#else
	#define	DIR_PATH	"/tmp/installUI/res/"
#endif

//#define UPDATE_SCREEN(screen, x,y,w,h)	SDL_UpdateRect(screen, x,y,w,h)
#define UPDATE_SCREEN(screen, x,y,w,h)	SDL_Flip(screen)

const char * imageFilename[] = {
	DIR_PATH "update_bg.png",
	DIR_PATH "update_bg_ok.png",
	DIR_PATH "update_bg_fail.png",
	DIR_PATH "update_focus.png",
	DIR_PATH "update_unfocus.png"
};

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

const char* ttf_filename = DIR_PATH "DroidSans.ttf";

int main(int argc, char *argv[])
{
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0){
		printf("[%s,%d] failed!\n", __func__, __LINE__);
		printf("----- %s -----\n",SDL_GetError());
		return -1;
	}

	if((IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG) != IMG_INIT_PNG) {
		printf("[%s,%d] failed!\n", __func__, __LINE__);
		return -1;
	}

	if(TTF_Init() != 0) {
		printf("[%s,%d] failed!\n", __func__, __LINE__);
		return -1;
	}

	printf("SDL init OK\n");

#if 0
	printf("do pan display\n");
	// do pan display
	 int framebuffer_handler;
	//struct fb_fix_screeninfo fixed_info;
	struct fb_var_screeninfo variable_info;
	framebuffer_handler = open("/dev/fb0", O_RDWR);
	if(framebuffer_handler == -1) {
	    printf("\nstanely> [%s, %d] open /dev/fb0 failed!\n", __func__, __LINE__);
	}
	ioctl (framebuffer_handler,FBIOGET_VSCREENINFO, &variable_info);
	//ioctl(framebuffer_handler, FBIOPUT_VSCREENINFO, &variable_info);
	//ioctl (framebuffer_handler,FBIOGET_FSCREENINFO, &fixed_info);
	variable_info.yoffset = 0;
	ioctl (framebuffer_handler,FBIOPAN_DISPLAY, &variable_info);
#else
	printf("do pan display using new mechanism\n");
	int framebuffer_handler;
	int i;
	//struct fb_fix_screeninfo fixed_info;
	struct fb_var_screeninfo variable_info;
	framebuffer_handler = open ("/dev/fb0", O_RDWR);
	if(framebuffer_handler == -1) {
	    printf("\nstanely> [%s, %d] open /dev/fb0 failed!\n", __func__, __LINE__);
	}
	ioctl (framebuffer_handler,FBIOGET_VSCREENINFO, &variable_info);
	//ioctl(framebuffer_handler, FBIOPUT_VSCREENINFO, &variable_info);
	//ioctl (framebuffer_handler,FBIOGET_FSCREENINFO, &fixed_info);
	variable_info.yoffset = 0;
	for(i=0;i<10;i++)
	    ioctl (framebuffer_handler,FBIOPAN_DISPLAY, &variable_info);
#endif

	//SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_HWSURFACE|SDL_FULLSCREEN);
	SDL_Surface *screen = SDL_SetVideoMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32, SDL_SWSURFACE);
	if( screen == NULL ) {
		printf("Couldn't set video mode: %s\n", SDL_GetError());
		return -1;
	}
	SDL_ShowCursor(SDL_DISABLE);

	printf("screen---------\n");
	printf("Mask,A(%#x)R(%#x)G(%#x)B(%#x)\n", screen->format->Amask,screen->format->Rmask,screen->format->Gmask,screen->format->Bmask);
	printf("Shift,A(%#x)R(%#x)G(%#x)B(%#x)\n", screen->format->Ashift,screen->format->Rshift,screen->format->Gshift,screen->format->Bshift);
	printf("Loss,A(%#x)R(%#x)G(%#x)B(%#x)\n", screen->format->Aloss,screen->format->Rloss,screen->format->Gloss,screen->format->Bloss);
	printf("colorkey(%#x),alpha(%#x)\n", screen->format->colorkey,screen->format->alpha);
	printf("flags(%#x),BitsPerPixel(%#x),BytesPerPixel(%#x)\n",screen->flags, screen->format->BitsPerPixel,screen->format->BytesPerPixel);

	SDL_Surface *image = IMG_Load(imageFilename[UPDATE_IMG_BG]);
	if( image == NULL ) {
		printf("Couldn't load image: %s\n", IMG_GetError());
		return -1;
	}
	printf("png image.....\n");
	printf("Mask,A(%#x)R(%#x)G(%#x)B(%#x)\n", image->format->Amask,image->format->Rmask,image->format->Gmask,image->format->Bmask);
	printf("Shift,A(%#x)R(%#x)G(%#x)B(%#x)\n", image->format->Ashift,image->format->Rshift,image->format->Gshift,image->format->Bshift);
	printf("Loss,A(%#x)R(%#x)G(%#x)B(%#x)\n", image->format->Aloss,image->format->Rloss,image->format->Gloss,image->format->Bloss);
	printf("colorkey(%#x),alpha(%#x)\n", image->format->colorkey,image->format->alpha);
	printf("flags(%#x),BitsPerPixel(%#x),BytesPerPixel(%#x)\n",image->flags, screen->format->BitsPerPixel,screen->format->BytesPerPixel);

	if(SDL_BlitSurface(image, NULL, screen, NULL) < 0)
		printf("BlitSurface error: %s\n", SDL_GetError());

	SDL_FreeSurface(image);

	TTF_Font *font = TTF_OpenFont(ttf_filename, FONT_SIZE);
	if(font == NULL) {
		printf("TTF_OpenFont: %s\n", TTF_GetError());
		TTF_CloseFont(font);
		return -1;
	}

	SDL_Color colorHint = {0xFF, 0xFF, 0xFF, 0x28};
	SDL_Surface *text = TTF_RenderUTF8_Blended(font, "5566", colorHint);
	if(text == NULL) {
		printf("[%s, %d] TTF_RenderUTF8_Blended failed!\n", __func__, __LINE__);
		return -1;
	}
	printf("text.....\n");
	printf("Mask,A(%#x)R(%#x)G(%#x)B(%#x)\n", text->format->Amask,text->format->Rmask,text->format->Gmask,text->format->Bmask);
	printf("Shift,A(%#x)R(%#x)G(%#x)B(%#x)\n", text->format->Ashift,text->format->Rshift,text->format->Gshift,text->format->Bshift);
	printf("Loss,A(%#x)R(%#x)G(%#x)B(%#x)\n", text->format->Aloss,text->format->Rloss,text->format->Gloss,text->format->Bloss);
	printf("colorkey(%#x),alpha(%#x)\n", text->format->colorkey,text->format->alpha);
	printf("flags(%#x),BitsPerPixel(%#x),BytesPerPixel(%#x)\n",text->flags, screen->format->BitsPerPixel,screen->format->BytesPerPixel);

	SDL_BlitSurface(text,NULL,screen,NULL);
	SDL_FreeSurface(text);

	//printf("save to /tmp/png.bmp\n");
	//SDL_SaveBMP(screen, "/tmp/png.bmp");
	printf("UPDATE_SCREEN\n");
	UPDATE_SCREEN(screen, 0, 0, 0, 0);

	printf("Press any key to draw Rectangle\n");
	getchar();

	Uint32 r = SDL_MapRGBA(screen->format, 0xff, 0, 0, 0xff);
	Uint32 g = SDL_MapRGBA(screen->format, 0, 0xff, 0, 0xff);
	Uint32 b = SDL_MapRGBA(screen->format, 0, 0, 0xff, 0xff);
	printf("----- r(%#x) g(%#x) b(%#x)\n", r,g,b);

	SDL_Rect rect;
	rect.x = 0;
	rect.y = 0;
	rect.w = 100;
	rect.h = 100;
	SDL_FillRect(screen, &rect, r);

	rect.x = 100;
	rect.y = 100;
	rect.w = 200;
	rect.h = 200;
	SDL_FillRect(screen, &rect, g);

	rect.x = 300;
	rect.y = 300;
	rect.w = 200;
	rect.h = 200;
	SDL_FillRect(screen, &rect, b);
	UPDATE_SCREEN(screen, 0, 0, 0, 0);

	printf("Press any key to end\n");
	getchar();

	TTF_CloseFont(font);
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();

	return 0;
}
