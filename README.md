# lydia-print-head

Lydia-v4 as printhead

### Specs

- Voltage : 220V
- Max. 120Kg, first rev. might be around 60KG
- Controller cable length around 20 meter
- PID : +/- 5 degc tolerance, alarm/cooling outputs

### Gantry Mount

- the gantry mount should be detached from the head-mount -> adapter plate
- @todo : run static tests

### Firmware

- For now, we base all on Arduino based and existing PlasticHub firmware framework, in particular we will use Controllino-Mega which gives us Ethernet and RS485
- VFD and PID control is mapped to Serial communication which might accept G-Code
- VFD will be Omron-M2X series since we can use existing code

### PID Controller

- We should go for [Omron E5-C](https://assets.omron.com/m/6f7cd0d93654a7a4/original/E5_C-Temperature-Controller-to-NB-Screen-Template-Tech-Guide.pdf) series which come with Modbus (consider EMI and cable length to the main controller)
- Each PID will be exposed on the internal ModBus mapping

### Barrel

- easy to shorten
- OD should be standard to EU heatbands
- Between Nozzel and barrel we might have some adaptery to exchange different screw tips, up to 10 cm
- should have a mount near the nozzle
- Barrel - motor flange should have some redunancy to enable different barrels

### Screw

- As mentioned, the longer the better. For composites we might use shorter but bigger ones.

### Hopper interface and Hopper

- Feed control ? -> waiting for the robot pictures
- should have a thread interface

### Heatbands

#### Fans