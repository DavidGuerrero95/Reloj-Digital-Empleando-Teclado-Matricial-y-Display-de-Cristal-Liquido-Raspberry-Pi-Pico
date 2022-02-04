# Reloj-Digital-Empleando-Teclado-Matricial-y-Display-de-Cristal-Liquido-Raspberry-Pi-Pico

Laboratorio de Sistemas Embebidos Avanzados

Reloj Digital Empleando Teclado Matricial y Display de Cristal Líquido

Diciembre 15, 2021

### Introducción

En esta práactica de laboratorio, el objetivo principal serpa el desarrollo de un par de controladores o drivers para el manejo tanto de un teclado matricial como de un display de cristal líquido (LCD), empleando las entradas y salidas de propósito general (GPIO) y el módulo de TIMER disponibles en el MCU RP2040 del sistema de desarrollo Raspberry Pi Pico.

Como aplicación, el grupo de trabajo desarrollará un reloj digital con alarma mediante el uso del módulo RTC del MCU RP2040.

Para la implementación, el grupo de estudiantes podrá emplear el sistema de desarrollo Raspberry Pi Pico junto con la SDK de C/C++, además de librerías propias.

El esquema del sistema a implementar se muestra en la Fig. 1.

![](https://i.ibb.co/6rWKk3v/Aspose-Words-08b779c8-53a2-4a54-9156-d1203e8c79dd-002.png "Fig. 1: Esquema del sistema a diseñar")

### Consideraciones

Las consideraciones para el desarrollo de los drivers para el teclado y el LCD se muestran a continuación:

- Utilice entradas y salidas de propósito general GPIO para manejar las filas y columnas del teclado. Tenga en cuenta que algunas de ellas se deben configurar como interrupciones. Emplee también líneas de GPIO para manipular el LCD.
- Active resistores de pull-up o pull-down en los GPIOs según corresponda para la correcta lectura de filas y columnas del teclado matricial.
- Haga uso del módulo de TIMER para esperar a que la senañal generada al presionar una tecla se estabilice. Haga uso también de este módulo para el manejo de tiempos del LCD o utilice la función sleep us para tiempos de espera en microsegundos. Una estrategia recomendada para evitar utilizar la función sleep us es leer la bandera de busy del LCD.

Las consideraciones para el desarrollo del reloj digital con alarma se muestran a continuación:

- Habrá dos modos de operación: configuración y normal.
- En modo configuración se podrá establecer mediante teclado matricial, con visualización en LCD:
   * La fecha y hora actual (Ej. Diciembre 15 de 2021, 3PM).
   * Una alarma para una fecha y hora determinada.
- En modo normal, el sistema:
1) Visualizará la fecha y hora actual (Ej. Diciembre 15 de 2021, 3PM) en el display de cristal líquido.
2) Activará una sen˜al sonora o luminosa cuando la fecha y hora de la alarma coincida con la fecha y hora actual. La señal de alarma se desactivaría transcurridos 60 segundos o cuando el usuario presione una tecla específica.

### Consideraciones generales:

1. La programación se deberá realizar empleando el SDK de C/C++ para la Raspberry Pi Pico y librerías propias. Es mandatorio utilizar los módulos TIMER, GPIO y RTC para el desarrollo de la aplicación, en el cual se haga uso de interrupciones (trate de evitar el uso de la función sleep us a menos que no encuentre otra solución). El módulo UART puede ser empleado a través de las funciones de la librería de entrada y salida estándar STDIO (printf, getchar, etc.) para la realización de pruebas de verificación.
2. Haga uso de programación con metodología de eventos (interrupciones + polling).
3. Las funciones de los drivers que realice, tanto para el teclado matricial como para el LCD, no deben bloquear el flujo de ejecución a la espera de una acción por parte del usuario. Por ejemplo, si realiza una función llamada GetKey() para obtener la tecla presionada por el usuario, dicha función no deberá quedarse a la espera de que el usuario presione una determinada tecla.
