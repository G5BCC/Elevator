#include <Adafruit_NeoPixel.h>


// Variaveis barra de led
// ---------------------------//------------------------
#define PIN 4   // input pin Neopixel is attached to
#define NUMPIXELS      10 // number of neopixels in Ring

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
int andares = 10;
int delayval = 200; // timing delay

int redColor = 255; int greenColor = 0; int blueColor = 0;

int andar_destino = 0; int andar_atual = 0;

// Variaveis Painel Andares 
// ---------------------------//------------------------
int porta_painel_andares = 2;
const int andarT_subir = 100; const int andar9_descer = 190;
const int andar1_subir = 110; const int andar1_descer = 200;
const int andar2_subir = 120; const int andar2_descer = 210;
const int andar3_subir = 130; const int andar3_descer = 220;
const int andar4_subir = 140; const int andar4_descer = 230;
const int andar5_subir = 150; const int andar5_descer = 240;
const int andar6_subir = 160; const int andar6_descer = 250;
const int andar7_subir = 170; const int andar7_descer = 260;
const int andar8_subir = 180; const int andar8_descer = 270;


// Variaveis Painel Elevador 
// ---------------------------//------------------------

const int painel_elevadorT = 100;
const int painel_elevador1 = 110;
const int painel_elevador2 = 120;
const int painel_elevador3 = 130;
const int painel_elevador4 = 140;
const int painel_elevador5 = 150;
const int painel_elevador6 = 160;
const int painel_elevador7 = 170;
const int painel_elevador8 = 180;
const int painel_elevador9 = 190;
const int painel_elevador_portaFechar = 200;
const int painel_elevador_portaAbrir = 210; 

int porta_painel_elevador = 3;

void int_botao_andares(){
    Serial.print("Botão apertado andares: ");
    int codigo = analogRead(A0);

    Serial.println(codigo);

    switch (codigo){
        case andarT_subir: Serial.println("Subir Terreo");   andar_atual = 0; break;        
        case andar1_subir: Serial.println("Subir 1");        andar_atual = 1; break;       
        case andar2_subir: Serial.println("Subir 2");        andar_atual = 2; break;        
        case andar3_subir: Serial.println("Subir 3");        andar_atual = 3; break;        
        case andar4_subir: Serial.println("Subir 4");        andar_atual = 4; break;        
        case andar5_subir: Serial.println("Subir 5");        andar_atual = 5; break;       
        case andar6_subir: Serial.println("Subir 6");        andar_atual = 6; break;        
        case andar7_subir: Serial.println("Subir 7");        andar_atual = 7; break;        
        case andar8_subir: Serial.println("Subir 8");        andar_atual = 8; break;

        case andar9_descer: Serial.println("Descer 9");       andar_atual = 9; break;
        case andar1_descer: Serial.println("Descer 1");       andar_atual = 1; break;
        case andar2_descer: Serial.println("Descer 2");       andar_atual = 2; break;
        case andar3_descer: Serial.println("Descer 3");       andar_atual = 3; break;
        case andar4_descer: Serial.println("Descer 4");       andar_atual = 4; break;
        case andar5_descer: Serial.println("Descer 5");       andar_atual = 5; break;
        case andar6_descer: Serial.println("Descer 6");       andar_atual = 6; break;
        case andar7_descer: Serial.println("Descer 7");       andar_atual = 7; break;
        case andar8_descer: Serial.println("Descer 8");       andar_atual = 8; break;
        

    }

}


void int_botao_elevador(){
    Serial.print("Botão apertado elevador: ");
    int codigo = analogRead(A1);

    Serial.println(codigo);
}
// setColor()
// picks random values to set for RGB

void setColor(){

  Serial.print("red: ");
  Serial.println(redColor);
  Serial.print("green: ");
  Serial.println(greenColor);
  Serial.print("blue: ");
  Serial.println(blueColor);
  
}



void change_destiny(int i){

if(andar_atual == andar_destino){
  		abrir_porta();
    	delay(2000);
  		
      	int random_int = random(0,9);
      	while(random_int == andar_destino){
          random_int = random(0,9);
      	}
      	pixels.setPixelColor(andar_atual, pixels.Color(0, 0, 0));
     	andar_destino = random_int;
      	Serial.println(andar_destino);
		fechar_porta();
  		pixels.setPixelColor(andar_atual, pixels.Color(0, 0, 0));
  
}
  
}


void abrir_porta(){
		pixels.setPixelColor(10, pixels.Color(0, 255, 0));
  		pixels.setPixelColor(11, pixels.Color(0, 0, 0));
  		pixels.setPixelColor(andar_atual, pixels.Color(redColor, greenColor, blueColor));
  		pixels.show();
  		

}

void fechar_porta(){
		pixels.setPixelColor(11, pixels.Color(255, 0, 0));
  		pixels.setPixelColor(10, pixels.Color(0, 0, 0));
  		pixels.setPixelColor(andar_atual, pixels.Color(redColor, greenColor, blueColor));
  		pixels.show();
}


void setPins(){
  for(int i = 5; i <= 7; i++){
  	pinMode(i, OUTPUT);
    digitalWrite(i, HIGH);
  }
  
}

void setup() {
  setPins();
  Serial.begin(9600);
  pixels.begin(); // Initializes the NeoPixel library.
//  Serial.begin(9600);
  //abrir_porta();
  pinMode(porta_painel_andares, INPUT_PULLUP);
  attachInterrupt(0, int_botao_andares, FALLING);
  
  pinMode(porta_painel_elevador, INPUT_PULLUP);
  attachInterrupt(1, int_botao_elevador, FALLING);
  

}

void loop() {
   	
    
	change_destiny(andar_atual);
  	if(andar_destino>andar_atual){
      andar_atual++;
    }else{andar_atual--;}
    // pixels.Color takes RGB values, from 0,0,0 up to 255,255,255
    pixels.setPixelColor(andar_atual, pixels.Color(redColor, greenColor, blueColor)); // Moderately bright green color.

    pixels.show(); // This sends the updated pixel color to the hardware.

    delay(delayval); // Delay for a period of time (in milliseconds).
    
    pixels.setPixelColor(andar_atual, 0,0,0); // Moderately bright green color.
    delay(delayval);
    // Serial.println(i);
    
    
    
   

}

