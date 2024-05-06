ESP32 Water Quality Monitoring

Este proyecto se enfoca en el monitoreo de la calidad del agua utilizando un ESP32 y varios sensores. Proporciona mediciones en tiempo real de parámetros importantes como temperatura, TDS, turbidez y pH, lo que permite una gestión más efectiva de los recursos hídricos.

Contenido del Repositorio:
    ![4 Sensores_bb](https://github.com/UzielCruzG/ESP32-Water-Quality-Monitoring/assets/83683151/c6e76795-68d9-43ff-930c-0313dc8167ec): 
    Esquema de conexión que muestra cómo conectar correctamente los sensores al ESP32.

	Carpeta Codes: En esta carperta se encuentran los codigos utilizados para el proyecto.
 		- WaterQualityMonitor_ESP32: Codigo para obtener datos de los sensores de calidad del agua en 
   		  tiempo real, mostrando los datos obtenidos por medio del puerto "Serial" de Arduino IDE.
	   	- ESP32_WaterSensorDataRecorder: Codigo para obtener datos de los sensores de la calidad de
	 	  agua en tiempo real, separando los valores por medio de "Separacion por comas", esto 
		  para extraer los datos del Arduino y pasarlos a un programa o excel para recoleccion de muestras.
 
    Registros_Sensores.csv: Archivo que contiene datos de los sensores en entornos simulados.
    Este contiene 60,000 registros en total, 6 clases de agua, 10,000 registros por cada clase.

 	Clasificacion de Condicion del Agua.xlsx: Archivo que cotiene las nomenclatura de nombres 
  	usados para clasificar cada tipo de agua.

Instrucciones de Uso

    Configuración del Hardware: Sigue el esquema de conexión proporcionado para conectar los sensores al ESP32.

    Instalación del Software: Abre el archivo WaterQualityMonitor_ESP32.ino en el entorno de desarrollo Arduino IDE 
	y carga el código en el ESP32.

    Visualización de Datos: Abre el monitor serial en Arduino IDE para ver los datos de los sensores en tiempo real.

Sensores Utilizados

    Sensor de pH líquido (Liquid PH 0-14 Value Sensor Module Meter V1.0)
    Sensor de temperatura (Sensor DS18B20)
        Resistencia de 4.7k Ohm (esta debe ir entre el pin de datos y el sensor)
    Sensor de turbidez (Módulo del Sensor de Turbidez TSW-20M)
    Sensor de conductividad (TDS Analógico Meter V1.0)
