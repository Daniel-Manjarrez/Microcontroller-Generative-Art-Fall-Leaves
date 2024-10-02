/**************************************************************************
  Draws concentric leaf shapes using anti-aliasing techniques
 **************************************************************************/
#include <TFT_eSPI.h> 
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI(); 

// Array of the colors of the leaf to cycle through
uint16_t colors[] = {
  TFT_GREEN,
  TFT_YELLOW,
  TFT_ORANGE,
  TFT_RED,
  TFT_PURPLE,
  TFT_BROWN
};

// Number of colors
const int numColors = sizeof(colors) / sizeof(colors[0]);

void setup() {
  tft.init();
  // 1 = landscape, 2 = portrait
  tft.setRotation(1);
}

void loop() {
  concentricLeaves();
}

// Function to draw a filled triangle
void fillTriangle(int x0, int y0, int x1, int y1, int x2, int y2, uint16_t color) {
  int a, b, y, last;

  // Sort the vertices by y-coordinate
  if (y0 > y1) { std::swap(y0, y1); std::swap(x0, x1); }
  if (y0 > y2) { std::swap(y0, y2); std::swap(x0, x2); }
  if (y1 > y2) { std::swap(y1, y2); std::swap(x1, x2); }

  // Process the triangle
  for (y = y0; y <= y2; y++) {
    // Y1 is the point where the left and right sides meet
    if (y == y1) last = x1;
    else last = x0 + (x2 - x0) * (y - y0) / (y2 - y0);

    // Draw a horizontal line from the left edge to the right edge
    tft.drawLine(x0 + (x1 - x0) * (y - y0) / (y1 - y0), y, last, y, color);
  }
}

// Function to draw a leaf shape (a combination of triangles)
void drawLeaf(int x, int y, int width, int height, uint16_t color) {
  // Draw the left side of the leaf
  tft.fillTriangle(x, y, x - width / 2, y + height / 2, x, y + height, color);
  
  // Draw the right side of the leaf
  tft.fillTriangle(x, y, x + width / 2, y + height / 2, x, y + height, color);
}

void concentricLeaves() {
  tft.fillScreen(TFT_DARKGREEN);
  // Counter for the number of draws done
  int drawCount = 0;
  // Starting index for color selection
  int colorIndex = 0;
  
  for (int16_t size = 0; size < tft.width(); size += 10) {
    // Coordinates for where to draw
    int x = tft.width() / 2;
    int y = tft.height() / 2;

    // Draw the leaf shape
    drawLeaf(x, y - 50, size, size, colors[colorIndex]);
    
    // Increment draw count
    drawCount++;

    // Change color of the leaf every 4 draws
    if (drawCount % 4 == 0) {
      // Update color index
      colorIndex = (colorIndex + 1) % numColors;
    }

    delay(500);
  }
}
