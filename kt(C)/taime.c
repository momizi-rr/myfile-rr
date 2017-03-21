#include <stdio.h>

int main(void)
{
  FILE *fp;   //ファイルポインタ宣言
  char *fname = "遠征.txt"; //読み込むファイル名
  int c,a1;
//ファイルオープン
  fp = fopen( fname, "r" );  
  if( fp == NULL ){
    printf( "ファイルが開けません", fname ); //エラーの場合
    return -1;
  }
//ファイルの読み込み
  while( (c = fgetc( fp )) != EOF ){   
    printf( "%c", c );//テキストを表示
  }
  printf("海域の数字を入力してください。\n");
  scanf("%d",&a1);  //入力された値をa1に入力
	
  switch(a1)   //入力された数字の海域名表示
	{
	case 1:
		printf("鎮守府海域");
		break;
	case 2:
		printf("南西諸島海域");
		break;
	case 3:
		printf("北方海域");
		break;
	case 4:
		printf("西方海域");
		break;
	case 5:
		printf("南方海域");
		break;
	default:
		printf("１から５を選択してください。");
		break;
	}
	getchar(); //ファイル実行後閉じるのを抑制
	getchar();
  fclose( fp );
  return 0;
}