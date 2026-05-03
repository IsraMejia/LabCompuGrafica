# Laboratorio de Computación Gráfica e Interacción Humano-Computadora 👾

## Práctica 9-2: Animación Compleja y Textura Animada

### 🎓 Datos del Alumno
* **Nombre:** Mejía Alba Israel Hipólito
* **No. Cuenta:** 315348079
* **Semestre:** 2026-2
* **Grupo de Laboratorio:** 03
* **Grupo de Teoría:** 06
* **Profesor:** Ing. José Roque Román Guadarrama

---

## 🎥 Demostración en Video
Puedes visualizar el funcionamiento de todas las animaciones en tiempo real, así como la interacción con el entorno en el siguiente enlace:
🔗 **[Ver demostración en YouTube](https://www.youtube.com/watch?v=jmkLNb6Hg7Y)**

---

## ⌨️ Controles de Interacción

La práctica está diseñada para ser interactiva. Puedes controlar el flujo de las animaciones utilizando las siguientes teclas:

| Tecla | Acción | Descripción |
| :---: | :--- | :--- |
| **`F`** | **Secuencia Completa (Fuego + Humo + Catapulta)** | Inicia el fuego en el *Aeolipile*. El humo comienza a expandirse y, al "tocar" la catapulta, esta se dispara automáticamente lanzando el proyectil. |
| **`C`** | **Catapulta Manual** | Dispara únicamente la catapulta de forma aislada e independiente del humo. Ejecuta el lanzamiento de la esfera y sus respectivos rebotes. |

---

## 🎬 Descripción de las Animaciones

A lo largo de la práctica se implementaron distintas técnicas de gráficos por computadora para dar vida a los elementos de la escena:

1. **Humo del Aeolipile (Textura Animada y Blending):** 
   Un sistema de partículas simulado mediante un plano con textura `.png` (canal Alpha). La animación consiste en el crecimiento gradual (escalado) de la geometría y la transición de su opacidad (*blending*) a lo largo del tiempo para simular la expansión y desvanecimiento de gas caliente.

2. **Brazo de la Catapulta (Modelado Jerárquico):** 
   Se aplicó modelado jerárquico multinivel (Base -> Cuchara) para articular mecánicamente la catapulta. La animación interpola los grados de rotación de la cuchara mientras esta hereda la posición y escala exactas de la estructura base, asegurando un movimiento sincronizado de lanzamiento.

3. **Esfera / Proyectil (Cinemática y Física Básica):** 
   Una vez que el brazo de la catapulta alcanza su límite, la esfera se independiza de la jerarquía e inicia una simulación de física. Se utilizan ecuaciones de cinemática temporal (`deltaTime`) para trazar un **tiro parabólico** afectado por una gravedad simulada, culminando con **4 rebotes** en el suelo, donde cada impacto reduce la velocidad de la esfera en un 25% simulando la pérdida de energía cinética. 