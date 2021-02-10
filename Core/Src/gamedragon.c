
typedef enum 
{
	ready,playing,gameover
}GAMESTS;

typedef struct 
{
	uint8_t is_jump;
	uint8_t is_jumpfinish;
	uint8_t is_fire;
}PLAYSTS;

PLAYSTS status;
void blue_click(){
    if(playstatus==is_jumpfinish)
		{
        status.is_jump=True;
        status.is_jumpfinish=False;
		}
}

GAMESTS gamestatus;
void red_click(){
    //start game
    if(gamestatus==ready){
        gamestatus=playing;}
    //fire
    if(gamestatus==playing){
        fire();}
    //restart game
    if(gamestatus==gameover){
        begin();
        gamestatus=playing;}
}
    
void fire(){
}

gamestatus=loading;

int gametime=0;
int distance=0;

status.is_jump=False;
status.is_fire=False;
status.is_jumpfinish=True;

//player.x = 10
//player.y = 44
unsigned char player[] = {
        0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,
        0,0,0,0,0,0,0,0,0,0,1,1,0,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,
        0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,
        0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,
        1,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,
        1,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,
        1,1,0,0,0,0,1,1,1,1,1,1,1,1,0,1,0,0,0,0,
        1,1,1,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
        1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,
        0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,
        0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,
        0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,
        0,0,0,0,1,1,1,1,0,1,1,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,0,0,0,0,
        0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0
};

//obj.x = 130
//obj.y = 44
unsigned char obj[] = {
    0,0,0,0,1,1,1,1,0,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    1,1,0,1,1,1,1,1,1,0,1,1,
    1,1,0,1,1,1,1,1,1,0,1,1,
    1,1,0,1,1,1,1,1,1,0,1,1,
    1,1,0,1,1,1,1,1,1,0,1,1,
    1,1,0,1,1,1,1,1,1,0,1,1,
    1,1,0,1,1,1,1,1,1,0,1,1,
    0,1,1,1,1,1,1,1,1,1,1,0,
    0,0,1,1,1,1,1,1,1,1,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0,
    0,0,0,1,1,1,1,1,1,0,0,0
};

typedef enum
{
	x,y
}POS;
POS obj;
POS player;
void begin(){
    gamestatus=loading;
    gametime=0;
    distance=0;
    status.is_jump=False;
    status.is_fire=False;
    status.is_jumpfinish=True;

    obj.x = 130;
    obj.y = 44;
    player.y = 44;
}


void draw_player(){
    
    if(playstatus==is_jump){
        player.y-=3;
        if(player.y<15)
            playstatus==is_jump;
			}
    else{
        player.y+=3;
        if(player.y>=43)
            player.y=43;
            playstatus[is_jumpfinish]=True;
			}

			//draw(player.x,player.y,20,20,player);

void draw_obj(){
    obj.x-=4;

			//draw(obj.x,obj.y,20,20,obj);
    
    if(obj.x<=-10){
        obj.x=130;}
			}

void check(){
    if(obj.x-player.x<15 && obj.y-player.y<15){
        gamestatus=gameover;
    }
}
    // pixels_a = player[pixel]
    // pixels_b = obj[pixel]


while (True){
//    oled.fill(0)
//    oled.contrast(1)

//    blue = pin_blue.value()
//    red = pin_red.value()

//    if(pin_red.value() == 0):
//        red_click()
    if(gamestatus==loading){
//        oled.text(loading game..format(distance),10,30)
//        oled.show()
//        sleep(1)
//        oled.text(loading game...format(distance),10,30)
//        oled.show()
//        sleep(1)
//        oled.text(loading game....format(distance),10,30)
//        oled.show()
//        sleep(1)
        gamestatus=ready;
	}
    if(gamestatus==ready){
			
//        oled.text(> play.format(distance),10,20)
//        oled.text(code by.format(distance),10,30)
//        oled.text(cr4fun.format(distance),10,40)
		}
    if(gamestatus==playing){
//        if(pin_blue.value() == 0):
//            blue_click()
        distance+=1;
        gametime+=1;
        graphics.line(0, 63, 127, 63, 1);
//        oled.text({0} km.format(distance),2,0);
        draw_player();
        draw_obj();
        check();
		}
    if(gamestatus==gameover)
		{
//        oled.text({0} km.format(distance),2,0);
//        oled.text(game over,25,30);
			}
    oled.show()
		}