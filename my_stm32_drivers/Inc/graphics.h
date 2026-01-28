#ifndef GRAPHICS_H        // If this macro is NOT defined
#define GRAPHICS_H        // Then define it

void drawPixel(int x, int y);

void erasePixel(int x, int y);

void drawLine(int x0, int y0, int x1, int y1);

void eraseLine(int x0, int y0, int x1, int y1);

//void erase(int x, int y, int w, int h);
//
//void erase(int xc, int yc, int r);
//
//void erase(int x0, int y0, int x1, int y1, int x2, int y2);

void drawRect(int x, int y, int w, int h);

void eraseRect(int x, int y, int w, int h);

void fillRect(int x, int y, int w, int h);

void clearRect(int x, int y, int w, int h);

// Algorithm source:   https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
void drawCircle(int xc, int yc, int r);

// Algorithm source:   https://www.geeksforgeeks.org/bresenhams-circle-drawing-algorithm/
void eraseCircle(int xc, int yc, int r);

void fillCircle(int xc, int yc, int r);

void drawTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

void eraseTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2);

int abs_int(int x);

int min(int a, int b);

int max(int a, int b);

void circleUtil(int xc, int yc, int x, int y, char erase);

#endif
