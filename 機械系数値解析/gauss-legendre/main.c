/*

以下の式をガウスルジャンドル積分を用いて相対精度0.0001で解を求めよ．
I=∫[0,1]log(x^2)dx
3分割して一番左端を残して積分を繰り返す作戦で，
14回目で-1.999993に収束．
*/

#include <stdio.h>
#include <math.h>
#define N (3)	// 分割数
#define EPS (1e-5)	// 計算誤差の許容値
#define limit (1000)	//計算回数の制限

//プロトタイプ宣言
double gli5(double a, double b);
double f(double x);

//メイン関数
int main(void){
	int k, i, j;
	double a, b, I, Isum, dx, absx;

	//初期設定
	j=1;
	Isum=0.0;
	absx=0.0;

	do{
		a=1.0/pow(N,(double)j);
		b=a+1.0/pow(N,(double)j);
		I=0.0;
		for(i=2;i<=N;i++){	//一番左は次に残しておくから２番めから
			I+=gli5(a,b);
			a+=1.0/pow(N,(double)j);
			b+=1.0/pow(N,(double)j);
		}
		dx=fabs(I);
		absx+=dx;
		Isum+=I;
		printf("%d回目で%f\n",j,Isum);
		j++;
	}while(dx/absx>EPS && j<limit);


	printf("I=%f\n",Isum);
	printf("おわり\n");
	scanf("%d", &k);
	return 0;
}

double gli5(double a, double b){
	double x0[5+1];
	double w[5+1];
	double sum;
	int i;

	//5分点
	//[-1,1]の範囲において，端点を含まず5点サンプリング
	x0[1]=-0.906179845938664;
	x0[2]=-0.538469310105683;
	x0[3]=0.0;
	x0[4]=0.538469310105683;
	x0[5]=0.906179845938664;

	//5分点での重み
	w[1]=0.236926885056189;
	w[2]=0.478628670499366;
	w[3]=0.568888888888889;
	w[4]=0.478628670499366;
	w[5]=0.236926885056189;

	sum=0;
	for(i=1;i<=5;i++){
		sum+=w[i]*f(0.5*(b+a)+x0[i]*(b-a)*0.5);
	}

	return sum*(b-a)*0.5;
}

double f(double x){
	return log(x*x);
}