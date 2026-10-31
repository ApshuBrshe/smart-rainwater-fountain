# Smart Rainwater Fountain

**Green Guardian | 2024–2025**

This is another school project I built together with **Lev Svetoshev**. We wanted to make a small fountain that could collect rainwater and reuse it instead of relying on a constant water supply.

We built a working prototype with a collection surface, filter, storage tank, sensors, and two pumps. The system stored rainwater, used it to run the fountain after the rain stopped, and could also water soil when needed. We worked together on planning, wiring, Arduino programming, assembly, and testing.

## The Arduino code

The file `Smart_Rainwater_Fountain/Smart_Rainwater_Fountain.ino` is a sketch I recovered from the project. It handles:

- Water-level readings from `A7` and pump control on `D3`
- Soil-moisture readings from `A0` and pump control on `D2`
- TDS (dissolved solids) readings on `A1`, with filtering and an estimate printed to Serial Monitor

The code uses thresholds of **100** for the water-level reading and **600** for soil moisture. Serial output is **9600 baud**.

## Hardware

The prototype used an Arduino, water-level and soil-moisture sensors, a water-quality sensor, two pumps with relay modules, a filter, and a water tank. The sketch requires a board with an `A7` analog input, which not every Arduino has.

To inspect the code, open the `.ino` file in Arduino IDE. If you're rebuilding the setup, check the relay logic and sensor thresholds first. Pumps need their own suitable power supply and driver or relay circuit; they shouldn't be powered directly from Arduino output pins.

## Notes

This is an archived version of our code and I haven't tested it again on the original hardware. The sketch doesn't contain every part of the physical setup, such as rain detection and the water collection structure. The TDS calculation uses a fixed temperature of 25 °C, and it was never meant to prove that the water was safe to drink.

After we graduated, we left the prototype at school for younger students to learn from.

## Team

- **Stanislav Kim** — co-developer
- **Lev Svetoshev** — co-developer
