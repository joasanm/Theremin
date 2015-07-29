#include "fmod.hpp"
#include "fmod_errors.h"

using namespace FMOD;

//clase audio
class audio{

	private:
		System    *sys;
		Channel   *channel;
		DSP       *dsp;
		FMOD_RESULT      result;
		unsigned int version;		//variable con la informacion de la version de las librerias del FMod
		float frequency;			//variable necesaria para acualizar la frecuencia del objeto audio
		float volumen;				//variable necesaria para acualizar el volumen del objeto audio

	public:
		void init();
		void FMODErrorCheck(FMOD_RESULT result);
		void setFreq(float freq);
		void setVolumen(float vol);
		void activeSound(char tecla);
		void stopSound();
		int isChannel();
};
