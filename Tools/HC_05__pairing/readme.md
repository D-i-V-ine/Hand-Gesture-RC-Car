# HC-05 Master–Slave Configuration

This document records the configuration procedure used to pair two HC-05 modules as a **Master–Slave Bluetooth link** for the RC car.

The goal is to configure one HC-05 on the controller side as the **Master** and the other on the car as the **Slave**, allowing the two modules to reconnect automatically.

---

## Hardware

* 2 × HC-05 Bluetooth modules
* 2 × Arduino Nano
* USB/serial connection for AT-mode configuration

---

## 1. Enter AT Mode

For each HC-05:

1. Disconnect the module from power.
2. Press and hold the **KEY/EN** button.
3. Power the module while holding the button.
4. Release the button after the module enters AT mode. This is usually indicated by a slower LED blink pattern.
5. Open a serial terminal and send:

```text
AT
```

Expected response:

```text
OK
```

The AT-mode baud rate can vary between HC-05 variants. Confirm communication with `AT` before continuing.

---

## 2. Configure the Slave

The Slave is the HC-05 connected to the **RC car**.

### Reset configuration — optional

`AT+ORGL` restores the module's stored configuration to its factory/default settings.

It is **not required** when the module is already in a known configuration. Use it when starting with a fresh configuration or when previous settings need to be cleared.

```text
AT+ORGL
```

Then configure the Slave:

```text
AT+ROLE=0
AT+NAME=RC_CAR_SLAVE
AT+UART=9600,0,0
```

Obtain the Slave's Bluetooth address:

```text
AT+ADDR?
```

Expected response:

```text
+ADDR:<SLAVE_ADDR>
```

Save this address. It will be used when configuring the Master.

---

## 3. Configure the Master

The Master is the HC-05 connected to the **controller**.

### Reset configuration — optional

If the Master already has a known configuration, `AT+ORGL` can be skipped.

Otherwise, reset it:

```text
AT+ORGL
```

Then configure the Master:

```text
AT+ROLE=1
AT+NAME=RC_CAR_MASTER
AT+UART=9600,0,0
```

Bind the Master to the Slave:

```text
AT+BIND=<SLAVE_ADDR>
```

Replace `<SLAVE_ADDR>` with the address obtained from the Slave.

Expected response:

```text
OK
```

Then set the Master to connect only to the bound device:

```text
AT+CMODE=0
```

Expected response:

```text
OK
```

`AT+BIND` specifies the Slave that the Master should connect to, while `AT+CMODE=0` puts the Master into fixed-address mode.

---

## 4. Verify the Configuration

### Master

```text
AT+ROLE?
AT+BIND?
AT+CMODE?
AT+UART?
```

Expected configuration:

```text
ROLE  = 1
BIND  = <Slave address>
CMODE = 0
UART  = 9600,0,0
```

### Slave

```text
AT+ROLE?
AT+UART?
AT+ADDR?
```

Expected configuration:

```text
ROLE = 0
UART = 9600,0,0
ADDR = <Slave address>
```

Final configuration:

```text
Master → ROLE = 1
Slave  → ROLE = 0

Both   → UART = 9600
Master → BIND = Slave address
Master → CMODE = 0
```

---

## 5. Return to Normal Mode

Turn both HC-05 modules off and on again **without pressing or holding the KEY/EN button**.

They should now operate in normal Bluetooth mode.

The Master should automatically attempt to connect to the configured Slave.

LED blink patterns can differ between HC-05 variants, so use actual Bluetooth data transfer to confirm that the connection is working.

---

## 6. Normal UART Configuration

The HC-05 is configured for:

```text
Baud rate : 9600
Data      : 8 bits
Parity    : None
Stop bits : 1
```

This project uses `SoftwareSerial` for communication between the Arduino and HC-05.

```cpp
#include <SoftwareSerial.h>

SoftwareSerial BT(RX_PIN, TX_PIN);

void setup()
{
    BT.begin(9600);
}
```

Replace `RX_PIN` and `TX_PIN` with the pins used in the project.

The `9600` configured with `AT+UART=9600,0,0` is the normal Bluetooth UART speed. The baud rate used while communicating with the module in AT mode may be different.

> **Wiring:** The Arduino's TX pin should connect to the HC-05's RX pin, and the Arduino's RX pin should connect to the HC-05's TX pin.
