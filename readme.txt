��������ʱ����ʹ��windows�����뷨
֧��VBע��

sleep milliseconds

paint addr, x,y,w,h [,mode]
x,y�����Ǹ���
mode	0:copy 1:or 2:not 3:and 4:xor

load addr, int, ...
int�����ǳ���

point(x,y)
x,y�����Ǹ���

checkkey(wqxkey)

/////////////////////////////

open a$ for binary as #n
��random���ƣ�������binary�����½��ļ������Ҳ�����len

���������ļ�:
fopen(fnum)
�ļ��Ƿ��

����random��binary:

fgetc(fnum)
��ȡһ�ֽ�(num)

fputc fnum, c$
д��c$�ĵ�һ�ֽ�

fread fnum, addr, size

fwrite fnum, addr, size

ftell(fnum)

fseek fnum, pt