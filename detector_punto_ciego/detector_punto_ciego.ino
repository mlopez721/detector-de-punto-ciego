/*
Detector de punto ciego

Basado en Medidor de distacia por ultrasonidos en:
http://www.circuitbasics.com/how-to-set-up-an-ultrasonic-range-finder-on-an-arduino/
*/

#define trigPin 10
#define echoPin 13
#define tiempoEspera 500
#define velocidadADetectar 10 //velocidad en Km/h que queremos detectar

// definimos el array donde vamos a meter los valores temporales
int valorDistancia[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
// definimos el índice del array
int indiceValores = 0;
// guardamos el valor del último índice del array en una variable
int ultimaPosicion = sizeof(valorDistancia) - 1;

void setup() {
  Serial.begin (9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {
  float duration, distance;
  // definimos la variable para detectar la diferencia en la distancia de dos mediciones
  int   diferenciaDistancia = 0;
  
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0344;

  // almacenamos el valor en el array
  valorDistancia[indiceValores] = distance;  
  
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
  
  // solo tenemos en cuenta si el objeto se aproxima a una velocidad de 5 Km/h o más
  // 5 Km/h equivalen a 138.89 cm/s
  // como hacemos una medición cada 0.5 s, dividimos el valor entre dos, que equivale aproximadamente a 70 cm/s
  
  // vamos a mostrar los valores de las mediciones
  Serial.print("valorDistancia[0]: ");
  Serial.print(valorDistancia[0]);
  Serial.print(" -- valorDistancia[1]: ");
  Serial.println(valorDistancia[1]);
  
  if (diferenciaDistancia <= -10)
  {
      Serial.print("Diferencia entre distancias: ");
      Serial.println(diferenciaDistancia);
      Serial.println("****** Objeto acercándose ******");      
  }
  
  // valorar que la diferencia se repita en al menos 3 medidas para evitar falsos positivos, por ejemplo
  
  
  // mostramos el valor a través de Serial
  if (distance >= 400 || distance <= 2){
    Serial.print("Distance = ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" cm");
    delay(tiempoEspera);
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
