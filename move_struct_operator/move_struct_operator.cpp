#include <iostream>
using namespace std;

#include "human.hpp"

Human GetHuman(Human &s)
{
	//return s;

	Human tmp(s.getAge());//����tmp
	return tmp;//tmp ��ֵ����ʱ���󣬵����ƶ����캯��

}
int main()
{
	//////////////////////////////////////////////////////////////////////////
	//�ƶ�������ƶ���ֵ����
	//copy class
	Human s0(12, "woman"); //Constructor
	Human s1 = s0;//copy Constructor
	Human s11;
	s11 = s1;//copy operator=

	//move class (std::move)(std::move���ں���̬��Դ��������ϣ�������ھ�̬��Դ�ϣ��� char[10],��Ȼ�Ḵ�ƣ���Ϊû�ж�Ӧ���ƶ����캯��)
	Human s2(std::move(s1));//move Constructor  //��ֵת��ֵ����ֹ���Ʋ���������s1�Ķ�̬��Դ�Ѿ�����գ��޷��������ʣ���name��
	//Human s2 = std::move(s1);//����ʽ��Ч 
	Human s3;
	s3 = std::move(s2);//move operator=

	//move class (�ƶ�������ƶ���ֵ)
	Human s4 = GetHuman(s0);//move Constructor
	Human s5;
	s5 = GetHuman(s0);//move operator=  ��ȡ��ֵʱ�������ƶ���ֵ����ֹ���Ʋ���

	//[�ܽ�]������ͬʱ��ʼ���Ķ��ǵ��ù��캯����������ɺ��ٸ�ֵ�ĵ��ø�ֵ�������
	//����Ϊ��ֵ����ó���Ĺ����ֵ������Ϊ��ֵ������ƶ�������ƶ���ֵ��

	return 0;
}