# lydia-print-head

Lydia-v4 as printhead

***brief*** this is part of our work of PP OpenSource contributions, please check our [library](https://library.precious-plastic.org/) or our [main page](https://plastic-hub.com/products/) for more.

This print head is a modification of [Lydia-v4](https://plastic-hub.com/products/lydia-v4.html) and aims at a generic solution for most of it´s components.

**Status** : Confirmed & in progress, ETA Mid. of October

## Todos

- [x] Hopper interface & lead-out -> Daniel
- [x] Motor selection
- [ ] Firmware updates
  - [ ] Check TCP stack memory/CPU footprint on the ControllinoMega
- [ ] Sensors / Audio, LED feedback (status, etc... )
- [ ] Bonus : test HMI

### Todos Frederike@3dtreehouse

- [ ] Gantry mount plate, we need the size and locations of the mount holes. We'd need a test rig here.
- [ ] Modbus over TCP seems a better fit after all which gives us CAT5 shielding as well some standard connectivity. RS485 has no real standard plugs, ...
- [ ] Location and size of the cabinet, what on the control panel (controls & plugs) ?

### Todos TimberStar

- [x] we need a laser cut barrel shield which holds the insulation material
- [x] on the barrel tip, we need some mounting possibilties for fans which are a little heavy as well

## Components

- [x] [OMRON MX2 VFD](https://industrial.omron.es/es/products/3G3MX2-A2022-E) [BOM](https://es.wiautomation.com/omron/variadores-motores-proteccion-de-circuitos/3G3MX2A2022E?SubmitCurrency=1&id_currency=1&gclid=Cj0KCQjwy8f6BRC7ARIsAPIXOjgnGbRSd_DaX-o59wjtIBqI2mGllXWWOvjhSC1sPQgOTmGMvc7toncaAqejEALw_wcB)
- [x] Controllino - Mega as already mentioned a few times, adds 280 Euro
- [x] a better motor and gearbox, adds 600 Euro - MotoVario
- [ ] custom firmware development: 1200 Euro
- [x] ModBus capable PID controllers (3x) : [Omron - E5DC-B](https://industrial.omron.eu/en/products/E5DC-QX2ASM-002) - Din - Rail Module [BOM](https://es.wiautomation.com/omron/plc-sistemas/E5DCQX2ASM002?SubmitCurrency=1&id_currency=1&gclid=Cj0KCQjwy8f6BRC7ARIsAPIXOjgWMeg-P2iN7wysDExpeHJHtEHAv06B2pKTz3DQlKjn67Al2K7YvvMaAs4IEALw_wcB)

# Status

- [Firmware](https://github.com/plastic-hub/hydra), in development but mostly done
- Hardware, in re-design

### Specs

- Voltage : 220V
- Max. 120Kg, first rev. might be around 60KG
- Controller cable length around 20 meter
- PID : +/- 5 degc tolerance, alarm/cooling outputs

### Gantry Mount

- the gantry mount should be detached from the head-mount -> adapter plate
- @todo : run static tests

### Firmware

- For now, we base all on Arduino based and existing PlasticHub firmware framework, in particular we will use [Controllino-Mega](https://www.controllino.biz/product/controllino-mega/) which gives us Ethernet and RS485. The board has been proven reliable and robust (ideal for prototyping).
- VFD and PID control is mapped to Serial communication which might accept G-Code
- VFD will be Omron-M2X series since we can use existing code. The VFD also supports Modbus (rather difficult).

![](./assets/lydia-print-head-logical.jpg)

**[Diagram source](https://app.diagrams.net/#G1L7Prviy9U-2gpcZHm8Z5dj39gxDHd_V_)**

### PID Controller

- We should go for [Omron E5-C](https://assets.omron.com/m/6f7cd0d93654a7a4/original/E5_C-Temperature-Controller-to-NB-Screen-Template-Tech-Guide.pdf) series which come with Modbus (consider EMI and cable length to the main controller)
- Each PID will be exposed on the internal ModBus mapping
- There is currently a [custom PID controller](https://github.com/plastic-hub/pid-controller) in development but as mentioned, it's unlikely that it sees the light before November. However, I can imagine we use sub-routines as they're avaiable
- We should create logging module as well a an API to set temperature profiles for different materials. The logging module will help analyzing different PID settings as well experimenting with barrel cooling (when overshooting becomes an issue).
- Additional, we forward alarms for overshooting on the internal bus

- **Changes** :  Turns out that the [E5DC / E5DC-B](http://www.ia.omron.com/products/family/3242/) family is better choice.

![](./assets/E5DC-B.jpg)

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

----
## Todos

- Investigate Omron - PID feedback to enable more control via M/G codes (purge,...)
- Investigate GCode to ROS mapping
- Investigate screw tip machining, if lucky we get away with 4 axis
- Test different VFD/PID checksum/parity settings with larger EMI around

# References

- [ROS - Github](https://github.com/ros-industrial)
- [ROS - GBRL - Github](https://github.com/openautomation/ROS-GRBL/wiki/GRBLtron)
- [ROS - Matlab|SimuLnk](https://www.mathworks.com/help/ros/ug/get-started-with-ros-in-simulink.html)
- [ROS Modbus - Wiki](http://wiki.ros.org/modbus)
- [ROS Modbus - PLC - Github](https://github.com/sonyccd/ros_plc_modbus)
- [Project Noah](https://github.com/plastic-hub/noah)
