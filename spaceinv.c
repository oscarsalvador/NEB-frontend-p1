
#include <stdlib.h>

#define INVAS_STEPSX 20
#define INVAS_STEPSY 20
#define BALA_STEPSY 40

struct movil
{
    /* cualquier objeto movil en pantalla */
    int c0x;  // coordenadas en c
    int c0y;
    int cw;   // semitamaño en c
    int ch;
	int vivo; 
};

int ancho=1000;
int alto=1000;
int lienzo_ancho=100;
int lienzo_alto=100;
int spacing_x=0;
int spacing_y=0;
int invas_filas=0;
int invas_cols=0;
int invas_ancho=0;
int invas_alto=0;
int invas_sentido=+1;
int invas_deltax=0;
int invas_deltay=0;
int balas_max=0;
int balas_ancho=0;
int balas_alto=0;
int balas_deltay=0;
struct movil *invasores;
struct movil *balas;
int mensaje[1000];
int *m=mensaje;

void toLog(int d) {
    *(m++)=d;
}

__attribute__((export_name("getMensaje")))
int *getMensaje() {
    return mensaje;
}

int js2c_x(int jsx, struct movil a){
    return (jsx*ancho)/lienzo_ancho+a.cw/2;
}

int js2c_y(int jsy, struct movil a){
    return ((100-jsy)*alto)/lienzo_alto+a.ch/2;
}

int js2c_w(int jsw){
    return (jsw*ancho)/lienzo_ancho;
}

int js2c_h(int jsh){
    return (jsh*alto)/lienzo_alto;
}

int c2js_x(int cx, struct movil a){
    return ((cx-a.cw/2)*lienzo_ancho)/ancho;
}

int c2js_y(int cy, struct movil a){
    return ((alto-cy-a.ch/2)*lienzo_alto)/alto;
}


int colision(struct movil a, struct movil b){
    m=mensaje;
    toLog(a.c0x);
    toLog(b.c0x);
    int dx=a.c0x-b.c0x;
    toLog(dx);
    if (dx<0) dx=-dx;
    toLog(dx);
    toLog(a.cw+b.cw);
    toLog(a.c0y);
    toLog(b.c0y);
    int dy=a.c0y-b.c0y;
    toLog(dy);
    if (dy<0) dy=-dy;
    toLog(dy);
    toLog(a.ch+b.ch);
    if ((dx < a.cw+b.cw) && (dy < a.ch+b.ch)) {
        toLog(1001);
        return 1;
    }
    toLog(1000);
    return 0;
}

void colisiones() {
    int invas_total=invas_filas*invas_cols;
	struct movil *bala=balas;
    for(int b=0; b<balas_max; b++, bala++){
        if(bala->vivo>0) {
			struct movil *invasor=invasores;
            for(int i=0; i<invas_total; i++, invasor++){
                if(invasor->vivo>0) {
                    if(colision(*bala,*invasor)) {
                        bala->vivo=0;
                        invasor->vivo=0;
                        toLog(b);
                        toLog(bala->vivo);
                        toLog(i);
                        toLog(invasor->vivo);
                        toLog(2000);
                        break;
                    }
                }
            }
        }
    }
}



__attribute__((export_name("mueve_balas")))
int mueve_balas() {
    struct movil *bala=balas;
    for(int b=0; b<balas_max; b++, bala++){
        if(bala->vivo>0) {
            balas[b].c0y+=balas_deltay;
            if(balas[b].c0y+balas[b].ch>alto){
                bala->vivo=0;
            }
        }
    }
    colisiones();
    int fin=1;
    int invas_total=invas_filas*invas_cols;
	struct movil *invasor=invasores;
    for(int i=0; i<invas_total; i++, invasor++){
        if (invasor->vivo>0) {
            fin=0;
            break;
        }
    }
    return fin;
}


__attribute__((export_name("mueve_invas")))
int mueve_invas() {
    int invas_total=invas_filas*invas_cols;
    int bajar=0;
    int fin=1;
    struct movil *invasor;
    if(invas_sentido>0) {
        invasor=invasores;
        for(int i=0; i<invas_total; i++, invasor++){
            if (invasor->vivo>0) {
                fin=0;
                if (invasores[i].c0x+invasores[i].cw+invas_deltax>=ancho) {
                    bajar=1;
                    break;
                }
            }
        }
    } else {
        invasor=invasores;
        for(int i=0; i<invas_total; i++, invasor++){
            if (invasor->vivo>0) {
                fin=0;
                if (invasores[i].c0x-invasores[i].cw-invas_deltax<=0) {
                    bajar=1;
                    break;
                }
            }
        }
    }
    if (fin==1) return fin;
    if (bajar==0) {
        invasor=invasores;
        for(int i=0; i<invas_total; i++, invasor++){
            if (invasor->vivo>0) {
                invasores[i].c0x+=invas_sentido*invas_deltax;
            }
        }
    } else {
        invasor=invasores;
        for(int i=0; i<invas_total; i++, invasor++){
            if (invasor->vivo>0) {
                if(invasores[i].c0y-invasores[i].ch-invas_deltay<=0) fin=10;
                invasores[i].c0y-=invas_deltay;
            }
        }
        invas_sentido=-invas_sentido;
    }
    colisiones();
    return fin;
}


int movil_vivo(struct movil *moviles, int id) {
    return moviles[id].vivo;
}

int movil_x(struct movil *moviles, int id) {
    return c2js_x(moviles[id].c0x, moviles[id]);
}

int movil_y(struct movil *moviles, int id) {
    return c2js_y(moviles[id].c0y, moviles[id]);
}


__attribute__((export_name("bala_viva")))
int bala_viva(int id) {
    return movil_vivo(balas,id);
}

__attribute__((export_name("bala_x")))
int bala_x(int id) {
    return movil_x(balas,id);
}

__attribute__((export_name("bala_y")))
int bala_y(int id) {
    return movil_y(balas,id);
}

__attribute__((export_name("nueva_bala")))
int nueva_bala(int jsx) {
	struct movil *bala=balas;
    for(int b=0; b<balas_max; b++, bala++){
        if(bala->vivo==0) {
            bala->c0x=js2c_x(jsx, *bala);
            bala->c0y=js2c_h(8);
            bala->vivo=1;
            return b;
        }
    }
    return -1;
}


__attribute__((export_name("invasor_vivo")))
int invasor_vivo(int id) {
    return movil_vivo(invasores,id);
}

__attribute__((export_name("invasor_x")))
int invasor_x(int id) {
    return movil_x(invasores,id);
}

__attribute__((export_name("invasor_y")))
int invasor_y(int id) {
    return movil_y(invasores,id);
}


__attribute__((export_name("init")))
int init(int canvas_height,
    int canvas_wide,
    int invaders_rows, 
    int invaders_cols,
    int invaders_wide, 
    int invaders_height, 
    int max_bullets, 
    int bullets_wide, 
    int bullets_height) {

    lienzo_ancho=canvas_wide;
    lienzo_alto =canvas_height;
    invas_filas =invaders_rows;
    invas_cols  =invaders_cols;
    balas_max   =max_bullets;

    invas_ancho=invaders_wide  *ancho/lienzo_ancho;
    invas_alto =invaders_height*alto /lienzo_alto;
    balas_ancho=bullets_wide   *ancho/lienzo_ancho;
    balas_alto =bullets_height *alto /lienzo_alto;

    invasores=malloc(invas_filas*invas_cols*sizeof(struct movil));
    balas=malloc(balas_max*sizeof(struct movil));

    invas_deltax=ancho/INVAS_STEPSX;
    invas_deltay=alto/INVAS_STEPSY;
    balas_deltay=alto/BALA_STEPSY;

    spacing_x=invas_ancho+invas_deltax;
    if (spacing_x*invas_cols>=ancho) return 2;

    spacing_y=invas_alto+invas_deltay;
    if (spacing_y*invas_filas>=alto) return 3;

    struct movil *invasor=invasores;
    int w=invas_ancho/2;
    int h=invas_alto/2;
    int y=alto-h;
    for(int f=0; f<invas_filas; f++) {
        int x=w+invas_deltax;
        for(int c=0; c<invas_cols; c++) {
            invasor->c0x=x;
            invasor->c0y=y;
            invasor->cw=w;
            invasor->ch=h;
            invasor->vivo=1;

            x+=spacing_x;
            invasor++;
        }
        y-=spacing_y;
    }

    struct movil *bala=balas;
    w=balas_ancho/2;
    h=balas_alto/2;
    for(int b=0; b<balas_max; b++) {
        bala->c0x=0;
        bala->c0y=0;
        bala->cw=w;
        bala->ch=h;
        bala->vivo=0;

        bala++;
    }
    return 0;
}
