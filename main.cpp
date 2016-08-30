#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <unistd.h>
#include <fstream> 

#define PI 3.1415926535897932
#define ZERO 1E-4
#define INF 1E100
#define DEGREES_RADIANS(angle) (((angle) / 180.0) * PI)
#define rotationX(X, Y, radians) ( ( X * cos(radians) ) - ( Y * sin (radians) ) )
#define rotationY(X, Y, radians) ( ( X * sin(radians) ) + ( Y * cos (radians) ) )

typedef struct{
  double x, y;
} _point;

typedef struct{
	int n;
	_point *v;
} _polygon;

typedef struct {
	double a;		// x-coefficient 
	double b;		// y-coefficient 
	double c;		// constant term 
} _line;

double polygonArea(_polygon p){
	int i,j;
	double area = 0.0;
	// Accumulates area in the loop
	j = p.n-1; // The last vertex is the 'previous' one to the first
	for (i=0; i<p.n; i++){ 
		area = area + (p.v[j].x+p.v[i].x) * (p.v[j].y-p.v[i].y);
		j = i; //j is previous vertex to i
	}

	return (fabs(area/2));
}

double distance(_point a, _point b){
	double d=0.0;
	d = (a.x-b.x) * (a.x-b.x) + (a.y-b.y) * (a.y-b.y);

	return(sqrt(d));
}

double d_func(_point a, _point b, _point p){
	double d = (a.x-b.x)*(a.y-p.y)-(a.y-b.y)*(a.x-p.x);
	if (fabs(d) < ZERO)
		return (0);
	else if (d < -ZERO)
		return (-1);
	else
		return (1);
}

int main(int argc,char *argv[]){

	srand(time(NULL));
	int i, ii, j, jj,k;
	int flag;
	double theta, X, Y;
	double minX, maxX, minY, maxY;
	double area=100;
	char *arg1  = argv[1];
	int aux = atoi(arg1);
	int l = aux;//number sides
	arg1  = argv[2];
	aux = atoi(arg1);
	int altura = aux;//number sides
	arg1  = argv[3];
	aux = atoi(arg1);
	int comprimento = aux;//number sides
	char *saida;
	saida = argv [4];
	//add first triangle
	_polygon p;
	_polygon p_aux;
	p.v = (_point *) calloc(l,sizeof(_point));
	p_aux.v = (_point *) calloc(l,sizeof(_point));
	p.n = 0;
	//add O = (0,0)
	p.v[0].x = 0;
	p.v[0].y = 0;
	p.n = p.n + 1;	
	//add (rand,0)
	p.v[1].x = (rand() % int(2*sqrt(2*area)))+1;//a ideia eh gerar um valor entre 1 e (lado retangulo equilatero)
	p.v[1].y = 0;
	p.n = p.n + 1;	
	//calc point
	p.v[2].x = rand()% int(p.v[1].x);
	p.v[2].y = (2*area)/(distance(p.v[0], p.v[1]));
	p.n = p.n + 1;	

	//all polygon has n-2 ears
	//adding to the polygon p to the other ears
  	for (i = 0; i < (l-2)-1; i++){
		//find the ear: find two consecutive points who are the right of all other
		flag = 0;
		int ear = 0;
		while(flag == 0){
			flag = 1;
  		for (k = 0; k < p.n; k++){
					if(d_func(p.v[ear%p.n], p.v[(ear+1)%p.n], p.v[k]) == -1){
						flag = 0;
  					printf("orelha %d %d %d\n", ear%p.n, (ear+1)%p.n, k);
					}
			}
			ear++;
		}
		//insert the new vertex
		p_aux.n = p.n+1;
		k = ear+1;
  		for (j = 0, jj = 0; j < p.n; j++, jj++){
			p_aux.v[jj].x = p.v[j].x;
			p_aux.v[jj].y = p.v[j].y;
			if(j == k){

				jj++;
				double hipotenusa = distance(p.v[j],p.v[jj]);
				double cateto_oposto = (p.v[j].y-p.v[jj].y);
				theta = DEGREES_RADIANS(asin (cateto_oposto/hipotenusa)* 180.0 / PI);
				X = 0;
				Y = (2*area)/(distance(p.v[j],p.v[jj]));
				//printf("ponto (%.2f, %.2f) :%.2f \n", (p.v[j].x+p.v[jj].x)/2 + rotationX(X,Y,theta), (p.v[j].y+p.v[jj].y)/2 + rotationY(X,Y,theta),asin (cateto_oposto/hipotenusa)* 180.0 / PI);
				p_aux.v[jj].x = (p.v[j].x+p.v[jj].x)/2 + rotationX(X,Y,theta);
				p_aux.v[jj].y = (p.v[j].y+p.v[jj].y)/2 + rotationY(X,Y,theta);
			}
		}
		p.n = p.n + 1;	
		//copy to original polygon
		minX = minY = INF;
	  	maxX = maxY = -INF;
  		for (j = 0; j < p.n; j++){
			p.v[j].x = p_aux.v[j].x;
			p.v[j].y = p_aux.v[j].y;
			if(p.v[j].x > maxX) maxX = p.v[j].x;
			if(p.v[j].y > maxY) maxY = p.v[j].y;
			if(p.v[j].x < minX) minX = p.v[j].x;
			if(p.v[j].y < minY) minY = p.v[j].y;
		}
  		for (j = 0; j < p.n; j++){
			p.v[j].x = p.v[j].x-minX;
			p.v[j].y = p.v[j].y-minY;
		}
  		for (j = 0; j < p.n; j++){
			p.v[j].x = (p.v[j].x*comprimento)/(maxX-minX);
			p.v[j].y = (p.v[j].y*altura)/(maxY-minY);
		}
	}

	if(p.n == 3){
		for (j = 0; j < p.n; j++){
			p_aux.v[j].x = p.v[j].x;
			p_aux.v[j].y = p.v[j].y;
		}
		//copy to original polygon
		minX = minY = INF;
	  	maxX = maxY = -INF;
		for (j = 0; j < p.n; j++){
			p.v[j].x = p_aux.v[j].x;
			p.v[j].y = p_aux.v[j].y;
			if(p.v[j].x > maxX) maxX = p.v[j].x;
			if(p.v[j].y > maxY) maxY = p.v[j].y;
			if(p.v[j].x < minX) minX = p.v[j].x;
			if(p.v[j].y < minY) minY = p.v[j].y;
		}
		for (j = 0; j < p.n; j++){
			p.v[j].x = p.v[j].x-minX;
			p.v[j].y = p.v[j].y-minY;
		}
		for (j = 0; j < p.n; j++){
			p.v[j].x = (p.v[j].x*comprimento)/(maxX-minX);
			p.v[j].y = (p.v[j].y*altura)/(maxY-minY);
		}
	}


	//out file
	FILE *arq;
	char local[50];
	sprintf(local,"out/%s_items.txt",saida);
	// write mode with file creation -> w +

    if((fopen(local,"r"))!=NULL){
		arq = fopen(local, "a"); 
		if (arq == NULL) {
			printf("\nError creating output file!\n");
			return(-1);
		}
    }
    else{
		arq = fopen(local, "a"); 
		if (arq == NULL) {
			printf("\nError creating output file!\n");
			return(-1);
		}
		fprintf(arq,"10\n");
     }




	// writes the information in the file
	fprintf(arq,"\n%d\n", p.n);
	for (i = 0; i < p.n; i++){
	  fprintf(arq,"%.2f\t%.2f\n",  p.v[i].x , p.v[i].y);
	}

	//out file
	FILE *arq2;
	char local2[50];
	sprintf(local2,"out/%s_draw.txt",saida);
	// write mode with file creation -> w +
	arq2 = fopen(local2, "a"); 
	if (arq == NULL) {
		printf("\nError creating output file!\n");
		return(-1);
	}
	// writes the information in the file
	fprintf(arq2,"\n\\begin{tikzpicture}\n\\draw[fill=gray]\t");
	for (i = 0; i < p.n; i++){
	  fprintf(arq2,"( %.2f, %.2f)--",  p.v[i].x , p.v[i].y);
	}
	fprintf(arq2, "cycle;\n\\end{tikzpicture}\n");

	
	for (i = 0; i < 600000000; i++){
		j = j+1;
	}
}
