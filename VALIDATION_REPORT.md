# Rapport V3

## Corrections

### 1. Amortissement

L'ancienne V2 déduisait `B` d'un `damping_ratio` via une formule qui n'était
pas dimensionnellement suffisante pour le modèle utilisé avec `K=0`.

La V3 utilise directement :
- `B_trans` en N.s/m ;
- `B_rot` en N.m.s/rad.

Le modèle est :

```text
M * x_ddot + B * x_dot = F
```

### 2. Limitation d'accélération

La V2 contenait une double saturation incorrecte.

La V3 impose directement :

```text
|v(k) - v(k-1)| <= a_max * dt
```

ce qui donne :

```text
a = (v(k)-v(k-1))/dt
```

et donc une borne mathématique sur l'accélération discrète.

### 3. Timeout capteur

Si aucun wrench n'arrive pendant `force_timeout`, les commandes articulaires
sont remises à zéro.

### 4. Capteur UR

Le mode `sensor=UR` utilise maintenant le topic zeroed :

```text
/force_sensor_ur_zeroed
```

Le capteur interne peut donc être utilisé comme source de développement avant
la résolution EtherCAT du Bota.

### 5. Teach

Les temps d'une démonstration sont maintenant basés sur `time.monotonic()`,
ce qui évite d'utiliser l'horloge ROS comme chronomètre de durée.

Les démonstrations stockent :
- pose TCP ;
- frame ;
- joints ;
- wrench ;
- temps relatif ;
- fréquence d'échantillonnage.

### 6. Validation six axes

Un outil hors ligne teste :
- dynamique translationnelle ;
- dynamique rotationnelle ;
- limites ;
- stabilité ;
- Jacobien ;
- pseudoinverse DLS ;
- conditionnement.

## Paramètres de départ

Ils restent conservateurs :

```text
M_trans = 12 kg
B_trans = 8.31 N.s/m
M_rot = 0.10 kg.m²
B_rot = 0.76 N.m.s/rad
max_linear_speed = 0.20 m/s
max_linear_accel = 0.50 m/s²
```

Ils ne doivent pas être considérés comme des paramètres validés pour le robot
réel. La sensation de guidage dépend du montage, du poids de la poignée,
de la gravité et de la compensation du capteur.

## Prochaine étape

La prochaine modification doit être une calibration expérimentale du wrench :
zéro, gravité outil, orientation, axes et signe des forces.
