#include <Adafruit_NeoPixel.h>

// Pin settings
const int analogPin = 2;
const int barPin = 3;
const int elevatorDoorLEDPin = 4; 

/* LED Colors */
int red = 255;
int green = 165;
int blue = 0;

/* State machine */
enum { 
    INOPERANTE,
    OCIOSO,
    ALINHADO_S,
    MOVENDO_S,
    ESTACIONADO_S,
    FIM_S,
    ALINHADO_D,
    MOVENDO_D,
    ESTACIONADO_D,
    FIM_D
} estado = INOPERANTE;

/* Button codes */
enum {
	buttonFloor0Up=120,
	buttonFloor1Up=128,
	buttonFloor2Up=136,
	buttonFloor3Up=144,
	buttonFloor4Up=151,
	buttonFloor5Up=159,
	buttonFloor6Up=166,
	buttonFloor7Up=173,
	buttonFloor8Up=180,
} buttonsUp;

enum {
	buttonFloor0Down=187,
	buttonFloor1Down=194,
	buttonFloor2Down=201,
	buttonFloor3Down=208,
	buttonFloor4Down=214,
	buttonFloor5Down=221,
	buttonFloor6Down=227,
	buttonFloor7Down=234,
	buttonFloor8Down=240,
} buttonsDown;


enum {
	buttonFloor0=246,
	buttonFloor1=252,
	buttonFloor2=258,
	buttonFloor3=263,
	buttonFloor4=269,
	buttonFloor5=275,
	buttonFloor6=286,
	buttonFloor7=291,
	buttonFloor8=296,
	buttonCloseDoor=302,
	buttonEmergency=307,
	buttonOn=312,
	buttonOff=317
} buttonsElevator;

/* Useful functions */
void turnOn(int pin) {
    digitalWrite(pin, HIGH);
}

void turnOff(int pin) {
    digitalWrite(pin, LOW);
}

/* Elevator main class */
class Elevator {
    private:
        // Attributes
        int currentFloor = 0;
        int floorCount;
        int pixelCount;
        int embusTime = 2000;
        int floorDelay = 1500;
        bool doorState = false;
        Adafruit_NeoPixel pixels;
        
        // Methods
        void goUp(int floor);
        void goDown(int floor);
        void startPixels();
        void openDoor();
        void closeDoor();
        void doorAction();

    public:
        Elevator(int _floorCount){
            floorCount = _floorCount;
            pixels = Adafruit_NeoPixel(10, barPin, NEO_GRB + NEO_KHZ800);
            startPixels();
        }
        bool isDoorOpened();
        void move(int floor);

};

/* Elevator methods */
bool Elevator::isDoorOpened() {
    return doorState;
};

void Elevator::openDoor() {
    doorState = true;
    turnOn(elevatorDoorLEDPin);
};

void Elevator::closeDoor() {
    doorState = false;
    turnOff(elevatorDoorLEDPin);
};

void Elevator::doorAction() {
    openDoor();
    delay(embusTime);
    closeDoor();
}

void Elevator::goUp(int floor) {
    while(floor >= currentFloor) {
        delay(floorDelay);
        currentFloor++;
        pixels.setPixelColor(currentFloor - 1, red, green, blue);
        pixels.show();
        pixels.setPixelColor(currentFloor - 1, 0, 0, 0);
    }
    doorAction();
};

void Elevator::goDown(int floor) {
    while(floor < currentFloor - 1) {
        delay(floorDelay);
        currentFloor--;
        pixels.setPixelColor(currentFloor - 1, red, green, blue);
        pixels.show();
        pixels.setPixelColor(currentFloor - 1, 0, 0, 0);
    }
    doorAction();
};

void Elevator::move(int floor) {
    pixels.setPixelColor(currentFloor - 1, 0, 0, 0);
    
    if(isDoorOpened()) {
        Serial.println("The door is opened. Please, wait until it closes to proceed.");
    } else {
        if(floor > currentFloor) {
            goUp(floor);
        } else {
            goDown(floor);
        }
    }
};

void Elevator::startPixels() {
    pixels.begin();
    pixels.setPixelColor(currentFloor, red, green, blue);
    pixels.show();
};

// 'global' Elevator object
Elevator *elevator;

/* Elevator moves according to the desired floor by the button pressed */
void gotoFloor() {
    int floor = analogRead(A0);
    switch (floor) {
        case buttonFloor0: elevator->move(0); break;
        case buttonFloor1: elevator->move(1); break;
        case buttonFloor2: elevator->move(2); break;
        case buttonFloor3: elevator->move(3); break;
        case buttonFloor4: elevator->move(4); break;
        case buttonFloor5: elevator->move(5); break;
        case buttonFloor6: elevator->move(6); break;
        case buttonFloor7: elevator->move(7); break;
        case buttonFloor8: elevator->move(8); break;
    }
}

void setup() {
    pinMode(analogPin, FALLING);

    Serial.begin(9600);

    attachInterrupt(0, gotoFloor, INPUT_PULLUP);

    elevator = new Elevator(9); 
}

void loop() {

}
