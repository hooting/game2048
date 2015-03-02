#include <iostream>
#include <ctime>
#include <cmath>
#include <cstdio>
#include <conio.h>
#include <windows.h>
#include <mmsystem.h>			//包含多媒体函数库
#pragma comment(lib,"winmm.lib")//包含多媒体函数库对应的库文件
#include <fstream>
using namespace std;

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77

const int colors[] = { 2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 13, 14 };
const char* horizontal = "═";
const char* vertical = "║";

const int LENGTH = 10;//布局，每个方块的长
const int SMALL_LENGTH = 6;//每个数字的长
HANDLE hout = GetStdHandle(STD_OUTPUT_HANDLE); //取标准输出设备对应的句柄

int** array;//保存数字
int speed = 10;

/* 将光标移动到指定位置 */
void gotoxy(HANDLE hout, const int X, const int Y)
{
	COORD coord;
	coord.X = X;
	coord.Y = Y;
	SetConsoleCursorPosition(hout, coord);
}

/* 设置指定的颜色

颜色的取值由背景色+前景色组成，各16种
fg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
bg_color：0-7    黑   蓝   绿   青   红   粉   黄   白
8-15 亮黑 亮蓝 亮绿 亮青 亮红 亮粉 亮黄 亮白
最终的颜色为 背景色*16+前景色 */
void setcolor(HANDLE hout, const int bg_color, const int fg_color)
{
	SetConsoleTextAttribute(hout, bg_color * 16 + fg_color);
}
//画水平线
void drawHorLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
}

//画垂直线
void drawVerLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
}

//画水平线
void drawBoxHorLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
}

//画垂直线
void drawBoxVerLine(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color){
	for (int i = 0; i < SMALL_LENGTH+1; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
}

//清理水平线
void clearBoxHorLine(HANDLE hout, const int X, const int Y, const int bg_color){
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, bg_color);
		putchar(' ');
		putchar(' ');
	}
}

//清理垂直线
void clearBoxVerLine(HANDLE hout, const int X, const int Y, const int bg_color){
	for (int i = 0; i < SMALL_LENGTH+1; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, bg_color);
		putchar(' ');
	}
}

/* 在指定位置，用指定颜色，显示一个字符 */
void showch(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color)
{
	//背景色
	for (int y = 0; y < LENGTH + 1; y++){
		for (int x = 0; x < (LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, 15, 15);
			putchar(' ');
		}
	}
	//正方形上边
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
	//正方形下边
	for (int i = 0; i < LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y + LENGTH);
		setcolor(hout, bg_color, fg_color);
		putchar(horizontal[0]);
		putchar(horizontal[1]);
	}
	//正方形左边
	for (int i = 1; i < LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
	//正方形右边
	for (int i = 1; i < LENGTH; i++){
		gotoxy(hout, X + LENGTH * 2, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(vertical[0]);
		putchar(vertical[1]);
	}
}

void drawMoveBox(HANDLE hout, const int X, const int Y, const int bg_color, const int fg_color, int value){
	//背景色
	for (int y = 0; y < SMALL_LENGTH + 1; y++){
		for (int x = 0; x < (SMALL_LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, bg_color, bg_color);
			putchar(' ');
		}
	}
	//正方形上边
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//正方形下边
	for (int i = 0; i < SMALL_LENGTH + 1; i++){
		gotoxy(hout, X + i * 2, Y + SMALL_LENGTH);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//正方形左边
	for (int i = 1; i < SMALL_LENGTH; i++){
		gotoxy(hout, X, Y + i);
		setcolor(hout, bg_color, fg_color);
		putchar(' ');
		putchar(' ');
	}
	//正方形右边
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
//清空小方块
void clearMoveBox(HANDLE hout, const int X, const int Y, const int bg_color){
	//背景色
	for (int y = 0; y < SMALL_LENGTH + 1; y++){
		for (int x = 0; x < (SMALL_LENGTH + 1) * 2; x++){
			gotoxy(hout, X + x, y + Y);
			setcolor(hout, bg_color, bg_color);
			putchar(' ');
		}
	}
}
//判断是不是2的n次方
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

//判断是不是2的n次方
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
//画小方块
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
							//移动到指定位置
							int posY = j;
							int posX = i;
							while (posY > k){
								for (int m = posY * LENGTH + 2; m >(posY - 1) * LENGTH + 2; m--){
									clearBoxHorLine(hout, posX * LENGTH * 2 + 3, m + SMALL_LENGTH, 15);
									drawBoxHorLine(hout, posX * LENGTH * 2 + 3, m - 1, colors[log2N(array[j][i]) % 12], 0);
									Sleep(speed);
								}
								//将抹掉的边界补上
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
							//移动到指定位置
							int posY = j;
							int posX = i;
							while (posY < k){
								//移动到指定位置
								for (int m = posY * LENGTH + 2; m < (posY + 1) * LENGTH + 2; m++){
									clearBoxHorLine(hout, posX*LENGTH * 2 + 3, m, 15);
									drawBoxHorLine(hout, posX * LENGTH * 2 + 3, m + SMALL_LENGTH + 1, colors[log2N(array[j][i]) % 12], 0);
									Sleep(speed);
								}
								posY++;
								//将抹掉的边界补上
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
								//移动到指定位置
								for (int m = posX * LENGTH * 2 + 3; m >(posX - 1) * LENGTH * 2 + 3; m--){
									//clearMoveBox(hout, m, posY * LENGTH + 2, 15);
									//drawMoveBox(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
									clearBoxVerLine(hout, m + SMALL_LENGTH * 2 + 1, posY * LENGTH + 2, 15);
									drawBoxVerLine(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0);
									Sleep(speed);
								}
								//将抹掉的边界补上
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
								//移动到指定位置
								for (int m = posX * LENGTH * 2 + 3; m < (posX + 1) * LENGTH * 2 + 3; m++){
									//clearMoveBox(hout, m, posY * LENGTH + 2, 15);
									//drawMoveBox(hout, m + 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
									clearBoxVerLine(hout, m , posY * LENGTH + 2, 15);
									drawBoxVerLine(hout, m + SMALL_LENGTH * 2, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0);
									Sleep(speed);
								}
								posX++;
								//将抹掉的边界补上
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



//合并
bool merge(int rows, int columns, int order){
	bool flag = false;
	if (order == KEY_UP){
		for (int i = 0; i < columns; i++){
			for (int j = 0; j < rows - 1; j++){
				if (array[j][i] != 0 && array[j][i] == array[j + 1][i]){
					int posX = i;
					int posY = j + 1;
					//移动到指定位置
					for (int m = posY * LENGTH + 2; m >(posY - 1) * LENGTH + 2; m--){
						clearMoveBox(hout, posX * LENGTH * 2 + 3, m, 15);
						drawMoveBox(hout, posX * LENGTH * 2 + 3, m - 1, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
						Sleep(speed);
					}
					//将抹掉的边界补上
					drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					posY--;
					//乘以2
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
					//移动到指定位置
					for (int m = posY * LENGTH + 2; m < (posY + 1) * LENGTH + 2; m++){
						clearMoveBox(hout, posX * LENGTH * 2 + 3, m, 15);
						drawMoveBox(hout, posX * LENGTH * 2 + 3, m + 1, colors[log2N(array[j][i]) % 12], 0, array[j][i]);
						Sleep(speed);
					}
					posY++;
					//将抹掉的边界补上
					drawHorLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					//乘以2
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
					//移动到指定位置
					for (int m = posX * LENGTH * 2 + 3; m >(posX - 1) * LENGTH * 2 + 3; m--){
						clearMoveBox(hout, m, posY * LENGTH + 2, 15);
						drawMoveBox(hout, m - 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						Sleep(speed);
					}
					//将抹掉的边界补上
					drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					posX--;
					//乘以2
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
					//移动到指定位置
					for (int m = posX * LENGTH * 2 + 3; m < (posX + 1) * LENGTH * 2 + 3; m++){
						clearMoveBox(hout, m, posY * LENGTH + 2, 15);
						drawMoveBox(hout, m + 1, posY * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						Sleep(speed);
					}
					posX++;
					//将抹掉的边界补上
					drawVerLine(hout, posX * LENGTH * 2, posY * LENGTH + 1, 15, 0);
					//乘以2
					array[i][j + 1] *= 2;
					array[i][j] = 0;
					drawMoveBox(hout, posX * LENGTH * 2 + 3, posY * LENGTH + 2, colors[log2N(array[i][j + 1]) % 12], 0, array[i][j + 1]);
				}
			}
		}
	}
	return flag;
}


//随机生成2或4，概率分别为80%或20%
int rand2_4(){
	int value = rand() % 10;
	if (value < 8){
		return 2;
	}
	else{
		return 4;
	}
}

//判断当前table，能否继续合并
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
		cout << "请输入行[4..8]: ";
		cin >> rows;
	}
	while (columns < 4 || columns > 10){
		cout << "请输入列[4..10]: ";
		cin >> columns;
	}
	while (!is2Log(target)){
		cout << "请输入结束值[1024..65536]间2的整数次幂:";
		cin >> target;
	}
	speed = -1;
	while (speed < 0){
		cout << "请输入动画延时[0-5]" << endl;
		cin >> speed;
	}
	speed = speed * 100 / LENGTH;
	char cmd[80];
	sprintf(cmd, "mode con cols=%d lines=%d", (columns)* LENGTH * 2 + 2, (rows)*LENGTH + 3);
	system(cmd);
	system("cls"); //调用系统命令清除屏幕
	ifstream infile;
	infile.open("highest.txt", ios::in);
	int highest = 0;
	if (infile){
		infile >> highest;
		infile.close();
	}

	cout << "目标： " << target << " 分数  " << 0 << "  最高  " << highest << "  时间 " << "  0  ";
	cout << "<箭头键移动,Q退出>" << endl;
	//画布局
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
	//随机生成2个2
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
	//画目标方块
	int speed = 100;//移动速度0.1秒
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
		//找出当前表格的最大值，0的个数
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
						cout << "恭喜成功.........";
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
		cout << "目标： " << target << " 分数  " << largest << "  最高   " << highest << " 时间  " << (curTime - startTime) << "   ";
		cout << "<箭头键移动,Q退出>" << endl;
		//随机产生第pos个填新数据
		int pos = rand() % zeroTimes;
		int temp = 0;
		for (int i = 0; i < rows; i++){
			for (int j = 0; j < columns; j++){
				if (array[i][j] == 0){
					if (temp == pos){
						array[i][j] = rand2_4();
						drawMoveBox(hout, j * LENGTH * 2 + 3, i * LENGTH + 2, colors[log2N(array[i][j]) % 12], 0, array[i][j]);
						//结束生成新数据
						i = rows;
						j = columns;
					}
					temp++;
				}

			}
		}
		//如果0的位置只有1个，那么要检查是否还有移动的可能性
		if (zeroTimes == 1){
			if (!mergeAble(rows, columns, KEY_UP) && !mergeAble(rows, columns, KEY_DOWN) && !mergeAble(rows, columns, KEY_LEFT) && !mergeAble(rows, columns, KEY_RIGHT)){
				setcolor(hout, 0, 15);
				cout << endl;
				gotoxy(hout, 0, rows*LENGTH + 2);
				cout << "失败.........";
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