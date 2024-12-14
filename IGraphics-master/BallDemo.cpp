# include "iGraphics.h"
#include<bits/stdc++.h>
#define TOTALBR 150
#define SCREENWIDTH 1400
#define SCREENHEIGHT 750
#define MARGIN 50
#define RAD 10
#define PI 3.141592653589793
#define paddle 0
#define ball_speed 1
#define errorPage 7
#define demoPage 8

int perkType = -1, show_2nd_paddle = 0;
struct _perk_duration
{
    int perk_time = -1;
} perk_duration[2];
struct brick
{
    int x;
    int y;
    int dx=100;
    int dy=20;
    int show=1;
    int r=0;
    int g=0;
    int b=0;
} bricks[TOTALBR];
struct Perk
{
    int x;
    int y;
    int show=0;
} perk_faller[15];
struct ButtonCo_ordinate
{
    int x;
    int y;
} b1_coordinate[5];
struct _saveScore
{
    char name[100];
    int score;
}saveScore[11];

char instruction[10][100] = {
        "Choose Start from Home menu",
        "Brick Value : 1",
        "There are certain perks / damages :  Wider Paddle, ",
        "Faster ball speed, Extra life",
        "When the timer ends//life finishes//no bricks remains",
        " the Player needs to input his/her NAME",
        "You can see your high score from the HIGH SCORE option",
        "Player can RESUME this game from the LAST PLAYED STATE"
};
char button1[ ][20]= {"Start.bmp","Resume.bmp","highscore.bmp","instruction.bmp","exit.bmp"};
char button2[ ][20]= {"resume_button.bmp","Pause_button.bmp","home_button.bmp"};
char button3[ ][25]= {"wider_paddle1.bmp","extra_life1.bmp","faster1.bmp"};
char homemenu[ ]="dx-ball-home.bmp";
char ins[ ]="instruct.bmp";
char backround[ ]="dx-wood.bmp";
char playerName[100]="  ";
int name_len=0;
char name[10][100];
char timer[20]="Time : 00 : 00";
int timer_second = 0;
int total_life=3;
char life[9]="Life : ";
char score[20]="Score : 000";
int perk[15]= {17,41,48,90,71,80,92,108,122,147,133,63,12,115,29};
int page=0,resume=0;
int xball=SCREENWIDTH/2,yball=MARGIN,cnt=0,song=1,song6=1,song1=1;
int demo[4]={1};
int idx=0;
int v=10,qeta=45;
int dx=(int) v*cos(qeta*PI/180);
int dy=(int) v*sin(qeta*PI/180);
int xboard=xball-50;
int yboard=yball-30;
int dxboard=100;
int dyboard=30-RAD;
int start=0;
int check_game=-1;
void ballchange();
void restartball();
void buildbricks();
void getperks();
void timer_change();
void score_change();
void perk_changer();
void save_score_and_name();
void show_scores();
void reset_play_screen();
void saveGameState();
void loadGameState();
void set_game_to_null();
void check_previous_game();
void check_for_game();
void set_game_to_value();

void iDraw()
{
    //place your drawing codes here
    iClear();
    iShowBMP(0,0,backround);
    if(page== 0)
    {
        iShowBMP(0,0,homemenu);
        if(song)
        {
            PlaySound("video_game_ringtone.wav",NULL,SND_ASYNC | SND_LOOP);
            song=0;
        }
        for(int i=0; i<5; i++)
        {
            iShowBMP(b1_coordinate[i].x,b1_coordinate[i].y,button1[i]);
        }
    }
    else if(page==1)
    {
        iShowBMP(1335,700,button2[resume]);
        iShowBMP(1330,630,button2[2]);
         if(song1){
        PlaySound("beep.wav",NULL,SND_ASYNC);
        song1=0;}

        iSetColor(0,0,0);

        iFilledCircle(xball,yball,RAD,100);

      //  iFilledRectangle(xboard,yboard,dxboard,dyboard);

  if (show_2nd_paddle) iShowBMP2(xboard, yboard, "paddle_big.bmp", 0x00ffffff);
        else iShowBMP2(xboard, yboard, "paddle.bmp", 0x00ffffff);

        //timer
        iSetColor(255,0,0);
        iText(1250,605,timer,GLUT_BITMAP_TIMES_ROMAN_24);
        //life
        iText(1250,575,life,GLUT_BITMAP_TIMES_ROMAN_24);
        int life_x=1315;

        for(int i=0; i<total_life; i++)
        {
            iSetColor(0,0,0);//black
            iFilledCircle(life_x,582,8,100);
            life_x+=20;
        }
        //Score
        iSetColor(255,0,0);//red
        iText(1250,545,score,GLUT_BITMAP_TIMES_ROMAN_24);
        //perks
        int j;
        for(j=0; j<15; j++)
        {
            if(perk_faller[j].show)
            {
                iShowBMP2(perk_faller[j].x,perk_faller[j].y,button3[j%3],0x00ffffff);
            }
        }
        int i;
        for(i=0; i<TOTALBR; i++)
        {
            if(bricks[i].show)
            {
                iSetColor(bricks[i].r,bricks[i].g,bricks[i].b);

                iFilledRectangle(bricks[i].x,bricks[i].y,bricks[i].dx,bricks[i].dy);
            }
        }
    }
    else if(page==2 && check_game==1)
    {
      loadGameState();
      page=1;
    }
    else if(page==3)
    {
        iSetColor(255,0,0);iText(600,660,"Names and Scores",GLUT_BITMAP_TIMES_ROMAN_24);

        for(int i=0,text_y=600;i<10;i++,text_y-=35){
            iText(550,text_y,name[i],GLUT_BITMAP_TIMES_ROMAN_24);
        }
        iSetColor(255,150,150);iFilledRectangle(450,50,450,40);
        iSetColor(255,0,0);iText(600,60,"GO BACK" , GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else if(page==4)
    {
        iSetColor(255,150,150); iFilledRectangle(250,550,300,50);
        iSetColor(255, 0, 0);iText(260,560,"See demonstration",GLUT_BITMAP_TIMES_ROMAN_24);
         for(int i=0,int_y=500;i<8;i++,int_y-=50)
         {
             iText(300,int_y,instruction[i]);
         }
         iSetColor(255,150,150);iFilledRectangle(450,50,450,40);
        iSetColor(255,0,0);iText(600,60,"GO BACK" , GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else if(page==5)
    {

    }
    else if(page==6){

        if(song6){
        PlaySound("game_over.wav",NULL,SND_ASYNC);
        song6=0;}
        iShowBMP2(500,600,"game_over.bmp",0x00ffffff);
        iSetColor(255,0,0);
        iText(500,520,score,GLUT_BITMAP_TIMES_ROMAN_24);
        iText(400,500,"Your name must be within 100 letters!",GLUT_BITMAP_HELVETICA_18);

        iSetColor(0,150,150);
        iFilledRectangle(400,350,500,50);

        iSetColor(255,255,0);
        iText(425,365,"Enter your name : ",GLUT_BITMAP_TIMES_ROMAN_24);
        iSetColor(255,255,255);
        iText(625,365,playerName,GLUT_BITMAP_TIMES_ROMAN_24);

        iSetColor(0,150,150);
        iFilledRectangle(400,280,280,50);

        iSetColor(255,255,0);
        iText(425,295,"Back To Main Menu",GLUT_BITMAP_TIMES_ROMAN_24);

    }
    else if(page==errorPage)
    {
        iSetColor(255,0,0);iText(350,600,"There is no savegame,Please go back and start new game",GLUT_BITMAP_TIMES_ROMAN_24);
       iSetColor(255,150,150);iFilledRectangle(450,50,450,40);
      iSetColor(255,0,0);iText(600,60,"GO BACK" , GLUT_BITMAP_TIMES_ROMAN_24);
    }
    else if(page==demoPage)
    {
        iShowBMP(0,0,ins);
        for (int i=500; i<=800; i+=100) iCircle(i, 240, 10);
        if (demo[0]) { iSetColor(0, 0, 255); iFilledCircle(500, 240, 10);iShowBMP(340, 260, "demo1.bmp"); }
        else if (demo[1]) { iSetColor(0, 0, 255); iFilledCircle(600, 240, 10); iShowBMP(340, 260, "demo2.bmp"); }
        else if (demo[2]) { iSetColor(0, 0, 255); iFilledCircle(700, 240, 10); iShowBMP(340, 260, "demo3.bmp"); }
        else if (demo[3]) { iSetColor(0, 0, 255); iFilledCircle(800, 240, 10); iShowBMP(340, 260, "demo4.bmp"); }
        iSetColor(255,150,150);iFilledRectangle(450,50,450,40);
       iSetColor(255,0,0);iText(600,60,"GO BACK" , GLUT_BITMAP_TIMES_ROMAN_24);
    }

}

/*
	function iMouseMove() is called when the user presses and drags the mouse.
	(mx, my) is the position where the mouse pointer is.
*/
void iMouseMove(int mx, int my)
{
    if(mx>=2 && mx<=1400-dxboard){
            if(start==1)
        xboard=mx;
        if(start==0)
        xball=xboard+50;
    }
}

/*
   function iMouse() is called when the user presses/releases the mouse.
   (mx, my) is the position where the mouse pointer is.
*/
void iMouse(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if(page==0){
        for(int i=0; i<5; i++){
            if(mx>=b1_coordinate[i].x && mx<=b1_coordinate[i].x+180 && my>=b1_coordinate[i].y && my<=b1_coordinate[i].y+77){
                   if(i==0){
                        reset_play_screen();
                        page=i+1;
                   }
                   else if(i==1)
                   {
                    check_for_game();
                    if(check_game ==0)
                        page=errorPage;
                        else
                            page=i+1;
                   }
                    else if(i==2)
                    {
                        show_scores();
                        page=i+1;
                    }
                    else if(i==3)
                    {
                        page=i+1;
                    }
                    else if(i==4)
                    {
                        saveGameState();
                        exit(0);
                         page=i+1;
                    }
            }
            }
        }

        if(page==1){
                if(mx>=1340 && mx<=1340+40 && my>=700 && my<=740){
            resume=resume?0:1;
            start=start?0:1;
        }
        if(mx>=1330 && mx<=1330+60 && my>=630 && my<=690){
            //------------------------------------------------------------------------------------------

            start=0;
        //------------------------------------------------------------------------

                saveGameState();
                set_game_to_value();
            page=0;
        }
                }
        if(page==3)
        {
            if(mx>=450 && mx<=900 && my>=50 && my<=90 )
            {
                page=0;
            }
        }
        if(page==4)
        {
             if(mx>=450 && mx<=900 && my>=50 && my<=90 )
              {
                page=0;
              }
              if(mx>=250 && mx<=550 && my>=550 && my<=600)
              {
                  page=demoPage;
                  idx=0;
              }
        }
        if(page==6){
            if(mx>=400 && mx<=680 && my>=280 && my<=330)
            {
                page=0;
              save_score_and_name();
              reset_play_screen();
              set_game_to_null();
              strcpy(playerName," ");
            }
        }
        if(page==errorPage){
             if(mx>=450 && mx<=900 && my>=50 && my<=90 )
              {
                page=0;
              }
        }
        if(page==demoPage)
        {
             if(mx>=450 && mx<=900 && my>=50 && my<=90 )
            {
                page=0;
            }
        }

    }
    if(button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
    {
        //place your codes here
    }
}

/*
	function iKeyboard() is called whenever the user hits a key in keyboard.
	key- holds the ASCII value of the key pressed.
*/
void iKeyboard(unsigned char key)
{
    if(key == 'q')
    {
        start=start?0:1;
    }
    if(page==6)
    {
        if(key=='\b')
        {
            name_len--;
            playerName[name_len]='\0';
            if(name_len<=0)
                name_len = 0;
        }
        else
        {
            playerName[name_len]=key;
            name_len++;
        }
        set_game_to_null();
    }
    //place your codes for other keys here
}

/*
	function iSpecialKeyboard() is called whenver user hits special keys like-
	function keys, home, end, pg up, pg down, arraows etc. you have to use
	appropriate constants to detect them. A list is:
	GLUT_KEY_F1, GLUT_KEY_F2, GLUT_KEY_F3, GLUT_KEY_F4, GLUT_KEY_F5, GLUT_KEY_F6,
	GLUT_KEY_F7, GLUT_KEY_F8, GLUT_KEY_F9, GLUT_KEY_F10, GLUT_KEY_F11, GLUT_KEY_F12,
	GLUT_KEY_LEFT, GLUT_KEY_UP, GLUT_KEY_RIGHT, GLUT_KEY_DOWN, GLUT_KEY_PAGE UP,
	GLUT_KEY_PAGE DOWN, GLUT_KEY_HOME, GLUT_KEY_END, GLUT_KEY_INSERT
*/
void iSpecialKeyboard(unsigned char key)
{
     if (page == demoPage){
        if (key == GLUT_KEY_UP) {
            for (int i=0; i<9; i++) {
                demo[i] = 0;
            }
            if (idx <= 7) demo[++idx] = 1;
            demo[idx] = 1;
        }
        if (key == GLUT_KEY_DOWN) {
            for (int i=0; i<9; i++) {
                demo[i] = 0;
            }
            if (idx >= 1) demo[--idx] = 1;
            demo[idx] = 1;
        }
    }
    else if(key==GLUT_KEY_LEFT)
    {
        if(xboard>0)
        {
            xboard-=10;
            if(!start)
            {
                xball-=10;
            }
        }
    }
    else if(key==GLUT_KEY_RIGHT)
    {
        if(xboard<SCREENWIDTH-dxboard)
        {
            xboard+=10;
            if(!start)
            {
                xball+=10;
            }
        }
    }
    //place your codes for other keys here
}
void time_changer()
{
    if(resume==0 && page==1)
    {
        if(timer_second<10*60)
        {
            timer[8]=48+(timer_second/60);
            timer[12]=48+((timer_second%60)/10);
            timer[13]=48+(timer_second%60)%10;
            timer_second++;
        }
        else{
            page=0;
        }

        //----------------------------------------------------------------------perk_duration-------------------
        if (perk_duration[perkType].perk_time >= 0) perk_duration[perkType].perk_time--; /// Condition for active perk duration
        if (perk_duration[perkType].perk_time == 0) { dxboard = 100; show_2nd_paddle = 0;}

    }
}

void ballchange()
{
    if(start&&resume==0)
    {
        xball+=dx;
        yball+=dy;
        if(xball>=SCREENWIDTH||xball<=0)
        {
            dx=-dx;
        }
        if(yball>=SCREENHEIGHT||yball<=0)
        {
            dy=-dy;
        }
        //Collision
        for(int i=0; i< TOTALBR; i++)
        {
            if(bricks[i].show)
            {
                if(xball>=bricks[i].x && xball<=bricks[i].x+bricks[i].dx)
                {
                    if(yball>=bricks[i].y && yball<=bricks[i].y+bricks[i].dy)
                    {
                        dy=-dy;
                        bricks[i].show=0;
                        cnt++;
                    }
                }
                else if(yball>=bricks[i].y && yball<=bricks[i].y+bricks[i].dy)
                {
                    if(xball>=bricks[i].x && xball<=bricks[i].x+bricks[i].dx)
                    {
                        dx=-dx;
                        bricks[i].show=0;
                        cnt++;
                    }
                }
            }
        }
        if(xball>=xboard && xball<=xboard+dxboard && yball>=yboard && yball<=yboard+dyboard)
        {
            qeta=(xboard+dxboard-xball)+40;
            dx=(int)v*cos(qeta*PI/180);
            dy=(int)v*sin(qeta*PI/180);
        }
      /*  else if(yball>=yboard&&yball<=yboard+dyboard)
        {
            if(xball>=xboard && xball<=xboard+dxboard)
            {
                dx=-dx;
            }
        }*/
        else
        {
            if (yball<yboard)
            {
                total_life--;
                if(total_life<=0)
                    page=6;
                restartball();
            }
        }
    }
}
void perk_changer()
{
    if(resume==0)
    {
        for(int i=0; i<15; i++)
        {
            if(perk_faller[i].show==0)
            {
                if(xball>=perk_faller[i].x && xball<=perk_faller[i].x+80)
                {
                    if(yball>=perk_faller[i].y && yball<=perk_faller[i].y+20)
                    {
                        perk_faller[i].show=1;
                    }
                }
                else if(yball>=perk_faller[i].y && yball<=perk_faller[i].y+20)
                {
                    if(xball>=perk_faller[i].x && xball<=perk_faller[i].x+80)
                    {
                        perk_faller[i].show=1;
                    }
                }
            }
            else if(perk_faller[i].y>yboard)
            {
                perk_faller[i].y-=5;
            }
            else
            {
                perk_faller[i].y-=40;
                perk_faller[i].show=0;
            }
            if(xboard<=perk_faller[i].x && xboard+dxboard>=perk_faller[i].x)
            {
                if(perk_faller[i].y<=dyboard && perk_faller[i].y>=yboard)
                {
                    PlaySound("definite.wav",NULL,SND_ASYNC);
                    if(i%3==0)
                    {
                        xboard-=10;
                        dxboard=120;
                        perk_duration[perkType = paddle].perk_time += 10; /// It will increase duration time for paddle
                        show_2nd_paddle = 1;
                    }
                    else if(i%3==1 && total_life<3)
                    {
                        total_life++;
                    }
                    else
                    {
                        dx=(int)dx*1.5;
                        dy=(int)dy*1.5;

                        //---------------------------------------------------ball_speed--------------------
                         perk_duration[perkType = ball_speed].perk_time += 10;
                         //-------------------------------------------

                    }
                }
            }
        }
    }
}

void score_change()
{
    score[8]=48+cnt/100;
    score[9]=48+(cnt-(cnt/100)*100)/10;
    score[10]=48+(cnt-(cnt/100)*100)%10;
    if(cnt==TOTALBR)
    {
        page=6;
    }
}
void restartball()
{
    xball=SCREENWIDTH/2,yball=MARGIN;
    qeta=45,v=10;
    dx=(int)v*cos(qeta*PI/180);
    dy=(int)v*sin(qeta*PI/180);
    xboard=xball-50;
    yboard=yball-30;
    dxboard=100;
    dyboard=30-RAD;
    start=0;
}
void save_score_and_name()
{
    FILE* fp =NULL;
    int i=0;
    fp = fopen("save_score.txt","r");
    for( i=0;i<10;i++){
        fscanf(fp," %s %d",&saveScore[i].name,&saveScore[i].score);
    }
    strcpy(saveScore[i].name, playerName);
    for (int len = strlen(playerName); len>=0; len--){
        playerName[len] = '\0';}

    saveScore[i].score = cnt;

    struct _saveScore temp;
    for(int j=0 ; j<i ; j++){
        for(int k=0;k<i-j;k++){
            if(saveScore[k].score<saveScore[k+1].score){
                temp=saveScore[k];
                saveScore[k]=saveScore[k+1];
                saveScore[k+1]=temp;
            }
        }
    }
    fclose(fp);
    fp = fopen("save_score.txt","w");
    for(i=0;i<10;i++){
        fprintf(fp,"%s %d\n",saveScore[i].name,saveScore[i].score);
    }
    fclose(fp);
}
void show_scores()
{
    FILE* fp=NULL;
    fp=fopen("save_score.txt","r");
    for(int i=0;i<10;i++){
      fgets(name[i],100,fp);
    }
    fclose(fp);
    page=3;
}
void buildbricks()
{
    int sx=100,sy=700,i;
    for(i=0; i<TOTALBR; i++)
    {
        bricks[i].show=1;
        bricks[i].r=rand()%150;
        bricks[i].g=rand()%150;
        bricks[i].b=rand()%150;
        bricks[i].x=sx;
        bricks[i].y=sy;
        sx+=100;
        if(sx>1000)
        {
            sx=100;
            sy-=20;
        }
    }
}
void getperks()
{
    for(int i=0; i<15; i++)
    {
        perk_faller[i].x=bricks[perk[i]].x;
        perk_faller[i].y=bricks[perk[i]].y;
        perk_faller[i].show=0;
    }
}
void reset_play_screen()
{
    song=1;
    song6=1;
    song1=1;
    total_life=3;
    timer[8]='0';
    timer[12]='0';
    timer[13]='0';
    timer_second=0;
    name_len=0;
    score[8]='0';
    score[9]='0';
    score[10]='0';
    cnt=0;
    dxboard=100;
    restartball();
    buildbricks();
    getperks();
}
void saveGameState()
{
    FILE* fp=fopen("saved_game.txt","w");
    for(int i=0;i<TOTALBR;i++)
        fprintf(fp,"%d %d %d %d %d %d ",bricks[i].x,bricks[i].y,bricks[i].show,bricks[i].r,bricks[i].g,bricks[i].b);
    fprintf(fp,"\n");
    for(int i=0;i<15;i++)
        fprintf(fp,"%d %d %d ",perk_faller[i].x,perk_faller[i].y,perk_faller[i].show);
    fprintf(fp,"\n");

    fprintf(fp,"%d %d %d\n",timer_second,cnt,total_life);

      for (int i=0; i<2; i++)
        fprintf(fp, "%d", perk_duration[i].perk_time);
    fprintf(fp, "\n");
    fprintf(fp,"%d %d %d %d %d %d %d\n",xball,yball,xboard,dxboard,qeta,dx,dy);
    fclose(fp);
}
void check_for_game()
{
    FILE* check=fopen("check_previous_game.txt","r");
    fscanf(check,"%d",&check_game);
    fclose(check);
}
void set_game_to_null()
{
    FILE* fp=fopen("saved_game.txt","w");
    fclose(fp);
    FILE* check=fopen("check_previous_game.txt","w");
    fprintf(check,"%d",0);
    fclose(check);
}
void set_game_to_value()
{
    FILE* check=fopen("check_previous_game.txt","w");
    fprintf(check,"%d",1);
    fclose(check);
}
void loadGameState()
{
    FILE* fp=fopen("saved_game.txt","r");
    for(int i=0;i<TOTALBR;i++){
        fscanf(fp,"%d %d %d %d %d %d",&bricks[i].x,&bricks[i].y,&bricks[i].show,&bricks[i].r,&bricks[i].g,&bricks[i].b);
    }
    for(int i=0;i<15;i++)
    {
        fscanf(fp,"%d %d %d",&perk_faller[i].x,&perk_faller[i].y,&perk_faller[i].show);
    }
    fscanf(fp,"%d %d %d",&timer_second,&cnt,&total_life);

      for (int i=0; i<2; i++)
        fscanf(fp, "%d", &perk_duration[i].perk_time);

    fscanf(fp,"%d %d %d %d %d %d %d",&xball,&yball,&xboard,&dxboard,&qeta,&dx,&dy);
    fclose(fp);
}
int main()
{
    //place your own initialization codes here.
    int sum=100,i;
    for(i=4; i>=0; i--)
    {
        b1_coordinate[i].x=50;
        b1_coordinate[i].y=sum;
        sum+=100;
    }
    if(timer_second%20==0)
    {
        dx++;
        dy++;
    }
    buildbricks();
    getperks();
    iSetTimer(1000,time_changer);
    iSetTimer(10,score_change);
    iSetTimer(10,perk_changer);
    iSetTimer(10,ballchange);
    iInitialize(SCREENWIDTH,SCREENHEIGHT, "Dx-ball");
    return 0;
}

