#include "ssd1306.h"
#include "graphics.h"
#include "ssd1306_fonts.h"

void drawLine(int x0, int y0, int x1, int y1){
	int x = x0, y = y0;
	int dx = abs_int(x1-x0);
	int dy = -abs_int(y1-y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;
	int e2;
	while(1){
		drawPixel(x,y);
		if(x == x1 && y == y1){ break; }
		e2 = 2*err;
		if(e2 >= dy){
			err += dy;
			x += sx;
		}
		if(e2 <= dy){
			err += dx;
			y += sy;
		}
	}
}

void eraseLine(int x0, int y0, int x1, int y1){
	int x = x0, y = y0;
	int dx = abs_int(x1-x0);
	int dy = -abs_int(y1-y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx + dy;
	int e2;
	while(1){
		erasePixel(x,y);
		if(x == x1 && y == y1){ break; }
		e2 = 2*err;
		if(e2 >= dy){
			err += dy;
			x += sx;
		}
		if(e2 <= dy){
			err += dx;
			y += sy;
		}
	}
}

//void erase(int x, int y, int w, int h){
//	for(int i = y; i < y + h; i++){
//		for(int j = x; j < x + w; j++){
//			erasePixel(j, i);
//		}
//	}
//}
//
//void erase(int xc, int yc, int r){
//
//}
//
//void erase(int x0, int y0, int x1, int y1, int x2, int y2){
//
//}

//draw outline of a rectangle
void drawRect(int x, int y, int w, int h){
  for(int LR = y; LR < y+h; LR++){
    drawPixel(x,LR);
    drawPixel(x + w - 1,LR);
  }
  for(int TB = x; TB < x + w; TB++){
    drawPixel(TB,y);
    drawPixel(TB,y + h - 1);
  }
}

//erase outline of rect
void eraseRect(int x, int y, int w, int h){
  for(int i = y; i < y+h; i++){
    erasePixel(x,i);
    erasePixel(x + w - 1,i);
  }
  for(int i = x; i < x + w; i++){
    erasePixel(i,y);
    erasePixel(i,y + h - 1);
  }
}

void fillRect(int x, int y, int w, int h){
	for(int i = y; i < y + h; i++){
		for(int j = x; j < x + w; j++){
			drawPixel(j, i);
		}
	}
}

void clearRect(int x, int y, int w, int h){
	for(int i = y; i < y + h; i++){
		for(int j = x; j < x + w; j++){
			erasePixel(j, i);
		}
	}
}

// Algorithm source:   https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
void drawCircle(int xc, int yc, int r){
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    circleUtil(xc, yc, x, y, 0);
    while (y >= x){
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else{
          d = d + 4 * x + 6;
        }
        x++;
        circleUtil(xc, yc, x, y, 0);
    }
}

// Algorithm source:   https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
void eraseCircle(int xc, int yc, int r){
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;
    circleUtil(xc, yc, x, y, 1);
    while (y >= x){
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        }
        else{
          d = d + 4 * x + 6;
        }
        x++;
        circleUtil(xc, yc, x, y, 1);
    }
}

void fillCircle(int xc, int yc, int r){
	//Do this===============================================================================
}

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
	drawLine(x0,y0,x1,y1);
	drawLine(x0,y0,x2,y2);
	drawLine(x2,y2,x1,y1);
}

void eraseTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
	eraseLine(x0,y0,x1,y1);
	eraseLine(x0,y0,x2,y2);
	eraseLine(x2,y2,x1,y1);
}

void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2){
	//Do this===============================================================================
}

int abs_int(int x){
	if(x>=0){ return x; }
	return (-1*x);
}

int min(int a, int b){
	if(a>b){ return b; }
	return a;
}

int max(int a, int b){
	if(a<b){ return b; }
	return a;
}

void circleUtil(int xc, int yc, int x, int y, char erase){
    if(erase == 0){
      drawPixel(xc+x, yc+y);
      drawPixel(xc-x, yc+y);
      drawPixel(xc+x, yc-y);
      drawPixel(xc-x, yc-y);
      drawPixel(xc+y, yc+x);
      drawPixel(xc-y, yc+x);
      drawPixel(xc+y, yc-x);
      drawPixel(xc-y, yc-x);
    }
    else{
      erasePixel(xc+x, yc+y);
      erasePixel(xc-x, yc+y);
      erasePixel(xc+x, yc-y);
      erasePixel(xc-x, yc-y);
      erasePixel(xc+y, yc+x);
      erasePixel(xc-y, yc+x);
      erasePixel(xc+y, yc-x);
      erasePixel(xc-y, yc-x);
    }

}
