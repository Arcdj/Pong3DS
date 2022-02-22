// Pong 3DS by @Arcdj#8451 on Discord
// Built upon the "2d_shapes" example from DevkitPro
#include "./include/citro2d.h"
#include "./include/3ds.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>


#define SCREEN_WIDTH  400
#define SCREEN_HEIGHT 240

//---------------------------------------------------------------------------------
int main(int argc, char* argv[]) {
//---------------------------------------------------------------------------------
	// Init libs
	gfxInitDefault();
	C3D_Init(C3D_DEFAULT_CMDBUF_SIZE);
	C2D_Init(C2D_DEFAULT_MAX_OBJECTS);
	C2D_Prepare();
	mcuHwcInit();
	consoleInit(GFX_BOTTOM, NULL);
	

	// Create screens
	C3D_RenderTarget* top = C2D_CreateScreenTarget(GFX_TOP, GFX_LEFT);

	// Create colors
	u32 clrWhite = C2D_Color32(0xFF, 0xFF, 0xFF, 0xFF);
	//u32 clrRed = C2D_Color32(0xFF, 0x00, 0x00, 0xFF);
	u32 clrClear = C2D_Color32(0x00, 0x00, 0x00, 0xFF);

	// Create variables for positions of objects
	u8    paddleL;
	float paddleR;

	u16 ballX = 190;
	u8  ballY = 110;

	// Create booleans for the direction of the ball
	bool ballLeft = false;
	bool ballUp   = false;
	// And death
	bool fail = false;

	u8 leftFail  = 0;
	u8 rightFail = 0;

	// Main loop
	while (aptMainLoop())
	{
		hidScanInput();

		// Respond to user input
		u32 kDown = hidKeysDown();
		if (kDown & KEY_START)
			break; // break in order to return to hbmenu
		
		// Get value of sliders and convert them to numbers from 0 to 150
		MCUHWC_GetSoundSliderLevel(&paddleL);
		paddleL = ((paddleL*2.39)-150)*-1;
		
		paddleR = (((osGet3DSliderState()-.1)*230)-150)*-1;
		if (paddleR>150){paddleR=150;}else if (paddleR<0){paddleR=0;}

		// Move ball
		if(ballUp){
			if(ballY==0){
				ballUp=false;
				ballY+=2;
			}else{
				ballY-=2;
			}
		}else{
			if(ballY==220){
				ballUp=true;
				ballY-=2;
			}else{
				ballY+=2;
			}
		}

		// x

		
		if(fail){
			if(ballLeft){
				if(ballX==0){
					leftFail=255;
					ballX=190;
					fail=false;
					ballLeft=false;
				}else{
					ballX-=2;
				}
			}else{
				if(ballX==380){
					rightFail=255;
					ballX=190;
					fail=false;
					ballLeft=true;
				}else{
					ballX+=2;
				}
			}
		}else{
			if(ballLeft){
				if(ballX==20){
					if((ballY+21)>paddleL && ballY<(paddleL+91)){
						ballLeft=false;
						ballX+=2;
					}else{
						fail=true;
						ballX-=2;
					}
				}else{
					ballX-=2;
				}
			}else{
				if(ballX==360){
					if((ballY+21)>paddleR && ballY<(paddleR+91)){
						ballLeft=true;
						ballX-=2;
					}else{
						fail=true;
						ballX+=2;
					}
				}else{
					ballX+=2;
				}
			}
		}

		
		// Debug output
		printf("\x1b[1;1HPong");
		printf("\x1b[3;1HCPU:     %6.2f%%\x1b[K", C3D_GetProcessingTime()*6.0f);
		printf("\x1b[4;1HGPU:     %6.2f%%\x1b[K", C3D_GetDrawingTime()*6.0f);
		printf("\x1b[5;1HCmdBuf:  %6.2f%%\x1b[K", C3D_GetCmdBufUsage()*100.0f);
		printf("\x1b[7;1HpaddleL: %i\x1b[K", paddleL);
		printf("\x1b[8;1HpaddleR: %f\x1b[K", paddleR);
		printf("\x1b[10;1HballX:  %i\x1b[K", ballX);
		printf("\x1b[11;1HballY:  %i\x1b[K", ballY);


		// Render the scene
		C3D_FrameBegin(C3D_FRAME_SYNCDRAW);
		C2D_TargetClear(top, clrClear);
		C2D_SceneBegin(top);

		// The paddles,
		C2D_DrawRectSolid(0, paddleL, 1, 20, 90, clrWhite);
		C2D_DrawRectSolid(380, paddleR, 1, 20, 90, clrWhite);

		// and the "ball"
		C2D_DrawRectSolid(ballX, ballY, 1, 20, 20, clrWhite);

		// Screen effect in case someone had an oopsie
		if(leftFail){
			C2D_DrawRectangle(0, 0, 0, 200, 240, C2D_Color32(leftFail, 0x00, 0x00, 0xFF), clrClear, C2D_Color32(leftFail, 0x00, 0x00, 0xFF), clrClear);
			leftFail--;
		}
		if(rightFail){
			C2D_DrawRectangle(200, 0, 0, 200, 240, clrClear, C2D_Color32(rightFail, 0x00, 0x00, 0xFF), clrClear, C2D_Color32(rightFail, 0x00, 0x00, 0xFF));
			rightFail--;
		}


		C3D_FrameEnd(0);
	}

	// Deinit libs
	C2D_Fini();
	C3D_Fini();
	gfxExit();
	mcuHwcExit();
	return 0;
}
