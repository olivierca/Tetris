/****************************************************************************/
/*			programme de jeu TETRIS											*/
/*											du	02.05.94					*/
/*																			*/
/****************************************************************************/

typedef	unsigned char 	uchar;
typedef unsigned int	uint;
typedef	unsigned long	ulong;

#include	"c:\dev\lib\genl.h"
#include	"c:\dev\lib\mcga.h"

/*		LES DATAS		*/
uchar	*Pgraph;		/* pointeur sur data graphique	*/
uchar	*adrspr;		/* pointeur sur fichier de sprite	*/
int		t;				/* variable clavier				*/
int		score;			/* variable du score			*/
uchar	fact;			/* flag de gestion motif		*/
uchar 	perdu=1;		/* flag indiquant fin de partie	*/
uchar	fauto;			/* flag indiquant une tomb‚ auto*/
int		cptatt;			/* compteur boucle pour tomb‚	*/
int		level;			/* niveau du jeu				*/
int		xm,ym,lm,hm;	/* coordonn‚es du motif			*/
int		vit[10]={20,18,16,14,12,10,8,6,4,2};
int		levscr[10]={1000,2000,5000,8000,10000,15000,20000,25000,28000,30000};
uchar	Map[20][10]={	/* map du tetris 10*20			*/
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0}
};
uchar	motif[4][4];		/* matrice motif courant	*/

uchar	taille_motif[7][2]={
	{3,2},
	{4,1},
	{3,2},
	{3,2},
	{2,2},
	{3,2},
	{3,2}
};

uchar	tab_motif[7][4][4]={
	{{3,3,0,0},
	{0,3,3,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{8,8,8,8},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,0,4,0},
	{4,4,4,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{0,2,2,0},
	{2,2,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{1,1,0,0},
	{1,1,0,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{6,0,0,0},
	{6,6,6,0},
	{0,0,0,0},
	{0,0,0,0}},
	{{7,7,7,0},
	{0,7,0,0},
	{0,0,0,0},
	{0,0,0,0}}
};

/* prototype des fonctions	*/
void	Aff_Map(void);
void	Aff_Score(int val);
void	Gere_Tet(void);
void	Init_motif(void);
int		Test_pos(int xt, int yt);
void	Gere_act(void);
void	Rot_motif(void);
void	Set_motif(void);
void	Init_game(void);
void	Test_map(void);
void	Del_ligne(int l);
void	Get_Pal(void);

void main()
{
	/*	init le jeu		*/
	InitScreen();				/* 320*200 256 couleurs	*/
	Adr_Ecr = M_Alloc(64000L);	/* m‚moire ecran		*/
	adrspr = M_Alloc(5000L);	/* m‚moire pour les graphes	*/
	Load_F("graph.spr",adrspr); /* charge les graphes	*/

	/* prepare ecran	*/
	Cls();
	Get_Pal();
	Boxf(115,25,4,149,8);
	Boxf(200,25,4,149,8);
	Boxf(119,25,81,4,8);
	Boxf(119,170,81,4,8);

	/* boucle principale du jeu		*/
	while(t!=1)
	{
		t = Inkey();				/* lecture clavier		*/
		t = (t>>8)&0xFF;			/* code clavier			*/
		if(t==59)	Init_game();	/* si demarrage			*/
		if(!perdu)	Gere_Tet();		/* gestion du jeu		*/
		Aff_Map();					/* affiche le jeu		*/
		Aff_Score(score);			/* affiche score		*/
		Send_Video();				/* affiche ecran		*/
		Vsync();					/* synchro				*/
	}

	Mfree(adrspr);
	Mfree(Adr_Ecr);
	RestoScreen();				/* ecran comme avant	*/
	exit(0);
}

/* routine de prise en compte de la palette	*/
void	Get_Pal(void)
{
	uchar	*Ptr;
	int		i,index,nb;

	Ptr = adrspr;
	i = *(int *)Ptr;				/* type de fichier 	*/
	Ptr += 2;						/* pointe sur palette eventuel	*/
	if(i&2)							/* si palette		*/
	{
		while(*(int *)Ptr!=-1)	/* jusqu'a fin de palette	*/
		{
			index = (int)*Ptr++;	/* indice palette	*/
			nb = (int)*Ptr++;		/* nb de couleur	*/
			Outb(0x3C8,index);
			/*	place les nb couleurs dans palette	*/
			for(i=0 ; i<(nb*3) ; i++)	Outb(0x3C9,(int)*Ptr++);
		}
		Ptr += 2;
	}
	Pgraph = Ptr;					/* init pointeur graphique		*/
}

/* routine d'init du jeu	*/
void	Init_game(void)
{
	uchar	*Ptr;
	int		i;

	Init_Rnd();
	perdu = 0;
	fact = 0;
	score = 0;
	fauto = 0;
	level = 0;
	cptatt = vit[level];
	Ptr = Map[0];
	for(i=0 ; i<200 ; i++)	*Ptr++ = 0;	/* init map	*/
}

/* routine principale de gestion du jeu	*/
void	Gere_Tet(void)
{
	int		x,y;

	if(!fact)	Init_motif();		/* init motif a jouer	*/
	else
	{
		Gere_act();					/* action joueur		*/
		if(!cptatt-- || fauto)
		{
			/* gere descente auto	*/
			if(!Test_pos(xm,ym+1))	/* motif arriv‚ en bas	*/
			{
				Set_motif();		/* place motif dans map	*/
				fauto = 0;
				fact = 0;			/* motif annul‚			*/
				score += 10;
				Test_map();			/* test de la map		*/
				while(score>levscr[level])	level++;
			}
			else	ym++;			/* descente				*/
			cptatt = vit[level];
		}
	}
}

/* routine de mise a jour de la map	*/
void	Test_map(void)
{
	int		x,y,c;

	for(y=0 ; y<20 ; y++)	/* test toutes les lignes	*/
	{
		c = 0;				/* init compteur de brique	*/
		for(x=0 ; x<10 ; x++)	if(Map[y][x])	c++;
		if(c==10)			/* si ligne complete		*/
		{
			Del_ligne(y);		/* efface ligne	*/
			score += 100;
		}
	}
}

/* routine deletant une ligne de la map	*/
void	Del_ligne(int l)
{
	uchar	*Ptr1,*Ptr2;
	int		x,y;

	for(y=l ; y>0 ; y--)
	{
		Ptr1 = Map[y];
		Ptr2 = Map[y-1];
		for(x=0 ; x<10 ; x++)	*Ptr1++ = *Ptr2++;
	}
	Ptr1 = Map[0];
	for(x=0 ; x<10 ; x++)	*Ptr1++ = 0;
}

/* routine de rotation du motif	*/
void	Rot_motif(void)
{
	static 	uchar	Buf[4][4];
	int		x,y;
	uchar	*Ptr1,*Ptr2,c;

	/*	motif dans buffer	*/
	Ptr2 = Buf[0];
	Ptr1 = motif[0];
	for(x=0 ; x<16 ; x++)	*Ptr2++ = *Ptr1++;
	/* init motif			*/
	Ptr1 = motif[0];
	for(x=0 ; x<16 ; x++)	*Ptr1++ = 0;
	/* rotation				*/
	for(x=0 ; x<lm ; x++)
	{
		for(y=0 ; y<hm ; y++)	motif[x][hm-y-1] = Buf[y][x];
	}
	/* inverse dimension	*/
	x = lm;
	lm = hm;
	hm = x;
	/* ajuste X et Y	*/
	x = xm;
	y = ym;
	if(lm>hm)
	{
		xm -= (lm>>1);
		ym += (hm>>1);
	}
	else if(lm<hm)
	{
		xm += (lm>>1);
		ym -= (hm>>1);
	}
	if((xm+lm)>9)	xm = 10-lm;
	if((ym+hm)>19)	ym = 20-hm;
	if(xm<0)		xm = 0;
	if(ym<0)		ym = 0;
	if(!Test_pos(xm,ym))	/* pas de rotation si pas valide	*/
	{
		xm = x;				/* annule rotation	*/
		ym = y;
		x = lm;
		lm = hm;
		hm = x;				/* inverse dimension	*/
		Ptr1 = motif[0];
		for(x=0 ; x<16 ; x++)	*Ptr1++ = 0;
		Ptr1 = Buf[0];
		Ptr2 = motif[0];
		for(x=0 ; x<16 ; x++)	*Ptr2++ = *Ptr1++;
	}
}

/* routine de gestion action joueur	*/
void	Gere_act(void)
{
	int		x,y;

	switch(t)				/* action joueur		*/
	{
	case 77:				/* droit		*/
		x = xm+1;			/* nouvel x		*/
		if(x<(11-lm) && Test_pos(x,ym))	xm++;
		break;
	case 75:				/* gauche		*/
		x = xm-1;			/* nouvelle position	*/
		if(x>=0 && Test_pos(x,ym))	xm--;
		break;
	case 80:				/* bas			*/
		fauto = 1;			/* arriv‚ automatique	*/
		break;
	case 72:				/* haut			*/
		Rot_motif();		/* rotation du motif	*/
		break;
	default:
		break;
	}
}

/* routine placant le motif dans la map	*/
void	Set_motif(void)
{
	uchar	*Ptr;
	int		x,y;

	Ptr = motif[0];
	for(y=ym ; y<(ym+4) ; y++)
	{
		for(x=xm ; x<(xm+4) ; x++)
		{
			if(*Ptr)	Map[y][x] = *Ptr;	/* integre	motif	*/
			Ptr++;
		}
	}
	fact = 0;
}

/* routine verifiant si position motif valide	*/
int		Test_pos(int xt, int yt)
{
	uchar	*Ptr;
	int		x,y;

	Ptr = motif[0];
	for(y=yt ; y<(yt+4) ; y++)
	{
		for(x=xt ; x<(xt+4) ; x++)	if(*Ptr++ && Map[y][x])	return(0);	/* position non valide	*/
	}
	return(1);
}

/* routine d'initialisation	du motif */
void	Init_motif(void)
{
	uchar 	*Ptr1,*Ptr2;
	int		i;

	i = Rnd(7);
	Ptr1 = tab_motif[i][0];
	lm = taille_motif[i][0];
	hm = taille_motif[i][1];
	Ptr2 = motif[0];
	for(i=0 ; i<16 ; i++)	*Ptr2++ = *Ptr1++;
	ym = 0;
	xm = 3;
	fact = 1;							/* motif pret	*/
	if(!Test_pos(xm,ym))	perdu = 1;	/* si pas possible	*/
/*	while(!Test_pos(xm,ym) && xm<(9-lm)) xm++;
	if(xm==(9-lm))	perdu = 1;		/* fin de partie	*/
}

/* routine d'affiche du jeu	*/
void	Aff_Map(void)
{
	int		x,y;
	uchar	num,*Ptr;

	/* affichage du fond	*/
	Ptr = Map[0];
	for(y=30 ; y<170 ; y+=7)		/* boucle sur les lignes	*/
	{
		for(x=120 ; x<200 ; x+=8)	/* boucle sur les colonnes	*/
		{
			num = *Ptr++;
			if(!num)	Boxf(x,y,7,6,0);			/* pas de brique	*/
			else		AffSpr(x,y,Pgraph,num+9);	/* affiche brique	*/
		}
	}
	/* affichage du motif courant	*/
	if(fact)
	{
		Ptr = motif[0];
		for(y=(30+ym*7) ; y<(30+(ym+4)*7) ; y += 7)
		{
			for(x=(120+(xm<<3)) ; x<(120+((xm+4)<<3)) ; x+=8)
			{
				if(*Ptr)	AffSpr(x,y,Pgraph,(*Ptr)+9);
				Ptr++;
			}
		}
	}
}

/* routine d'affichage du score	*/
void	Aff_Score(int val)
{
	char	cscr[8],*Ptr,c;
	int		x,l;

	Gstr(val,cscr);				/* convertie score en texte	*/
	Ptr = cscr;
	l = Glen(cscr)*16;			/* largeur en pixels du score	*/
	x = 160-(l/2);				/* init X d'origine			*/
	Boxf(112,0,96,18,0);		/* init affichage			*/
	while(*Ptr)
	{
		c = *Ptr++;				/* code ascii				*/
		c -= 48;
		AffSpr(x,0,Pgraph,(int)c);
		x += 16;				/* caractere suivant		*/
	}
	Gstr(level+1,cscr);			/* level					*/
	Ptr = cscr;
	l = Glen(cscr)*16;			/* largeur en pixels du score	*/
	x = 58-(l/2);				/* init X d'origine			*/
	Boxf(42,30,32,18,0);		/* init affichage			*/
	while(*Ptr)
	{
		c = *Ptr++;				/* code ascii				*/
		c -= 48;
		AffSpr(x,30,Pgraph,(int)c);
		x += 16;				/* caractere suivant		*/
	}
}

