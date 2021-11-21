TRÜKKPLAAT
----------

Kahepoolne, paksus 1 mm.

FAILID
------
schematic.pdf		-- skeem
pcb.png			-- üldjoonis
layout_top.pdf		-- ülemine pool
layout_bottom.pdf	-- alumine pool
CommBox_edge.top	-- gerber: serv (NB! ka augud tuleb välja lõigata)
CommBox.bot		-- gerber: alumine pool
CommBox.top		-- gerber: ülemine pool
CommBox.drl		-- gerber: augud
CommBox.inf		-- gerber: puurid

OSTUNIMEKIRI
------------

Kogus	Tähis		Väärtus		Tevalo kood

Takistid				
5	R1,R2,R4-R6	10k		60-177-35
1	R3		100k		60-179-74
1	R7		39k		60-178-75
2	R8,R9		1		60-167-52

Kondensaatorid				
4	C1,C2,C6,C7	0.1uF		65-766-31
1	C3		330uF		67-730-22
3	C4,C5,C8	22pF		65-798-25
1	C9		820pF		65-763-34

Mikroskeemid				
1	U1		ATMEGA88	73-646-98
1	U2		LM3578AM	73-267-02
1	U3		SN74AHC1G86	73-561-67

Transistorid				
1	Q1		STT3PF30L	71-131-60
2	Q2,Q4		PDTC144ET	71-303-88
1	Q3		IRF7416	SO8	71-159-59

Dioodid
1	D1		BYS10-45	70-304-89

Muud				
1	J13		CONN-CT6	43-856-47
1	X1		3.68640 crystal	74-515-11
1	L1		1000uH		58-104-52
1	korpus		IDG-A1 (DIN)	50-179-00

KOOSTAMINE
----------

J14 jätta paigaldamata.

