#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include "fx-serial.h"
void* read_print_input(void *a)
{
	struct fx_serial *ss=(struct fx_serial *)a;
	unsigned int X0=0,X1=0,X2=0,X3=0,i=0;
	unsigned int x0[8]={0},x1[8]={0},x2[8]={0},x3[8]={0};
	read_x0(ss,&X0);
	X0=(X0>>8)&0XFF;
	printf("input is :0X%X\n", X0);
	for(i=0;i<8;i++)
	{
		x0[i]=(X0>>i)&1;
		printf("x0[%d]=%d\n",i,x0[i]);
	}


	read_x1(ss,&X1);
	X1=(X1>>8)&0XFF;
	printf("input is :0X%X\n", X1);
	for(i=0;i<8;i++)
	{
		x1[i]=(X1>>i)&1;
		printf("x1[%d]=%d\n",i,x1[i]);
	}

	read_x2(ss,&X2);
	X2=(X2>>8)&0XFF;
	printf("input is :0X%X\n", X2);
	for(i=0;i<8;i++)
	{
		x2[i]=(X2>>i)&1;
		printf("x2[%d]=%d\n",i,x2[i]);
	}


	read_x3(ss,&X3);
	X3=(X3>>8)&0XFF;
	printf("input is :0X%X\n", X3);
	for(i=0;i<8;i++)
	{
		x3[i]=(X3>>i)&1;
		printf("x3[%d]=%d\n",i,x3[i]);
	}
}

void* read_print_output(void *b)
{
	struct fx_serial *ss=(struct fx_serial *)b;
// void read_print_output(struct fx_serial *ss)
// {
	unsigned int Y0=0,Y1=0,Y2=0,Y3=0,i=0;
	unsigned int y0[8]={0},y1[8]={0},y2[8]={0},y3[8]={0};
	read_y0(ss,&Y0);
	Y0=(Y0>>8)&0XFF;
	printf("output is :0X%X\n", Y0);
	for(i=0;i<8;i++)
	{
		y0[i]=(Y0>>i)&1;
		printf("y0[%d]=%d\n",i,y0[i]);
	}


	read_y1(ss,&Y1);
	Y1=(Y1>>8)&0XFF;
	printf("output is :0X%X\n", Y1);
	for(i=0;i<8;i++)
	{
		y1[i]=(Y1>>i)&1;
		printf("y1[%d]=%d\n",i,y1[i]);
	}

	read_y2(ss,&Y2);
	Y2=(Y2>>8)&0XFF;
	printf("output is :0X%X\n", Y2);
	for(i=0;i<8;i++)
	{
		y2[i]=(Y2>>i)&1;
		printf("y2[%d]=%d\n",i,y2[i]);
	}


	read_y3(ss,&Y3);
	Y3=(Y3>>8)&0XFF;
	printf("output is :0X%X\n", Y3);
	for(i=0;i<8;i++)
	{
		y3[i]=(Y3>>i)&1;
		printf("y3[%d]=%d\n",i,y3[i]);
	}
}
typedef struct
{
	struct fx_serial *ss;
	int id;	
}Test,*p_Test;
void* read_print_d(void *c)
{
	p_Test p=(p_Test)c;
	struct fx_serial * ss=p->ss;
	int id=p->id;
// void read_print_d(struct fx_serial * ss,int id)
// {
	int d=0;
	read_registerD(ss,id,&d);
	printf("d[%d] register is 0X%X\n",id,d);
}
void error(char *msg)
{
    fprintf(stderr,"%s:%s\n",msg,strerror(errno));
    exit(1);
}
int main(int argc, char *argv[]) 
{
	
	//struct fx_serial *ss = fx_serial_start("/dev/ttyS1", 9600, '7', 'E', '1');//for ubuntu
	struct fx_serial *ss = fx_serial_start("/dev/ttymxc1", 9600, '7', 'E', '1');//for develop board
	
	// fx_register_set(ss, 120, 0xab3d);
	// fx_register_get(ss, 120, &data);
	// while(1)
	// {
		
		//测试顺序读取
		// read_print_input(ss);
		 //read_print_output(ss);
		//read_print_d(ss,123);
	//}

	//多线程测试
	
	pthread_t t0,t1,t2;
	Test t;
	t.ss=ss;
	t.id=123;		//想要读取的D寄存器地址
	if(pthread_create(&t0,NULL,read_print_input,(void *)ss)==-1)
	{
		error("无法创建输入线程");
	}
	if(pthread_create(&t1,NULL,read_print_output,(void *)ss)==-1)
	{
		error("无法创建输出线程");
	}
	if(pthread_create(&t2,NULL,read_print_d,(void *)&t)==-1)
	{
		error("无法创建读D寄存器线程");
	}
	
	void *result;
    if(pthread_join(t0,&result)==-1)
        error("读输入线程回收失败");
    if(pthread_join(t1,&result)==-1)
        error("读输出线程回收失败");
	if(pthread_join(t2,&result)==-1)
        error("读D寄存器线程回收失败");

	
	
	
	fx_serial_stop(ss);
	return 0;
}
