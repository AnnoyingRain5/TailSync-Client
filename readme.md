# TailSync Client

NOTICE: THIS PROJECT IS IN A VERY EARLY STAGE, EXPECT BUGS AND RAPID UNVERSIONED PROTOCOL CHANGES, **DO NOT DEPLOY!!!**

## What?

TailSync is a protocol to allow event AV teams to control fursuit lighting. This is the client code, in other words, the code that runs on the fursuit!

## How?

Well, it's complicated... <br>
Packets are sent over ESP-NOW (broadcast), the client listens for these messages, and displays the colours requested by the AV team.

Most of the logic is in libTailSync, so for fursuit integration, you won't have to write much code at all! <br>
Just register callbacks for the different packet types, change your LEDs for colour packets, and/or flick your tail for pulse packets, and you'll be good to go!

It's important to note that the client should NEVER send packets, ever.

### Packets

Each packet is sent using ESP-NOW, as a broadcast (recipient FF:FF:FF:FF)

All packets follow this format (hex)

`<0x5453 ("TS")> <4-bit version> <4-bit packetType> <content (if applicable)>`

#### Packet Types

0: Pulse (no content) - Sent once for each beat in the music, Intended to be used for motorised parts like tails or ears. Please do not use this for lighting, use the colour packets for that.

1: Colour (8x8 pixel grid, see below)

2: Meta (Transmitter name (char[32]), DJ name (char[32]), Song name (char[32]), Colour packets per second (uint8))

F: End session (no content)

#### Colour Packet

Being the only packet with content, this packet is special! Interpreting this packet is how you are intended to colour your fursuit or accessory! 

Example packet: <br>
` (54 53) (01) (FF AA 00) (F3 A4 1A) (CA CA 06) (73 96 87) (...)`<br>
`(magic) (header) (colour) (colour) (colour) (colour) (colour data repeats for 64 total pixels)`

Magic: `64 54` ("TS")
header: `01` (version 0, type 1)<br>
1: red: `FF` green: `AA` blue: `00`<br>
2: red: `F3` green: `A4` blue: `1A`<br>
3: red: `CA` green: `CA` blue: `06`<br>
4: red: `73` green: `96` blue `87`<br>

The image data should be assembled into an 8x8 pixel image

An example of how the data could be interpreted is below, however, feel free to interpret it however you please (note that not all of the pixels are used in the example!)

![](doc/image.png)


## Why?

Becuase I can.
