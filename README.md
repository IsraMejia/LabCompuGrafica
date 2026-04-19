# Laboratorio de Computación Gráfica e Interacción Humano-Computadora 👾

## Práctica 7: Iluminación

### 🎓 Datos del Alumno
* **Nombre:** Mejía Alba Israel Hipólito
* **No. Cuenta:** 315348079
* **Semestre:** 2026-2
* **Grupo de Laboratorio:** 03
* **Grupo de Teoría:** 06
* **Profesor:** Ing. José Roque Román Guadarrama

---

### 📝 Descripción de la Práctica
En esta práctica se implementaron y configuraron múltiples sistemas de iluminación (luces direccionales, puntuales y reflectores tipo SpotLight) utilizando OpenGL. Se aplicó modelado jerárquico para vincular dinámicamente posiciones de luces a modelos 3D móviles dentro de la escena, incluyendo un helicóptero, un Ford Mustang y un modelo de farola texturizada.

---

### ⌨️ Controles Interactivos (Teclado)

A continuación se listan las teclas configuradas para la manipulación y traslado de los objetos dentro del escenario (extraídas del archivo `Window.cpp`):

| Tecla | Acción en el Entorno 3D | Variable Modificada |
| :---: | :--- | :--- |
| **ESC** | Cierra la ventana de la aplicación de OpenGL | `glfwSetWindowShouldClose` |
| **Y** | Desplaza el vehículo **Kitt** | `muevex -= 1.0` |
| **U** | Desplaza el vehículo **Kitt** en sentido inverso | `muevex += 1.0` |
| **J** | Avanza el vehículo **Mustang** | `mueveMustang += 1.0` |
| **H** | Retrocede el vehículo **Mustang** | `mueveMustang -= 1.0` |
| **M** | Avanza el **Helicóptero** (y su cubo/reflector) | `mueveHelicoptero += 1.0` |
| **N** | Retrocede el **Helicóptero** (y su cubo/reflector) | `mueveHelicoptero -= 1.0` |

*Nota: El movimiento del mouse controla la dirección de la cámara (tipo Free Camera / FPS) dentro de la escena interactiva.*