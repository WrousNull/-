#include<stdio.h>
#include<Windows.h>
#include<conio.h>
#include<stdlib.h>
//�����Ŀ⡪������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
#define HIGH 20                                 //����߶�
#define WIDE 35                                 //������
#define UP 72                                   //���������
#define DOWN 80                                 //���������
#define LEFT 75                                 //���������
#define RIGHT 77                                //���������
#define SPACE 32                                //�ո��
#define ESC 27                                  //Esc��
#define EnemyNum 5                              //С����
//����ĺꡪ������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
struct Face                                     //����һ�����ڴ�С���ǵĶ�ά���飬�洢ÿһ����λ��С����״̬
{
    int data[HIGH][WIDE];                       //���޷���      0�޷��飬1��ͼ���飬2��ҷ��飬3Boss���飬4boss��Ļ��5��ҵ�Ļ, 6С��~
    int color[HIGH][WIDE];                      //������ɫ
}face;     

struct BOSS{
    int x,y;
    int HP;
    int Form;
}Boss,enemy[EnemyNum];

struct PLAYER
{
    int x,y;
    int HP;
    int BOMB;
}Player;

struct GARDEN
{
    int x,y;
    int HP;
    int Shape[2][2];
};

//����Ľṹ���������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������� 
struct GARDEN Garden[4];                    //BOSS��4��ʹħ
int IsPrime;
char NameOfSpellCard[30];                   //��ǰ��������
int DanmuMove;                              //���ڿ��Ƶ�ĻƵ��
int MOVE = 1;                               //Ƶ�ʱ���
int Std_Speed = 5;
int Std_Time = 5;
int enemy_max = 100000000;
int enemy_num = 0;
int Form1_Time = 0;
int Score=0;
//ȫ�ֱ�����������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������������
void enemy_update()
{
	static int enemy_speed = 0;
	static int enemy_time = 0;
	int flag = 0;
	if (enemy_speed < Std_Speed){				//����ѭ��������
		enemy_speed++;
    }
	if (enemy_time < Std_Time){
		enemy_time++;
    }
	if (enemy_num < enemy_max && enemy_time >= Std_Time){
		int i, j;
		do
		{
			i = rand() % (HIGH / 5) + 2;
			j = rand() % (WIDE - 4) + 2;		//j�ķ�Χ:[2, WIDE - 3]
		} while (face.data[i][j] != 0);
		face.data[i][j] = 6;
		enemy_num++;
		enemy_time = 0;
	}

	if (enemy_speed >= Std_Speed){
	
		flag = 1;
		enemy_speed = 0;
	}

	for (int i = HIGH - 1; i >= 0; i--){
		for (int j = WIDE - 1; j >= 0; j--){
			if (face.data[i][j] == 6){			//���˴���С��
				if (i == HIGH - 1){			//С�ַɵ��߽�	
					enemy_num--;
					face.data[i][j] = 0;
				}
				else if (i < HIGH - 1 && face.data[i + 1][j] == 5){//����Ƿ��ӵ�����
                    Form1_Time++;
                    Score++;
					printf("\a");
					enemy_num--;
					face.data[i][j] = 0;
				}else if (flag){					//flagΪ1����С��λ��
					face.data[i + 1][j] = 6;
					face.data[i][j] = 0;
				}else if( i == Player.y && j == Player.x ){             //С��ײ���Ի�
                    printf("\a");                       //Biu~����
                    Player.HP--;                        //�Ի��л�-1
                    face.data[i][j] = 0;                //С����ʧ
                    Player.x = WIDE/2;                  //�Ի���λ
                    Player.y = HIGH - 5;
                    face.data[Player.y][Player.x] = 2;
                }
			}
		}
	}
}
void HideCursor()                               //���ع�꣬������Windos.h��
{
	CONSOLE_CURSOR_INFO curInfo;                //��������Ϣ�Ľṹ�����
	curInfo.dwSize = 1;                         //���û��ֵ�Ļ������ع����Ч
	curInfo.bVisible = FALSE;                   //���������Ϊ���ɼ�
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE); //��ȡ����̨���
	SetConsoleCursorInfo(handle, &curInfo);     //���ù����Ϣ
}                            
void CursorJump(int x, int y)                   //�����ת���ں�����������Windos.h��
{
	COORD pos;
	pos.X = x;
	pos.Y = y;
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(handle, pos);
}                    
void color(int c)                               //��ɫ����
{
	switch (c)
	{
	case 0:
		c = 13; //��ɫ
		break;
	case 1:
	case 2:
		c = 12; //��ɫ
		break;
	case 3:
	case 4:
		c = 10; //��ɫ
		break;
	case 5:
		c = 14; //��ɫ
		break;
	case 6:
		c = 11; //ǳ��ɫ
		break;
	default:
		c = 7; //��ɫ
		break;
	}
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), c); //��ɫ����
	//ע��SetConsoleTextAttribute��һ��API��Ӧ�ó����̽ӿڣ�
}
void makeMap()                                  //��ͼ����
{
    system("cls");                                          //�����Ļ
    for( int i = 1 ; i<=HIGH ; i++ ){
        for( int j =1  ; j<=WIDE ; j++ ){
            if( i == 1 || j == 1 || j == WIDE || i == HIGH ){
                face.data[i][j] = 1;
                CursorJump(j,i);
                printf("#");
            }
        }
    }                                                                       //��ӡ�Ի�
}
void Menue()
{
    for( int i = 1 ; i<=HIGH ; i++ ){
        for( int j =1  ; j<=WIDE ; j++ ){
            if( i == 1 || j == 1 || j == WIDE || i == HIGH ){
                face.data[i][j] = 1;
                CursorJump(j,i);
                printf("#");
            }else if( i == 7 && j == 7 ){
                CursorJump(j,i);
                printf("�|����Ļ��Ц");
            }else if( i == HIGH - 15 && j == 4 ){
                CursorJump(j,i);
                printf("��Start(S)");
            }else if( i == HIGH - 13 && j == 4 ){
                CursorJump(j,i);
                printf("��Quit(Q)");
            }
        }
    }
    while(!IsPrime){
        char isPrime;
        isPrime = getch();
        if( isPrime == 's'|| isPrime == 'S' ){
            IsPrime = 1;
            makeMap();
        }else if( isPrime == 'q' || isPrime == 'Q' ){
            exit(0);
        }
    }
}
void BossForm()                                  //Boss�׶�
{
    if( Form1_Time == 5 ){
        //�׶�2;
        //Boss.Form = 2;
    }/*else if( ʹħ�������� ){
        //�׶�3;
        Boss.Form = 3;
        
    }*/
}
void DataFirst()                                //���ݳ�ʼ��
{
    DanmuMove = 20;                            //Խ��ʹħ��ĻԽ��
    

    Player.x = WIDE/2;
    Player.y = HIGH - 5;
    Player.HP = 9;
    Player.BOMB = 3;
    face.data[Player.y][Player.x] = 2;

    Boss.x = NULL;
    Boss.y = NULL;
    Boss.HP = 114;
    Boss.Form = 1;
    //face.data[Boss.y][Boss.x] = 3;

    for( int i = 0 ; i<4 ; i++ ){
        for( int j = 0 ; j<2 ; j++ ){
            for( int k = 0 ; k<2 ; k++ ){
                Garden[i].Shape[j][k] = 1;
            }
        }
    }
}
void DrawEnemy(int k,int x,int y)
{
    CursorJump(x,y);
    for ( int i = 0 ; i < 2 ; i++ ){
        for ( int j = 0 ; j < 2 ; j++ ){
            if ( Garden[k].Shape[i][j] == 1 ){ //�����λ���з���
                CursorJump(x + j, y + i); //�����ת��ָ��λ��
                printf("��"); //�������
            }
        }
    }
}
void BossAttack(int n)
{
    
    switch (n)
    {
    
    case 1:    
                                       //�׶�1������С�֣�BOSS������
        enemy_update();
        break;
    case 2:                                              //BOSS�ڶ��׶Σ� �ٻ�4��ʹħ�������䵯Ļ��

        Garden[1].x = 5;                                                     //����ʹħλ��
        Garden[2].x = 8;
        Garden[3].x = 15;
        Garden[4].x = 23;
        for( int i = 0 ; i<4 ; i++ ){
            Garden[i].y = 4;
        }
        for( int k = 0 ; k<4 ; k++ ){
            face.data[Garden[k].y][Garden[k].x] = 7;
            DrawEnemy(k,Garden[k].x,Garden[k].y);                                   //����ʹħ
        }
        if( MOVE % DanmuMove == 0 ){
            for( int i = 0 ; i<4 ; i++ ){
                face.data[Garden[i].y+3][Garden[i].x] = 4;                          //ʹħ����
            }
        }
        break;
    case 3:

        break;
    default:

        break;
    }
}
void NOinteractUpdate()
{
    if( Score == 1000 ) {
    	system("cls");
        CursorJump(WIDE/2-7,HIGH/2);
        printf("STAGE CLEAR");
        getchar();
        exit(0);
	}

    if( MOVE < 10000000){                       //MOVEѭ������
        MOVE++;
    }else{
        MOVE = 1;
    }

    BossForm();    
    
    if( Boss.HP == 0 ){
        face.data[Boss.y][Boss.x] = 0;
        Boss.x = NULL;
        Boss.y = NULL;
        system("cls");
        CursorJump(15,15);
        printf("STAGE CLEAR");
        getchar();
        exit(0);
    }
    if( Player.HP <= 0 ){
        system("cls");
        CursorJump(WIDE/2-7,HIGH/2);
        printf("�� �� �� ��");
        CursorJump(WIDE/2-9,HIGH/2+3);
        printf("������յ÷�Ϊ:%d",Score);
        getchar();
        exit(0);
    }
    //���µ�Ļ
    for( int i = 0 ; i < HIGH ; i++ ){
        for( int j = 0 ; j < WIDE ; j++ ){              
            if( face.data[i][j] == 4 ){                 //���˴���ʹħ��Ļ
                if( i == Player.y && j == Player.x ){   
                    printf("\a");                       //Biu~����
                    Player.HP--;                        //�Ի��л�-1
                    face.data[i][j] = 2;                //��Ļ��ʧ
                    Player.x = WIDE/2;                  //�Ի���λ
                    Player.y = HIGH - 5;
                }else if( i < HIGH  && MOVE % DanmuMove == 0 ){  //ʹħ��Ļ����
                    face.data[i][j] = 0;
                    face.data[i+1][j] = 4;
                }else{                                  //ʹħ��Ļ������ʧ
                    face.data[i][j] = 0;
                }
            }else if( face.data[i][j] == 5 ){           //���˴����Ի���Ļ
                if( face.data[i-1][j] == 6 ){           //������С��
                    face.data[i-1][j] = 0;              //С����ʧ
                    face.data[i][j] = 0;                //�ӵ���ʧ
                }
                if( i == Boss.y && j == Boss.x ){       //������Boss
                    Boss.HP--;                          //BossѪ��-1
                    face.data[i][j] = 3;                //��Ļ��ʧ
                }else if( i>2 ){
                    face.data[i][j] = 0;
                    face.data[i-1][j] = 5;
                }else{
                    face.data[i][j] = 0;
                }
            }
        }
    }
}
void YESinteractUpdate()
{
    char input;
	if (_kbhit()) // �ж��Ƿ�������
	{
		input = _getch(); // �����û��Ĳ�ͬ�������ƶ�����������س�
		if (input == LEFT && Player.x > 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.x--; // λ������
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == RIGHT && Player.x < WIDE - 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.x++; // λ������
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == UP && Player.y > 2 )
		{
			face.data[Player.y][Player.x] = 0;
			Player.y--; // λ������
			face.data[Player.y][Player.x] = 1;
		}
		else if (input == DOWN )
		{
            if( Player.y != HIGH - 1){
			face.data[Player.y][Player.x] = 0;
			Player.y++; // λ������
			face.data[Player.y][Player.x] = 1;
            }
		}
		else if (input == 'z' || input == 'Z') // �����ӵ�
		{
			face.data[Player.y-1][Player.x] = 5;

		}else if (input == 'x' || input == 'X'){
            if( Player.BOMB > 0 ){
                Player.BOMB--;
                for( int i = 0 ; i<HIGH ; i++ ){
                    for( int j = 0 ; j<WIDE ; j++ ){
                        if( face.data[i][j] == 6 ){
                            face.data[i][j] = 0;
                        }
                    }
                }
            }
        }
	}
}
void Show()
{
    for( int i = 0 ; i<HIGH ; i++ ){
        for( int j = 0 ; j<WIDE ; j++ ){
            for( int k = 0 ; k<4 ; k++ ){
                if( i == Garden[k].y && j == Garden[k].x ){
                    CursorJump(j,i);
                    printf("%%");
                }
            }
            if( face.data[i][j] == 0 ){             //���ÿո�
            	CursorJump(j,i); 
                printf(" ");
            }else if(  i == Player.y && j == Player.x ){      //ˢ���Ի�λ��
            	CursorJump(j,i);
                printf("��");
            }else if( i == Boss.y && j == Boss.x ){       //ˢ��BOSSλ��
            	CursorJump(j,i);
                
                color(Boss.Form);
                printf("$");
            }else if( face.data[i][j] == 4 && DanmuMove % 100 == 0 ){       //ˢ��ʹħ��Ļ
            	CursorJump(j,i);
                printf("��");
            }else if( face.data[i][j] == 5 ){       //ˢ���Ի���Ļ
            	CursorJump(j,i);
                printf("|");
            }else if( face.data[i][j] == 6 ){           //��ʾС��
                CursorJump(j,i);
                printf("*");
            }
            if( j == WIDE - 7 && i == 2 ){         //��ʾUI
            for( int k = 0 ; k<6 ; k++ ){
                face.data[2][WIDE-7+k] = 7;
            }
            CursorJump(j,i);
            printf("�л�:%d",Player.HP);
            //����Ӳл���
            }else if( j == WIDE - 7 && i == 3 ){   //��ʾUI
                for( int k = 0 ; k<6 ; k++ ){
                    face.data[3][WIDE-7+k] = 7;
                }
                CursorJump(j,i);
                printf("BOMB:%d",Player.BOMB);
                //�����BOMB��
            }else if( j == 3 && i == 3 ){
                for( int k = 0 ; k<15 ; k++ ){
                    face.data[4][3+k] = 7;
                }
                for( int k = 0 ; k<11 ; k++ ){
                    face.data[3][3+k] = 7;
                }
                CursorJump(j,i);
                printf("��ǰ����:\n # %s",NameOfSpellCard);
                //����ӷ�������
            }
        }
    }
}

int main()
{
    #pragma warning (disable:4996);             //��������
    system("mode con lines=22 cols=37");        //����cmd���ڵĴ�С
    HideCursor();                               //���ع��
    Menue();                                    //��ӡ�˵�
    DataFirst();                                //��ʼ������
    while(IsPrime){
        Show();
        BossAttack(Boss.Form);
        NOinteractUpdate();
        YESinteractUpdate();
    }
    return 0;
}

    // Boss.x = WIDE/2;
    // Boss.y = 10;
    // Boss.HP = 114;
    // Boss.Form = 1;
    // face.data[Boss.y][Boss.x] = 3;
    


















