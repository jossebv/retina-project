# RETINA CONTROLLER Y CONVOICE
El proyecto RETINA consiste en un controlador por infrarrojo para enviar y recibir señales de esta naturaleza. Se dispone de controles mediante botones y un controlador de voz para emplear con luces LED RGB, aunque puede ser extendido a cualquier dispositivo que acepte esta comunicación. Además, se puede conectar un panel LCD para disponer de trazas (la flag LCD_CONNECTED debe ser activada en el retina.h)

Este proyecto dispone de dos versiones: 
- Placa STM32F446RE: Se dispone de un mando con botonera reprogramable. El mando puede recibir señales de hasta 5 mandos distintos y guardarlos en su memoria. El mando inicia en modo transmisión, y mediante la pulsación larga del botón de usuario, este cambia para guardar los comandos que reciba.
También se puede configurar para que en receptor maneje un led conectado.
- Placa STM32F411-DISCO: En este modelo, además de lo anterior, se puede activar el control de voz. Para utilizarlo, se debe decir la palabra de activación: "Oye Retina", a continuación, puedes decir un comando de los que se aportan en el anexo de este fichero.

Puedes acceder al vídeo del demostrador del proyecto pinchando en la imagen:

[![Demostrador del proyecto Retina y ConVoice](/images/miniatura_video.png)](https://youtu.be/4zYQlVsk_SM "Demostrador del proyecto Retina y ConVoice")

Incluya la carpeta `retina-main` en la carpeta sdg. Esta carpeta incluye los drivers necesarios para la ejecución del sistema. Cabe recalcar que para utilizar la STM32F411-DISCO hace falta añadir otra carpeta con los drivers extra necesarios.

```
📂sdg
┣ 📂drivers
┃ ┣ 📂stm32f4xx
┃ ┃ ┃ ┣ 📂CMSIS
┃ ┃ ┃ ┗ 📂Drivers
┃ ┗ 📂stm32f411
┃     ┣ 📂CMSIS
┃     ┣ 📂BSP
┃     ┗ 📂STM32F4xx_HAL_Driver
┣ 📂projects
┃ ┣ 📦projects.code-workspace
┃ ┣ 📂hello
┃ ┣ 📂blink
┃ ┣ 📂retina
┃ ┃ ┣ 📂.vscode    
┃ ┃ ┃ ┣ 📜launch.json
┃ ┃ ┃ ┗ 📜tasks.json
┃ ┃ ┣ 📂common
┃ ┃ ┃ ┣ 📂include
┃ ┃ ┃ ┣ 📂src
┃ ┃ ┃ ┗ 📜Makefile.common
┃ ┃ ┣ 📂port
┃ ┃ ┃ ┃ ┣ 📂nucleo_stm32f446re
┃ ┃ ┃ ┃ ┃ ┣ 📂include
┃ ┃ ┃ ┃ ┃ ┣ 📂src
┃ ┃ ┃ ┃ ┃ ┣ 📜STM32F446RETx_FLASH.ld
┃ ┃ ┃ ┃ ┃ ┣ 📜Makefile.port
┃ ┃ ┃ ┃ ┃ ┗ 📜openocd.cfg
┃ ┃ ┣ 📂lib
┃ ┃ ┗ 📜Makefile
┃ ┃ ┗ ...
┃ ┗ ...
┗
```

Dispone de toda la API generada mediante doxygen con el archivo [index.html](docs/html/index.html).

## Configuración y funcionamiento del sistema

El sistema puede correr en varias configuraciones mediante la modificación de ciertas variables ubicadas en las cabeceras. Existe un ajuste global, el control de la pantalla LCD, está se puede habilitar o desahibilitar poniendo a true/false la variable `LCD_CONNECTED` del fichero `retina.h`. El resto de ajustes dependen del harware utilizado:

### STM32F446
En esta placa están disponibles dos principales versiones, que se alternan variando la variable `VERSION` del fichero `retina.h`: 
- Version 4: En esta versión se dispone del sistema retina en su versión base: Transmisor al encender, con 3 comandos que se suceden al presionar el botón user; y un modo receptor, activado al mantener dicho botón. En este último modo, el usuario puede controlar con un mando el led rgb conectado a la placa.
- Version 5: En esta versión se disponen todas la mejoras del sistema. Se amplía el mando, ofreciendo la posibilidad de conectar varios botones configurables. Por defecto el sistema tiene tres botones (variable `NUMBER_BUTTONS` en el fichero `fsm_retina_v2.h`): el primero es el botón de encendido del sistema, el segundo es el botón de cambio de mando y cambio de modo, y el resto son botones de función configurables. Como se ha mencionado, esta versión dispone de la posibilidad de grabar hasta 5 mandos (aumentable si fuese necesario). Estos mandos pueden ser configurados por software o grabados mediante la función receptora. 

    Para grabar un mando con el modo recepción se ha de seguir los siguientes pasos:
    1. Entrar en modo recepción manteniendo pulsado el botón 1.
    2. Con el mando a querer copiar ir pulsando los distintos comandos que queramos grabar en el orden deseado. Cuando se haya detectado un comando nuevo el led verde parpaderá indicando que ha sido recibido.
    3. Al enviar todos los comandos para llenar todos los botones, el sistema cambiará automaticamente a modo transmisión y se habilitará el nuevo mando en el menú.

> **Warning**
Cuando se quiera utilizar esta placa, la variable `PICOVOICE_ACTIVE` debe estar a *false*

---

### STM32F411
Con esta placa, además de todo lo anterior, se puede activar el sistema de control por voz. Es importante recalcar que estos sistemas son incompatibles, es decir, no es posible disponer del mando con botones y del control por voz simultáneamente. Para configurar el sistema solo hace falta poner la variable `PICOVOICE_ACTIVE = true` en el archivo `retina.h`.

## Comandos por voz
La placa puede ser configurada para utilizar cualquier comando por voz, las instrucciones detalladas se encuentran en [Picovoice Instructions](https://console.picovoice.ai/). 

En esta versión viene un modelo de lenguaje con los siguientes comandos:

- Palabra de activación: **Oye retina**

- Comandos de control:
    - Encendido y apagado de luces:
        1. Enciende las luces de la placa.
        2. Apaga las luces de la placa.
        3. Enciende las luces de la tira.
        4. Apaga las luces de la tira.
    - Contro de colores:
        1. Haz que la luz de la placa sea @color
        2. Haz que la luz de la tira sea @color

> **Note**
Por defecto si no se le dice que luz quieres cambiar el programa usará el transmisor infrarrojo para realizar el comando.