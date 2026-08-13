#ifndef __PROTOCOL__
#define __PROTOCOL__

// Baud speed
constexpr unsigned long SERIAL_BAUD_RATE = 115200uL;

// prefissi che richiedono parola chiave
#define MODE_PREFIX "mode:"
#define NET_PREFIX "net:"

// prefissi che richiedono valore int
#define OPEN_PREFIX "open:"

// valori possibili
#define MSG_MODE_AUTO "AUTO"
#define MSG_MODE_MANUAL "MANUAL"
#define MSG_NET_OK "OK"
#define MSG_NET_LOST "LOST"

#endif