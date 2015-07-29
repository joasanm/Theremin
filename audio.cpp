#include "audio.h"
#include <iostream>

//metodo que inicializa el objeto audio
void audio::init(){
	
	channel=0;

	 //Crear objeto system y inicializarlo

    result = FMOD::System_Create(&sys);
	FMODErrorCheck(result);

	// Comprobar version

	result = sys->getVersion(&version);
	FMODErrorCheck(result);
 
	if (version < FMOD_VERSION){
		std::cout << "Error! You are using an old version of FMOD " << version << ". This program requires " << FMOD_VERSION << std::endl;    
		//return 0; 
	}

	//inicializar FMod

	result = sys->init(32, FMOD_INIT_NORMAL, 0);
	FMODErrorCheck(result);

    //crear un oscilador DSP con la nota A3 -> 440Hz.

    result = sys->createDSPByType(FMOD_DSP_TYPE_OSCILLATOR, &dsp);
    result = dsp->setParameterFloat(FMOD_DSP_OSCILLATOR_RATE, 440.0f);
	FMODErrorCheck(result);

}

//metodo para mostrar informacion sobre el error generado
void audio::FMODErrorCheck(FMOD_RESULT result){

    if (result != FMOD_OK)
    {
        std::cout << "FMOD error! (" << result << ") " << FMOD_ErrorString(result) << std::endl;
        exit(-1);
    }

}

//metodo que asigna una frecuencia al sonido
void audio::setFreq(float freq){

	result = channel->getFrequency(&frequency);
    //FMODErrorCheck(result);
            
	frequency = freq;
            
    result = channel->setFrequency(frequency);
    //FMODErrorCheck(result);

}

//metodo que asigna un volumen al sonido
void audio::setVolumen(float vol){

	 result = channel->getVolume(&volumen);
     //FMODErrorCheck(result);

	 volumen = vol;
           
     result = channel->setVolume(volumen);
     //FMODErrorCheck(result);
}

//metodo que activa el sonido con un oscilador de onda senosoidal, cuadrada, triangular o de dientes de sierra
void audio::activeSound(char tecla){

	if (!channel){
		result = channel->stop();
		result = sys->playDSP(dsp, 0, true, &channel);
		result = channel->setVolume(0.5f);
		switch(tecla){
		case 's':
			result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 0);
			break;
		case 'q':
			result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 2);
			break;
		case 'd':
			result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 3);
			break;
		case 't':
			result = dsp->setParameterInt(FMOD_DSP_OSCILLATOR_TYPE, 4);
			break;
		}
		result = channel->setPaused(false);
	}

}

//metodo que detiene el sonido
void audio::stopSound(){

	if (channel){
		result = channel->stop();
		channel = 0;
	}

}

//metodo que comprueba si esta activado/desactivado el sonido
int audio::isChannel(){
	
	if(channel) return 1;
	else return 0;
}