/*
Detector de punto ciego

Basado en información de (entre otras) las siguientes webs
http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
http://programarfacil.com/blog/arduino-blog/sensor-ultrasonico-arduino-medir-distancia/
http://programarfacil.com/podcast/31-api-de-arduino/
*/

// definimos los parámetros del sensor de ultrasonidos
const int pinDisparo = 10;
const int pinEco = 13;
const int tiempoEspera = 500;
// definimos el pin que vamos a usar para la salida del led
const int LED = 8;
// velocidad en Km/h que queremos detectar
const float velocidadADetectarEnKmH = 1.5;

float duracion, distancia;
// pasamos los Km/h a cm/s y lo dividimos entre dos porque hacemos una medición cada medio segundo
float velocidadADetectarEnCmS = (velocidadADetectarEnKmH * 27.78) / 2;
// definimos el array donde vamos a meter los valores temporales
float valorDistancia[2] = { 0, 0 };
// definimos el índice del array
int indiceValores = 0;
// guardamos el valor del último índice del array en una variable
int ultimaPosicion = sizeof(valorDistancia) - 1;

// milis del contador interno de Arduino, nos indican el momento en el tiempo en que la alarma se ha activado
float milisInicioAlarma = 0;
// tiempo en milis que va a durar la alarma activada
float duracionDeAlarma = 3000;
// el tiempo que ha pasado desde que la alarma se ha activado
float milisDesdeInicioAlarma = 0;
// indicador de que la alarma está activada
boolean alarmaActivada = false;

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

	// lanzamos el pulso y medimos lo que tarda en volver
	digitalWrite(pinDisparo, LOW);
	delayMicroseconds(2);

	digitalWrite(pinDisparo, HIGH);
	delayMicroseconds(10);
	digitalWrite(pinDisparo, LOW);

	// convertimos la medida en distancia, concretamente en centímetros
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

	// si la diferencia es positiva, significa que el objeto detectado está más lejos
	// si la diferencia es negativa, significa que el objeto detectado está más cerca <-- lo que nos intersa en este caso 
	// solo tenemos en cuenta si el objeto se aproxima a una velocidad igual o superior a la indicada
	if (diferenciaDistancia <= (velocidadADetectarEnCmS * -1))
	{
		// activamos la alarma
		activarAlarma();
		// guardamos el momento en el que se ha iniciado la alarma
		milisInicioAlarma = float(millis());

		Serial.print("El valor de milisInicioAlarma: "); Serial.println(milisInicioAlarma);

		Serial.print("Diferencia entre distancias: ");
		Serial.println(diferenciaDistancia);
		Serial.print("El valor es superior o igual a ");
		Serial.println(velocidadADetectarEnCmS);
		Serial.println("********************** Objeto acercándose **********************");

	}


	// comprobamos si tenemos que mantener la alarma activada
	// mientras no pase el tiempo que hemos indicado como duración de la alarma, sigue activa
	if (alarmaActivada == true)
	{
		milisDesdeInicioAlarma = float(millis()) - milisInicioAlarma;
		//Serial.print("El valor de float(millis()): "); Serial.println(float(millis()));
		//Serial.print("El valor de tiempoDesdeInicioAlarma: "); Serial.println(tiempoDesdeInicioAlarma);
		if (milisDesdeInicioAlarma >= duracionDeAlarma)
		{
			apagarAlarma();
		}
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
	Serial.print("Distancia = ");
	if (distancia >= 400 || distancia <= 2) {
		Serial.println("El objeto está más cerca de 2 cm o más lejos de 400 cm");
	}
	else {
		Serial.print(distancia);
		Serial.println(" cm");
	}
}
