/*
Detector de punto ciego

Basado en información de (entre otras) las siguientes webs
http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
http://programarfacil.com/blog/arduino-blog/sensor-ultrasonico-arduino-medir-distancia/
http://programarfacil.com/podcast/31-api-de-arduino/
*/

const int pinDisparo	= 10;
const int pinEco		= 13;
const int tiempoEspera	= 500;
// velocidad en Km/h que queremos detectar
const float velocidadADetectarenKmH = 1.5;
// definimos el pin que vamos a usar para la salida del led
const int LED			= 11;

float duracion, distancia;
// pasamos los Km/h a cm/s y lo dividimos entre dos porque hacemos una medición cada medio segundo
float velocidadADetectarenCmS = (velocidadADetectarenKmH * 27.78) / 2;
// definimos el array donde vamos a meter los valores temporales
float valorDistancia[2] = { 0, 0 };
// definimos el índice del array
int indiceValores = 0;
// guardamos el valor del último índice del array en una variable
int ultimaPosicion = sizeof(valorDistancia) - 1;

// milis del contador interno de Arduino, nos indican el momento en el tiempo en que la alarma se ha activado
float milisInicioAlarma	= 0;
// tiempo en milis que va a durar la alarma activada
float duracionDeAlarma	= 1;
// el tiempo que ha pasado desde que la alarma se ha activado
float tiempoDesdeInicioAlarma = 0;
// indicador de que la alarma está activada
boolean alarmaActivada	= false;

void setup() {
	Serial.begin(9600);

	// especificamos los modos de los distintos pines
	pinMode(pinDisparo, OUTPUT);
	pinMode(pinEco, INPUT);
	pinMode(LED, OUTPUT);

	// apagamos el led
	digitalWrite(LED, LOW);
}

void loop() {

	// definimos la variable para detectar la diferencia en la distancia de dos mediciones
	int   diferenciaDistancia = 0;

	digitalWrite(pinDisparo, LOW);
	delayMicroseconds(2);

	digitalWrite(pinDisparo, HIGH);
	delayMicroseconds(10);
	digitalWrite(pinDisparo, LOW);

	duracion = pulseIn(pinEco, HIGH);
	distancia = (duracion / 2) * 0.0344;

	// almacenamos el valor en el array
	valorDistancia[indiceValores] = distancia;

	// comparamos con el valor anterior
	if (indiceValores == 0)
	{
		// en este caso comparamos con el último valor del array
		diferenciaDistancia = (valorDistancia[indiceValores] - valorDistancia[ultimaPosicion]);
	}
	else
	{
		// en el caso general comparamos con el valor anterior del array
		diferenciaDistancia = (valorDistancia[indiceValores] - valorDistancia[indiceValores - 1]);
	}

	// mostramos los datos que tenemos en el array
	//mostrarDatosArray();

	// si la diferencia es positiva, significa que el objeto detectado está más cerca <-- lo que nos intersa en este caso
	// si la diferencia es negativa, significa que el objeto detectado está más lejos 
	// solo tenemos en cuenta si el objeto se aproxima a una velocidad igual o superior a la indicada
	if (diferenciaDistancia >= velocidadADetectarenCmS)
	{
		// activamos la alarma
		activarAlarma();
		//apagarAlarma();
		// guardamos el momento en el que se ha iniciado la alarma
		milisInicioAlarma = millis();
		// mientras no pase el tiempo que hemos indicado como duración de la alarma, sigue activa
		tiempoDesdeInicioAlarma = millis() - milisInicioAlarma;
		if (tiempoDesdeInicioAlarma >= duracionDeAlarma)
		{
			apagarAlarma();
		}

		Serial.print("Diferencia entre distancias: ");
		Serial.println(diferenciaDistancia);
		Serial.print("El valor es superior o igual a " );
		Serial.println(velocidadADetectarenCmS);
		Serial.println("****** Objeto acercándose ******");
	}

	mostrarDatosSerial();

	delay(tiempoEspera);

	// sumamos 1 al índice del array
	indiceValores++;

	// si pasamos la última posición del array, volvemos al principio
	if (indiceValores == (ultimaPosicion + 1))
	{
		indiceValores == 0;
	}
}

// muestra el valor de los datos guardados en el array
void mostrarDatosArray()
{
	Serial.print("---------- valorDistancia[0]: ");
	Serial.print(valorDistancia[0]);
	Serial.print(" -- valorDistancia[1]: ");
	Serial.println(valorDistancia[1]);
}

// activamos la alarma
void activarAlarma()
{
	// encendemos el led
	digitalWrite(LED, HIGH);
	// activamos el flag de alarma
	alarmaActivada = true;
}

// apagamos la alarma
void apagarAlarma()
{
	// apagamos el led
	digitalWrite(LED, LOW);
	// desactivamos el flag de alarma
	alarmaActivada = false;
}

// mostramos el valor a través de Serial
void mostrarDatosSerial()
{
	if (distancia >= 400 || distancia <= 2) {
		Serial.print("Distancia = ");
		Serial.println("El objeto está más cerca de 2 cm o más lejos de 400 cm");
	}
	else {
		Serial.print("Distancia = ");
		Serial.print(distancia);
		Serial.println(" cm");
	}
}