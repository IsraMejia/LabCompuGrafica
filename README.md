# Laboratorio de Computación Gráfica e Interacción Humano-Computadora 👾

## Práctica 5: Modelado Jerárquico y Carga de Modelos

### 🎓 Datos del Alumno
* **Nombre:** Mejía Alba Israel Hipólito
* **No. Cuenta:** 315348079
* **Semestre:** 2026-2
* **Grupo de Laboratorio:** 03
* **Grupo de Teoría:** 06
* **Profesor:** Ing. José Roque Román Guadarrama

> **Nota:** El código relacionado y los ejecutables de cada uno de los ejercicios realizados están en sus respectivas carpetas dentro del proyecto. Este repositorio contiene la implementación del modelo jerárquico de un **Mustang GT** con transformaciones anidadas.

---

### 🎮 Controles y Mapeo de Teclas: Mustang Jerárquico

El modelo del automóvil está estructurado jerárquicamente: **Chasis (Padre) -> [Llantas (Hijos), Cofre (Hijo)]**. A continuación, se detallan las teclas asignadas para controlar el movimiento y la cinemática del modelo:

| Acción | Tecla | Descripción |
| :--- | :---: | :--- |
| **Avanzar** | `I` | Traslada el chasis en el eje Z negativo y rota las llantas hacia adelante. |
| **Retroceder** | `K` | Traslada el chasis en el eje Z positivo y rota las llantas hacia atrás. |
| **Rodado Manual (+)** | `U` | Rota las 4 llantas moradas hacia adelante sin mover el chasis. |
| **Rodado Manual (-)** | `J` | Rota las 4 llantas moradas hacia atrás sin mover el chasis. |
| **Abrir Cofre** | `Y` | Rota el cofre azul hacia arriba (Cinemática Directa). |
| **Cerrar Cofre** | `H` | Rota el cofre azul hacia su posición original (Límite 0°). |
| **Cámara** | `Mouse` | Control de *Look-at* para navegar por la escena. |
| **Salir** | `ESC` | Cierra la ventana del visualizador. |

---

### 🛠️ Detalles de Implementación

Para lograr que el modelo funcione como una unidad, se aplicó el concepto de **Grafo de Escena** básico:

1.  **Modelo 3D:** Se importaron modelos independientes en formato `.obj` para el chasis, cofre y cada una de las 4 llantas.
2.  **Jerarquía:** Se utilizó una `parentMatrix` para el chasis, la cual hereda su escala (10x), rotación de orientación y traslación global a todos los componentes hijos mediante la multiplicación de matrices.
3.  **Colorización:** * **Chasis:** Dorado (`1.0f, 0.84f, 0.0f`)
    * **Cofre:** Azul (`0.0f, 0.0f, 1.0f`)
    * **Llantas:** Morado (`0.5f, 0.0f, 0.5f`)
    * **Suelo:** Pasto sintético (`0.01f, 0.9f, 0.02f`)

---

### 📂 Estructura de Ejercicios
* **Ej04-315348079:** Introducción a transformaciones.
* **P04-GRUA-315348079:** Ejercicio de brazo robótico.
* **P04-ANIMAL-315348079:** Modelo de Mustang Jerárquico (Rama actual).

---
*Generado para la asignatura de Computación Gráfica - UNAM FI.*