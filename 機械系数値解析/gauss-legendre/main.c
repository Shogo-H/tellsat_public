/*

�ȉ��̎����K�E�X���W�����h���ϕ���p���đ��ΐ��x0.0001�ŉ������߂�D
I=��[0,1]log(x^2)dx
3�������Ĉ�ԍ��[���c���Đϕ����J��Ԃ����ŁC
14��ڂ�-1.999993�Ɏ����D
*/

#include <stdio.h>
#include <math.h>
#define N (3)	// ������
#define EPS (1e-5)	// �v�Z�덷�̋��e�l
#define limit (1000)	//�v�Z�񐔂̐���

//�v���g�^�C�v�錾
double gli5(double a, double b);
double f(double x);

//���C���֐�
int main(void){
	int k, i, j;
	double a, b, I, Isum, dx, absx;

	//�����ݒ�
	j=1;
	Isum=0.0;
	absx=0.0;

	do{
		a=1.0/pow(N,(double)j);
		b=a+1.0/pow(N,(double)j);
		I=0.0;
		for(i=2;i<=N;i++){	//��ԍ��͎��Ɏc���Ă�������Q�Ԃ߂���
			I+=gli5(a,b);
			a+=1.0/pow(N,(double)j);
			b+=1.0/pow(N,(double)j);
		}
		dx=fabs(I);
		absx+=dx;
		Isum+=I;
		printf("%d��ڂ�%f\n",j,Isum);
		j++;
	}while(dx/absx>EPS && j<limit);


	printf("I=%f\n",Isum);
	printf("�����\n");
	scanf("%d", &k);
	return 0;
}

double gli5(double a, double b){
	double x0[5+1];
	double w[5+1];
	double sum;
	int i;

	//5���_
	//[-1,1]�͈̔͂ɂ����āC�[�_���܂܂�5�_�T���v�����O
	x0[1]=-0.906179845938664;
	x0[2]=-0.538469310105683;
	x0[3]=0.0;
	x0[4]=0.538469310105683;
	x0[5]=0.906179845938664;

	//5���_�ł̏d��
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