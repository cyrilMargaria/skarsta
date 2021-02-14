## Overview

 This is based on Panel-v2, with the following differences:
   - use PCB to mount the switches
   - The front is adapted to my display (measure your 
   - 
<p align="center">
  <img src="./img/panel-v2/panel.jpg"/>
</p>

## Printing

- [buttons.stl](../models/panel-v3/buttons.stl) - print with `0.15mm` profile.
- [panel-cover.stl](../models/panel-v2/panel-cover-v3.stl) - print with `0.2mm` profile, can be aligned with [panel-inset.stl](../models/panel-v2/panel-inset.stl) for dual-color print
- [panel-bottom.stl](../models/panel-v2/panel-bottom.stl) - print with `0.2mm` profile

## Shopping list

|                                                                                                                                                        Item |  Price  |
| ----------------------------------------------------------------------------------------------------------------------------------------------------------: | :-----: |
|      TM1637 - 7 Segment display  | ~$1.69  |
|        [Tactile switches - 12x12x7.3mm](https://www.ebay.com/itm/20PCS-TC-1212T-12x12x7-3-mm-Tact-Tactile-Push-Button-Momentary-Switch-PNDHAH/303636919207) | ~$1.83  |
| [Prototype PCB ](https://www.ebay.de/itm/5PCS-DIY-Prototype-Paper-PCB-Universal-Experiment-Matrix-Circuit-Board-5x7cm/303369572898) |

## Assembly

- Solder tactile switches to prototype PCB
- Solder wire to Protype PCB
- Check (with multimeter) the LED block pinout 
- Solder wire to TM1637:
  - common line: to pin 19 or 20 (not both)
  - switch: to pin 2, 3.. 8 (I soldered then on the LED block pins, bigger pitch)

    <summary>Tactile switches</summary>
    <p align="center">
      <img src="./img/panel-v3/panel-v3.jpg"/>
    </p>
- Connect the TM1637 4 wires to arduino, you can use an USB cable 



