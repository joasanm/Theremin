#include "leapMotion.h"

//metodo que devuelve el valor de una posicion de la matriz handsGl en el ultimo frame registrado
Vector leapMotion::getHands(int i){return handsGl[i];}

Vector leapMotion::getPalma(int i){return palma[i];}

Matrix leapMotion::getDireccionHueso(int i){return direccionHuesos[i];}

Matrix leapMotion::getDireccionMano(int i){return direccionMano[i];}

Vector leapMotion::getPosicionHueso(int i){return centroHuesos[i];}

float leapMotion::getLongitudHueso(int i){return longitudHuesos[i];}

float leapMotion::getAnchuraHueso(int i){return anchuraHuesos[i];}

//float leapMotion::getAnchuraPalma(int i){return anchuraPalmas[i];}

//metodo que devuelve el valor de la frecuencia detectada en el ultimo frame registrado
float leapMotion::getFreq(){return freq;}

//metodo que devuelve el valor del volumen detectado en el ultimo frame registrado
float leapMotion::getVolumen(){return volumen;};

//metodo que devuelve el numero de manos detectadas en el ultimo frame registrado
int leapMotion::getNumManos(){return numManos;};

//metodo que se activa cuando se conecta el Leap Motion
void leapMotion::onConnect(const Controller& controller) {
    std::cout << "Connected" << std::endl;
}

//metodo en bucle que actualiza la informacion de frecuencia, volumen y handsGl
void leapMotion::onFrame(const Controller& controller) {
	
	const Frame frame = controller.frame();

	int j = 0;
	float mejor = 1000;
	float mejorVol = 1000;
	int nada=-1000;
	Vector vNada = Vector(nada,nada,nada);
	Matrix mNada = Matrix(vNada,vNada,vNada);

	HandList hands = frame.hands();
	numManos = hands.count();

	for(int i=0; i<numManos; i++){
	
		palma[i] = hands[i].palmPosition();
		//anchuraPalmas[i] = hands[i].palmWidth();
		direccionMano[i] = hands[i].basis();

		float distVol=palma[i].y;

		if((palma[i].x/50)<-4 && (palma[i].x/50)>-7 && distVol<=mejorVol) mejorVol=distVol;
		
		//if(distVol<=mejorVol) mejorVol=distVol;

		for (int f = 0; f < hands[i].fingers().count(); f++) {
			Finger finger = hands[i].fingers()[f];
      
			for (int b = 0; b < 4; b++) {
				Bone bone = finger.bone(static_cast<Leap::Bone::Type>(b));
				handsGl[i*20+f*4+b]=bone.nextJoint();

				float dist=sqrt(pow(4.125-(handsGl[i*20+f*4+b].x/50),2)+pow((-0.75)-(handsGl[i*20+f*4+b].z/50),2));
				if(dist<=mejor) mejor=dist;

				centroHuesos[i*20+f*4+b]=bone.nextJoint();
				direccionHuesos[i*20+f*4+b]=bone.basis();
				longitudHuesos[i*20+f*4+b]=bone.length();
				anchuraHuesos[i*20+f*4+b]=bone.width()/2;
			}
		}
	}

	for(int i=numManos*20;i<40;i++) handsGl[i] = vNada;
	for(int i=numManos; i<2; i++) palma[i] = vNada;
	//for(int i=numManos; i<2; i++) anchuraPalmas[i] = 0;
	for(int i=numManos; i<2; i++) direccionMano[i] = mNada;
	for(int i=numManos*20; i<40; i++) direccionHuesos[i] = mNada;
	for(int i=numManos*20; i<40; i++) centroHuesos[i] = vNada;
	for(int i=numManos*20; i<40; i++) longitudHuesos[i] = 0;
	for(int i=numManos*20; i<40; i++) anchuraHuesos[i] = 0;

	if(mejor<=0.1) freq=7902.13; //se supone que al tocar la antena del theremin, se produce una frecuencia de sonido muy alta
	else if(mejor<=5){
		freq=4000*(1-mejor/5);
	}
	else freq=0;

	if(mejorVol>=250 && mejorVol<=350) volumen=1*((mejorVol-250)/100);
	else if(mejorVol<250) volumen=0;
	else volumen=1;
	
}

void leapMotion::onFocusGained(const Controller& controller) {
  //m_lastFrame = controller.frame();
}

void leapMotion::onFocusLost(const Controller& controller) {
  std::cout << "Focus Lost" << std::endl;
}