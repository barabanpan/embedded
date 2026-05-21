#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ray_casting.h>
#include <vector>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define STEP_LENGTH 0.3
#define CAMERA_TURN_ANGLE_STEP_DEGREES 15

// pin 5 gave error "adc unit not supported"
#define LEFTpin 4
#define DOWNpin 18
#define RIGHTpin 6
#define UPpin 7

#define LEFT_CAMERA_TURN_pin 3
#define RIGHT_CAMERA_TURN_pin 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);


int n = 12;
int m = 12;
std::vector<bool> grid = {  // n*m
  1,1,1,1,1,1,1,1,1,1,1,1,  // 0
  1,0,0,0,1,0,0,1,0,0,0,1,  // 1
  1,0,1,1,1,1,0,1,0,1,0,1,  // 2
  1,0,0,0,0,1,0,0,0,1,1,1,  // 3
  1,0,0,0,0,0,0,0,0,0,0,1,  // 4
  1,0,0,0,1,0,0,0,0,1,0,1,  // 5
  1,1,0,1,1,1,1,1,1,1,0,1,  // 6
  1,0,0,0,1,0,0,0,0,1,0,1,  // 7
  1,0,0,0,1,0,1,0,0,1,0,1,  // 8
  1,0,0,0,1,0,0,0,0,1,0,1,  // 9
  1,0,0,0,1,0,0,0,0,0,0,1,  // 10
  1,1,1,1,1,1,1,1,1,1,1,1   // 11
};
Point playerPos = {2.5, 10.5};
double angle = -90;  // in relation to X axis, -90 means looking up


int n2 = 6;
int m2 = 8;
std::vector<bool> grid2 = {  // n*m
  1,1,1,1,1,1,1,1,
  1,1,0,0,0,0,0,1,
  1,0,0,1,1,1,1,1,
  1,1,0,0,0,1,0,1,
  1,1,0,0,0,0,0,1,
  1,1,1,1,1,1,1,1
};
std::vector<RayResult> rayResults;
Point playerPos2 = {3.5, 4.5};
double angle2 = -90;  // in relation to X axis, -90 means looking up

const int height = SCREEN_HEIGHT;  // 15;
const int halfRaysN = 63;  // 30 // will have (2*halfRaysN+1) rays
 

void render(std::vector<RayResult>& res) {
    // from heights to arrays of pixels/symbols
    display.clearDisplay();

    int j = 0;
    for (const RayResult& r : res) {
        int ceilingH = (SCREEN_HEIGHT - r.h) / 2;
        int floorH = r.h + (SCREEN_HEIGHT - r.h) / 2;
        if (r.is_ver) {  // just draw a solid line
            display.drawLine(j,ceilingH,j,floorH, WHITE);
        } else {  // chess board pattern (Dithering)
            for (int i=ceilingH; i<=floorH; i++) {
                if ((i + j) % 2 == 0) display.drawPixel(j, i, WHITE);
            }
        }            
        j++;
    }
    display.display();
}

void render_print(std::vector<RayResult>& res) {
    // from heights to arrays of pixels/symbols
    for (int i=0; i<SCREEN_HEIGHT; i++) {
      std::vector<char> row;
        for (const RayResult r : res) {
            int ceilingH = (SCREEN_HEIGHT - r.h) / 2;
            char ch;
            if (i < ceilingH) ch = ' ';   // ceiling
            else if (i < ceilingH + r.h) ch = (r.is_ver ? 'v' : '.');
            else ch = ' ';  // floor
            row.push_back(ch);
        }
        for (char& c : row) {
            Serial.print(c);
        }
        Serial.println();
    }  
}

void print_debug(std::vector<RayResult>& res) {
  for (RayResult& r : rayResults) {
      Serial.print(r.h);
      Serial.print(" ");
      Serial.print(r.is_ver ? "VER" : "HOR");
      Serial.print(" angle=");
      Serial.print(r.a_i);
      Serial.print(" dist=");
      Serial.println(r.dist);
  }
}

void setup() {
  pinMode(LEFTpin, INPUT);
  pinMode(DOWNpin, INPUT);
  pinMode(RIGHTpin, INPUT);
  pinMode(UPpin, INPUT);

  pinMode(LEFT_CAMERA_TURN_pin, INPUT);
  pinMode(RIGHT_CAMERA_TURN_pin, INPUT);

  Serial.begin(9600);
  Wire.begin(8, 9);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();

  rayResults.reserve(2*halfRaysN+1);
  getRows(grid, n, m, angle, playerPos.x, playerPos.y, height, halfRaysN, rayResults);
  
  //print_debug(rayResults);
  render(rayResults);
  //render_print(rayResults);
}

void pp(Point p) {
  Serial.print("NEW x,y = ");
  Serial.print(p.x);
  Serial.print(", ");
  Serial.println(p.y);
}

void move(double step_angle) {
  playerPos = step(grid, m, playerPos, step_angle, STEP_LENGTH);
  pp(playerPos);
  getRows(grid, n, m, angle, playerPos.x, playerPos.y, height, halfRaysN, rayResults);
  render(rayResults);
}

void turn(double new_angle) {
  angle = (int)std::round(new_angle) % 360;  // keep angle in range 0-359
  getRows(grid, n, m, new_angle, playerPos.x, playerPos.y, height, halfRaysN, rayResults);
  render(rayResults);
}

void loop() {
  if (digitalRead(LEFTpin) == HIGH) {
    Serial.print("<-LEFT   ");
    move(angle - 90);
  }
  if (digitalRead(RIGHTpin) == HIGH) {
    Serial.print("->RIGHT   ");
    move(angle + 90);
  }
  if (digitalRead(DOWNpin) == HIGH) {
    Serial.print("v DOWN   ");
    move(angle + 180);
  }
  if (digitalRead(UPpin) == HIGH) {
    Serial.print("^ UP   ");
    move(angle);
  }

  if (digitalRead(LEFT_CAMERA_TURN_pin) == HIGH) {
    Serial.print("<-Turn CAMERA to the LEFT   new a=");
    Serial.println(angle - CAMERA_TURN_ANGLE_STEP_DEGREES);
    turn(angle - CAMERA_TURN_ANGLE_STEP_DEGREES);
  }

  if (digitalRead(RIGHT_CAMERA_TURN_pin) == HIGH) {
    Serial.print("Turn CAMERA to the RIGHT->    new a=");
    Serial.println(angle + CAMERA_TURN_ANGLE_STEP_DEGREES);
    turn(angle + CAMERA_TURN_ANGLE_STEP_DEGREES);
  }

  delay(100);
}