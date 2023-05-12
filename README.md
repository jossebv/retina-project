# RETINA CONTROLLER
El proyecto RETINA consiste en un controlador por infrarrojo para enviar y recibir señales de esta naturaleza. Se dispone de controles mediante botones y un controlador de voz para emplear con luces LED RGB, aunque puede ser extendido a cualquier dispositivo que acepte esta comunicación. Además, se puede conectar un panel LCD para disponer de trazas (la flag LCD_CONNECTED debe ser activada en el retina.h)

Este proyecto dispone de dos versiones: 
- Placa STM32F446RE: Se dispone de un mando con botonera reprogramable. El mando puede recibir señales de hasta 5 mandos distintos y guardarlos en su memoria. El mando inicia en modo transmisión, y mediante la pulsación larga del botón de usuario, este cambia para guardar los comandos que reciba.
También se puede configurar para que en receptor maneje un led conectado.
- Placa STM32F411-DISCO: En este modelo, además de lo anterior, se puede activar el control de voz. Para utilizarlo, se debe decir la palabra de activación: "Oye Retina", a continuación, puedes decir un comando de los que se aportan en el anexo de este fichero.

Puedes acceder al vídeo del demostrador del proyecto pinchando en la imagen:

[![Demostrador del proyecto Retina](https://github.com/sdg2DieUpm/retina/blob/assets/images/rgb_led_system_midjourney.png?raw=true)](https://youtu.be/7yMZnfwStgs "Demostrador del proyecto Retina. Imagen generada con Midjourney.")

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

Dispone de toda la API generada mediante doxygen con el archivo `📜index.html`, que puede encontrar en la carpeta `📂docs`.