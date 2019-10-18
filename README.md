Little endian storage
The sprite is always 32x32 = 1024 need to keep that in mind to populate the image.

Header:
[86][EA][B9][41] 			spr_version == 41 B9 EA 86
[0F][27]					spr_count   == 9999

Offsets:
To get the offset you need to skip 6 bytes and get every other 4 bytes. 6 + (4 * i)
[42][9C][00][00]			offset sprite 1
[47][9C][00][00]			offset sprite 2
[50][A8][00][00]			offset sprite 3

...

[??][??][??][??]			offset sprite 9999 i didnt look in the hex file

Data: 
to get to the data you need to find an offset of a sprite and jump that offset based on the begining of the file.

1st sprite:
[F8][F8][F0]				transparent color
[00][00]					length in bytes of sprite data not including header data(transparent color + length data)


2nd sprite:
[F8][F8][F0]				transparent color
[04][0C]					length in bytes of sprite data not including header data(transparent color + length data) 
[00][00]					transparent count
[00][04]					colored count
[56][51][00]				pixel 1
[56][67][4D]				pixel 2
...
[XX][XX][XX]				pixel count

[00][00]					transparent count
[00][04]					colored count
[56][51][00]				pixel 1
[56][67][4D]				pixel 2
...
[XX][XX][XX]				pixel count

until 0x0400 bytes

Verification:
0C04 = 3076  A850 - 9C47 = 3081
3081 - 3076 = 5 the size doesnt include the header of data. 5 = 3 bytes of transp color and 2 bytes of lenght bytes data. AHA

3rd sprite:
[F8][F8][F0]
[E4][01]					484 bytes to parse				

