
#include <iostream>
#include <cmath>
#include <sstream>

#include <glut.h>
#include <glui.h>

#include "leapMotion.h"
#include "audio.h"

#define PI 3.1415926

int window;
int enable=0;
int simulator=0;

GLUI * subGlui;
GLUI_Button * bt1;
GLUI_Button * bt2;
GLUI_Button * bt3;
GLUI_Button * bt4;

GLUquadricObj *quadratic = gluNewQuadric();

Controller controller;			//controller para inicializar Leap Motion
leapMotion leap;				//objeto de la clase leapMotion

audio sonido;					//objeto de la clase audio

char texto[4][80];				//matriz con la informacion que se muestra en pantalla
int filas=4;					//valor del numero de filas de la matriz texto

float relacion = 48000/440;		//valor con la relacion entre la tasa de muestreo y la frecuencia de la nota A3
bool afinado=false;				//booleano que confirma el uso del modo afinado
bool piano=false;				//booleano que confirma el uso del piano de referencia
int nota;						//valor que contiene la posicion del array de frecuencias/notas

//array de frecuencias
float nfrec[109]=
{0.00,16.35, 17.32, 18.35, 19.45, 20.60, 21.83, 23.12, 24.50, 25.96, 27.50, 29.14, 30.87,
32.70, 34.65, 36.71, 38.89, 41.20, 43.65, 46.25, 49.00, 51.91, 55.00, 58.27, 61.74,
65.41, 69.30, 73.42, 77.78, 82.41, 87.31, 92.50, 98.00, 103.83, 110.00, 116.54, 123.47,
130.81, 138.59, 146.83, 155.56, 164.81, 174.61, 185.00, 196.00, 207.65, 220.00, 233.08, 246.94,
261.63, 277.18, 293.66, 311.13, 329.63, 349.23, 369.99, 392.00, 415.30, 440.00, 466.16, 493.88,
523.25, 554.37, 587.33, 622.25, 659.25, 698.46, 739.99, 783.99, 830.61, 880.00, 932.33, 987.77,
1046.50, 1108.73, 1174.66, 1244.51, 1318.51, 1396.91, 1479.98, 1567.98, 1661.22, 1760.00, 1864.66, 1975.53,
2093.00, 2217.46, 2349.32, 2489.02, 2637.02, 2793.83, 2959.96, 3135.96, 3322.44, 3520.00, 3729.31, 3951.07,
4186.01, 4434.92, 4698.63, 4978.03, 5274.04, 5587.65, 5919.91, 6271.93, 6644.88, 7040.00, 7458.62, 7902.13};

//array con las notas equivalentes a los valores del array de frecuencias respecto a sus posiciones
char notas[109][4]=
{"~~","C0","C#0","D0","D#0","E0","F0","F#0","G0","G#0","A0","A#0","B0",
"C1","C#1","D1","D#1","E1","F1","F#1","G1","G#1","A1","A#1","B1",
"C2","C#2","D2","D#2","E2","F2","F#2","G2","G#2","A2","A#2","B2",
"C3","C#3","D3","D#3","E3","F3","F#3","G3","G#3","A3","A#3","B3",
"C4","C#4","D4","D#4","E4","F4","F#4","G4","G#4","A4","A#4","B4",
"C5","C#5","D5","D#5","E5","F5","F#5","G5","G#5","A5","A#5","B5",
"C6","C#6","D6","D#6","E6","F6","F#6","G6","G#6","A6","A#6","B6",
"C7","C#7","D7","D#7","E7","F7","F#7","G7","G#7","A7","A#7","B7",
"C8","C#8","D8","D#8","E8","F8","F#8","G8","G#8","A8","A#8","B8"};

using namespace std;


void luces(){

	//habilito la iluminacion;
	glEnable(GL_LIGHTING);
	glShadeModel(GL_SMOOTH);

	//light0
	static const GLfloat A0[] = {0.05,0.05,0.05,1.0};
	static const GLfloat D0[] = {0.05,0.05,0.05,1.0};
	static const GLfloat S0[] = {0.0,0.0,0.0,1.0};
	static const GLfloat p0[] = {20.0,20.0,0.0,0.0};
	glLightfv(GL_LIGHT0, GL_AMBIENT, A0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, D0);
    glLightfv(GL_LIGHT0, GL_SPECULAR, S0);
	glLightfv(GL_LIGHT0, GL_POSITION, p0);
	glEnable(GL_LIGHT0);

	//light1
	static const GLfloat A2[] = {0.0,0.0,0.0,1.0};
	static const GLfloat D2[] = {0.5,0.5,0.2,1.0};
	static const GLfloat S2[] = {0.0,0.0,0.0,1.0};
	static const GLfloat d2[] = {0.0,-1.0,0.0};
	static const GLfloat p2[] = {0.0,20.0,0.0,1.0};
	glLightfv(GL_LIGHT1, GL_AMBIENT, A2);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, D2);
    glLightfv(GL_LIGHT1, GL_SPECULAR, S2);
	glLightf(GL_LIGHT1, GL_SPOT_CUTOFF,45.0);
	glLightf(GL_LIGHT1, GL_SPOT_EXPONENT,10.0);
	glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION,d2);
	glLightfv(GL_LIGHT1, GL_POSITION, p0);
	glEnable(GL_LIGHT1);

	//material
	glColorMaterial(GL_FRONT, GL_DIFFUSE);
	glEnable(GL_COLOR_MATERIAL);

	const GLfloat Dm[] = {0.8,0.8,0.8,1.0};
	const GLfloat Sm[] = {0.3,0.3,0.3,1.0};
    glMaterialfv(GL_FRONT, GL_DIFFUSE, Dm);
    glMaterialfv(GL_FRONT, GL_SPECULAR, Sm);
    glMaterialf(GL_FRONT, GL_SHININESS, 3);
}

//metodo que dibuja el texto quue hay en la matriz texto
void textodisplay(){

	glDisable(GL_LIGHTING);

	glPushMatrix();
		glTranslatef(-3,5,0);
		glScalef(0.7,0.7,0.7);
		glRotatef(-45,1,0,0);
	
		for(int l=0; l<filas; l++){
			int letras = (int)strlen(texto[l]);
			glPushMatrix();
			glTranslatef(-8.0,4.0-(1*l),0);
			glScalef(0.005,0.005,0.005);
			for (int i=0; i<letras; i++){
				glColor3f(1.0,1.0,1.0);
				glLineWidth(3);
				glutStrokeCharacter(GLUT_STROKE_ROMAN, texto[l][i]);
			}
			glPopMatrix();
		}
	glPopMatrix();

	//glEnable(GL_LIGHTING);

}

//metodo que dibuja un teclado de piano, marcando la nota que genera el theremin
void dibujarPiano(){

	glDisable(GL_LIGHTING);

	float k=0.0;
	glPushMatrix();
	glTranslatef(-6,3,4);
	glScalef(0.25,0.5,0.5);
	glRotatef(45,1,0,0);

	for(int i=10;i<98;i++){				
		if(i%12==1 || i%12==3 || i%12==5 || i%12==6 || i%12==8 || i%12==10 || i%12==0){
			glPushMatrix();
				if(i==nota) glColor3f(1,0,0);
				else glColor3f(1,1,1);
				glBegin(GL_QUADS);
					glVertex3f(k,1,0);
					glVertex3f(k+1,1,0);
					glVertex3f(k+1,1,2);
					glVertex3f(k,1,2);
				glEnd();
				glColor3f(0,0,0);
				glLineWidth(3);
				glBegin(GL_LINE_STRIP);
					glVertex3f(k,1,0);
					glVertex3f(k+1,1,0);
					glVertex3f(k+1,1,2);
					glVertex3f(k,1,2);
				glEnd();
			glPopMatrix();
			k=k+1.0;
		}
		else{
			glPushMatrix();
			if(i==nota) glColor3f(1,0,0);
			else glColor3f(0,0,0);
			glBegin(GL_QUADS);
				glVertex3f(k-0.25,2,0);
				glVertex3f(k+0.25,2,0);
				glVertex3f(k+0.25,2,1);
				glVertex3f(k-0.25,2,1);
			glEnd();
			glPopMatrix();
		}
	}
	glPopMatrix();

	//glEnable(GL_LIGHTING);
}

//metodo que dibuja el theremin
void dibujarTheremin(){

	glPushMatrix();
		glTranslatef(0,3,0);
		glScalef(0.75,0.75,0.75);

		//dibujar antena vertical (frecuencia)
	
		glPushMatrix();
			glColor3f(0.5,0.5,0.5);
			glTranslatef(5.5,-1,-1);
			glRotatef(-90,1,0,0);
			gluCylinder(quadratic,0.1,0.1,8,10,10);
		glPopMatrix();

		glPushMatrix();
			glColor3f(0.5,0.5,0.5);
			glTranslatef(5.6,-1,-1);
			glRotatef(-90,0,1,0);
			gluCylinder(quadratic,0.2,0.2,0.6,10,10);
		glPopMatrix();

		//dibujar antena horizontal (volumen)

		glPushMatrix();
			glColor3f(0.5,0.5,0.5);
			glLineWidth(10);
			glTranslatef(-8,0,0);
			glScalef(1.5,1.5,1.5);
			glBegin(GL_LINE_STRIP);
				for(double i=45;i<=270;i=i+5){
					glVertex3f(cos(i*(PI/180)),0,sin(i*(PI/180)));
				}
			glEnd();

			glPushMatrix();
				glTranslatef(1.42,0,1.4);
				glBegin(GL_LINE_STRIP);
					for(double i=225;i<=270;i=i+5){
						glVertex3f(cos(i*(PI/180)),0,sin(i*(PI/180)));
					}
				glEnd();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(0,0,-1);
				glBegin(GL_LINE_STRIP);
					glVertex3f(0,0,0);
					glVertex3f(2,0,0);
				glEnd();
			glPopMatrix();

			glPushMatrix();
				glTranslatef(1.4,0,0.4);
				glBegin(GL_LINE_STRIP);
					glVertex3f(0,0,0);
					glVertex3f(1,0,0);
				glEnd();
			glPopMatrix();
		glPopMatrix();


		//dibujar caja
	
		glPushMatrix();
			glColor3f(0.8,0.5,0.0);
			glBegin(GL_QUAD_STRIP);
				glVertex3f(5,0,2);
				glVertex3f(5,0,-2);
				glVertex3f(-5,0,2);
				glVertex3f(-5,0,-2);
				glVertex3f(-5,-2,2);
				glVertex3f(-5,-2,-2);
				glVertex3f(5,-2,2);
				glVertex3f(5,-2,-2);
				glVertex3f(5,0,2);
				glVertex3f(5,0,-2);
			glEnd();
		glPopMatrix();
		
	glPopMatrix();

}

//metodo que dibuja las manos partiendo de la informacion que genera el Leap Motion
void dibujarManos(){

	glPushMatrix();
	glScalef(0.02,0.02,0.02);

	//dibujar palma de las manos

	for(int i=0; i<2; i++){
		
		glPushMatrix();
			glColor3f(1.0,1.0,0.0);
			glTranslatef(leap.getPalma(i).x,leap.getPalma(i).y,leap.getPalma(i).z);
			glMultMatrixf(leap.getDireccionMano(i).toArray4x4());
			glRotatef(-90,1,0,0);
			glTranslatef(0,-25,0);
			glutSolidTorus(2.5,25,20,20);
			glPushMatrix();
				glColor3f(1.0,0.0,0.0);
				glScalef(1,1,0.5);
				glutSolidSphere(20,20,20);
			glPopMatrix();
		glPopMatrix();
	}

	for(int i=0; i<40; i++){

		//dibujar puntos de flexion de las manos

		glPushMatrix();
			glColor3f(1.0,0.0,0.0);
			glTranslatef(leap.getHands(i).x,leap.getHands(i).y,leap.getHands(i).z);
			glMultMatrixf(leap.getDireccionHueso(i).toArray4x4());
			glScalef(1,0.5,1);
			glutSolidSphere(leap.getAnchuraHueso(i),20,20);
		glPopMatrix();

		//dibujar huesos de las manos
	
		if(i%4!=0){
			glPushMatrix();
				glColor3f(1.0,1.0,0.0);
				glTranslatef(leap.getPosicionHueso(i).x,leap.getPosicionHueso(i).y,leap.getPosicionHueso(i).z);
				glMultMatrixf(leap.getDireccionHueso(i).toArray4x4());
				glScalef(1,0.5,1);
				gluCylinder(quadratic,leap.getAnchuraHueso(i),leap.getAnchuraHueso(i),leap.getLongitudHueso(i),20,1);
			glPopMatrix();
		}

	}

	glPopMatrix();

}

//metodo que se ejecuta en bucle, que dibuja los metodos mencionados anteriormente
void display(){

	//borrar la pantalla
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//seleccionar la matriz del modelo
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	//situo y oriento la camara
	gluLookAt(0,13,10,	//ojo
			  0,3,0,	//punto al que miro
			  0,1,0);	//vertical subjetiva

	if(simulator==1){

		//dibujar theremin

		dibujarTheremin();

		//si se reconocen manos, dibujarlas

		if(leap.getNumManos()>0) dibujarManos();

		//dibujar piano de referencia

		if(piano) dibujarPiano();

		//mostrar informacion

		textodisplay();

	}
	
	glutSwapBuffers();

}

//metodo que configura la camara de la aplicacion
void reshape(int w, int h){
	
	//calcular la razon de aspecto
	float razon=(float)w/h;

	//defino el marco
	glViewport(0,0,w,h);

	//configurar la camara
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//camara ortografica
	glOrtho(-10,10,-(float)h*10/w,(float)h*10/w,-1,1000);

	/*
	//mantener la isotropica en camara perspectiva
	gluPerspective(45,					//fovy
				   razon,				//razon de aspecto
				   1,100.0				//cerca y lejos
				   );					//perspectiva
	*/
}

//callback de atencion al pulsado de tecla
void onKey(unsigned char tecla, int x, int y){
    switch(tecla){
		case 'a':						//activa/desactiva el modo afinado
			if(afinado) afinado=false;
			else afinado=true;
			break;
		case 's':						//activa el sonido con un oscilador de onda senosoidal
			sonido.activeSound('s');
			break;
		case 'q':						//activa el sonido con un oscilador de onda cuadrada
			sonido.activeSound('q');
			break;
		case 'd':						//activa el sonido con un oscilador de onda dientes de sierra
			sonido.activeSound('d');
			break;
		case 't':						//activa el sonido con un oscilador de onda triangular
			sonido.activeSound('t');
			break;
		case 'p':						//desactiva el sonido
			sonido.stopSound();
			break;
		case 'h':						//activa/desactiva el uso del piano de referencia
			if(piano) piano=false;
			else piano=true;
			break;
		case 27:						//salir de la aplicacion
			controller.removeListener(leap);
			exit(0);
	}
    glutPostRedisplay();
}

//metodo que busca la posicion en la array frecuencias mas cercana a una frecuencia dada
int search(float n[],int i, int l, float f){
	
	if(i+1==l){
		float t=(n[l]-n[i])/2;
		if(f<n[i]+t) return i;
		else return l;
	}
	int p = (l-i)/2;
	if(f<n[i+p]) search(n,i,i+p,f);
	else search(n,i+p,l,f);

}

//hace lo mismo que search(), pero sirve para el modo afinado del theremin
int modoAfinado(float n[],int i, int l, float f){

	if(i+1==l) return i;
	int p = (l-i)/2;
	if(f<n[i+p]) modoAfinado(n,i,i+p,f);
	else modoAfinado(n,i+p,l,f);

}

void gluiMainWindow (int control){
	switch(control){
	case 0:
		subGlui->hide();
		bt3->enable();
		break;
	case 1:
		//inicializar LeapMotion y FMod
		controller.addListener(leap);
		sonido.init();
		simulator=1;
		luces();
		bt1->disable();
		break;
	case 3:
		enable=1;
		bt3->disable();
		break;
	case -3:
		bt4->disable();
		controller.removeListener(leap);
		exit(0);
		break;
	}
	glutPostRedisplay();
}

//metodo que se ejecuta en bucle, que actualiza la pantalla, la frecuencia y el volumen que genera el theremin, y el contenido de la matriz texto
void idle(){

	float frec;
	float volumen;
	char newText[80];

	/* Update main and sub window */
	glutSetWindow (window);

	//actualizar frecuencia de sonido

	frec=leap.getFreq();
	if (frec<0) frec=0.00;

	if(afinado){ 
		nota=modoAfinado(nfrec,0,108,frec);
		sonido.setFreq(nfrec[nota]*relacion);
		strcpy_s(texto[0],"Modo afinado: ON");
		sprintf_s(newText,"Frecuencia: %0.2f",nfrec[nota]);
		strcpy_s(texto[1],newText);
		sprintf_s(newText,"Nota: %s",notas[nota]);
		strcpy_s(texto[2],newText);
	}
	else{ 
		float f=frec*relacion;
		sonido.setFreq(f);
		strcpy_s(texto[0],"Modo afinado: OFF");
		sprintf_s(newText,"Frecuencia: %0.2f",frec);
		strcpy_s(texto[1],newText);
		nota=search(nfrec,0,108,frec);
		sprintf_s(newText,"Nota: %s",notas[nota]);
		strcpy_s(texto[2],newText);
	}

	//actualizar volumen

	volumen=leap.getVolumen();
	sonido.setVolumen(volumen);
	sprintf_s(newText,"Volumen: %0.1f",volumen*100);
	strcpy_s(texto[3],newText);

	if(enable){
		subGlui = GLUI_Master.create_glui ("Acerca de",0,300,300);
		subGlui->add_statictext("Aqui se debe mostrar informacion sobre el creador");
		subGlui->add_button ("Volver", 0, gluiMainWindow);
		enable=0;
	}
	
	glutPostRedisplay();

}

//metodo main de la aplicacion
void main(int argc, char** argv){

	//inicializar la glut y los buffers
	glutInit(&argc, argv);

	//frame buffer doble y z-bufer
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);

	//construir ventana
	glutInitWindowSize(1280,650);
	glutInitWindowPosition(20,20);
	window=glutCreateWindow("Prueba Theremin");

	std::cout << "Arranca el programa" << std::endl;

	//registrar las callbacks
	GLUI_Master.set_glutDisplayFunc(display);
	//glutReshapeFunc(reshape);
	GLUI_Master.set_glutKeyboardFunc(onKey);
	GLUI_Master.set_glutReshapeFunc(reshape);

	GLUI * glui;
	glui = GLUI_Master.create_glui_subwindow (window,GLUI_SUBWINDOW_TOP);
	bt1 = glui->add_button ("Play", 1, gluiMainWindow);
	glui->add_column (0);
	bt2 = glui->add_button ("Configuracion", 2, gluiMainWindow);
	glui->add_column (0);
	bt3 = glui->add_button ("Acerca de", 3, gluiMainWindow);
	glui->add_column (0);
	bt4 = glui->add_button ("Quit", -3, gluiMainWindow);
	
	GLUI_Master.set_glutIdleFunc(idle);

	glEnable(GL_DEPTH_TEST);

	//Poner en marcha el bucle de eventos
    glutMainLoop();

}