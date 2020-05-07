#include <Adafruit_NeoPixel.h>

// Pin settings
const int elevatorDestinationPin = 12;
const int elevatorActivatedLEDPin = 3;
const int elevatorDoorLEDPin = 5;
const int emergencyLEDPin = 4;
const int elevatorBarPin = 11;
const int downBarPin = 13;
const int analogPin = 2;
const int upBarPin = 10;

/* LED Colors */
int red = 100;
int green = 65;
int blue = 255;

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
} state = INOPERANTE;

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
	buttonFloor1Down=187,
	buttonFloor2Down=194,
	buttonFloor3Down=201,
	buttonFloor4Down=208,
	buttonFloor5Down=214,
	buttonFloor6Down=221,
	buttonFloor7Down=227,
	buttonFloor8Down=234,
	buttonFloor9Down=240
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
    buttonFloor9=322,
	buttonCloseDoor=302,
	buttonEmergency=307,
	buttonOn=312,
	buttonOff=317
} buttonsElevator;

/* Useful functions */
void turnOnLED(int pin) {
    digitalWrite(pin, HIGH);
}

void turnOffLED(int pin) {
    digitalWrite(pin, LOW);
}

class StateMachine {
    public:
        void operative() {
            state = OCIOSO;
            Serial.println("Elevator idling");
        };

        void inoperative() {
            state = INOPERANTE;
            Serial.println("Inoperative elevator");
        };

        void idle() {
            state = ALINHADO_S;
        };

        void lined_up() {
            state = FIM_S; 
        };

        void moving_up() {
            state = ALINHADO_S;
        };

        void parked_up() {
            state = ALINHADO_S;
        };

        void end_up() {
            state = OCIOSO;
        };

        void lined_down() {
            state = ESTACIONADO_D;
        };

        void moving_down() {
            state = ALINHADO_D;
        };

        void parked_down() {
            state = ALINHADO_D;
        };

        void end_down() {
            state = OCIOSO;
        };
};

/* Elevator main class */
class Elevator {
    private:
        Adafruit_NeoPixel currentFloorBar;
        Adafruit_NeoPixel destinationBar;
        bool isEmergencyTriggered = false;
        bool isDoorOpened = false;
        int floorDelay = 1500;
        int currentFloor = 0;
        int floorCount = 10;
        
        void changeDestinationBar(int destination) {
            destinationBar.setPixelColor(currentFloor, 0, 0, 0);
            destinationBar.setPixelColor(destination, red, green, blue);
            destinationBar.show();
        };     

    protected:
        void openDoor() {
            isDoorOpened = true;
            turnOnLED(elevatorDoorLEDPin);
        };

        void closeDoor() {
            isDoorOpened = false;
            turnOffLED(elevatorDoorLEDPin);
        };


        void startPixels() {
            currentFloorBar.begin();
            currentFloorBar.setPixelColor(currentFloor, red, green, blue);
            currentFloorBar.show();

            destinationBar.begin();
            destinationBar.setPixelColor(currentFloor, red, green, blue);
            destinationBar.show();
        };

        void clearLEDBars() {
            currentFloorBar.setPixelColor(currentFloor, 0, 0, 0);
            currentFloorBar.show();

            destinationBar.setPixelColor(currentFloor, 0, 0, 0);
            destinationBar.show();
        };

        void goDown(int floor) {
            while(floor < currentFloor - 1) {
                delay(floorDelay);
                currentFloor--;
                currentFloorBar.setPixelColor(currentFloor - 1, red, green, blue);
                currentFloorBar.show();
                currentFloorBar.setPixelColor(currentFloor - 1, 0, 0, 0);
            }
            openDoor();
        };

        void goUp(int floor) {
            while(floor >= currentFloor) {
                delay(floorDelay);
                currentFloor++;
                currentFloorBar.setPixelColor(currentFloor - 1, red, green, blue);
                currentFloorBar.show();
                currentFloorBar.setPixelColor(currentFloor - 1, 0, 0, 0);
            }
            openDoor();
        }; 

        void triggerEmergency() {
            isEmergencyTriggered = true;
            turnOnLED(emergencyLEDPin);
        };

        void untriggerEmergency() {
            isEmergencyTriggered = false;
            turnOffLED(emergencyLEDPin);
        };

    public:
        Elevator(){
            Serial.println("Setting up elevator");

            currentFloorBar = Adafruit_NeoPixel(floorCount, elevatorBarPin, NEO_GRB + NEO_KHZ800);
            destinationBar = Adafruit_NeoPixel(floorCount, elevatorDestinationPin, NEO_GRB + NEO_KHZ800);

            Serial.println("OK");
            openDoor();
        }
        
        int getCurrentFloor() {
            return currentFloor;
        };

        bool hasDoorOpened() {
            return isDoorOpened;
        }

        bool hasEmergencyTriggered() {
            return isEmergencyTriggered;
        };
};

/* Controls the elevator */
class ElevatorController: public Elevator {
    private:
        StateMachine state_machine = StateMachine();

    public:
        void turnOff() {
            if(state != INOPERANTE) {
                turnOffLED(elevatorActivatedLEDPin);
                Serial.println("Deactivating elevator");
                Elevator::clearLEDBars();
                Elevator::untriggerEmergency();
                Elevator::closeDoor();
                state_machine.inoperative();
            } else {
                Serial.println("Elevator already deactivated");
            }
        };

        void turnOn() {
            if(state == INOPERANTE) {
                turnOnLED(elevatorActivatedLEDPin);
                Serial.println("Activating elevator");
                Elevator::startPixels();
                state_machine.operative();
            } else {
                Serial.println("Elevator already activated");
            }
        };

        void goUp(int floor) {
            if(Elevator::hasDoorOpened()) {
                Serial.println("Wait for the door to close");
            } else {
                Elevator::goUp(floor);
            }
        };

        void goDown(int floor) {
            if(Elevator::hasDoorOpened()) {
                Serial.println("Wait for the door to close");
            } else {
                Elevator::goDown(floor);
            }
        };

        void triggerEmergency() {
            if(state == INOPERANTE) {
                Serial.println("Cannot perform action because the elevator is deactivated");
            } else {
                if(!Elevator::hasEmergencyTriggered()) {
                    Elevator::triggerEmergency();
                    Serial.println("The emergency button has been triggered");
                } else {
                    Elevator::untriggerEmergency();
                    Serial.println("The emergency button has been untriggered");
                }
            }
        };

        void closeDoor() {
            if(state == INOPERANTE) {
                Serial.println("Cannot perform action because the elevator is deactivated");
            } else {
                if(!Elevator::hasDoorOpened()) {
                    Serial.println("The door is already closed");
                } else {
                    Elevator::closeDoor();
                    Serial.println("Closing door...");
                    delay(500);
                    Serial.println("The door has been closed");
                }
            }
        };
};

ElevatorController *elevator;

/* Elevator moves according to the desired floor by the button pressed */
void actions() {
    int button = analogRead(A0);

    switch (button) {
        case buttonOn: elevator->turnOn(); break;
        case buttonOff: elevator->turnOff(); break;
        case buttonEmergency: elevator->triggerEmergency(); break;
        case buttonCloseDoor: elevator->closeDoor(); break;
    }
}

void setup() {
    pinMode(analogPin, FALLING);
    pinMode(elevatorActivatedLEDPin, OUTPUT);
    pinMode(emergencyLEDPin, OUTPUT);
    pinMode(elevatorDoorLEDPin, OUTPUT);

    Serial.begin(9600);

    attachInterrupt(0, actions, INPUT_PULLUP);

    elevator = new ElevatorController();
}

void loop() {

}
