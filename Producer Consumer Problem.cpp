#include "iostream"
#include "windows.h"
#include "stdio.h"
using namespace std;
bool lock[4] = { false,false,false,false };//�İ������ֱ���ĸ�����������ͬ��
int buffer[4] = { 0,0,0,0 };//����������ʾ��Դ�ĸ���
void display() {
	cout << "--------------------------------" << endl;
	cout << "������    0   1     2         3" << endl;
	for (int i = 0; i<4; i++) { cout << "  " << buffer[i] << "   "; }
	cout << endl;
}
DWORD WINAPI Producer(LPVOID lpParameter) {
	while (true) {
		for (int j = 0; j<4; j++) {
			if (buffer[j] == 0) {//�ҵ��ջ�����
				if (lock[j] == false) {//ͬ����Ϊfalse�����Խ��в���
					lock[j] = true;//��������ֹ�����̲߳����˻�����
					if (buffer[j]<1) {//�޶�һ��������ֻ�ܴ��һ����Դ
						++buffer[j];//ģ��������Դ
						cout << "����һ����Դ�����뻺����" << j << "��" << endl;
						lock[j] = false;//����
						break;//һ������һ��
					}
				}
				if (j == 3) {
					cout << "�Ҳ����ջ��������ȴ��С���" << endl;
					Sleep(2000);
				}
			}
		}
	}
	return 0;
}
DWORD WINAPI Customer(LPVOID lpParameter) {
	while (true) {
		for (int n = 0; n<4; n++) {
			if (buffer[n] == 1) {//�ҵ���������
				if (lock[n] == false) {//ͬ����Ϊfalse�����Խ��в���
					lock[n] = true;//��������ֹ�����̲߳����˻�����
					if (buffer[n] >= 1) {
						--buffer[n];//ģ��������Դ
						cout << "����һ����Դ���ӻ�����" << n << "��ȡ��" << endl;
						lock[n] = false;//����
						break;//һ������һ��
					}
				}
			}
			if (n == 3) {
				cout << "�Ҳ��������������ȴ��С���" << endl;
				Sleep(2000);
			}
		}
	}
	return 0;
}
int main(int argc, char* argv[]) {
	display();
	HANDLE handle[3];
	DWORD dw1, dw2, dw3, dw4, dw0;
	handle[0] = CreateThread(NULL, 0, Producer, NULL, 0, &dw1);
	handle[1] = CreateThread(NULL, 0, Producer, NULL, 0, &dw2);
	handle[2] = CreateThread(NULL, 0, Producer, NULL, 0, &dw3);
	handle[3] = CreateThread(NULL, 0, Customer, NULL, 0, &dw4);
	handle[4] = CreateThread(NULL, 0, Customer, NULL, 0, &dw0);
	display();
	Sleep(3000);
	display();
	return 0;
}
