#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>			//������ý�庯����
#pragma comment(lib,"winmm.lib")//������ý�庯�����Ӧ�Ŀ��ļ�
#include <fstream>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

const int colors[] = { 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14 };
const char* horizontal = "�T";
const char* vertical = "�U";

const int LENGTH = 10;//���֣�ÿ������ĳ�
const int SMALL_LENGTH = 6;//ÿ�����ֵĳ�
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //ȡ��׼����豸��Ӧ�ľ��

int** array;//��������
int speed = 10;

/* ������ƶ���ָ��λ�� */
void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/* ����ָ������ɫ

��ɫ��ȡֵ�ɱ���ɫ+ǰ��ɫ��ɣ���16��
fg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
8-15 ���� ���� ���� ���� ���� ���� ���� ����
bg_color��0-7    ��   ��   ��   ��   ��   ��   ��   ��
8-15 ���� ���� ���� ���� ���� ���� ���� ����
���յ���ɫΪ ����ɫ*16+ǰ��ɫ */
void setcolor(HANDLE hout, const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
}
//��ˮƽ��
void drawHorLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
}

//����ֱ��
void drawVerLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
}

//��ˮƽ��
void drawBoxHorLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
}

//����ֱ��
void drawBoxVerLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < SMALL_LENGTH+1; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
}

//����ˮƽ��
void clearBoxHorLine(HANDLE hout, const int X, const int Y, const int bg_color){
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, bg_color);
		putchar(' ');
		putchar(' ');
	}
}

//����ֱ��
void clearBoxVerLine(HANDLE hout, const int X, const int Y, const int bg_color){
	for (int i = 0; i < SMALL_LENGTH+1; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, bg_color);
		putchar(' ');
	}
}

/* ��ָ��λ�ã���ָ����ɫ����ʾһ���ַ� */
void showch(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color)
{
	//����ɫ
	for (int y = 0; y < LENGTH + 1; y++){
		for (int x = 0; x < (LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, 15, 15);
			putchar(' ');
		}
	}
	//�������ϱ�
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
	//�������±�
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y + LENGTH);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
	//���������
	for (int i = 1; i < LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
	//�������ұ�
	for (int i = 1; i < LENGTH; i++){
		gotoxy(hout, X + LENGTH * 2, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
}

void drawMoveBox(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color, int value){
	//����ɫ
	for (int y = 0; y < SMALL_LENGTH + 1; y++){
		for (int x = 0; x < (SMALL_LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, bg_color, bg_color);
			putchar(' ');
		}
	}
	//�������ϱ�
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//�������±�
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y + SMALL_LENGTH);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//���������
	for (int i = 1; i < SMALL_LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//�������ұ�
	for (int i = 1; i < SMALL_LENGTH; i++){
		gotoxy(hout, X + SMALL_LENGTH * 2, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	gotoxy(hout, X + 5, Y + 3);
	setcolor(hout, bg_color, fg_color);
	if (value < 10){
		cout << "  ";
	}
	else if (value < 100){
		cout << " ";
	}
	cout << value;
}
//���С����
void clearMoveBox(HANDLE hout, const int X, const int Y, const int bg_color){
	//����ɫ
	for (int y = 0; y < SMALL_LENGTH + 1; y++){
		for (int x = 0; x < (SMALL_LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, bg_color, bg_color);
			putchar(' ');
		}
	}
}
//�ж��ǲ���2��n�η�
int log2N(int target){
	int time = 0;
	if (target == 0){
		return 0;
	}
	while (1){
		if (target % 2 == 1){
			return time;
		}
		else{
			target /= 2;
			time++;
		}
	}
	return time;
}

//�ж��ǲ���2��n�η�
bool is2Log(int target){
	if (target < 128 || target > 65536){
		return false;
	}
	while (1){
		if (target % 2 == 1){
			return false;
		}
		else{
			target /= 2;
		}
		if (target == 1){
			return true;
		}
	}
}
//��С����
void print(int rows, int columns){
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			if (array[i][j] != 0){
				drawMoveBox(hout, j * LENGTH * 2 + 3, i * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
			}
		}
	}
}

bool remove(int rows, int columns, int order){
	bool flag = false;
	if (order == KEY_UP){
		for (int i = 0; i < columns; i++){
			for (int j = 1; j < rows; j++){
				if (array[j][i] != 0){
					for (int k = 0; k < j; k++){
						if (array[k][i] == 0){
							//�ƶ���ָ��λ��
							int posY = j;
							int posX = i;
							while (posY > k){
								for (int m = posY * LENGTH + 2; m >(posY - 1) * LENGTH + 2; m--){
									clearBoxHorLine(hout, posX * LENGTH * 2 + 3, m + SMALL_LENGTH, 15);
									drawBoxHorLine(hout, posX * LENGTH * 2 + 3, m - 1, colors[log2N(array[j][i]) % 12], 0);
									Sleep(speed);
								}
								//��Ĩ���ı߽粹��
								drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
								posY--;
								drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
							}
							array[k][i] = array[j][i];
							array[j][i] = 0;
							break;
						}
					}
				}
			}
		}
	}
	else if (order == KEY_DOWN){
		for (int i = 0; i < columns; i++){
			for (int j = rows - 2; j >= 0; j--){
				if (array[j][i] != 0){
					for (int k = rows - 1; k > j; k--){
						if (array[k][i] == 0){
							//�ƶ���ָ��λ��
							int posY = j;
							int posX = i;
							while (posY < k){
								//�ƶ���ָ��λ��
								for (int m = posY * LENGTH + 2; m < (posY + 1) * LENGTH + 2; m++){
									clearBoxHorLine(hout, posX*LENGTH * 2 + 3, m, 15);
									drawBoxHorLine(hout, posX * LENGTH * 2 + 3, m + SMALL_LENGTH + 1, colors[log2N(array[j][i]) % 12], 0);
									Sleep(speed);
								}
								posY++;
								//��Ĩ���ı߽粹��
								drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
								drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
							}
							array[k][i] = array[j][i];
							array[j][i] = 0;
							break;
						}
					}
				}
			}
		}
	}
	else if (order == KEY_LEFT){
		for (int i = 0; i < rows; i++){
			for (int j = 1; j < columns; j++){
				if (array[i][j] != 0){
					for (int k = 0; k < j; k++){
						if (array[i][k] == 0){
							int posY = i;
							int posX = j;
							while (posX > k){
								//�ƶ���ָ��λ��
								for (int m = posX * LENGTH * 2 + 3; m >(posX - 1) * LENGTH * 2 + 3; m--){
									//clearMoveBox(hout, m, posY * LENGTH + 2, 15);
									//drawMoveBox(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
									clearBoxVerLine(hout, m + SMALL_LENGTH * 2 + 1, posY * LENGTH + 2, 15);
									drawBoxVerLine(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0);
									Sleep(speed);
								}
								//��Ĩ���ı߽粹��
								drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
								posX--;
								drawMoveBox(hout, posX * LENGTH * 2 + 2, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
							}
							array[i][k] = array[i][j];
							array[i][j] = 0;
							break;
						}

					}
				}
			}
		}
	}
	else if (order == KEY_RIGHT){
		for (int i = 0; i < rows; i++){
			for (int j = columns - 2; j >= 0; j--){
				if (array[i][j] != 0){
					for (int k = columns - 1; k > j; k--){
						if (array[i][k] == 0){
							int posY = i;
							int posX = j;
							while (posX < k){
								//�ƶ���ָ��λ��
								for (int m = posX * LENGTH * 2 + 3; m < (posX + 1) * LENGTH * 2 + 3; m++){
									//clearMoveBox(hout, m, posY * LENGTH + 2, 15);
									//drawMoveBox(hout, m + 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
									clearBoxVerLine(hout, m , posY * LENGTH + 2, 15);
									drawBoxVerLine(hout, m + SMALL_LENGTH * 2, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0);
									Sleep(speed);
								}
								posX++;
								//��Ĩ���ı߽粹��
								drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
								drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
							}
							array[i][k] = array[i][j];
							array[i][j] = 0;
							break;
						}
					}
				}
			}
		}
	}
	return flag;
}



//�ϲ�
bool merge(int rows, int columns, int order){
	bool flag = false;
	if (order == KEY_UP){
		for (int i = 0; i < columns; i++){
			for (int j = 0; j < rows - 1; j++){
				if (array[j][i] != 0 && array[j][i] == array[j + 1][i]){
					int posX = i;
					int posY = j + 1;
					//�ƶ���ָ��λ��
					for (int m = posY * LENGTH + 2; m >(posY - 1) * LENGTH + 2; m--){
						clearMoveBox(hout, posX * LENGTH * 2 + 3, m, 15);
						drawMoveBox(hout, posX * LENGTH * 2 + 3, m - 1, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
						Sleep(speed);
					}
					//��Ĩ���ı߽粹��
					drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					posY--;
					//����2
					array[j][i] *= 2;
					array[j + 1][i] = 0;
					drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[j][i]) % 12], 0, array[j][i]);

				}
			}
		}

	}
	else if (order == KEY_DOWN){
		for (int i = 0; i < columns; i++){
			for (int j = rows - 2; j >= 0; j--){
				if (array[j][i] != 0 && array[j][i] == array[j + 1][i]){
					int posX = i;
					int posY = j;
					//�ƶ���ָ��λ��
					for (int m = posY * LENGTH + 2; m < (posY + 1) * LENGTH + 2; m++){
						clearMoveBox(hout, posX * LENGTH * 2 + 3, m, 15);
						drawMoveBox(hout, posX * LENGTH * 2 + 3, m + 1, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
						Sleep(speed);
					}
					posY++;
					//��Ĩ���ı߽粹��
					drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					//����2
					array[j + 1][i] *= 2;
					array[j][i] = 0;
					drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[j + 1][i]) % 12], 0, array[j + 1][i]);

				}
			}
		}
	}
	else if (order == KEY_LEFT){
		for (int i = 0; i < rows; i++){
			for (int j = 1; j < columns; j++){
				if (array[i][j] != 0 && array[i][j - 1] == array[i][j]){
					int posX = j;
					int posY = i;
					//�ƶ���ָ��λ��
					for (int m = posX * LENGTH * 2 + 3; m >(posX - 1) * LENGTH * 2 + 3; m--){
						clearMoveBox(hout, m, posY * LENGTH + 2, 15);
						drawMoveBox(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						Sleep(speed);
					}
					//��Ĩ���ı߽粹��
					drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					posX--;
					//����2
					array[i][j - 1] *= 2;
					array[i][j] = 0;
					drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[i][j - 1]) % 12], 0, array[i][j - 1]);
				}
			}
		}
	}
	else if (order == KEY_RIGHT){
		for (int i = 0; i < rows; i++){
			for (int j = columns - 2; j > 0; j--){
				if (array[i][j] != 0 && array[i][j] == array[i][j + 1]){
					int posX = j;
					int posY = i;
					//�ƶ���ָ��λ��
					for (int m = posX * LENGTH * 2 + 3; m < (posX + 1) * LENGTH * 2 + 3; m++){
						clearMoveBox(hout, m, posY * LENGTH + 2, 15);
						drawMoveBox(hout, m + 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						Sleep(speed);
					}
					posX++;
					//��Ĩ���ı߽粹��
					drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					//����2
					array[i][j + 1] *= 2;
					array[i][j] = 0;
					drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[i][j + 1]) % 12], 0, array[i][j + 1]);
				}
			}
		}
	}
	return flag;
}


//�������2��4�����ʷֱ�Ϊ80%��20%
int rand2_4(){
	int value = rand() % 10;
	if (value < 8){
		return 2;
	}
	else{
		return 4;
	}
}

//�жϵ�ǰtable���ܷ�����ϲ�
bool mergeAble(int rows, int columns, int order){
	if (order == KEY_UP){
		for (int i = 0; i < rows - 1; i++){
			for (int j = 0; j < columns; j++){
				if (array[i][j] != 0 && array[i][j] == array[i + 1][j]){
					return true;
				}
			}
		}
	}
	else if (order == KEY_DOWN){
		for (int i = rows - 1; i >= 1; i--){
			for (int j = 0; j < columns; j++){
				if (array[i][j] != 0 && array[i][j] == array[i - 1][j]){
					return true;
				}
			}
		}
	}
	else if (order == KEY_LEFT){
		for (int i = 0; i < columns - 1; i++){
			for (int j = 0; j < rows; j++){
				if (array[j][i] != 0 && array[j][i] == array[j][i + 1]){
					return true;
				}
			}
		}
	}
	else if (order == KEY_RIGHT){
		for (int i = columns - 1; i >= 1; i--){
			for (int j = 0; j < rows; j++){
				if (array[j][i] != 0 && array[j][i] == array[j][i - 1]){
					return true;
				}
			}
		}
	}
	return false;
}

int main(){
	srand((unsigned)time(0));
	int rows = 0;
	int columns = 0;
	int target = 3;
	while (rows < 4 || rows > 8){
		cout << "��������[4..8]: ";
		cin >> rows;
	}
	while (columns < 4 || columns > 10){
		cout << "��������[4..10]: ";
		cin >> columns;
	}
	while (!is2Log(target)){
		cout << "���������ֵ[1024..65536]��2����������:";
		cin >> target;
	}
	speed = -1;
	while (speed < 0){
		cout << "�����붯����ʱ[0-5]" << endl;
		cin >> speed;
	}
	speed = speed * 100 / LENGTH;
	char cmd[80];
	sprintf(cmd, "mode con cols=%d lines=%d", (columns)* LENGTH * 2 + 2, (rows)*LENGTH + 3);
	system(cmd);
	system("cls"); //����ϵͳ���������Ļ
	ifstream infile;
	infile.open("highest.txt", ios::in);
	int highest = 0;
	if (infile){
		infile >> highest;
		infile.close();
	}

	cout << "Ŀ�꣺ " << target << " ����  " << 0 << "  ���  " << highest << "  ʱ�� " << "  0  ";
	cout << "<��ͷ���ƶ�,Q�˳�>" << endl;
	//������
	for (int i = 0; i < rows; i++){
		for (int j = 0; j < columns; j++){
			showch(hout, j * LENGTH * 2, i * LENGTH + 1, 15, 0);
		}
	}


	array = new int*[rows];
	for (int i = 0; i < rows; i++){
		array[i] = new int[columns];
		for (int j = 0; j < columns; j++){
			array[i][j] = 0;
		}
	}
	//�������2��2
	for (int i = 0; i < 2; i++){
		int x = rand() % rows;
		int y = rand() % columns;
		if (array[x][y] == 2){
			i--;
			continue;
		}
		else{
			array[x][y] = 2;
		}
	}
	//��Ŀ�귽��
	int speed = 100;//�ƶ��ٶ�0.1��
	char order;
	print(rows, columns);
	int startTime = (unsigned)time(0);
	while (1)
	{
		order = getch();
		if (order == 'Q' || order == 'q'){
			break;
		}
		else if (order == KEY_UP || order == KEY_DOWN || order == KEY_LEFT || order == KEY_RIGHT){
			remove(rows, columns, order);
			merge(rows, columns, order);
			remove(rows, columns, order);
			gotoxy(hout, 1, 1);
		}
		else{
			gotoxy(hout, 1, 1);
			continue;
		}


		int largest = 0;
		int zeroTimes = 0;
		//�ҳ���ǰ�������ֵ��0�ĸ���
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				if (array[i][j] == 0){
					zeroTimes++;
				}
				else if (largest < array[i][j]){
					largest = array[i][j];
					if (largest == target){
						setcolor(hout, 0, 15);
						cout << endl;
						gotoxy(hout, 0, rows*LENGTH + 2);
						cout << "��ϲ�ɹ�.........";
						if (largest > highest){
							highest = largest;
							ofstream ofile("highest.txt");
							ofile << highest;
							ofile.close();
						}
						system("pause");
						return 0;
					}
				}
			}
		}
		gotoxy(hout, 0, 0);
		setcolor(hout, 0, 15);
		int curTime = (unsigned)time(0);
		if (largest > highest){
			highest = largest;
			ofstream ofile;
			ofile.open("highest.txt", ios::out);
			if (ofile){
				ofile << highest;
				ofile.close();
			}
			else{
				cout << "out" << endl;
			}
		}
		cout << "Ŀ�꣺ " << target << " ����  " << largest << "  ���   " << highest << " ʱ��  " << (curTime - startTime) << "   ";
		cout << "<��ͷ���ƶ�,Q�˳�>" << endl;
		//���������pos����������
		int pos = rand() % zeroTimes;
		int temp = 0;
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				if (array[i][j] == 0){
					if (temp == pos){
						array[i][j] = rand2_4();
						drawMoveBox(hout, j * LENGTH * 2 + 3, i * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						//��������������
						i = rows;
						j = columns;
					}
					temp++;
				}

			}
		}
		//���0��λ��ֻ��1������ôҪ����Ƿ����ƶ��Ŀ�����
		if (zeroTimes == 1){
			if (!mergeAble(rows, columns, KEY_UP) && !mergeAble(rows, columns, KEY_DOWN) && !mergeAble(rows, columns, KEY_LEFT) && !mergeAble(rows, columns, KEY_RIGHT)){
				setcolor(hout, 0, 15);
				cout << endl;
				gotoxy(hout, 0, rows*LENGTH + 2);
				cout << "ʧ��.........";
				system("pause");
				return 0;
			}
		}
	}

	setcolor(hout, 0, 15);
	cout << endl;
	gotoxy(hout, 0, rows*LENGTH + 2);
	system("pause");
	return 0;
}