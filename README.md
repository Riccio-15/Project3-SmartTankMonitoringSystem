# Smart Tank Monitoring System

Progetto per il corso di Embedded Systems and IoT (a.a. 2025/2026) — Assignment #03.

## Struttura del repository

```
.
├── tms/    # Tank Monitoring Subsystem - ESP32, monitora il livello dell'acqua (MQTT verso CUS)
├── cus/    # Control Unit Subsystem - backend PC, coordina tutto il sistema
├── wcs/    # Water Channel Subsystem - Arduino, controlla la valvola (seriale verso CUS)
├── DBS/    # Dashboard Subsystem - frontend/web app per operatori remoti (HTTP verso CUS)
└── doc/    # Report del progetto e schema/breadboard
```