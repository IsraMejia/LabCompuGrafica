# Laboratorio de Computación Gráfica e Interacción Humano-Computadora
## Práctica 4: Modelado Jerárquico 

### 🎓 Datos del Alumno
* **Nombre:** Mejía Alba Israel Hipólito
* **No. Cuenta:** 315348079
* **Semestre:** 2026-2
* **Grupo de Laboratorio:** 03
* **Grupo de Teoría:** 06
* **Profesor:** Ing. José Roque Román Guadarrama
 

Nota: El codigo relacionado y ejecutables decada uno de los ejercicios realizados esta en sus respectivas carpetas.

* Ej04-315348079
* P04-GRUA-315348079
* P04-ANIMAL-315348079

Para controlar los ejecutables se usan las siguientes teclas 
## 🎮 Controles y Mapeo de Teclas

El proyecto consta de dos modelos interactivos. A continuación, se detallan las teclas asignadas para la cinemática directa y el movimiento de cada parte de la jerarquía.

### 🏗️ Modelo 1: Grúa Articulada

| Componente | Acción / Movimiento | Tecla Asignada |
| :--- | :--- | :---: |
| **Cámara Global** | Navegación por el escenario | `W` `A` `S` `D` + `Mouse` |
| **Articulación 1** | Rotación de la base (Hombro de la grúa) | `F` |
| **Articulación 2** | Rotación del primer brazo (Codo) | `G` |
| **Articulación 3** | Rotación del segundo brazo | `H` |
| **Articulación 4** | Rotación de la canastilla / cabina | `J` |
| **Llanta Delantera Der.**| Rotación independiente sobre su eje | `V` |
| **Llanta Delantera Izq.**| Rotación independiente sobre su eje | `B` |
| **Llanta Trasera Der.** | Rotación independiente sobre su eje | `N` |
| **Llanta Trasera Izq.** | Rotación independiente sobre su eje | `M` |

---

### 🐊 Modelo 2: Cocodrilo Robot

| Componente / Pata | Hombro / Cadera | Codo / Rodilla | Movimiento Maestro (Ambos) |
| :--- | :---: | :---: | :---: |
| **Pata Delantera Izquierda** | `Z` | `X` | `C` |
| **Pata Delantera Derecha** | `V` | `B` | `N` |
| **Pata Trasera Izquierda** | `U` | `I` | `O` |
| **Pata Trasera Derecha** | `J` | `K` | `L` |
| **Cola Articulada** | `H` (Izq) / `Y` (Der) | *N/A* | `G` (Coletazo completo) |