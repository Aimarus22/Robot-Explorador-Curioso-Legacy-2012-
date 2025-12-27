# Robot Explorador Curioso (Legacy 2012) 🤖
https://doi.org/10.5281/zenodo.18072271
> **3er Premio - Premios Don Bosco (DBSariak) 2013**
> *Innovación en Electrónica y Robótica*

Este repositorio sirve como cápsula del tiempo para la preservación digital del proyecto **"Robot Explorador Curioso"**, desarrollado íntegramente en el curso 2012-2013 como proyecto PBL de fin del Ciclo Formativo de Grado Superior en Desarrollo de Productos Electrónicos en **La Salle Berrozpe** (Andoain).

## Objetivos del Repositorio

Este archivo no es solo un backup, tiene un doble propósito académico y de investigación:

1.  **Preservación Histórica:** Mantener accesible el código fuente, esquemas y documentación original tal y como fueron concebidos en 2012-2013, sin refactorizaciones modernas, para documentar el estado del arte de la educación técnica de la época.
2.  **Estudio Comparativo (Arqueología de Software):** Servir como "Línea Base" (v.0) para analizar la evolución de la ingeniería de sistemas embebidos. El objetivo futuro es contrastar estas técnicas "bare-metal" artesanales (control de tiempos por software, gestión manual de registros) con implementaciones modernas (RTOS, periféricos dedicados, TinyML) en la misma plataforma robótica.

## Autores y Créditos
Proyecto desarrollado por el equipo técnico de "Robotic Solutions" (empresa simulada para el proyecto PBL):

* **Axier Albín** - *Desarrollo de aplicación Android.*
* **Jokin Mugika** - *Diseño Mecánico, fabricación de PCB, montaje y puesta a punto.*
* **Aimar Rollán-González** - *Firmware, gerbers y documentación.*

**Institución:** La Salle Berrozpe (Andoain, Gipuzkoa, España).

## Stack Tecnológico (La "Vieja Escuela" de 2012)
El robot destaca por integrar múltiples subsistemas complejos en un microcontrolador de 8 bits con recursos limitados, utilizando técnicas de programación de bajo nivel hoy en desuso en favor de capas de abstracción (HAL).

* **Microcontrolador:** Microchip **PIC16F876A** (8-bit, 20MHz).
* **Lenguaje:** C (Compilador CCS).
* **Hardware:** PCB diseñada a medida en **OrCAD** y fabricada/soldada manualmente.
* **Sensores:** Ultrasonidos SRF08 (protocolo I2C implementado a mano) + Telemetría de batería (Calibración ADC empírica).
* **Actuadores:** Servomotores de rotación continua controlados mediante **PWM por Software (Bit-Banging)**.
* **Comunicación:** Módulo Bluetooth (UART) + App Android nativa (App Inventor).

## Estructura del Repositorio

* `/firmware`: Código fuente original (`main.c`) recuperado del archivo de 2013. Muestra la implementación manual de la máquina de estados y los retardos bloqueantes (`delay_us`) para la generación de señales.
* `/hardware`: Archivos de diseño electrónico (Esquemáticos, PCBs y Gerbers) generados con OrCAD.
* /applet_android : Programa original .apk
* `/docs`: Memoria técnica completa, incluyendo el Plan de Negocio ficticio y diagramas de flujo.

## Nota de Preservación
El código contenido en la carpeta `/firmware` se presenta **"tal cual" (as-is)** fue compilado en enero de 2013. Contiene prácticas de programación específicas de aquel contexto (uso de retardos bloqueantes para temporización, variables globales, números mágicos) que se mantienen deliberadamente para preservar la integridad histórica y permitir el análisis comparativo con versiones futuras optimizadas.

---
*Repositorio mantenido por Aimar Rollán-González como parte del proyecto de investigación sobre evolución de sistemas embebidos (2012-2026).*

