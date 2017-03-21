/****************************************
       人狼アプリの準備 (jinro_junbi05.c)
       役割の決定(入れ替えを使う)
 ****************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> /*1文字の処理*/
#include <time.h>

#define LenName 32 /*半角で32文字、全角で16文字*/
#define PLAYER_LIM 20 /*Max20人までプレイ可能*/
#define PLAYER_MIN 4  /*最低4人以上でプレイ*/

struct playerInfo{
    char name[(LenName+4)];/*プレーヤー名、日本語なら16文字まで(本当は18文字まで入るけど)*/
    int role;
    /*=========================================*
	     https://chouseisan.com/l/post-24068/
      1:村人、2:人狼、3:占い師、4:霊能者、
      5:多重人格、6:狩人、7：共有、8：妖狐、0:空き
      http://matome.naver.jp/odai/2140222753778443501
      サイコキラー、パパラッチ、コスプレイヤー
      猫又、を入れたいなら
      自分で設定する
     *=========================================*/
    int state;
    /*=========================================*
      1:生存、2:吊、3:噛、0:空き
     *=========================================*/
};

/*グローバル変数としてplayersを宣言すれば全ての関数で利用可能*/
struct playerInfo players[PLAYER_LIM];
int pNum;
char gameTitle[]="人狼ゲーム";
char roleName[][10]={"空き","村人","人狼","占い師","霊能者","多重人格","狩人","共有","妖狐"};
FILE *fp;   /*オリジナル*/


/* ==========shift-jis 全角文字判定 ====================*/
int isWBSpace(char* tmp, int i){
	/* 参考URL
	   http://mzs184.blogspot.jp/2007/08/shift-jis.html
	   全角・半角の判定（シフトJIS）
	   http://dencha.ojaru.jp/programs_sub/zenkaku.html
	 */
	if (((char)0x81 <= tmp[i] && tmp[i] <= (char)0x9F) ||
((char)0xE0 <= tmp[i] && tmp[i] <= (char)0xFF)) {
		if ((tmp[i] == (char)0x81) && (tmp[i+1] == (char)0x40)) {
			/* 全角スペースの時に「1」と返り値を返す */
			return 1;
		}else{
			/* 全角文字だが全角スペースではない */
			return 0;
		}
	}else{
		/* 全角文字ではない */
		return 0;
	}
}

int countJinro(void){
	int i,ans;
	ans = 0;
	for(i=0; i<pNum; i++){
		if (players[i].role==2 && players[i].state==1){
			ans++;
		}
	}
	return ans;
}

void roleArrange(void){
	int i,j,k,tmp;
	char buf[5];
	/* 後でシャッフル */
	srand(time(NULL));
	/* https://chouseisan.com/l/post-24068/ */
	players[0].role=2;/* 人狼1名分（人狼は小さい方から入れておく） */
	players[pNum-1].role=3;/* 占い師 */
	for(i=1;i<(pNum-1);i++){
		/*それ以外はとりあえず村人*/
		players[i].role=1;
	}
	for(i=4;i<7;i++){
		if (pNum >= i+5){
			players[pNum-i+2].role=i;/* 霊能者、多重、狩人 */
		}
	}
	if (pNum >= 13){
		/* 共有2名 */
		players[pNum-5].role=7;
		players[pNum-6].role=7;
		if (pNum >= 14){
			/* 妖狐1名 */
			players[pNum-7].role=8;
		}
	}
	/* 人数調整 */
	if (pNum == PLAYER_MIN){
		printf("\nプレーヤー数が%d名ですが、",PLAYER_MIN);
		printf("占い師を入れますか？\n(入れるなら「y」と入力して下さい。)");
		fgets(buf,2,stdin);
		if (buf[0] != 'y' && buf[0] != 'Y'){ /* ★★ */
			if (isWBSpace(buf,0)){
				players[pNum-1].role=1;/* 占い師が不要なので市民に戻す */
			}
		}
		fflush(stdin);
	}else if (pNum == 8){
		printf("\nプレーヤー数が%d名ですが、",pNum);
		printf("人狼は1名です。\n");
		printf("1名人狼を増やしますか？");
		fgets(buf,2,stdin);
		if (buf[0] == 'y' || buf[0] == 'Y'){ /* ★★ */
			players[1].role=2;/* 人狼を1名増やす */
		}
		fflush(stdin);
	}else if (pNum > 8){
		players[1].role=2;/* 人狼2名 */
		if (pNum > 15){
			players[2].role=2;/* 人狼3名 */
		}
	}
	/* ここでシャッフル（iとjのプレーヤーの役割を入れ替える）*/
	for (k=0;k<200;k++){
		i=rand()%pNum;
		do{
			j=rand()%pNum;
		}while(i == j);
		tmp=players[i].role;
		players[i].role=players[j].role;
		players[j].role=tmp;
	}
}

/* ===========プレーヤー名の入力受け付け関数================= */
void inputPName(void)
{
	int i,j,k,flg_name;
    char buf[(LenName+4)];
    printf("まずは");
    do{
		printf("プレーヤーの人数を入力して下さい。:");
		fgets(buf,3,stdin);/*一応2桁まで、なので(2+1)で3*/
		pNum=atoi(buf);
		fflush(stdin);
		if (pNum < PLAYER_MIN){
			printf("プレーヤーは%d人以上必要です。\n",PLAYER_MIN);
		}else if(pNum > PLAYER_LIM){
			printf("このゲームでプレーできるのは%d人以下です。\n",PLAYER_LIM);
		}
	}while (pNum < PLAYER_MIN || pNum > PLAYER_LIM);

	printf("\n\n");
	for(i=0;i<pNum;i++){
		flg_name = 0;
		do{
			printf("%d人目のプレーヤーの名前を入力して下さい。:",i+1);
			fgets(buf,(LenName+3),stdin);
			/* 終端の'\0'直前に改行コードが入っていた場合の処理 */
			if (buf[strlen(buf)-1] == '\n' || buf[strlen(buf)-1] == (char)0x0A){
				buf[strlen(buf)-1] = '\0';/* 'a','b','c','\0','\0' は文字列としては"abc" */
			}
			/*名前の文字数のチェック*/
			if (strlen(buf) > (LenName+1)){
				printf("名前は全角で16文字以内で入力して下さい。\n");
				flg_name = 1;
			}else if (strlen(buf) == 0){
				printf("名前が入力されていません。\n");
				flg_name = 1;
			}else{
				flg_name = 0;
			}
			
			/*名前の先頭にスペースがあったら削る*/
			if (flg_name == 0){
				j = 0;
				while( isspace( buf[j] ) || isWBSpace( buf , j )){
				    if (isWBSpace( buf, j )){
				    	j+=2;
    				}else{
    					j++;
    				}
				}
			    /* 前方の空白を詰める */
			    if( j > 0 ) {
			        k = 0;
			        while( buf[j] != '\0'){
			        	if (isWBSpace( buf, j )){
				        	buf[k] = buf[j];
					    	j+=2;/*全角つまり2バイト分進める*/
					    	k+=2;/*全角つまり2バイト分進める*/
    					}else{
				        	buf[k] = buf[j];
					    	j++;/*半角つまり1バイト分進める*/
					    	k++;/*半角つまり1バイト分進める*/
    					}
			        }
        			buf[k] = '\0';
			    	
			    	/* 全部スペースだったらkが0になっているはず */
        			if (k == 0){
						printf("スペースだけの名前は受け付けません。\n");
						flg_name = 1;
					}else{
						printf("先頭のスペース部分を削り「%s」にました。\n",buf);
					}
			    }
			}

			/*pNum以下の数字だけの名前はIDと被ってややこしいのでチェック*/
			if (flg_name == 0){
				/* 文字が半角で2バイト以下で1つ目も2つ目も数字かどうか判断 */
				if(strlen(buf) < 3 && isdigit(buf[0]) != 0  && (isdigit(buf[1]) != 0 || buf[1] == '\0')){
					/* pNum以下か判断 */
					if (atoi(buf) >= 0 && atoi(buf) <= pNum){
						printf("%d以下の数字だけを名前とすることはできません。\n",pNum);
						flg_name = 1;
					}
				}
			}
			
			/*名前の重複のチェック*/
			if (flg_name == 0 && i > 0){
				for (j=0;j<i;j++){
					if (strcmp(buf,players[j].name)==0){
						printf("名前が重複しています。\n");
						flg_name = 1;
					}
				}
			}
			
			/*名前の文字を1人ずつ登録*/
			strcpy(players[i].name,buf); /*文字列の入力はこのようにstrcpy関数を使う*/
			fflush(stdin);
		}while (flg_name);
	}
}

/* ----- プレーヤーの情報を画面に表示する関数 */
void display(int pattern)
{
	int i;
	switch (pattern){
		case 0:
			system("cls"); /*画面をクリア*/
			for (i=0;i<30;i++){
				printf("#");
			}
			printf("\n%sのスタートです\n",gameTitle);
			for (i=0;i<30;i++){
				printf("#");
			}
			printf("\n\n");
			break;
		case 1:
			system("cls");
			printf("それでは\n");
		    /*プレーヤー名の表示*/
    		for (i=0;i<pNum;i++){
    			printf("%d:%s\n",i+1,players[i].name);
		    }
			printf("の計%d名でゲームを始めます。\n",pNum);
			break;
		default:
			printf("\n");
			break;
	}
}

/* ----- Game Masterにプレーヤーの情報を画面に表示する関数 */
void gmModeDisplay(int pattern)
{
	char buf[5];
	int i;
	/* gmであることの確認 */
	system("cls");
	printf("あなたはGMですか？");
	fgets(buf,2,stdin);
	if (buf[0] == 'y' || buf[0] == 'Y'){ /* ★★ */
		switch (pattern){
			case 0:
			    fp = fopen("yaku.txt","w");    /*オリジナル*/
				system("cls");
				printf("役職リスト\n");
				for(i=0;i<pNum;i++){
				    fprintf(fp,"player%2d:%sさんは%s\n",(i+1),players[i].name,roleName[players[i].role]); /*オリジナル*/
					printf("player%2d:%sさんは%s\n",(i+1),players[i].name,roleName[players[i].role]);       
				}
				printf("\n\n");
				break;
			case 1:
				break;
			default:
				printf("\n");
				break;
		}
	 }
	  printf("役職はメモ帳（yaku.txt）にも記録されています。\n");   /*オリジナル*/
   }

/* ----- main 関数 */
int main(void)
{
	display(0);
    inputPName();
    display(1);
    roleArrange();
    gmModeDisplay(0);
    fclose(fp);   /*オリジナル*/
    return (0);
}