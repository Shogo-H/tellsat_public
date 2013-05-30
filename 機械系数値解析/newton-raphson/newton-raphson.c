/*


*/

#include <stdio.h>
#include <math.h>
#define EPS (1e-10)	// ŒvŽZŒë·‚Ì‹–—e’l
#define limit (1000)	//ŒvŽZ‰ñ”‚Ì§ŒÀ

int main(void){

	double x00, y00;	//‰Šú’l
	double t, s, x, y, x0, y0, dx, dy, absx;
	int k, n;

	FILE *fp;
	fp = fopen( "FILE_NAME.csv", "w" );

	for(y00=10.0; y00>=-10.0; y00-=0.1){
		for(x00=-10.0; x00<=10.0; x00+=0.1){

			//‰Šú‰»
			n=0;
			k=0;
			absx=0.0;
			x0=x00;
			y0=y00;

			//”½•œŒvŽZ
			do{
				t=3*x0*x0*x0-2*y0-1;
				s=3*x0-y0*y0*y0-2;
				x=x0-(1/(6-27*x0*x0*y0*y0))*(-3*y0*y0*t+2*s);
				y=y0-(1/(6-27*x0*x0*y0*y0))*(-3*t+9*x0*x0*s);
				dx=fabs(x-x0);
				dy=fabs(y-y0);
				absx+=dx+dy;
				x0=x;
				y0=y;
				n++;
			}while(dx/absx>EPS && n<limit);
			printf("x00=%2.4f, y00=%2.4f, x=%2.4f, y=%2.4f \n",x00 ,y00, x, y);
			fprintf(fp, "%d,", (int)(x+2.3) );
		}
		fprintf(fp, "\n");
	}
	fclose( fp );
	printf("‚¨‚í‚è");

	scanf("%d", &k);
	return 0;
}
