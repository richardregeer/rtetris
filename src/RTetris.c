
// Libraries
#include <stdio.h>
#include <allegro.h>
#include <rdat.h>

// Palletes
PALLETE	my_pallete;

// Datafiles
DATAFILE *datafile;

// Bitmaps
BITMAP *speelveld;

// Files
FILE *high_file;

// Alleen de driver meenemen die we nodig hebben
BEGIN_GFX_DRIVER_LIST
  GFX_DRIVER_VESA1
END_GFX_DRIVER_LIST

BEGIN_COLOR_DEPTH_LIST
  COLOR_DEPTH_8
END_COLOR_DEPTH_LIST


// Teksten
#define	VERSION	 "2.02"
#define	COPYRIGHT "RTETRIS, Copyright by Richard Regeer 1999"

// Begin instellingen
#define	START_HOKNUMMER	4
#define	BLOK 0
#define	BLOK_KANT 1
#define	SNELHEID 1300
#define	MENU_SNELHEID 500
#define	FADE 4
#define	SOUND_DEF 200

// Video mode
#define	B 640
#define	H 480

// Kleuren
#define	SCORE_KLEUREN 164
#define	DEPTH 8
#define	RANDEN 255
#define	LANGBLOK 166
#define	TBLOK 156
#define	VIERKANT 23
#define	LBLOK_LINKS 193
#define	LBLOK_RECHTS 128
#define	ZBLOK_LINKS 179
#define	ZBLOK_RECHTS  41

// Toetsen
#define	DRAAIEN	57
#define	BENEDEN	80
#define	LINKS 75
#define	RECHTS 77
#define	PAUSE 25

// variabelen voor grid en blokken
int gridx,gridy;
int hoknr= START_HOKNUMMER,nieuwblok,kant= BLOK_KANT,vblok;
int grid[191];
int tmplaag[4]=	{0,0,0,0};
int blokhokaan[201];
int blokkleuren[191];
// var voor tijd
volatile int snelheid= 1;
volatile int menutimer=	1;
int valsnelheid= SNELHEID;
// var voor kleur
int clearblok= FALSE,kleur= 108;
// var voor de scores
double score,highscore=	0;
int lines,level;
// var voor geluid
int geluid= SOUND_DEF;

struct high
{  char naam[14];
   double tscore;
}highdat[11];


// functies init
//////////////////////////////////////////////////////////////////////////////
void maak_grid();
int zoekgridy(int);
void printblok(int, int, int);
void kiesblok(int, int,	int);
void hoofdmenu();
void highscore_menu();
void options_menu();
void control_menu();
void print_info();
void kies_nieuwblok();
void blokstoppen();
void inc_snelheid();
void inc_menutimer();
void lijn_weghalen(int);
void lijn_controle();
void nieuw_spel();
void spel_stoppen();
void init_spel();
void stoppen();
void tekenvb(int);
void allehokjes(int);
void init_info();
void highscore_sorteren();

// begin hoofdfunctie
/////////////////////////////////////////////////////////////////////////////
int main()
{ int loop,loop2= TRUE,i,randen= 0,extra;

  init_spel();
  do
  {loop= TRUE;
   hoofdmenu();
   nieuw_spel();
   do
   {  //blokstoppen();

      // Kijkt of je al bovenaan zit
      for(i= 1;i<= 10;i++)
       if(blokhokaan[i]== TRUE)
	{  loop= FALSE;
	   spel_stoppen();
           break;
	}

      // Als de snelheids teller 4 is geworden wordt het blokje naar beneden
      // geschoven
      if(snelheid>= 2)
       {  blokstoppen();
          snelheid= 1;
	  clearblok= TRUE;
	  kiesblok(hoknr,nieuwblok,kant);
	  hoknr+= 10;
	  clearblok= FALSE;
	  lijn_controle();
	  kiesblok(hoknr,nieuwblok,kant);
       }

      // Als er op een toets wordt gedrukt */
      // Leest de keyboard buffer uit en gebruikt de scan codes()
      if(key[DRAAIEN])	//"Space" wordt ingedrukt
       {  /* kijkt of je mag draaien aan linkerkant */
	  randen= 0;
	  for(i=1;i<=181;i+= 10)
	   {  switch(nieuwblok)
	      {	 case 2: if(kant== 2 &&	hoknr==	i)
			  randen= -1;
			  else if(kant== 2 && blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]== TRUE	||
				  blokhokaan[hoknr+9]==	TRUE)
				randen=	-1;
			       break;
		 case 3: if(kant== 2 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	2 && blokhokaan[hoknr-1]== TRUE	|| blokhokaan[hoknr-11]== TRUE ||
				 blokhokaan[hoknr+9]== TRUE)
			       randen= -1;
			 break;
		 case 4: if(kant== 2 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	2 && blokhokaan[hoknr-1]== TRUE	|| blokhokaan[hoknr-11]== TRUE ||
				 blokhokaan[hoknr+9]== TRUE)
			       randen= -1;
			 break;
		 case 5: if(kant== 2 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	2 && blokhokaan[hoknr-1]== TRUE	|| blokhokaan[hoknr-10]== TRUE ||
				 blokhokaan[hoknr+9]== TRUE)
			       randen= -1;
			 break;
		 case 6: if(kant== 2 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	2 && blokhokaan[hoknr-1]== TRUE	|| blokhokaan[hoknr-11]== TRUE ||
				 blokhokaan[hoknr+10]==	TRUE)
			       randen= -1;
			 break;
	      }
	   }
	  /* kijkt of je mag draaien aan rechterkant */
	  for(i=10;i<=190;i+= 10)
	   {  switch(nieuwblok)
	      {	 case 0: if((kant== 2) && (hoknr+2== i || hoknr+1== i || hoknr== i))
			  randen= -1;
			 else if((kant== 2) && (blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr+1]== TRUE ||
				  blokhokaan[hoknr]== TRUE))
			       randen= -1;
			       break;
		 case 2: if(kant== 4 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	4 && blokhokaan[hoknr+1]== TRUE	|| blokhokaan[hoknr-9]== TRUE ||
				 blokhokaan[hoknr+11]==	TRUE)
			       randen= -1;
			 break;
		 case 3: if(kant== 4 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	4 && blokhokaan[hoknr+1]== TRUE	|| blokhokaan[hoknr-9]== TRUE ||
				 blokhokaan[hoknr+11]==	TRUE)
				randen=	-1;
			 break;
		 case 4: if(kant== 4 &&	hoknr==	i)
			  randen= -1;
			 else if(kant==	4 && blokhokaan[hoknr+1]== TRUE	|| blokhokaan[hoknr-9]== TRUE ||
				 blokhokaan[hoknr+11]==	TRUE)
			       randen= -1;
			 break;
	      }
	   }
	  /* Als het geen problemen geeft met draaien heeft randen niet
             de waarde -1 gekregen het blok draait dan */
	  if(randen!= -1)
	   {  if(nieuwblok!= 1)
	       play_sample(datafile[draaien].dat,geluid,128,1000,FALSE);

	      clearblok= TRUE;
	      if(nieuwblok== 0 || nieuwblok== 5	|| nieuwblok== 6)
	       {  kiesblok(hoknr,nieuwblok,kant);
		  kant++;
		  if(kant== 3)
		   kant=1;
	       }
	      else
	       {  kiesblok(hoknr,nieuwblok,kant);
		  kant++;
		  if(kant== 5)
		   kant= 1;
	       }
	      clearblok= FALSE;
	      kiesblok(hoknr,nieuwblok,kant);
	   }

       }
      if(key[LINKS]) //"Left_Arrow" wordt ingedrukt
       {  /* check op uitsteeksels linkerkant */
	  randen= 0;
	  extra= 0;
	  switch(nieuwblok)
	  {  case 2: switch(kant)
		     { case 1: extra = 1; break;
		       case 3: extra = 1; break;
		       case 4: extra = 1; break;
		     }
		     break;
	     case 3: switch(kant)
		     { case 1: extra = 1; break;
		       case 3: extra = 1; break;
		       case 4: extra = 1; break;
		     }
		     break;
	     case 4: switch(kant)
		     { case 1: extra = 1; break;
		       case 3: extra = 1; break;
		       case 4: extra = 1; break;
		     }
		     break;
	     case 5: if(kant== 1)
		      extra= 1;
		     break;
	     case 6: if(kant== 1)
		      extra= 1;
		     break;
	  }
	  for(i=1;i<=181;i+= 10)
	  if(i== hoknr-extra )
	   randen= -1;

	  /* kijkt of je niet tegen een ander blok aanzit */
	  switch(nieuwblok)
	  {  case 0: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-1]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr-21]== TRUE || blokhokaan[hoknr-31]== TRUE)
				 randen= -1;
				break;
		      }
		      break;
	     case 1: if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]== TRUE)
		      randen= -1;
		     break;
	     case 2: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr-11]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr+9]== TRUE)
				 randen= -1;
				break;
			case 3:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr+9]== TRUE)
				 randen= -1;
				break;
			case 4:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+9]== TRUE)
				 randen= -1;
				break;
		     }
		     break;

	     case 3: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr-10]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr+9]== TRUE)
				 randen= -1;
				break;
			case 3:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr+8]== TRUE)
				 randen= -1;
				break;
			case 4:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr+9]== TRUE ||
				   blokhokaan[hoknr-12]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	     case 4: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr-12]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr+9]== TRUE)
				 randen= -1;
				break;
			case 3:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr+10]==	TRUE)
				 randen= -1;
				break;
			case 4:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr+8]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	     case 5: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-12]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr+9]== TRUE ||
				   blokhokaan[hoknr-10]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	     case 6: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr-2]== TRUE || blokhokaan[hoknr-11]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr-1]== TRUE || blokhokaan[hoknr-11]==	TRUE ||
				   blokhokaan[hoknr+10]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	  }
	  /* als randen niet -1 is gaat het blok naar links */
	  if(randen!= -1)
	   {  play_sample(datafile[opzij].dat,geluid,128,1000,FALSE);
	      clearblok= TRUE;
	      kiesblok(hoknr,nieuwblok,kant);
	      hoknr--;
	      clearblok= FALSE;
	      kiesblok(hoknr,nieuwblok,kant);
	   }

       }
      if(key[RECHTS]) //"Right_Arrow" wordt ingedrukt
       {  /* check op uitsteeksels rechterkant */
	  randen= 0;
	  extra= 0;
	  switch(nieuwblok)
	  {  case 0: if(kant== 1)
		      extra= 3;
		     break;
	     case 1: extra= 1; break;
	     case 2: switch(kant)
		     { case 1: extra = 1; break;
		       case 2: extra = 1; break;
		       case 3: extra = 1; break;
		     }
		     break;
	     case 3: switch(kant)
		     { case 1: extra = 1; break;
		       case 2: extra = 1; break;
		       case 3: extra = 1; break;
		      }
		      break;
	     case 4: switch(kant)
		     { case 1: extra = 1; break;
		       case 2: extra = 1; break;
		       case 3: extra = 1; break;
		      }
		      break;
	     case 5: extra= 1; break;
		     case 6: extra= 1; break;
	  }
	  for(i=10;i<=190;i+= 10)
	   if(i== hoknr+extra)
	    randen= -1;

	  /* kijkt of je niet tegen een ander blok aanzit */
	  switch(nieuwblok)
	  {  case 0: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+4]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr-19]== TRUE || blokhokaan[hoknr-29]== TRUE)
				 randen= -1;
				break;

		     }
		     break;

	     case 1: if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-8]== TRUE)
		      randen= -1;
		     break;

	     case 2: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-9]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				break;
			case 3:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr+11]==	TRUE)
				 randen= -1;
				break;
			case 4:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	     case 3: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-8]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+12]== TRUE)
				 randen= -1;
				break;
			case 3:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr+10]==	TRUE)
				 randen= -1;
				break;
			case 4:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	     case 4: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-10]==	TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-8]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				 break;
			case 3:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr+12]==	TRUE)
				 randen= -1;
				break;

			case 4:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				break;
		     }
		     break;

	     case 5: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-9]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-8]== TRUE ||
				   blokhokaan[hoknr+11]== TRUE)
				 randen= -1;
				break;
		     }
		     break;

	     case 6: switch(kant)
		     {	case 1:	if(blokhokaan[hoknr+1]== TRUE || blokhokaan[hoknr-8]== TRUE)
				 randen= -1;
				break;
			case 2:	if(blokhokaan[hoknr+2]== TRUE || blokhokaan[hoknr-9]== TRUE ||
				   blokhokaan[hoknr+12]== TRUE)
				 randen= -1;
				break;
		     }
		     break;
	  }
	  /* als randen niet -1 is gaat het blok naar rechts */
	  if(randen!= -1)
	   {  play_sample(datafile[opzij].dat,geluid,128,1000,FALSE);
	      clearblok= TRUE;
	      kiesblok(hoknr,nieuwblok,kant);
	      hoknr++;
	      clearblok= FALSE;
	      kiesblok(hoknr,nieuwblok,kant);
	   }

       }
      //"Down_Arrow" wordt ingedrukt
      if(key[BENEDEN])
       snelheid= 2;

      //"P" wordt ingedrukt
      if(key[25])
       {  clear_keybuf();
	  remove_int(inc_snelheid);
	  textout(screen,datafile[score_font].dat,"PAUSE",365,180,255);

	  while(!keypressed());

	  clear(speelveld);
	  for(i= 190;i>= 1; i--)
	   if(blokhokaan[i]== TRUE)
	    printblok(grid[i],zoekgridy(i),blokkleuren[i]);

	  install_int(inc_snelheid,valsnelheid);
	  snelheid= 2;
       }

       //"F1" wordt ingedrukt
       if(key[59])
        {  clear_keybuf();
 	   remove_int(inc_snelheid);

           textout(screen,datafile[score_font].dat,"HELP",370,30,255);
  	   textout(screen,font,"MOVE LEFT      : LEFT ARROW",310,70,255);
           textout(screen,font,"MOVE RIGHT     : RIGHT ARROW",310,85,255);
           textout(screen,font,"GO FASTER DOWN : DOWN ARROW",310,100,255);
           textout(screen,font,"ROTATE         : SPACE",310,115,255);
           textout(screen,font,"PAUSE GAME     : P",310,130,255);
           textout(screen,font,"HELP           : F1",310,145,255);
           textout(screen,font,"QUIT TO MENU   : ESC",310,160,255);
 	   while(!keypressed());
 
 	   clear(speelveld);
 	   for(i= 190;i>= 1; i--)
 	    if(blokhokaan[i]== TRUE)
 	     printblok(grid[i],zoekgridy(i),blokkleuren[i]);
 
 	   install_int(inc_snelheid,valsnelheid);
	   snelheid= 2;
        }

      //"Esc" wordt ingedrukt
      if(key[1])
       {  loop=	FALSE;
	  spel_stoppen();
       }

      clear_keybuf();
   }while(loop!= FALSE);
  }while(loop2!= FALSE);
   //einde
   stoppen();
   return 0;
}

// begin van de functies
/////////////////////////////////////////////////////////////////////////////
void inc_snelheid()
{
   snelheid++;
}
END_OF_FUNCTION(inc_snelheid);

/////////////////////////////////////////////////////////////////////////////
void inc_menutimer()
{
   menutimer++;
}
END_OF_FUNCTION(inc_menutimer);

//////////////////////////////////////////////////////////////////////////////
void init_info()
{
   clrscr();
   printf(COPYRIGHT"\n");
   printf("\nVersion              : "VERSION"\n");
   printf("Compiled under       : DJGPP/ALLEGRO.\n");
   printf("Programmer           : Richard Regeer\n");
   printf("E-mail               : r.regeer@hccnet.nl\n");
   check_cpu();
   printf("\nSystem processor     : 80%d86\n",cpu_family);
}

//////////////////////////////////////////////////////////////////////////////
void init_spel()
   // zet alles aan wat nodig is
{  int i,teller= 1000;
   struct high leeg[11]={"R.Regeer",15000,"R.Regeer",12500,"R.Regeer",10000,
                         "R.Regeer",8000,"R.Regeer",5000,"R.Regeer",2500,
                         "R.Regeer",1000,"R.Regeer",750,"R.Regeer",500,
                         "R.Regeer",250};

   //allegro init
   allegro_init();
   install_keyboard();
   install_timer();

   // Kijkt of alles er is en print wat informatie
   init_info();
   if(install_sound(DIGI_AUTODETECT,MIDI_NONE,0)!= 0){
    printf("Error initialising sound system\n%s\n", allegro_error);
    install_sound(DIGI_NONE,MIDI_NONE,0);
    printf("Sound driver         : %s\n",digi_driver->name);
   }
   else	printf("Sound driver         : %s\n",digi_driver->name);

   datafile= load_datafile("rdat.dat");
   if(!datafile)
    {  printf("Error reading datafile \"rdat.dat\"\n");
       stoppen();
       readkey();
       exit(1);
    }
   else	printf("Datafile             : \"rdat.dat\" loaded\n");

   if((high_file= fopen("highs.dat","r"))== NULL)
    {  printf("Error reading datafile \"highs.dat\"\n");
       printf("Greating a new default \"highs.dat\"\n");
       high_file= fopen("highs.dat","w");
       fwrite(&leeg,sizeof(struct high),10,high_file);

    }
   else printf("Datafile             : \"highs.dat\" loaded\n");
   fclose(high_file);

   printf("\nPress any key to continue");
   readkey();

   if(set_gfx_mode(GFX_VESA1,B,H,0,0)!=0)
    {  printf("Error setting VESA1 640x480 video mode\n%s\n\n",allegro_error);
       stoppen();
       readkey();
       exit(1);
    }
   set_color_depth(DEPTH);
   text_mode(-1);
   maak_grid();

   // lock de snelheid var en functies
   LOCK_VARIABLE(snelheid);
   LOCK_VARIABLE(menutimer);
   LOCK_FUNCTION(inc_menutimer);
   LOCK_FUNCTION(inc_snelheid);

   // maak bitmaps aan
   speelveld= create_sub_bitmap(screen,301,1,253,480);
   clear(speelveld);
   clear_keybuf();
}

/////////////////////////////////////////////////////////////////////////////
void stoppen()
{
   unload_datafile(datafile);
   destroy_bitmap(speelveld);
   allegro_exit();
}

/////////////////////////////////////////////////////////////////////////////
void hoofdmenu()
{  int loop= TRUE,yt;

   install_int(inc_menutimer,MENU_SNELHEID);

   do
   { set_palette(black_pallete);
     blit(datafile[menu_kaal].dat,screen,0,0,0,0,640,480);
     masked_blit(datafile[main_text].dat,screen,0,0,213,110,230,200);
     fade_in(datafile[menu_pallete].dat,FADE);

     yt= 138;
     do
     {	// Als de snelheid 2 is verandert het aanwijs blokje van kleur
	if(menutimer>= 2)
	 {  if(kleur < 255)
	     kleur++;
	    else kleur=	108;
	    printblok(170,yt,kleur);
	    menutimer= 1;
	  }

	// Als er op de "Down arrow" gedrukt wordt
	if(key[80])
	 {  play_sample(datafile[menu_kiezen].dat,geluid,128,1000,FALSE);
	    if(yt < 214)
	     yt+= 38;
	    else yt= 138;
	    blit(datafile[achtergrond].dat,screen,0,0,0,100,213,276);
	    printblok(170,yt,kleur);
	 }

	// Als er op de "Up arrow" gedrukt wordt
	if(key[72])
	 {  play_sample(datafile[menu_kiezen].dat,geluid,128,1000,FALSE);
	    if(yt > 138)
	     yt-= 38;
	    else yt= 214;
	    blit(datafile[achtergrond].dat,screen,0,0,0,100,213,276);
	    printblok(170,yt,kleur);
       }

	// Als er op "Enter" wordt gedrukt
	if(key[28])
	 {  play_sample(datafile[menu_klik].dat,geluid,128,1000,FALSE);
	    loop= FALSE;
	 }
	// Als er op "Esc" wordt gedrukt
	if(key[1])
	 {  play_sample(datafile[menu_klik].dat,geluid,128,1000,FALSE);
	    loop= FALSE;
	    fade_out(FADE);
	    stoppen();
	    exit(0);
	 }
	clear_keybuf();
     }while(loop!= FALSE);

     // Kijkt waar het aanwijs blokje gestopt en voert dan die opdracht uit
     loop= TRUE;
     switch(yt)
     {	case 138:  loop= FALSE;	break;
	case 176:  fade_out(FADE);
		   highscore_menu();
		   break;
	case 214:  fade_out(FADE);
		   stoppen();
		   exit(0);
		   break;
     }

    clear_keybuf();
  }while(loop!=	FALSE);
  remove_int(inc_menutimer);
  fade_out(FADE);
}

/////////////////////////////////////////////////////////////////////////////
void highscore_menu()
{  int loop= TRUE,y= 170,i;

   set_pallete(black_pallete);
   blit(datafile[menu_kaal].dat,screen,0,0,0,0,640,480);
   masked_blit(datafile[highscore_text].dat,screen,0,0,145,105,400,350);
   fade_in(datafile[menu_pallete].dat,FADE);

   highscore_sorteren();
   for(i= 0;i<= 9;i++,y+= 29)
   {  textprintf(screen,datafile[highscore_font].dat,189,y,197,"%s",highdat[i].naam,highdat[i].naam);
      textprintf(screen,datafile[highscore_font].dat,405,y,197,"%.0f",highdat[i].tscore,highdat[i].tscore);
   }

   while(!keypressed());

   play_sample(datafile[menu_klik].dat,geluid,128,1000,FALSE);
   fade_out(FADE);
}

/////////////////////////////////////////////////////////////////////////////
void maak_grid()
   /* grid bestaat uit 190 hokjes, deze worden gevuld met alle
      x waarden van de hokjes */
{  int i;

   gridx= 302;
   for(i= 1;i<=	190;i++)
    {  grid[i]=	gridx;
       gridx+= 25;
       if(gridx== 552)
	gridx= 302;
    }
}

/////////////////////////////////////////////////////////////////////////////
int zoekgridy(int nummer)
   /* zoekt de y waarde op van het opgegeven hoknummer */
{  int min= 0,max= 10,starty= 4,i;

   for(i= 1;i<=	19; i++)
   {  if(nummer> min &&	nummer<= max)
       {  gridy= starty;
	  break;
       }
      min+= 10;	max+= 10;
      starty+= 25;
    }
   return gridy;
}

/////////////////////////////////////////////////////////////////////////////
void printblok(int x, int y, int kleur)
   /* print 1 blokje in het grid in de kleur naar je keuze */
   // BLACK heeft nu de pallete index waarde 0;
{
   if(clearblok== TRUE)
     rectfill(screen,x+1,y+1,x+24,y+24,0);
   else
   {  rectfill(screen,x+1,y+1,x+24,y+24,kleur);
      rect(screen,x+1,y+1,x+24,y+24,RANDEN);
   }
}

//////////////////////////////////////////////////////////////////////////////
void kiesblok(int hoknmr, int type, int	k)
   /* kies het blok dat in het grid moet komen, alleen het hoknummer is nodig
   van waar je het blok wilt hebben. Welk type blok je wilt hebben en welke
   kant het blok op staat en kleur */
{  int i;

   /* type0 langblok */
   /* type1 vierkant */
   /* type2 tblok */
   /* type3 lblokrechts */
   /* type4 lbloklinks */
   /* type5 zbloklinks */
   /* type6 zblokrechts */
   switch(type)
    {  case 0:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),LANGBLOK);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr),LANGBLOK);
		    printblok(grid[hoknmr+2],zoekgridy(hoknmr),LANGBLOK);
		    printblok(grid[hoknmr+3],zoekgridy(hoknmr),LANGBLOK);
		 }
		 else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LANGBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),LANGBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-20),LANGBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-30),LANGBLOK);
		      }
		break;

       case 1:	printblok(grid[hoknmr],zoekgridy(hoknmr),VIERKANT);
		printblok(grid[hoknmr+1],zoekgridy(hoknmr),VIERKANT);
		printblok(grid[hoknmr+1],zoekgridy(hoknmr-10),VIERKANT);
		printblok(grid[hoknmr],zoekgridy(hoknmr-10),VIERKANT);
		break;

       case 2:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),TBLOK);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr),TBLOK);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr),TBLOK);
		    printblok(grid[hoknmr],zoekgridy(hoknmr-10),TBLOK);
		 }
		else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),TBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),TBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),TBLOK);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),TBLOK);
		      }
		else if(k== 3)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),TBLOK);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr),TBLOK);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),TBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),TBLOK);
		      }
		else if(k== 4)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),TBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),TBLOK);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),TBLOK);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr),TBLOK);
		      }
		break;

       case 3:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_RECHTS);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr),LBLOK_RECHTS);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr),LBLOK_RECHTS);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr-10),LBLOK_RECHTS);
		 }
		else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_RECHTS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),LBLOK_RECHTS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),LBLOK_RECHTS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr+10),LBLOK_RECHTS);
		      }
		else if(k== 3)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_RECHTS);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr),LBLOK_RECHTS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),LBLOK_RECHTS);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr+10),LBLOK_RECHTS);
		      }
		else if(k== 4)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_RECHTS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),LBLOK_RECHTS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),LBLOK_RECHTS);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr-10),LBLOK_RECHTS);
		      }
		break;

       case 4:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_LINKS);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr),LBLOK_LINKS);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr),LBLOK_LINKS);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr-10),LBLOK_LINKS);
		 }
		else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_LINKS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),LBLOK_LINKS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),LBLOK_LINKS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr-10),LBLOK_LINKS);
		      }
		else if(k== 3)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_LINKS);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr),LBLOK_LINKS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),LBLOK_LINKS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr+10),LBLOK_LINKS);
		      }
		else if(k== 4)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),LBLOK_LINKS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),LBLOK_LINKS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),LBLOK_LINKS);
			 printblok(grid[hoknmr-1],zoekgridy(hoknmr+10),LBLOK_LINKS);
		      }
		break;

       case 5:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),ZBLOK_LINKS);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr-10),ZBLOK_LINKS);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr),ZBLOK_LINKS);
		    printblok(grid[hoknmr],zoekgridy(hoknmr-10),ZBLOK_LINKS);
		 }
		else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),ZBLOK_LINKS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),ZBLOK_LINKS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr+10),ZBLOK_LINKS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr-10),ZBLOK_LINKS);
		      }
		break;

       case 6:	if(k== 1)
		 {  printblok(grid[hoknmr],zoekgridy(hoknmr),ZBLOK_RECHTS);
		    printblok(grid[hoknmr],zoekgridy(hoknmr-10),ZBLOK_RECHTS);
		    printblok(grid[hoknmr-1],zoekgridy(hoknmr),ZBLOK_RECHTS);
		    printblok(grid[hoknmr+1],zoekgridy(hoknmr-10),ZBLOK_RECHTS);
		 }
		else if(k== 2)
		      {	 printblok(grid[hoknmr],zoekgridy(hoknmr),ZBLOK_RECHTS);
			 printblok(grid[hoknmr],zoekgridy(hoknmr-10),ZBLOK_RECHTS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr),ZBLOK_RECHTS);
			 printblok(grid[hoknmr+1],zoekgridy(hoknmr+10),ZBLOK_RECHTS);
		      }
		break;
    }
}

/////////////////////////////////////////////////////////////////////////////
void print_info()
{  blit(datafile[paneel].dat,screen,0,0,0,0,287,479);
   textprintf(screen,datafile[score_font].dat,60,80,SCORE_KLEUREN,"%.0f",highscore);
   textprintf(screen,datafile[score_font].dat,125,155,SCORE_KLEUREN,"%.0f",score);
   textprintf(screen,datafile[score_font].dat,125,207,SCORE_KLEUREN,"%d",lines);
   textprintf(screen,datafile[score_font].dat,125,259,SCORE_KLEUREN,"%d",level);
   tekenvb(vblok);
}

/////////////////////////////////////////////////////////////////////////////
void kies_nieuwblok()
{  // Zorgt dat er een nieuw blok gekozen wordt en dat het blok weer bovenaan
   // komt te staan

   kiesblok(hoknr,nieuwblok,kant);
   play_sample(datafile[blok_valt].dat,geluid,128,1000,FALSE);
   snelheid= 2;
   hoknr= START_HOKNUMMER;
   kant= BLOK_KANT;
   nieuwblok= vblok;
   vblok= random()%7;
   score+= 1*level;
   print_info();
}

/////////////////////////////////////////////////////////////////////////////
void blokstoppen()
{  /* kijkt of het blok al tegen een ander blok aanzit of dat het blok al
      aan de onderkant zit */
   int i;

   /* nieuwblok0 langblok */
   /* nieuwblok1 vierkant */
   /* nieuwblok2 tblok */
   /* nieuwblok3 lblokrechts */
   /* nieuwblok4 lbloklinks */
   /* nieuwblok5 zbloklinks */
   /* nieuwblok6 zblokrechts */

    if(nieuwblok== 0 &&	kant== 1 && (blokhokaan[hoknr+10]== TRUE || blokhokaan[hoknr+11]== TRUE
	     ||	blokhokaan[hoknr+12]== TRUE || blokhokaan[hoknr+13]== TRUE))
     {	blokhokaan[hoknr]= TRUE;
	blokhokaan[hoknr+1]= TRUE;
	blokhokaan[hoknr+2]= TRUE;
	blokhokaan[hoknr+3]= TRUE;

	blokkleuren[hoknr]= LANGBLOK;
	blokkleuren[hoknr+1]= LANGBLOK;
	blokkleuren[hoknr+2]= LANGBLOK;
	blokkleuren[hoknr+3]= LANGBLOK;
	kies_nieuwblok();
     }
    else if(nieuwblok==	0 && kant== 2 && blokhokaan[hoknr+10]==	TRUE)
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr-20]= TRUE;
	     blokhokaan[hoknr-30]= TRUE;

	     blokkleuren[hoknr]= LANGBLOK;
	     blokkleuren[hoknr-10]= LANGBLOK;
	     blokkleuren[hoknr-20]= LANGBLOK;
	     blokkleuren[hoknr-30]= LANGBLOK;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	1 && (blokhokaan[hoknr+10]== TRUE || blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr-9]= TRUE;

	     blokkleuren[hoknr]= VIERKANT;
	     blokkleuren[hoknr+1]= VIERKANT;
	     blokkleuren[hoknr-10]= VIERKANT;
	     blokkleuren[hoknr-9]= VIERKANT;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	2 && kant== 1 && (blokhokaan[hoknr+9]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	     ||	blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;

	     blokkleuren[hoknr]= TBLOK;
	     blokkleuren[hoknr+1]= TBLOK;
	     blokkleuren[hoknr-1]= TBLOK;
	     blokkleuren[hoknr-10]= TBLOK;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	2 && kant== 2 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+11]==	TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+1]= TRUE;

	     blokkleuren[hoknr]= TBLOK;
	     blokkleuren[hoknr+10]= TBLOK;
	     blokkleuren[hoknr-10]= TBLOK;
	     blokkleuren[hoknr+1]= TBLOK;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	2 && kant== 3 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+9]== TRUE
	    || blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr+1]= TRUE;

	     blokkleuren[hoknr]= TBLOK;
	     blokkleuren[hoknr+10]= TBLOK;
	     blokkleuren[hoknr-1]= TBLOK;
	     blokkleuren[hoknr+1]= TBLOK;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	2 && kant== 4 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+9]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+10]= TRUE;

	     blokkleuren[hoknr]= TBLOK;
	     blokkleuren[hoknr-1]= TBLOK;
	     blokkleuren[hoknr-10]= TBLOK;
	     blokkleuren[hoknr+10]= TBLOK;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	3 && kant== 1 && (blokhokaan[hoknr+9]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	     ||	blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr-9]= TRUE;

	     blokkleuren[hoknr]= LBLOK_RECHTS;
	     blokkleuren[hoknr+1]= LBLOK_RECHTS;
	     blokkleuren[hoknr-1]= LBLOK_RECHTS;
	     blokkleuren[hoknr-9]= LBLOK_RECHTS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	3 && kant== 2 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+21]==	TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+11]= TRUE;

	     blokkleuren[hoknr]= LBLOK_RECHTS;
	     blokkleuren[hoknr+10]= LBLOK_RECHTS;
	     blokkleuren[hoknr-10]= LBLOK_RECHTS;
	     blokkleuren[hoknr+11]= LBLOK_RECHTS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	3 && kant== 3 && (blokhokaan[hoknr+19]== TRUE || blokhokaan[hoknr+10]==	TRUE
	     ||	blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+9]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr+1]= TRUE;

	     blokkleuren[hoknr]= LBLOK_RECHTS;
	     blokkleuren[hoknr+9]= LBLOK_RECHTS;
	     blokkleuren[hoknr-1]= LBLOK_RECHTS;
	     blokkleuren[hoknr+1]= LBLOK_RECHTS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	3 && kant== 4 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr-1]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr-11]= TRUE;

	     blokkleuren[hoknr]= LBLOK_RECHTS;
	     blokkleuren[hoknr-10]= LBLOK_RECHTS;
	     blokkleuren[hoknr+10]= LBLOK_RECHTS;
	     blokkleuren[hoknr-11]= LBLOK_RECHTS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	4 && kant== 1 && (blokhokaan[hoknr+9]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	    || blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr-11]= TRUE;

	     blokkleuren[hoknr]= LBLOK_LINKS;
	     blokkleuren[hoknr+1]= LBLOK_LINKS;
	     blokkleuren[hoknr-1]= LBLOK_LINKS;
	     blokkleuren[hoknr-11]= LBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	4 && kant== 2 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+1]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr-9]= TRUE;

	     blokkleuren[hoknr]= LBLOK_LINKS;
	     blokkleuren[hoknr-10]= LBLOK_LINKS;
	     blokkleuren[hoknr+10]= LBLOK_LINKS;
	     blokkleuren[hoknr-9]= LBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	4 && kant== 3 && (blokhokaan[hoknr+9]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	    || blokhokaan[hoknr+21]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr+11]= TRUE;

	     blokkleuren[hoknr]= LBLOK_LINKS;
	     blokkleuren[hoknr+1]= LBLOK_LINKS;
	     blokkleuren[hoknr-1]= LBLOK_LINKS;
	     blokkleuren[hoknr+11]= LBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	4 && kant== 4 && (blokhokaan[hoknr+19]== TRUE || blokhokaan[hoknr+20]==	TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr+9]= TRUE;

	     blokkleuren[hoknr]= LBLOK_LINKS;
	     blokkleuren[hoknr-10]= LBLOK_LINKS;
	     blokkleuren[hoknr+10]= LBLOK_LINKS;
	     blokkleuren[hoknr+9]= LBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	5 && kant== 1 && (blokhokaan[hoknr-1]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	     ||	blokhokaan[hoknr+11]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr-11]= TRUE;

	     blokkleuren[hoknr]= ZBLOK_LINKS;
	     blokkleuren[hoknr+1]= ZBLOK_LINKS;
	     blokkleuren[hoknr-10]= ZBLOK_LINKS;
	     blokkleuren[hoknr-11]= ZBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	5 && kant== 2 && (blokhokaan[hoknr+20]== TRUE || blokhokaan[hoknr+11]==	TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr+10]= TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr-9]= TRUE;

	     blokkleuren[hoknr]= ZBLOK_LINKS;
	     blokkleuren[hoknr+10]= ZBLOK_LINKS;
	     blokkleuren[hoknr+1]= ZBLOK_LINKS;
	     blokkleuren[hoknr-9]= ZBLOK_LINKS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	6 && kant== 1 && (blokhokaan[hoknr+9]==	TRUE ||	blokhokaan[hoknr+10]== TRUE
	     ||	blokhokaan[hoknr+1]== TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-1]= TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr-9]= TRUE;

	     blokkleuren[hoknr]= ZBLOK_RECHTS;
	     blokkleuren[hoknr-1]= ZBLOK_RECHTS;
	     blokkleuren[hoknr-10]= ZBLOK_RECHTS;
	     blokkleuren[hoknr-9]= ZBLOK_RECHTS;
	     kies_nieuwblok();
	  }
    else if(nieuwblok==	6 && kant== 2 && (blokhokaan[hoknr+10]== TRUE || blokhokaan[hoknr+21]==	TRUE))
	  {  blokhokaan[hoknr]=	TRUE;
	     blokhokaan[hoknr-10]= TRUE;
	     blokhokaan[hoknr+1]= TRUE;
	     blokhokaan[hoknr+11]= TRUE;

	     blokkleuren[hoknr]= ZBLOK_RECHTS;
	     blokkleuren[hoknr-10]= ZBLOK_RECHTS;
	     blokkleuren[hoknr+1]= ZBLOK_RECHTS;
	     blokkleuren[hoknr+11]= ZBLOK_RECHTS;
	     kies_nieuwblok();
	  }
}

/////////////////////////////////////////////////////////////////////////////
void lijn_weghalen(int aantal)
{  int rijen,blok,laag,i,plaats;

   for(laag= 0;laag <= aantal-1; laag++)
    {  for(rijen= 0;rijen<= 9;rijen++)
	{  blokhokaan[tmplaag[laag]+rijen]= FALSE;
           blokkleuren[tmplaag[laag]+rijen]= 0;
	   printblok(grid[tmplaag[laag]+rijen],zoekgridy(tmplaag[laag]+rijen),255);
	}
    }
   play_sample(datafile[lijnweg].dat,geluid,128,1000,FALSE);
   rest(500);
   clear(speelveld);

   switch(aantal)
       {  case 1: blok=	10; break;
	  case 2: blok=	20; break;
	  case 3: blok=	30; break;
	  case 4: blok=	40; break;
       }

   play_sample(datafile[blok_valt].dat,geluid,128,1000,FALSE);
   for(i= tmplaag[0];i<= tmplaag[0]+9;i++)
    for(laag= i;laag>= 11; laag-= 10)
     for(plaats= 10;plaats<= blok; plaats+= 10)
      if(blokhokaan[laag-plaats]== TRUE)
       {  blokhokaan[laag]= blokhokaan[laag-plaats];
          blokkleuren[laag]= blokkleuren[laag-plaats];
          blokhokaan[laag-plaats]= FALSE;
          blokkleuren[laag-plaats]= 0;
          break;
       }

   /* tekent alle blokken weer opnieuw in goede kleur */
   for(i= 190;i>= 1; i--)
    if(blokhokaan[i]== TRUE)
     printblok(grid[i],zoekgridy(i),blokkleuren[i]);

   // rekent de score uit van het aantal lijnen dat je hebt gehaald
   switch(aantal)
   {  case 1: score+= 50; break;
      case 2: score+= 250; break;
      case 3: score+= 500; break;
      case 4: score+= 1000; break;
   }

   // telt er een line bij  kijkt of je al naar het volgende level gaat
   // print de scores op het linker paneel
   lines+= aantal;
   if(lines/10== level)
    {  level++;
       if(valsnelheid >	200)
	{  valsnelheid-= 100;
	   install_int(inc_snelheid,valsnelheid);
	}
    }

   for(i= 0;i<=	3;i++)
    tmplaag[i]=	0;
   print_info();
}

/////////////////////////////////////////////////////////////////////////////
void lijn_controle()
{  int niveau,rij,hokteller= 0,lijnteller= 0;
   int lijnb= 181,lijne= 190;

   for(niveau= 1;niveau<= 18;niveau++) /* loopt alle lagen af (19) */
    {  for(rij=	lijnb;rij<= lijne;rij++) /* loopt 1 hele rij af */
	{  if(blokhokaan[rij]==	TRUE)
	    hokteller++;
	   if(hokteller== 10)  /* als er 10 blokjes TRUE zijn naast elkaar */
	    {  tmplaag[lijnteller]= rij-9;
	       lijnteller++;
	    }
	}
       hokteller= 0;
       lijnb-= 10;
       lijne-= 10;
    }

   if(tmplaag[0] > 0)
    lijn_weghalen(lijnteller);
   lijnteller= 0;
}

//////////////////////////////////////////////////////////////////////////////
void nieuw_spel()
{  int i;

   for(i= 1;i<=	190;i++)      /* zet alle waarde in de array's op kleur zwart*/
    blokkleuren[i]= 0;
   for(i= 1;i<=	190;i++)      /* zet alle waarde in de array's op FALSE */
    blokhokaan[i]= FALSE;
   for(i= 191;i<= 200;i++)    /* zet 191 to 200 op TRUE in blokaan zodat het blok */
    blokhokaan[i]= TRUE;      /* aan het begin zal stopen op de onderste lijn   */

   level= 1; lines= 0; score= 0; // zet je scores weer op 0
   valsnelheid= SNELHEID;
   highscore_sorteren();

   // laad de bitmap in
   set_palette(black_pallete);
   blit(datafile[speelscherm].dat,screen,0,0,0,0,640,480);
   fade_in(datafile[spel_pallete].dat,FADE);

   // zet alle begin waarde weer goed
   hoknr= START_HOKNUMMER;
   srandom(time(NULL));
   nieuwblok= random()%7;
   vblok= random()%7;
   kiesblok(START_HOKNUMMER,nieuwblok,BLOK_KANT);
   install_int(inc_snelheid,valsnelheid);
   print_info();

}

//////////////////////////////////////////////////////////////////////////////
void allehokjes(int kleurtje) /* tekent allehokjes langzaam van beneden naar boven */
{  int x,y,i;

   x= 527; y= 454;
   for(i= 1;i<=190;i++)
    {  printblok(x,y,kleurtje);
       x-= 25;
       if(x< 298)
	{  x= 527;
	   y-= 25;
	   rest(40);
	}
    }
}

//////////////////////////////////////////////////////////////////////////////
void highscore_sorteren()
{ int ronde,stap;

  high_file= fopen("highs.dat","r");
  fread(&highdat,sizeof(struct high),10,high_file);
  for(ronde= 0;ronde<= 9; ronde++)
   {  for(stap= 0;stap<= 9-ronde;stap++)
       if(highdat[stap+1].tscore > highdat[stap].tscore)
        {    highdat[10]= highdat[stap+1];
             highdat[stap+1]= highdat[stap];
             highdat[stap]= highdat[10];
        }
   }

  highscore= highdat[0].tscore;
  high_file= fopen("highs.dat","w");
  fwrite(&highdat,sizeof(struct high),10,high_file);
  fclose(high_file);
}

//////////////////////////////////////////////////////////////////////////////                                                                                     /////////////////////////////////////////////////////////
void spel_stoppen()
{  int teller= 0,x= 438,i;
   char nummer;

   play_sample(datafile[game_over].dat,geluid,128,1000,FALSE);
   allehokjes(49);
   clearblok= TRUE;
   allehokjes(0);
   clearblok= FALSE;

   textout(screen,datafile[score_font].dat,"GAME OVER!",310,155,SCORE_KLEUREN);
   textprintf(screen,font,310,230,190,"Your score: %.0f",score);
   clear_keybuf();
   if(score > highdat[9].tscore)
    {
       textout(screen,font,"You got a highscore!",310,250,190);
       textout(screen,font,"Enter your name:",310,270,190);
       highdat[9].tscore= score;

       for(i= 0;i<= 13;i++,x+= 8)
        {  nummer= readkey();
           if(nummer== 8)
            { if(i > 0)
               {  i-= 2 ;
                  x-= 16;
                  rectfill(screen,x+8,265,x+16,280,0);
               }

            }
           else highdat[9].naam[i]= nummer;

           textprintf(screen,font,x,270,190,"%c",highdat[9].naam[i]);
           if(nummer== 13)
           {  for(i= i;i<= 13;i++)
               highdat[9].naam[i]= ' ';
               break;
           }
        }

       high_file= fopen("highs.dat","w");
       fwrite(&highdat,sizeof(struct high),10,high_file);
       fclose(high_file);
       highscore_sorteren();
    }
   else{ textout(screen,font,"Sorry no highscore!",310,250,190); readkey(); }
   fade_out(FADE);
   remove_int(inc_snelheid);
   snelheid= 1;
}
//////////////////////////////////////////////////////////////////////////////
void tekenvb(int vblok)
{ /* tekent het volgende blok dat komt onder next */
  switch(vblok)
   {  case 0: printblok(123,350,LANGBLOK);
	      printblok(123,375,LANGBLOK);
	      printblok(123,400,LANGBLOK);
	      printblok(123,425,LANGBLOK);
	      break;

      case 1: printblok(104,375,VIERKANT);
	      printblok(129,375,VIERKANT);
	      printblok(104,400,VIERKANT);
	      printblok(129,400,VIERKANT);
	      break;

      case 2: printblok(129,368,TBLOK);
	      printblok(104,393,TBLOK);
	      printblok(129,393,TBLOK);
	      printblok(129,418,TBLOK);
	      break;

      case 3: printblok(104,358,LBLOK_RECHTS);
	      printblok(104,383,LBLOK_RECHTS);
	      printblok(104,408,LBLOK_RECHTS);
	      printblok(129,408,LBLOK_RECHTS);
	      break;

      case 4: printblok(129,358,LBLOK_LINKS);
	      printblok(129,383,LBLOK_LINKS);
	      printblok(129,408,LBLOK_LINKS);
	      printblok(104,408,LBLOK_LINKS);
	      break;

      case 5: printblok(129,358,ZBLOK_LINKS);
	      printblok(129,383,ZBLOK_LINKS);
	      printblok(104,383,ZBLOK_LINKS);
	      printblok(104,408,ZBLOK_LINKS);
	      break;

      case 6: printblok(104,358,ZBLOK_RECHTS);
	      printblok(104,383,ZBLOK_RECHTS);
	      printblok(129,383,ZBLOK_RECHTS);
	      printblok(129,408,ZBLOK_RECHTS);
	      break;
   }
}
//////////////////////////////////////////////////////////////////////////////

