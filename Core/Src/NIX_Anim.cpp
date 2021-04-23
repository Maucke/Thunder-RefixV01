/*
This is my first Library to make writing to the OLED 1602
Display much easier, the display is based on the SSD1311.
The display has 16 characters and 2 lines.
The library is far from complete and may be prone to errors.
Feedback welcome, visit www.gadjet.co.uk
Phil Grant 2013
Scrolling contributed by Nathan Chantrell http://nathan.chantrell.net/

Updated 06/11/2013 to include the cursPos data in the sendString function
sendString("string", col, row)

*/
#include "NIX_Anim.h" 

NixAnim::NixAnim() {}
NixAnim::~NixAnim() {}
const unsigned char Fonts[5][91 * 5] =
{
{0x00,0x00,0x00,0x00,0x00,/*" ",0*/

0x00,0x00,0x5F,0x00,0x00,/*"!",1*/

0x01,0x00,0x00,0x01,0x00,/*""",2*/

0x7C,0x17,0x14,0x7C,0x17,/*"#",3*/

0x22,0x45,0x7F,0x4B,0x32,/*"$",4*/

0x49,0x37,0x0C,0x72,0x49,/*"%",5*/

0x4F,0x59,0x67,0x78,0x48,/*"&",6*/

0x00,0x01,0x00,0x00,0x00,/*"'",7*/

0x00,0x00,0x3E,0x41,0x00,/*"(",8*/

0x00,0x61,0x1E,0x00,0x00,/*")",9*/

0x14,0x0C,0x3E,0x0C,0x14,/*"*",10*/

0x08,0x08,0x3E,0x08,0x08,/*"+",11*/

0x40,0x40,0x00,0x00,0x00,/*",",12*/

0x08,0x08,0x08,0x08,0x08,/*"-",13*/

0x40,0x40,0x00,0x00,0x00,/*".",14*/

0x40,0x30,0x08,0x06,0x01,/*"/",15*/

0x3E,0x51,0x49,0x45,0x3E,/*"0",16*/

0x00,0x42,0x7F,0x40,0x00,/*"1",17*/

0x42,0x61,0x51,0x49,0x46,/*"2",18*/

0x21,0x41,0x45,0x4B,0x31,/*"3",19*/

0x18,0x14,0x12,0x7F,0x10,/*"4",20*/

0x27,0x45,0x45,0x45,0x39,/*"5",21*/

0x3C,0x4A,0x49,0x49,0x30,/*"6",22*/

0x01,0x71,0x09,0x05,0x03,/*"7",23*/

0x36,0x49,0x49,0x49,0x36,/*"8",24*/

0x06,0x49,0x49,0x29,0x1E,/*"9",25*/

0x00,0x00,0x24,0x00,0x00,/*":",26*/

0x00,0x00,0x44,0x00,0x00,/*";",27*/

0x08,0x14,0x22,0x22,0x41,/*"<",28*/

0x14,0x14,0x14,0x14,0x14,/*"=",29*/

0x41,0x22,0x22,0x14,0x08,/*">",30*/

0x07,0x01,0x51,0x09,0x07,/*"",31*/

0x3E,0x5D,0x63,0x5F,0x61,/*"@",32*/

0x60,0x1C,0x13,0x1C,0x60,/*"A",33*/

0x7F,0x49,0x49,0x49,0x76,/*"B",34*/

0x3f,0x41,0x41,0x41,0x22,/*"C",35*/

0x7F,0x41,0x41,0x41,0x3E,/*"D",36*/

0x7F,0x49,0x49,0x49,0x41,/*"E",37*/

0x7F,0x09,0x09,0x09,0x01,/*"F",38*/

0x3E,0x41,0x41,0x51,0x32,/*"G",39*/

0x7F,0x08,0x08,0x08,0x7F,/*"H",40*/

0x00,0x41,0x7F,0x41,0x00,/*"I",41*/

0x20,0x40,0x41,0x3F,0x01,/*"J",42*/

0x7F,0x08,0x0C,0x32,0x41,/*"K",43*/

0x7F,0x40,0x40,0x40,0x40,/*"L",44*/

0x7F,0x0C,0x30,0x0C,0x7F,/*"M",45*/

0x7F,0x02,0x0C,0x10,0x7F,/*"N",46*/

0x3E,0x41,0x41,0x41,0x3E,/*"O",47*/

0x7F,0x09,0x09,0x09,0x06,/*"P",48*/

0x3E,0x41,0x41,0x61,0x7E,/*"Q",49*/

0x7F,0x09,0x09,0x39,0x46,/*"R",50*/

0x46,0x49,0x49,0x49,0x31,/*"S",51*/

0x01,0x01,0x7F,0x01,0x01,/*"T",52*/

0x3F,0x40,0x40,0x40,0x3F,/*"U",53*/

0x03,0x1C,0x60,0x1C,0x03,/*"V",54*/

0x1F,0x60,0x1F,0x60,0x1F,/*"W",55*/

0x41,0x36,0x08,0x36,0x41,/*"X",56*/

0x03,0x04,0x78,0x04,0x03,/*"Y",57*/

0x61,0x51,0x49,0x45,0x43,/*"Z",58*/

0x00,0x00,0x7F,0x00,0x00,/*"[",59*/

0x01,0x06,0x08,0x30,0x40,/*"\",60*/

0x00,0x00,0x7F,0x00,0x00,/*"]",61*/

0x00,0x00,0x00,0x00,0x00,/*"^",62*/

0x08,0x08,0x08,0x08,0x08,/*"-",63*/

0x00,0x01,0x00,0x00,0x00,/*"'",64*/

0x68,0x54,0x54,0x54,0x78,/*"a",65*/

0x7F,0x48,0x44,0x44,0x38,/*"b",66*/

0x38,0x44,0x44,0x44,0x48,/*"c",67*/

0x38,0x44,0x44,0x44,0x7F,/*"d",68*/

0x38,0x54,0x54,0x54,0x18,/*"e",69*/

0x04,0x7E,0x45,0x05,0x05,/*"f",70*/

0x78,0x24,0x24,0x54,0x48,/*"g",71*/

0x7F,0x04,0x04,0x04,0x78,/*"h",72*/

0x04,0x44,0x7D,0x40,0x00,/*"i",73*/

0x00,0x00,0x04,0x05,0x79,/*"j",74*/

0x7F,0x10,0x10,0x2C,0x44,/*"k",75*/

0x00,0x41,0x7F,0x40,0x00,/*"l",76*/

0x78,0x04,0x7C,0x04,0x04,/*"m",77*/

0x7C,0x08,0x04,0x04,0x78,/*"n",78*/

0x38,0x44,0x44,0x44,0x38,/*"o",79*/

0x7C,0x44,0x44,0x44,0x38,/*"p",80*/

0x38,0x44,0x44,0x44,0x7C,/*"q",81*/

0x44,0x7C,0x08,0x04,0x04,/*"r",82*/

0x68,0x54,0x54,0x54,0x6C,/*"s",83*/

0x04,0x3C,0x44,0x44,0x40,/*"t",84*/

0x3C,0x40,0x40,0x44,0x7C,/*"u",85*/

0x0C,0x34,0x40,0x34,0x0C,/*"v",86*/

0x3C,0x60,0x1C,0x60,0x1C,/*"w",87*/

0x44,0x2C,0x10,0x2C,0x44,/*"x",88*/

0x04,0x1C,0x60,0x1C,0x04,/*"y",89*/

0x44,0x64,0x54,0x4C,0x44},/*"z",90*/
{
0x00,0x00,0x00,0x00,0x00,/*" ",0*/

0x00,0x00,0x5F,0x5F,0x00,/*"!",1*/

0x00,0x00,0x01,0x01,0x00,/*""",2*/

0x30,0x3E,0x7E,0x7E,0x10,/*"#",3*/

0x00,0x47,0x7F,0x79,0x00,/*"$",4*/

0x0F,0x3F,0x7F,0x72,0x70,/*"%",5*/

0x36,0x7F,0x7F,0x7A,0x40,/*"&",6*/

0x00,0x01,0x01,0x00,0x00,/*"'",7*/

0x00,0x00,0x7F,0x7F,0x00,/*"(",8*/

0x00,0x77,0x77,0x00,0x00,/*")",9*/

0x14,0x1C,0x1C,0x1C,0x14,/*"*",10*/

0x08,0x08,0x1C,0x1C,0x08,/*"+",11*/

0x00,0x40,0x40,0x00,0x00,/*",",12*/

0x00,0x08,0x08,0x08,0x00,/*"-",13*/

0x00,0x40,0x40,0x00,0x00,/*".",14*/

0x40,0x78,0x3E,0x07,0x01,/*"/",15*/

0x3E,0x7F,0x41,0x41,0x3E,/*"0",16*/

0x00,0x06,0x7F,0x7F,0x00,/*"1",17*/

0x62,0x73,0x79,0x6F,0x66,/*"2",18*/

0x21,0x61,0x4D,0x7F,0x33,/*"3",19*/

0x1C,0x12,0x7F,0x7F,0x10,/*"4",20*/

0x26,0x67,0x45,0x7D,0x38,/*"5",21*/

0x3E,0x7F,0x4D,0x7D,0x38,/*"6",22*/

0x01,0x71,0x7F,0x0F,0x03,/*"7",23*/

0x36,0x7F,0xc9,0x49,0x36,/*"8",24*/

0x0E,0x5B,0x79,0x7F,0x1E,/*"9",25*/

0x00,0x36,0x36,0x00,0x00,/*":",26*/

0x00,0x00,0x44,0x44,0x00,/*";",27*/

0x08,0x1C,0x36,0x22,0x00,/*"<",28*/

0x14,0x14,0x14,0x14,0x14,/*"=",29*/

0x00,0x22,0x36,0x1C,0x08,/*">",30*/

0x02,0x03,0x09,0x0F,0x06,/*"",31*/

0x3E,0x7F,0x5F,0x7F,0x3E,/*"@",32*/

0x7E,0x7F,0x09,0x09,0x7E,/*"A",33*/

0x7F,0x7F,0x49,0x4F,0x70,/*"B",33*/

0x3E,0x7F,0x41,0x63,0x62,/*"C",35*/

0x7F,0x7F,0x41,0x41,0x3E,/*"D",36*/

0x7F,0x7F,0x49,0x49,0x41,/*"E",37*/

0x7F,0x7F,0x09,0x09,0x01,/*"F",38*/

0x3E,0x7F,0x41,0x53,0x32,/*"G",39*/

0x7F,0x7F,0x08,0x08,0x7F,/*"H",40*/

0x41,0x7F,0x7F,0x41,0x00,/*"I",40*/

0x61,0x41,0x7F,0x7F,0x01,/*"J",40*/

0x7F,0x7F,0x1C,0x36,0x63,/*"K",40*/

0x7F,0x7F,0x40,0x40,0x40,/*"L",44*/

0x7F,0x0F,0x3C,0x0F,0x7F,/*"M",45*/

0x7F,0x7F,0x04,0x08,0x7F,/*"N",45*/

0x3E,0x7F,0x41,0x41,0x3E,/*"O",45*/

0x7F,0x7F,0x09,0x0F,0x06,/*"P",48*/

0x3E,0x7F,0x41,0x61,0x7E,/*"Q",45*/

0x7F,0x7F,0x09,0x79,0x4E,/*"R",45*/

0x66,0x6F,0x49,0x79,0x32,/*"S",51*/

0x01,0x7F,0x7F,0x01,0x01,/*"T",52*/

0x3F,0x7F,0x40,0x40,0x3F,/*"U",53*/

0x7F,0x7F,0x30,0x0C,0x03,/*"V",45*/

0x3F,0x78,0x1E,0x78,0x3F,/*"W",45*/

0x71,0x19,0x7F,0x4C,0x47,/*"X",45*/

0x07,0x0F,0x78,0x0C,0x07,/*"Y",45*/

0x63,0x73,0x49,0x45,0x63,/*"Z",45*/

0x00,0x00,0x7F,0x7F,0x00,/*"[",59*/

0x00,0x0E,0x7E,0x70,0x00,/*"\",60*/

0x00,0x00,0x00,0x00,0x00,/*"]",61*/

0x00,0x00,0x00,0x00,0x00,/*"^",62*/

0x08,0x08,0x08,0x08,0x08,/*"-",63*/

0x00,0x01,0x01,0x00,0x00,/*"'",64*/

0x68,0x7C,0x7C,0x7C,0x40,/*"a",65*/

0x3F,0x7F,0x44,0x7C,0x38,/*"b",66*/

0x38,0x7C,0x44,0x44,0x00,/*"c",67*/

0x38,0x7C,0x44,0x7F,0x7F,/*"d",68*/

0x28,0x7C,0x54,0x5C,0x18,/*"e",69*/

0x04,0x7E,0x7F,0x05,0x01,/*"f",70*/

0x78,0x7C,0x7C,0x5C,0x04,/*"g",71*/

0x7F,0x7F,0x44,0x7C,0x78,/*"h",72*/

0x00,0x7C,0x7C,0x00,0x00,/*"i",73*/

0x00,0x00,0x7D,0x7D,0x00,/*"j",74*/

0x6F,0x7F,0x7C,0x6C,0x44,/*"k",75*/

0x00,0x7F,0x7F,0x00,0x00,/*"l",76*/

0x7C,0x7C,0x7C,0x78,0x78,/*"m",77*/

0x7C,0x7C,0x44,0x7C,0x78,/*"n",78*/

0x38,0x7C,0x44,0x7C,0x38,/*"o",79*/

0x3C,0x7C,0x44,0x7C,0x38,/*"p",80*/

0x38,0x7C,0x44,0x7C,0x38,/*"q",81*/

0x04,0x7C,0x7C,0x04,0x00,/*"r",82*/

0x00,0x7C,0x7C,0x54,0x00,/*"s",83*/

0x04,0x7C,0x7C,0x44,0x00,/*"t",84*/

0x3C,0x7C,0x44,0x7C,0x78,/*"u",85*/

0x04,0x3C,0x3C,0x1C,0x04,/*"v",86*/

0x1C,0x3C,0x7C,0x7C,0x14,/*"w",87*/

0x44,0x6C,0x7C,0x7C,0x04,/*"x",88*/

0x04,0x1C,0x7C,0x7C,0x04,/*"y",89*/

0x00,0x74,0x7C,0x4C,0x00}, /*"z",90*/
{
0x00, 0x00, 0x00, 0x00, 0x00,/*" ",0*/

0x00, 0x00, 0x5F, 0x5F, 0x00,/*"!",1*/

0x00, 0x00, 0x01, 0x01, 0x00,/*""",2*/

0x30, 0x3E, 0x7E, 0x7E, 0x10,/*"#",3*/

0x00, 0x47, 0x7F, 0x79, 0x00,/*"$",4*/

0x0F, 0x3F, 0x7F, 0x72, 0x70,/*"%",5*/

0x36, 0x7F, 0x7F, 0x7A, 0x40,/*"&",6*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",7*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"(",8*/

0x00, 0x77, 0x77, 0x00, 0x00,/*")",9*/

0x14, 0x1C, 0x1C, 0x1C, 0x14,/*"*",10*/

0x08, 0x08, 0x1C, 0x1C, 0x08,/*"+",11*/

0x00, 0x40, 0x40, 0x00, 0x00,/*",",12*/

0x00, 0x08, 0x08, 0x08, 0x00,/*"-",13*/

0x00, 0x40, 0x40, 0x00, 0x00,/*".",14*/

0x40, 0x78, 0x3E, 0x07, 0x01,/*"/",15*/

	0x7f,0x41,0x41,0x7f,0x7f, // 0
	0x00,0x00,0x7f,0x7f,0x00, // 1
	0x79,0x79,0x49,0x49,0x4f, // 2
	0x41,0x49,0x49,0x7f,0x7f, // 3
	0x0e,0x08,0x08,0x7f,0x7f, // 4
	0x4f,0x49,0x49,0x79,0x79, // 5
	0x7f,0x49,0x49,0x79,0x78, // 6
	0x01,0x01,0x01,0x7f,0x7f, // 7
	0x7f,0x49,0x49,0x7f,0x7f, // 8
	0x0f,0x49,0x49,0x7f,0x7f, // 9

0x00, 0x36, 0x36, 0x00, 0x00,/*":",26*/

0x00, 0x00, 0x44, 0x44, 0x00,/*";",27*/

0x08, 0x1C, 0x36, 0x22, 0x00,/*"<",28*/

0x14, 0x14, 0x14, 0x14, 0x14,/*"=",29*/

0x00, 0x22, 0x36, 0x1C, 0x08,/*">",30*/

0x02, 0x03, 0x09, 0x0F, 0x06,/*"",31*/

0x3E, 0x7F, 0x5F, 0x7F, 0x3E,/*"@",32*/

0x7E, 0x7F, 0x09, 0x09, 0x7E,/*"A",33*/

0x7F, 0x7F, 0x49, 0x4F, 0x70,/*"B",33*/

0x3E, 0x7F, 0x41, 0x63, 0x62,/*"C",35*/

0x7F, 0x7F, 0x41, 0x41, 0x3E,/*"D",36*/

0x7F, 0x7F, 0x49, 0x49, 0x41,/*"E",37*/

0x7F, 0x7F, 0x09, 0x09, 0x01,/*"F",38*/

0x3E, 0x7F, 0x41, 0x53, 0x32,/*"G",39*/

0x7F, 0x7F, 0x08, 0x08, 0x7F,/*"H",40*/

0x41, 0x7F, 0x7F, 0x41, 0x00,/*"I",40*/

0x61, 0x41, 0x7F, 0x7F, 0x01,/*"J",40*/

0x7F, 0x7F, 0x1C, 0x36, 0x63,/*"K",40*/

0x7F, 0x7F, 0x40, 0x40, 0x40,/*"L",44*/

0x7F, 0x0F, 0x3C, 0x0F, 0x7F,/*"M",45*/

0x7F, 0x7F, 0x04, 0x08, 0x7F,/*"N",45*/

0x3E, 0x7F, 0x41, 0x41, 0x3E,/*"O",45*/

0x7F, 0x7F, 0x09, 0x0F, 0x06,/*"P",48*/

0x3E, 0x7F, 0x41, 0x61, 0x7E,/*"Q",45*/

0x7F, 0x7F, 0x09, 0x79, 0x4E,/*"R",45*/

0x66, 0x6F, 0x49, 0x79, 0x32,/*"S",51*/

0x01, 0x7F, 0x7F, 0x01, 0x01,/*"T",52*/

0x3F, 0x7F, 0x40, 0x40, 0x3F,/*"U",53*/

0x7F, 0x7F, 0x30, 0x0C, 0x03,/*"V",45*/

0x3F, 0x78, 0x1E, 0x78, 0x3F,/*"W",45*/

0x71, 0x19, 0x7F, 0x4C, 0x47,/*"X",45*/

0x07, 0x0F, 0x78, 0x0C, 0x07,/*"Y",45*/

0x63, 0x73, 0x49, 0x45, 0x63,/*"Z",45*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"[",59*/

0x00, 0x0E, 0x7E, 0x70, 0x00,/*"\",60*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"]",61*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"^",62*/

0x08, 0x08, 0x08, 0x08, 0x08,/*"-",63*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",64*/

0x68, 0x7C, 0x7C, 0x7C, 0x40,/*"a",65*/

0x3F, 0x7F, 0x44, 0x7C, 0x38,/*"b",66*/

0x38, 0x7C, 0x44, 0x44, 0x00,/*"c",67*/

0x38, 0x7C, 0x44, 0x7F, 0x7F,/*"d",68*/

0x28, 0x7C, 0x54, 0x5C, 0x18,/*"e",69*/

0x04, 0x7E, 0x7F, 0x05, 0x01,/*"f",70*/

0x78, 0x7C, 0x7C, 0x5C, 0x04,/*"g",71*/

0x7F, 0x7F, 0x44, 0x7C, 0x78,/*"h",72*/

0x00, 0x7C, 0x7C, 0x00, 0x00,/*"i",73*/

0x00, 0x00, 0x7D, 0x7D, 0x00,/*"j",74*/

0x6F, 0x7F, 0x7C, 0x6C, 0x44,/*"k",75*/

0x00, 0x7F, 0x7F, 0x00, 0x00,/*"l",76*/

0x7C, 0x7C, 0x7C, 0x78, 0x78,/*"m",77*/

0x7C, 0x7C, 0x44, 0x7C, 0x78,/*"n",78*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"o",79*/

0x3C, 0x7C, 0x44, 0x7C, 0x38,/*"p",80*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"q",81*/

0x04, 0x7C, 0x7C, 0x04, 0x00,/*"r",82*/

0x00, 0x7C, 0x7C, 0x54, 0x00,/*"s",83*/

0x04, 0x7C, 0x7C, 0x44, 0x00,/*"t",84*/

0x3C, 0x7C, 0x44, 0x7C, 0x78,/*"u",85*/

0x04, 0x3C, 0x3C, 0x1C, 0x04,/*"v",86*/

0x1C, 0x3C, 0x7C, 0x7C, 0x14,/*"w",87*/

0x44, 0x6C, 0x7C, 0x7C, 0x04,/*"x",88*/

0x04, 0x1C, 0x7C, 0x7C, 0x04,/*"y",89*/

0x00, 0x74, 0x7C, 0x4C, 0x00 }, /*"z",90*/
{
0x00, 0x00, 0x00, 0x00, 0x00,/*" ",0*/

0x00, 0x00, 0x5F, 0x5F, 0x00,/*"!",1*/

0x00, 0x00, 0x01, 0x01, 0x00,/*""",2*/

0x30, 0x3E, 0x7E, 0x7E, 0x10,/*"#",3*/

0x00, 0x47, 0x7F, 0x79, 0x00,/*"$",4*/

0x0F, 0x3F, 0x7F, 0x72, 0x70,/*"%",5*/

0x36, 0x7F, 0x7F, 0x7A, 0x40,/*"&",6*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",7*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"(",8*/

0x00, 0x77, 0x77, 0x00, 0x00,/*")",9*/

0x14, 0x1C, 0x1C, 0x1C, 0x14,/*"*",10*/

0x08, 0x08, 0x1C, 0x1C, 0x08,/*"+",11*/

0x00, 0x40, 0x40, 0x00, 0x00,/*",",12*/

0x00, 0x08, 0x08, 0x08, 0x00,/*"-",13*/

0x00, 0x40, 0x40, 0x00, 0x00,/*".",14*/

0x40, 0x78, 0x3E, 0x07, 0x01,/*"/",15*/
//0x3E,0x41,0x41,0x41,0x3E,/*"未命名文件",0*/
//0x08, 0x08, 0x08, 0x08, 0x08,/*2,0*/
//0x20,0x22,0x22,0x22,0x20,/*"未命名文件",0*/
//0x41,0x49,0x49,0x49,0x41,/*"未命名文件",0*/
//0x7F, 0x49, 0x47, 0x49, 0x7F,/*5,0*/
//0x49, 0x7F, 0x49, 0x79, 0x40,/*6,0*/
//0x44, 0x74, 0x05, 0x74, 0x44,/*7,0*/
//0x04, 0x7F, 0x44, 0x44, 0x40,/*8,0*/
//
//0x40,0x3F,0x00,0x3F,0x40,/*"未命名文件",0*/
//0x42, 0x3F, 0x02, 0x7E, 0x40,/*10,0*/
	0x7f,0x7f,0x41,0x7f,0x7f, //  0
	0x00,0x06,0x7f,0x7f,0x00, //  1
	0x79,0x79,0x49,0x4f,0x4f, //	 2
	0x49,0x49,0x49,0x7f,0x7f, //	 3
	0x0f,0x0f,0x08,0x7f,0x7f, //	 4
	0x4f,0x4f,0x49,0x79,0x79, //	 5
	0x7f,0x7f,0x49,0x79,0x78, //	 6
	0x01,0x01,0x71,0x7f,0x0f, //  7
	0x7f,0x7f,0x49,0x7f,0x7f, //	 8
	0x0f,0x4f,0x49,0x7f,0x7f, //	 9
0x00, 0x36, 0x36, 0x00, 0x00,/*":",26*/

0x00, 0x00, 0x44, 0x44, 0x00,/*";",27*/

0x08, 0x1C, 0x36, 0x22, 0x00,/*"<",28*/

0x14, 0x14, 0x14, 0x14, 0x14,/*"=",29*/

0x00, 0x22, 0x36, 0x1C, 0x08,/*">",30*/

0x02, 0x03, 0x09, 0x0F, 0x06,/*"",31*/

0x3E, 0x7F, 0x5F, 0x7F, 0x3E,/*"@",32*/

0x7E, 0x7F, 0x09, 0x09, 0x7E,/*"A",33*/

0x7F, 0x7F, 0x49, 0x4F, 0x70,/*"B",33*/

0x3E, 0x7F, 0x41, 0x63, 0x62,/*"C",35*/

0x7F, 0x7F, 0x41, 0x41, 0x3E,/*"D",36*/

0x7F, 0x7F, 0x49, 0x49, 0x41,/*"E",37*/

0x7F, 0x7F, 0x09, 0x09, 0x01,/*"F",38*/

0x3E, 0x7F, 0x41, 0x53, 0x32,/*"G",39*/

0x7F, 0x7F, 0x08, 0x08, 0x7F,/*"H",40*/

0x41, 0x7F, 0x7F, 0x41, 0x00,/*"I",40*/

0x61, 0x41, 0x7F, 0x7F, 0x01,/*"J",40*/

0x7F, 0x7F, 0x1C, 0x36, 0x63,/*"K",40*/

0x7F, 0x7F, 0x40, 0x40, 0x40,/*"L",44*/

0x7F, 0x0F, 0x3C, 0x0F, 0x7F,/*"M",45*/

0x7F, 0x7F, 0x04, 0x08, 0x7F,/*"N",45*/

0x3E, 0x7F, 0x41, 0x41, 0x3E,/*"O",45*/

0x7F, 0x7F, 0x09, 0x0F, 0x06,/*"P",48*/

0x3E, 0x7F, 0x41, 0x61, 0x7E,/*"Q",45*/

0x7F, 0x7F, 0x09, 0x79, 0x4E,/*"R",45*/

0x66, 0x6F, 0x49, 0x79, 0x32,/*"S",51*/

0x01, 0x7F, 0x7F, 0x01, 0x01,/*"T",52*/

0x3F, 0x7F, 0x40, 0x40, 0x3F,/*"U",53*/

0x7F, 0x7F, 0x30, 0x0C, 0x03,/*"V",45*/

0x3F, 0x78, 0x1E, 0x78, 0x3F,/*"W",45*/

0x71, 0x19, 0x7F, 0x4C, 0x47,/*"X",45*/

0x07, 0x0F, 0x78, 0x0C, 0x07,/*"Y",45*/

0x63, 0x73, 0x49, 0x45, 0x63,/*"Z",45*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"[",59*/

0x00, 0x0E, 0x7E, 0x70, 0x00,/*"\",60*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"]",61*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"^",62*/

0x08, 0x08, 0x08, 0x08, 0x08,/*"-",63*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",64*/

0x68, 0x7C, 0x7C, 0x7C, 0x40,/*"a",65*/

0x3F, 0x7F, 0x44, 0x7C, 0x38,/*"b",66*/

0x38, 0x7C, 0x44, 0x44, 0x00,/*"c",67*/

0x38, 0x7C, 0x44, 0x7F, 0x7F,/*"d",68*/

0x28, 0x7C, 0x54, 0x5C, 0x18,/*"e",69*/

0x04, 0x7E, 0x7F, 0x05, 0x01,/*"f",70*/

0x78, 0x7C, 0x7C, 0x5C, 0x04,/*"g",71*/

0x7F, 0x7F, 0x44, 0x7C, 0x78,/*"h",72*/

0x00, 0x7C, 0x7C, 0x00, 0x00,/*"i",73*/

0x00, 0x00, 0x7D, 0x7D, 0x00,/*"j",74*/

0x6F, 0x7F, 0x7C, 0x6C, 0x44,/*"k",75*/

0x00, 0x7F, 0x7F, 0x00, 0x00,/*"l",76*/

0x7C, 0x7C, 0x7C, 0x78, 0x78,/*"m",77*/

0x7C, 0x7C, 0x44, 0x7C, 0x78,/*"n",78*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"o",79*/

0x3C, 0x7C, 0x44, 0x7C, 0x38,/*"p",80*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"q",81*/

0x04, 0x7C, 0x7C, 0x04, 0x00,/*"r",82*/

0x00, 0x7C, 0x7C, 0x54, 0x00,/*"s",83*/

0x04, 0x7C, 0x7C, 0x44, 0x00,/*"t",84*/

0x3C, 0x7C, 0x44, 0x7C, 0x78,/*"u",85*/

0x04, 0x3C, 0x3C, 0x1C, 0x04,/*"v",86*/

0x1C, 0x3C, 0x7C, 0x7C, 0x14,/*"w",87*/

0x44, 0x6C, 0x7C, 0x7C, 0x04,/*"x",88*/

0x04, 0x1C, 0x7C, 0x7C, 0x04,/*"y",89*/

0x00, 0x74, 0x7C, 0x4C, 0x00 }, /*"z",90*/
{
0x00, 0x00, 0x00, 0x00, 0x00,/*" ",0*/

0x00, 0x00, 0x5F, 0x5F, 0x00,/*"!",1*/

0x00, 0x00, 0x01, 0x01, 0x00,/*""",2*/

0x30, 0x3E, 0x7E, 0x7E, 0x10,/*"#",3*/

0x00, 0x47, 0x7F, 0x79, 0x00,/*"$",4*/

0x0F, 0x3F, 0x7F, 0x72, 0x70,/*"%",5*/

0x36, 0x7F, 0x7F, 0x7A, 0x40,/*"&",6*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",7*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"(",8*/

0x00, 0x77, 0x77, 0x00, 0x00,/*")",9*/

0x14, 0x1C, 0x1C, 0x1C, 0x14,/*"*",10*/

0x08, 0x08, 0x1C, 0x1C, 0x08,/*"+",11*/

0x00, 0x40, 0x40, 0x00, 0x00,/*",",12*/

0x00, 0x08, 0x08, 0x08, 0x00,/*"-",13*/

0x00, 0x40, 0x40, 0x00, 0x00,/*".",14*/

0x40, 0x78, 0x3E, 0x07, 0x01,/*"/",15*/

0x70, 0x5F, 0x41, 0x5F, 0x70,/*"0",0*/
0x40, 0x41, 0x7F, 0x40, 0x70,/*"1",0*/
0x71, 0x51, 0x51, 0x51, 0x5F,/*"2",0*/
0x40, 0x41, 0x51, 0x51, 0x7F,/*"3",0*/
0x1F, 0x10, 0x10, 0x10, 0x7E,/*"4",0*/
0x5F, 0x51, 0x51, 0x51, 0x70,/*"5",0*/
0x7F, 0x51, 0x51, 0x50, 0x70,/*"6",0*/
0x01, 0x01, 0x61, 0x11, 0x0F,/*"7",0*/
0x70, 0x5F, 0x51, 0x5F, 0x70,/*"8",0*/
0x1F, 0x11, 0x51, 0x51, 0x7F,/*"9",0*/
0x00, 0x36, 0x36, 0x00, 0x00,/*":",26*/

0x00, 0x00, 0x44, 0x44, 0x00,/*";",27*/

0x08, 0x1C, 0x36, 0x22, 0x00,/*"<",28*/

0x14, 0x14, 0x14, 0x14, 0x14,/*"=",29*/

0x00, 0x22, 0x36, 0x1C, 0x08,/*">",30*/

0x02, 0x03, 0x09, 0x0F, 0x06,/*"",31*/

0x3E, 0x7F, 0x5F, 0x7F, 0x3E,/*"@",32*/

0x7E, 0x7F, 0x09, 0x09, 0x7E,/*"A",33*/

0x7F, 0x7F, 0x49, 0x4F, 0x70,/*"B",33*/

0x3E, 0x7F, 0x41, 0x63, 0x62,/*"C",35*/

0x7F, 0x7F, 0x41, 0x41, 0x3E,/*"D",36*/

0x7F, 0x7F, 0x49, 0x49, 0x41,/*"E",37*/

0x7F, 0x7F, 0x09, 0x09, 0x01,/*"F",38*/

0x3E, 0x7F, 0x41, 0x53, 0x32,/*"G",39*/

0x7F, 0x7F, 0x08, 0x08, 0x7F,/*"H",40*/

0x41, 0x7F, 0x7F, 0x41, 0x00,/*"I",40*/

0x61, 0x41, 0x7F, 0x7F, 0x01,/*"J",40*/

0x7F, 0x7F, 0x1C, 0x36, 0x63,/*"K",40*/

0x7F, 0x7F, 0x40, 0x40, 0x40,/*"L",44*/

0x7F, 0x0F, 0x3C, 0x0F, 0x7F,/*"M",45*/

0x7F, 0x7F, 0x04, 0x08, 0x7F,/*"N",45*/

0x3E, 0x7F, 0x41, 0x41, 0x3E,/*"O",45*/

0x7F, 0x7F, 0x09, 0x0F, 0x06,/*"P",48*/

0x3E, 0x7F, 0x41, 0x61, 0x7E,/*"Q",45*/

0x7F, 0x7F, 0x09, 0x79, 0x4E,/*"R",45*/

0x66, 0x6F, 0x49, 0x79, 0x32,/*"S",51*/

0x01, 0x7F, 0x7F, 0x01, 0x01,/*"T",52*/

0x3F, 0x7F, 0x40, 0x40, 0x3F,/*"U",53*/

0x7F, 0x7F, 0x30, 0x0C, 0x03,/*"V",45*/

0x3F, 0x78, 0x1E, 0x78, 0x3F,/*"W",45*/

0x71, 0x19, 0x7F, 0x4C, 0x47,/*"X",45*/

0x07, 0x0F, 0x78, 0x0C, 0x07,/*"Y",45*/

0x63, 0x73, 0x49, 0x45, 0x63,/*"Z",45*/

0x00, 0x00, 0x7F, 0x7F, 0x00,/*"[",59*/

0x00, 0x0E, 0x7E, 0x70, 0x00,/*"\",60*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"]",61*/

0x00, 0x00, 0x00, 0x00, 0x00,/*"^",62*/

0x08, 0x08, 0x08, 0x08, 0x08,/*"-",63*/

0x00, 0x01, 0x01, 0x00, 0x00,/*"'",64*/

0x68, 0x7C, 0x7C, 0x7C, 0x40,/*"a",65*/

0x3F, 0x7F, 0x44, 0x7C, 0x38,/*"b",66*/

0x38, 0x7C, 0x44, 0x44, 0x00,/*"c",67*/

0x38, 0x7C, 0x44, 0x7F, 0x7F,/*"d",68*/

0x28, 0x7C, 0x54, 0x5C, 0x18,/*"e",69*/

0x04, 0x7E, 0x7F, 0x05, 0x01,/*"f",70*/

0x78, 0x7C, 0x7C, 0x5C, 0x04,/*"g",71*/

0x7F, 0x7F, 0x44, 0x7C, 0x78,/*"h",72*/

0x00, 0x7C, 0x7C, 0x00, 0x00,/*"i",73*/

0x00, 0x00, 0x7D, 0x7D, 0x00,/*"j",74*/

0x6F, 0x7F, 0x7C, 0x6C, 0x44,/*"k",75*/

0x00, 0x7F, 0x7F, 0x00, 0x00,/*"l",76*/

0x7C, 0x7C, 0x7C, 0x78, 0x78,/*"m",77*/

0x7C, 0x7C, 0x44, 0x7C, 0x78,/*"n",78*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"o",79*/

0x3C, 0x7C, 0x44, 0x7C, 0x38,/*"p",80*/

0x38, 0x7C, 0x44, 0x7C, 0x38,/*"q",81*/

0x04, 0x7C, 0x7C, 0x04, 0x00,/*"r",82*/

0x00, 0x7C, 0x7C, 0x54, 0x00,/*"s",83*/

0x04, 0x7C, 0x7C, 0x44, 0x00,/*"t",84*/

0x3C, 0x7C, 0x44, 0x7C, 0x78,/*"u",85*/

0x04, 0x3C, 0x3C, 0x1C, 0x04,/*"v",86*/

0x1C, 0x3C, 0x7C, 0x7C, 0x14,/*"w",87*/

0x44, 0x6C, 0x7C, 0x7C, 0x04,/*"x",88*/

0x04, 0x1C, 0x7C, 0x7C, 0x04,/*"y",89*/

0x00, 0x74, 0x7C, 0x4C, 0x00 } /*"z",90*/
};


//0x70, 0x5F, 0x41, 0x5F, 0x70,/*"0",0*/
//0x40, 0x41, 0x7F, 0x40, 0x70,/*"1",0*/
//0x71, 0x51, 0x51, 0x51, 0x5F,/*"2",0*/
//0x40, 0x41, 0x51, 0x51, 0x7F,/*"3",0*/
//0x1F, 0x10, 0x10, 0x10, 0x7E,/*"4",0*/
//0x5F, 0x51, 0x51, 0x51, 0x70,/*"5",0*/
//0x7F, 0x51, 0x51, 0x50, 0x70,/*"6",0*/
//0x01, 0x01, 0x61, 0x11, 0x0F,/*"7",0*/
//0x70, 0x5F, 0x51, 0x5F, 0x70,/*"8",0*/
//0x1F, 0x11, 0x51, 0x51, 0x7F,/*"9",0*/


const unsigned char OLED_NumMt[] =
{
	0x7f,0x41,0x41,0x7f,0x7f, // 0
	0x00,0x00,0x7f,0x7f,0x00, // 1
	0x79,0x79,0x49,0x49,0x4f, // 2
	0x41,0x49,0x49,0x7f,0x7f, // 3
	0x0e,0x08,0x08,0x7f,0x7f, // 4
	0x4f,0x49,0x49,0x79,0x79, // 5
	0x7f,0x49,0x49,0x79,0x78, // 6
	0x01,0x01,0x01,0x7f,0x7f, // 7
	0x7f,0x49,0x49,0x7f,0x7f, // 8
	0x0f,0x49,0x49,0x7f,0x7f, // 9
};

unsigned char VFD_NumMtIndex[] = { 0 + 2,3 + 2,7 + 2,11 + 2,15 + 2,18 + 2,20 + 2,24 + 2,27 + 2,29 + 2 };
unsigned char onegram[48];
//graphic
unsigned char gram[][5] =
{
	//1
	0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00,0x00,//7
	0x00,0x00,0x00,0x00,0x00,//8
};
//target
unsigned char tram[][5] =
{
	//1
	0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00,0x00,//7
	0x00,0x00,0x00,0x00,0x00,//8
};
//before
unsigned char bram[][5] =
{
	//1
	0x00,0x00,0x00,0x00,0x00,//1
	0x00,0x00,0x00,0x00,0x00,//2
	0x00,0x00,0x00,0x00,0x00,//3
	0x00,0x00,0x00,0x00,0x00,//4
	0x00,0x00,0x00,0x00,0x00,//5
	0x00,0x00,0x00,0x00,0x00,//6
	0x00,0x00,0x00,0x00,0x00,//7
	0x00,0x00,0x00,0x00,0x00,//8
};

//graphic
char disram[16][2] = { 0 };

uint8_t bytejudge(uint8_t a)
{
	return (a != 0) ? 1 : 0;
}

unsigned char NixAnim::checkdiff(unsigned char* array1, unsigned char* array2, int len)
{
	int i;
	for (i = 0; i < len; i++)
	{
		if (array1[i] != array2[i])
			return 0;
	}
	return 1;
}
void NixAnim::animotion(const char* ch, int motion, int font, int mode)
{
	int i, k;
	static int animotionstep[20] = { 0 };
	if(font>4)
		font=4;
	for (k = 0; k < 5; k++)
		for (i = 0; i < 8; i++)
		{
			if (ch[i] != 0)
				tram[i][k] = Fonts[font][(ch[i] - ' ') * 5 + k];
			else
				tram[i][k] = 0;
		}
	for (k = 0; k < 16; k++)
	{
		if (checkdiff(gram[k], tram[k], 5) == 0)
		{
			switch (motion)
			{
			case 0:
				switch (animotionstep[k])
				{
				case 0:for (i = 0; i < 5; i++) bram[k][i] = gram[k][i]; break;
				case 1:gram[k][2] &= ~0x8; break;
				case 2:gram[k][1] &= ~0x8; gram[k][2] &= ~0x1C; gram[k][3] &= ~0x8; break;
				case 3:gram[k][0] &= ~0x8; gram[k][1] &= ~0x1C; gram[k][2] &= ~0x3E; gram[k][3] &= ~0x1C; gram[k][4] &= ~0x8; break;
				case 4:gram[k][0] &= ~0x1C; gram[k][1] &= ~0x3E; gram[k][2] = tram[k][2] & 0x8; gram[k][3] &= ~0x3E; gram[k][4] &= ~0x1C; break;
				case 5:gram[k][0] &= ~0x3E; gram[k][1] = tram[k][1] & 0x8; gram[k][2] = tram[k][2] & 0x1C; gram[k][3] = tram[k][3] & 0x8; gram[k][4] &= ~0x3E; break;
				case 6:gram[k][0] = tram[k][0] & 0x8; gram[k][1] = tram[k][1] & 0x1C; gram[k][2] = tram[k][2] & 0x3E; gram[k][3] = tram[k][3] & 0x1C; gram[k][4] = tram[k][4] & 0x8; break;
				case 7:gram[k][0] = tram[k][0] & 0x1C; gram[k][1] = tram[k][1] & 0x3E; gram[k][2] = tram[k][2]; gram[k][3] = tram[k][3] & 0x3E; gram[k][4] = tram[k][4] & 0x1C; break;
				case 8:gram[k][0] = tram[k][0] & 0x3E; gram[k][1] = tram[k][1]; gram[k][2] = tram[k][2]; gram[k][3] = tram[k][3]; gram[k][4] = tram[k][4] & 0x3E; break;
				case 9:for (i = 0; i < 5; i++) gram[k][i] = tram[k][i]; animotionstep[k] = 0; break;
				default:animotionstep[k] = 0; break;
				}break;
			case 1:
				switch (animotionstep[k])
				{
				case 0:for (i = 0; i < 5; i++) bram[k][i] = gram[k][i]; break;
				case 1:gram[k][4] &= ~0x40; break;
				case 2:gram[k][4] &= ~0x60; gram[k][3] &= ~0x40; break;
				case 3:gram[k][4] &= ~0x70; gram[k][3] &= ~0x60; gram[k][2] &= ~0x40; break;
				case 4:gram[k][4] &= ~0x78; gram[k][3] &= ~0x70; gram[k][2] &= ~0x60; gram[k][1] &= ~0x40;
					gram[k][4] |= tram[k][4] & 0x40; break;
				case 5:gram[k][4] &= ~0x7C; gram[k][3] &= ~0x78; gram[k][2] &= ~0x70; gram[k][1] &= ~0x60; gram[k][0] &= ~0x40;
					gram[k][4] |= tram[k][4] & 0x60; gram[k][3] |= tram[k][3] & 0x40; break;
				case 6:gram[k][4] &= ~0x7E; gram[k][3] &= ~0x7C; gram[k][2] &= ~0x78; gram[k][1] &= ~0x70; gram[k][0] &= ~0x60;
					gram[k][4] |= tram[k][4] & 0x70; gram[k][3] |= tram[k][3] & 0x60; gram[k][2] |= tram[k][2] & 0x40; break;
				case 7:gram[k][3] &= ~0x7E; gram[k][2] &= ~0x7C; gram[k][1] &= ~0x78; gram[k][0] &= ~0x70;
					gram[k][4] = tram[k][4] & 0x78; gram[k][3] |= tram[k][3] & 0x70; gram[k][2] |= tram[k][2] & 0x60; gram[k][1] |= tram[k][1] & 0x40; break;
				case 8:gram[k][2] &= ~0x7E; gram[k][1] &= ~0x7C; gram[k][0] &= ~0x78;
					gram[k][4] = tram[k][4] & 0x7C; gram[k][3] = tram[k][3] & 0x78; gram[k][2] |= tram[k][2] & 0x70; gram[k][1] |= tram[k][1] & 0x60; gram[k][0] |= tram[k][0] & 0x40; break;
				case 9:gram[k][1] &= ~0x7E; gram[k][0] &= ~0x7C;
					gram[k][4] = tram[k][4] & 0x7E; gram[k][3] = tram[k][3] & 0x7C; gram[k][2] = tram[k][2] & 0x78; gram[k][1] |= tram[k][1] & 0x70; gram[k][0] |= tram[k][0] & 0x60; break;
				case 10:gram[k][0] &= ~0x7E;
					gram[k][4] = tram[k][4]; gram[k][3] = tram[k][3] & 0x7E; gram[k][2] = tram[k][2] & 0x7C; gram[k][1] = tram[k][1] & 0x78; gram[k][0] |= tram[k][0] & 0x70; break;
				case 11:
					gram[k][4] = tram[k][4]; gram[k][3] = tram[k][3]; gram[k][2] |= tram[k][2] & 0x7E; gram[k][1] |= tram[k][1] & 0x7C; gram[k][0] |= tram[k][0] & 0x78; break;
				case 12:
					gram[k][4] = tram[k][4]; gram[k][3] = tram[k][3]; gram[k][2] = tram[k][2]; gram[k][1] |= tram[k][1] & 0x7E; gram[k][0] |= tram[k][0] & 0x7C; break;
				case 13:
					gram[k][4] = tram[k][4]; gram[k][3] = tram[k][3]; gram[k][2] = tram[k][2]; gram[k][1] = tram[k][1]; gram[k][0] |= tram[k][0] & 0x7E; break;
				case 14:
					gram[k][4] = tram[k][4]; gram[k][3] = tram[k][3]; gram[k][2] = tram[k][2]; gram[k][1] = tram[k][1]; gram[k][0] = tram[k][0]; break;
				default:animotionstep[k] = 0; break;
				}break;
			case 2:
				switch (animotionstep[k])
				{
				case 0:for (i = 0; i < 5; i++) bram[k][i] = gram[k][i]; break;
				case 1:gram[k][0] &= ~0x40; break;
				case 2:gram[k][0] &= ~0x60; gram[k][1] &= ~0x40; break;
				case 3:gram[k][0] &= ~0x70; gram[k][1] &= ~0x60; gram[k][2] &= ~0x40; break;
				case 4:gram[k][0] &= ~0x78; gram[k][1] &= ~0x70; gram[k][2] &= ~0x60; gram[k][3] &= ~0x40;
					gram[k][0] |= tram[k][0] & 0x40; break;
				case 5:gram[k][0] &= ~0x7C; gram[k][1] &= ~0x78; gram[k][2] &= ~0x70; gram[k][3] &= ~0x60; gram[k][4] &= ~0x40;
					gram[k][0] |= tram[k][0] & 0x60; gram[k][1] |= tram[k][1] & 0x40; break;
				case 6:gram[k][0] &= ~0x7E; gram[k][1] &= ~0x7C; gram[k][2] &= ~0x78; gram[k][3] &= ~0x70; gram[k][4] &= ~0x60;
					gram[k][0] |= tram[k][0] & 0x70; gram[k][1] |= tram[k][1] & 0x60; gram[k][2] |= tram[k][2] & 0x40; break;
				case 7:gram[k][1] &= ~0x7E; gram[k][2] &= ~0x7C; gram[k][3] &= ~0x78; gram[k][4] &= ~0x70;
					gram[k][0] = tram[k][0] & 0x78; gram[k][1] |= tram[k][1] & 0x70; gram[k][2] |= tram[k][2] & 0x60; gram[k][3] |= tram[k][3] & 0x40; break;
				case 8:gram[k][2] &= ~0x7E; gram[k][3] &= ~0x7C; gram[k][4] &= ~0x78;
					gram[k][0] = tram[k][0] & 0x7C; gram[k][1] = tram[k][1] & 0x78; gram[k][2] |= tram[k][2] & 0x70; gram[k][3] |= tram[k][3] & 0x60; gram[k][4] |= tram[k][4] & 0x40; break;
				case 9:gram[k][3] &= ~0x7E; gram[k][4] &= ~0x7C;
					gram[k][0] = tram[k][0] & 0x7E; gram[k][1] = tram[k][1] & 0x7C; gram[k][2] = tram[k][2] & 0x78; gram[k][3] |= tram[k][3] & 0x70; gram[k][4] |= tram[k][4] & 0x60; break;
				case 10:gram[k][4] &= ~0x7E;
					gram[k][0] = tram[k][0]; gram[k][1] = tram[k][1] & 0x7E; gram[k][2] = tram[k][2] & 0x7C; gram[k][3] = tram[k][3] & 0x78; gram[k][4] |= tram[k][4] & 0x70; break;
				case 11:
					gram[k][0] = tram[k][0]; gram[k][1] = tram[k][1]; gram[k][2] |= tram[k][2] & 0x7E; gram[k][3] |= tram[k][3] & 0x7C; gram[k][4] |= tram[k][4] & 0x78; break;
				case 12:
					gram[k][0] = tram[k][0]; gram[k][1] = tram[k][1]; gram[k][2] = tram[k][2]; gram[k][3] |= tram[k][3] & 0x7E; gram[k][4] |= tram[k][4] & 0x7C; break;
				case 13:
					gram[k][0] = tram[k][0]; gram[k][1] = tram[k][1]; gram[k][2] = tram[k][2]; gram[k][3] = tram[k][3]; gram[k][4] |= tram[k][4] & 0x7E; break;
				case 14:
					gram[k][0] = tram[k][0]; gram[k][1] = tram[k][1]; gram[k][2] = tram[k][2]; gram[k][3] = tram[k][3]; gram[k][4] = tram[k][4]; break;
				default:animotionstep[k] = 0; break;
				}break;
			case 3://
				for (i = 0; i < 5; i++)
				{
					gram[k][i] <<= 1;
					gram[k][i] |= (tram[k][i] >> (7 - animotionstep[k])) & 1;
				}break;
			case 4://
				for (i = 0; i < 5; i++)
				{
					gram[k][i] >>= 1;
					gram[k][i] |= (tram[k][i] << (7 - animotionstep[k])) & 0x80;
				}break;
			case 5://
				for (i = 0; i < 4; i++)
				{
					gram[k][i] = gram[k][i + 1];
				}
				if (animotionstep[k] > 0)
					gram[k][4] = tram[k][animotionstep[k] - 1];
				else
					gram[k][4] = 0;
				break;
			case 6://
				for (i = 4; i > 0; i--)
				{
					gram[k][i] = gram[k][i - 1];
				}
				if (animotionstep[k] > 0)
					gram[k][0] = tram[k][4 - animotionstep[k] + 1];
				else
					gram[k][0] = 0;
				break;
			case 7://
				for (i = 0; i < 5; i++)
				{
					if (i <= animotionstep[k])
					{
						if (animotionstep[k] - i <= 7)
						{
							gram[k][i] <<= 1;
							gram[k][i] |= (tram[k][i] >> (7 - (animotionstep[k] - i))) & 1;
						}
					}
				}break;
			case 8://
				for (i = 0; i < 5; i++)
				{
					if (i <= animotionstep[k])
					{
						if (animotionstep[k] - i <= 7)
						{
							gram[k][4 - i] <<= 1;
							gram[k][4 - i] |= (tram[k][4 - i] >> (7 - (animotionstep[k] - i))) & 1;
						}
					}
				}break;
			case 9:for (i = 0; i < 5; i++) gram[k][i] = tram[k][i];
				animotionstep[k] = 0; break;
			}
			animotionstep[k]++;
			if (animotionstep[k] > 16)
			{
				for (i = 0; i < 5; i++)
					gram[k][i] = tram[k][i];
				animotionstep[k] = 0;
			}
			if (mode)
				return;
		}
		else
		{
			for (i = 0; i < 5; i++)
				gram[k][i] = tram[k][i];
			animotionstep[k] = 0;
		}
	}
}

void NixAnim::gram2one(void)
{
	int k,i;
	for (k = 0; k < 8; k++)
		for (i = 0; i < 5; i++)
			onegram[k*6+i] = gram[k][i];
}