
#include <iostream>
#include <bitset>

using namespace std;

void coutp(unsigned int x)// ����� �� ����� ����� � binary ������������ �� ��� ��������
{
	int n = sizeof(int) * 8;
	unsigned maska = (1 << (n - 1));
	for (int i = 1; i <= n; i++)
	{
		cout << ((x & maska) >> (n - i));
		maska = maska >> 1;
	}
}
void t1(unsigned int value) {
	unsigned maska = 0x00000F00;

	value = value | maska;
	cout << endl << "�������� ��������: ";
	coutp(value);
	cout << endl;
}
void t2(unsigned int value) {
	unsigned maska = 0xFFFEBFFD;//  1111 1111 1111 1110 1011 1111 1111 1101

	value = value & maska;
	cout << endl << "�������� ��������: ";
	coutp(value);
	cout << endl;
}
void t3(unsigned int value) {
	value = value << 10;
	cout << endl << "�������� ��������: ";
	coutp(value);
	cout << endl;
}
void t4(unsigned int value) {
	value = value >> 10;
	cout << endl << "�������� ��������: ";
	coutp(value);
	cout << endl;
}
void t5(unsigned int value) {
	int n;
	cin >> n;
	unsigned maska = 0x0000001;
	maska = (maska << (n));
	maska = ~maska;
	value = value & maska;
	cout << endl << "�������� ��������: ";
	coutp(value);
	cout << endl;
}

int main() {


	setlocale(0, "");
	unsigned int a;
	int index;
	cout << "�������� ����� �������! : \n ";
	cout << "1 - ������������ ������� � 4� ����� ����� �� 9 ����\n 2 - ��������� 1���, 15���, 17��� �����\n 3 - �������� ����� �� 1024\n 4 - �������� ����� �� 1024\n 5 - �������� n-�� ��� \n";
	cin >> index;
	cin >> a;
	cout << bitset<32>(a) << endl;
	switch (index)
	{
	case 1:
	{
		t1(a);
		break;
	}
	case 2:
	{
		t2(a);
		break;
	}
	case 3:
	{
		t3(a);
		break;
	}
	case 4:
	{
		t4(a);
		break;
	}
	case 5:
	{
		t5(a);
		break;
	}
	default:
		break;
	}
	cout << endl;
}
