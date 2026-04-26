# Laboratorio de Computación Gráfica e Interacción Humano-Computadora 👾

## Práctica 8: Iluminación 2

### 🎓 Datos del Alumno
* **Nombre:** Mejía Alba Israel Hipólito
* **No. Cuenta:** 315348079
* **Semestre:** 2026-2
* **Grupo de Laboratorio:** 03
* **Grupo de Teoría:** 06
* **Profesor:** Ing. José Roque Román Guadarrama

 ---

## Tabla completa de controles

### Movimiento de objetos

| Tecla | Objeto | Acción | Eje |
|:---:|---|---|:---:|
| **Y** | Kitt (auto fantástico) | Retroceder | X |
| **U** | Kitt (auto fantástico) | Avanzar | X |
| **H** | Mustang | Retroceder | X |
| **J** | Mustang | Avanzar | X |
| **N** | Helicóptero | Retroceder | X |
| **M** | Helicóptero | Avanzar | X |
| **V** | Nave espacial | Avanzar | X |
| **B** | Nave espacial | Retroceder | X |
| **O** | Pez abisal | Avanzar (dentro de pecera) | X |
| **P** | Pez abisal | Retroceder (dentro de pecera) | X |

---

### Control de iluminación

#### Luces puntuales (PointLight)

| Tecla | Luz | Tipo | Color | Acción | Estado |
|:---:|---|---|---|---|:---:|
| **Z** | Lámpara (lamppost) | Puntual | Blanco | Toggle ON/OFF | ✅ |
| **F** | Foco del pez abisal | Puntual | Azul bioluminiscente | Toggle ON/OFF | ✅ |

#### Luces direccionales (SpotLight)

| Tecla | Luz | Objeto asociado | Color | Acción | Estado |
|:---:|---|---|---|---|:---:|
| **X** | Faros frontales | Mustang | Ciclo de 6 colores* | Cambiar color | ✅ |
| **V** | Luz frontal | Nave espacial | Cian | Activar al avanzar | ✅ |
| **B** | Luz trasera | Nave espacial | Naranja | Activar al retroceder | ✅ |
| **R** | Spotlight rojo | Pez abisal | Rojo | Toggle ON/OFF | ❌ |
| **G** | Spotlight rojo | Pez abisal | — | Rotar en eje X (+15°) | ❌ |
| **H** | Spotlight rojo | Pez abisal | — | Rotar en eje Y (+15°) | ❌ |
| **T** | Spotlight rojo | Pez abisal | — | Rotar en eje Z (+15°) | ❌ |

**\*Ciclo de colores de faros del Mustang (tecla X):**
1. Rojo
2. Amarillo
3. Verde
4. Cian
5. Azul
6. Magenta

---

### Luces automáticas (sin control directo)

| Luz | Objeto asociado | Tipo | Color | Comportamiento |
|---|---|---|---|---|
| Linterna | Cámara | SpotLight | Blanco | Sigue la posición y dirección de la cámara |
| Reflector | Helicóptero | SpotLight | Amarillo | Apunta al piso, se mueve con el helicóptero (M/N) |
| Luz roja fija | Escena | PointLight | Rojo | Siempre encendida, posición fija `(-6, 1.5, 1.5)` |
| Luz direccional | Sol | Directional | Blanco | Siempre encendida, ilumina desde arriba |

---

### Control de cámara

| Control | Acción |
|---|---|
| **Mouse** | Rotar vista (yaw y pitch) |
| **W** | Mover cámara hacia adelante |
| **S** | Mover cámara hacia atrás |
| **A** | Mover cámara hacia la izquierda |
| **D** | Mover cámara hacia la derecha |
| **Q** | Mover cámara hacia arriba |
| **E** | Mover cámara hacia abajo |

---

### Sistema

| Tecla | Acción |
|:---:|---|
| **ESC** | Cerrar aplicación |

---

## Resumen por categoría

### Movimiento de vehículos (6 objetos)
- **Kitt:** Y (retroceder), U (avanzar)
- **Mustang:** H (retroceder), J (avanzar)
- **Helicóptero:** N (retroceder), M (avanzar)
- **Nave:** V (avanzar), B (retroceder)
- **Pez:** O (avanzar), P (retroceder)

### Iluminación controlable (3 sistemas)
- **Lámpara:** Z (toggle)
- **Foco pez azul:** F (toggle)
- **Faros Mustang:** X (cambiar color)
- **Spotlight pez rojo:** R (toggle), G/H/T (rotar) — ❌ No funcional

### Iluminación automática (4 luces)
- Linterna de cámara (sigue al jugador)
- Reflector de helicóptero (sigue al helicóptero)
- Luz roja fija de escena
- Luz direccional del sol

---

## Notas importantes

1. **Movimiento del pez (O/P):** El pez está limitado a moverse dentro de la pecera. Se detiene automáticamente en los bordes (X = -2.0 a +2.0 en coordenadas locales de la pecera).

2. **Luces de la nave (V/B):** Las luces solo se activan mientras se mantiene presionada la tecla. Al soltar, la luz se apaga automáticamente.

3. **Faros del Mustang (X):** Cada presión de X cambia al siguiente color en el ciclo. Los dos faros (izquierdo y derecho) siempre tienen el mismo color.

4. **Spotlight rojo del pez (R/G/H/T):** Aunque el código está implementado, esta luz no se visualiza en la escena. Se requiere debugging adicional.

5. **Posicionamiento jerárquico:** Las luces del helicóptero, nave y pez se mueven automáticamente con sus objetos asociados. No es necesario controlarlas por separado.

---

## Límites del sistema

| Parámetro | Valor |
|---|---|
| Máximo de luces puntuales | 3 |
| Máximo de spotlights | 6 |
| Luces direccionales | 1 |

**Distribución actual:**
- **PointLights:** 3/3 (luz roja fija, lámpara, foco pez)
- **SpotLights:** 6/6 (linterna, faro izq. Mustang, faro der. Mustang, helicóptero, nave, spotlight rojo pez)
- **DirectionalLight:** 1/1 (sol)

---

## Leyenda de estado

| Símbolo | Significado |
|:---:|---|
| ✅ | Funcional — Control implementado y operativo |
| ❌ | No funcional — Control implementado pero no visible en escena |
| — | No aplica |
