#include "Leap.h"

using namespace Leap;

//clase leapMotion que hereda de Listener
class leapMotion : public Listener {

    public:

		Vector getHands(int i);
		Vector getPalma(int i);
		Matrix getDireccionHueso(int i);
		Matrix getDireccionMano(int i);
		Vector getPosicionHueso(int i);
		float getLongitudHueso(int i);
		float getAnchuraHueso(int i);
		//float getAnchuraPalma(int i);
		float getFreq();
		float getVolumen();
		int getNumManos();
		virtual void onConnect(const Controller&);
		virtual void onFrame(const Controller&);
		virtual void onFocusGained(const Controller&);
		virtual void onFocusLost(const Controller&);

	private:

		Vector handsGl[40];			//array con las posiciones x,y,z del comienzo y final de cada hueso de las manos
		Vector palma[2];			//array con las posiciones x,y,z del centro de las manos
		//float anchuraPalmas[2];		//array con la anchura de la palma de las manos
		Matrix direccionMano[2];	//array con la direccion de la mano
		Matrix direccionHuesos[40]; //array con las direcciones de los huesos
		Vector centroHuesos[40];	//array con las posiciones x,y,z de los huesos
		float longitudHuesos[40];	//array con las longitudes de los huesos
		float anchuraHuesos[40];	//array con las anchuras de los huesos
		float freq;					//valor de la frecuencia de tono que se debe pasar al objeto audio
		float volumen;				//valor del volumen del tono que se debe pasar al objeto audio
		int numManos;				//valor con el numero de manos registradas

};