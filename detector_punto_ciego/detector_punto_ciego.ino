/*
Detector de punto ciego

Basado en información de (entre otras) las siguientes webs
http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
http://programarfacil.com/blog/arduino-blog/sensor-ultrasonico-arduino-medir-distancia/
http://programarfacil.com/podcast/31-api-de-arduino/
*/

const int pinDisparo = 10;
const int pinEco = 13;
const int tiempoEspera = 500;
// velocidad en Km/h que queremos detectar
const int velocidadADetectarenKmH = 10;
// definimos el pin que vamos a usar para la salida del led
const int LED = 10;


// pasamos los Km/h a cm/s y lo dividimos entre dos porque hacemos una medición cada medio segundo
float velocidadADetectarenCmS = (velocidadADetectarenKmH * 27.78) / 2;
// definimos el array donde vamos a meter los valores temporales
float valorDistancia[2] = { 0, 0 };
// definimos el índice del array
int indiceValores = 0;
// guardamos el valor del último índice del array en una variable
int ultimaPosicion = sizeof(valorDistancia) - 1;

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
	float duracion, distancia;

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

	// si la diferencia es positiva, significa que el objeto detectado está más cerca <-- lo que nos intersa en este caso
	// si la diferencia es negativa, significa que el objeto detectado está más lejos 

	// vamos a mostrar los valores de las mediciones
	Serial.print("---------- valorDistancia[0]: ");
	Serial.print(valorDistancia[0]);
	Serial.print(" -- valorDistancia[1]: ");
	Serial.println(valorDistancia[1]);

	// solo tenemos en cuenta si el objeto se aproxima a una velocidad igual o superior a la indicada
	
	// si la alarma no esta activada,  sigue comprobando las distancias
	// if alarmaActivada = false
	if (diferenciaDistancia >= velocidadADetectarenCmS)
	{
		// encendemos el led
		digitalWrite(LED, HIGH);
		// aqui definiremos que el led esté encendido al menos durante 10 segundos
		// tiempoInicio = milis actuales;
		// alarmaActivada = true;


		Serial.print("Diferencia entre distancias: ");
		Serial.println(diferenciaDistancia);
		Serial.println("****** Objeto acercándose ******");
	}
	else
	{
		// si ha pasado un tiempo determinado
		//if milis actuales - tiempoInicio > 10000
		//alarmaActivada = false
		// apagamos el led
		digitalWrite(LED, LOW);
	}



	// mostramos el valor a través de Serial
	if (distancia >= 400 || distancia <= 2) {
		Serial.print("Distancia = ");
		Serial.println("El objeto está más cerca de 2 cm o más lejos de 400 cm");
	}
	else {
		Serial.print("Distancia = ");
		Serial.print(distancia);
		Serial.println(" cm");
	}
	delay(tiempoEspera);

	// sumamos 1 al índice del array
	indiceValores++;

	// si pasamos la última posición del array, volvemos al principio
	if (indiceValores == (ultimaPosicion + 1))
	{
		indiceValores == 0;
	}
}
