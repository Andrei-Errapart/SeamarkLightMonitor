1. Konverteerida signaalid.
2. Lülitada vajadusel sisse-välja.


IRF7416 - P-channel mosfet...


73-305-90, TPS76950DBVT, +5V regulaator
67-742-77, 6.8uF low-ESR tantaalkondekas, EIA size 3528
71-303-21, BC817-25
50-186-01	1	Korpus, DIN


43 - x
403 - 58mm
x = 58*43/403 = 6.2mm
54 - x2
x2 = 58/403*54 = 7.8mm

2.
	kellakvarts	74-640-01

3. keraamiline resonaator
	375KHz		74-703-21
	400KHz		74-703-39

Current budget:
total:	4.5 mA
uC:	1.0 mA (0.4MHz, 5V)
PSU:	0.02 mA (TPS76950DBVT)

R2:	1.2 mA
R1	0.6 mA

without regulator and uc: 1.9 mA
without uc: 

ERRORS IN REV. A.
-----------------
1. There should be at least 1 more GND point.
2. Too high current: 5.2mA. Should be less.
3. Resonator capacitors (27pF) missing; won't work without them.
4. Missing capability to produce wake-up SYNC pulses.
5. GPS input. Turn GPS on once in every 14 minutes in order to output one synchronization
   pulse at exactly 14 minutes interval.
6. PCB is not symmetrical.

ERRORS IN REV. B.
-----------------
1. PCB dimension error (too big corner).
2. GPS input should accept RS232 levels.
3. GPS TX should be used as debugging serial output.
4. add internal LED to watch for internal clock - not needed.
5. switched-mode power supply to save energy - not needed.

