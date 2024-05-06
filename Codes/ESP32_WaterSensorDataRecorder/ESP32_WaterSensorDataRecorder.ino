/****************************************
 * Librerías necesarias
 ****************************************/
#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/****************************************
 * Definiciones de constantes y variables
 ****************************************/
const int TEMPERATURE_SENSOR_PIN = 26; // Pin donde está conectado el sensor de temperatura
const int TDS_SENSOR_PIN = 32; // Pin donde está conectado el sensor TDS
const int TURBIDITY_SENSOR_PIN = 33; // Pin donde está conectado el sensor de turbidez
const int PH_SENSOR_PIN = 35; // Pin donde está conectado el sensor de pH

// Variables para el sensor de temperatura
OneWire oneWire(TEMPERATURE_SENSOR_PIN);
DallasTemperature temperatureSensor(&oneWire);
float temperatura; // Variable para almacenar el valor de temperatura

// Variables para el sensor TDS
#define VREF 3.3
#define SCOUNT 30
int analogBuffer[SCOUNT];
int analogBufferIndex = 0;
float temperaturaCompensada = 24; // Temperatura de compensación (ajustar según sea necesario)
float valorTDS = 0;

// Variables para el sensor de turbidez
float turbidez; // Variable para almacenar el valor de turbidez
float porcentajeTurbidez; // Variable para almacenar el porcentaje de turbidez
float escalaTurbidez = 0.13; // Factor de escala para ajustar los valores de turbidez

//Variables para el sensor de pH
#define Desviacion -2            // Compensación de desviación
#define IntervaloMuestreo 20     // Intervalo de muestreo en milisegundos
#define IntervaloImpresion 800   // Intervalo de impresión en milisegundos
#define LongitudArreglo 40       // Tamaño del arreglo de promedio
int ArregloPH[LongitudArreglo];  // Almacena el valor promedio de la retroalimentación del sensor
int IndiceArregloPH = 0;

/****************************************
 * Funciones
 ****************************************/

// Función para calcular la mediana de un conjunto de datos
int getMedianNum(int bArray[], int iFilterLen) {
  int bTab[iFilterLen];
  for (byte i = 0; i < iFilterLen; i++)
    bTab[i] = bArray[i];
  int i, j, bTemp;
  for (j = 0; j < iFilterLen - 1; j++) {
    for (i = 0; i < iFilterLen - j - 1; i++) {
      if (bTab[i] > bTab[i + 1]) {
        bTemp = bTab[i];
        bTab[i] = bTab[i + 1];
        bTab[i + 1] = bTemp;
      }
    }
  }
  if ((iFilterLen & 1) > 0) {
    bTemp = bTab[(iFilterLen - 1) / 2];
  } else {
    bTemp = (bTab[iFilterLen / 2] + bTab[iFilterLen / 2 - 1]) / 2;
  }
  return bTemp;
}

// Función para calcular el promedio de un arreglo de enteros (Sensor de pH)
double obtenerPromedio(int* arreglo, int numero) {
  int i;
  double promedio = 0.0;
  long suma = 0;

  if (numero <= 0) {
    //Serial.println("Error en el número para el promedio del array!");
    return 0.0;
  }

  // Calcular el promedio directamente si hay menos de 5 elementos
  if (numero < 5) {
    for (i = 0; i < numero; i++) {
      suma += arreglo[i];
    }
    promedio = (double)suma / numero;
    return promedio;
  }

  // Si hay más de 5 elementos, se excluyen el mínimo y el máximo
  int min = arreglo[0];
  int max = arreglo[0];

  for (i = 0; i < numero; i++) {
    suma += arreglo[i];

    // Actualizar el mínimo y el máximo
    if (arreglo[i] < min) {
      min = arreglo[i];
    }
    if (arreglo[i] > max) {
      max = arreglo[i];
    }
  }

  // Restar el mínimo y el máximo antes de calcular el promedio
  suma -= min;
  suma -= max;

  // Calcular el promedio
  promedio = (double)suma / (numero - 2);

  return promedio;
}

// Función para leer el valor de temperatura del sensor DS18B20
float readTemperatura() {
  temperatureSensor.requestTemperatures();
  float tempC = temperatureSensor.getTempCByIndex(0);
  return tempC;
}

// Función para leer el valor de TDS del sensor TDS
float readTDS() {
  analogBuffer[analogBufferIndex] = analogRead(TDS_SENSOR_PIN);
  analogBufferIndex = (analogBufferIndex + 1) % SCOUNT;
  float sum = 0;
  for (int i = 0; i < SCOUNT; i++) {
    sum += analogBuffer[i];
  }
  float averageVoltage = (sum / SCOUNT) * VREF / 4095.0;
  float compensationCoefficient = 1.0 + 0.02 * (temperaturaCompensada - 24.0);
  float compensatedVoltage = averageVoltage / compensationCoefficient;
  float tdsValue = (133.42 * pow(compensatedVoltage, 3) - 255.86 * pow(compensatedVoltage, 2) + 857.39 * compensatedVoltage) * 0.5;
  return tdsValue;
}

// Función para leer el valor de turbidez del sensor de turbidez
float readTurbidez() {
  int valorADC = analogRead(TURBIDITY_SENSOR_PIN);
  turbidez = valorADC * escalaTurbidez;
  porcentajeTurbidez = map(valorADC, 0, 4095, 0, 100);
  return turbidez;
}

//Función para leer el valor de ph del sensor de pH
float readPH() {
  ArregloPH[IndiceArregloPH++] = analogRead(PH_SENSOR_PIN);

  // Reiniciar el índice del arreglo si ha alcanzado el tamaño máximo
  if (IndiceArregloPH == LongitudArreglo)
    IndiceArregloPH = 0;

  // Cálculo del voltaje promedio del sensor
  float Voltaje = obtenerPromedio(ArregloPH, LongitudArreglo) * 2 / 1023.0;

  // Cálculo del valor de pH basado en el voltaje y la desviación
  float ValorPH = 3 * Voltaje + Desviacion;

  return ValorPH;
}

/****************************************
 * Configuración inicial
 ****************************************/
void setup() {
  Serial.begin(115200); // Inicia la comunicación serial
  temperatureSensor.begin(); // Inicializa el sensor de temperatura
  pinMode(TDS_SENSOR_PIN, INPUT); // Configura el pin del sensor TDS como entrada
}

/****************************************
 * Bucle principal
 ****************************************/
void loop() {
  // Leer valores de los sensores
  temperatura = readTemperatura();
  valorTDS = readTDS();
  turbidez = readTurbidez();
  float phValue = readPH();
  
  // Imprimir resultados
  Serial.print("Temperatura: ");
  Serial.print(",");
  Serial.print(temperatura);
  Serial.print(",");
  Serial.print(" °C");

  Serial.print(",");
  Serial.print("Valor TDS: ");
  Serial.print(",");
  Serial.print(valorTDS);
  Serial.print(",");
  Serial.print(" ppm");
  
  Serial.print(",");
  Serial.print("Valor de Turbidez: ");
  Serial.print(",");
  Serial.print(turbidez);
  Serial.print(",");
  Serial.print(" NTU");
  Serial.print(",");
  
  /*Serial.print("Porcentaje de Turbidez: ");
  Serial.print(porcentajeTurbidez);
  Serial.println("%");
  */

  Serial.print("Valor de pH: ");
  Serial.print(",");
  Serial.println(phValue, 2);
  
  
  delay(150); // Esperar 1 segundo antes de la próxima lectura
}
