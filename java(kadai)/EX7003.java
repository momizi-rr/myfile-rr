public class EX7003 {
	   public static void main(String[] args){
            int d; //int型の変数を宣言
            
		      TurtleFrame f =  new TurtleFrame();  
		      Turtle m = new Turtle(); 
		      f.add(m); 
		      
              d=0;  //変数を初期化
		      for(int i = 0; i < 12; i++){   //三角形の１２枚の花を作る
		    	  d = d + Triangle(m);  //描いた線の合計を計算
		      }
		      System.out.println("道のりの長さ=" + d); //描いた線の合計を印字
		  }
	   
	   public static int Triangle(Turtle m){
			 m.fd((int)(Math.sqrt(3) * 100));  //平方根を計算して前に進む
			 int d = 0;  
         	 m.rt(150);  //150度右に曲がる
			 m.fd(100);  //１００前に進む
			 m.rt(60);  //６０度右に曲がる
			 m.fd(100);  //１００前に進む
			 m.rt(180); //１８０度右に曲がる
          return d + (int)(Math.sqrt(3) * 100) + 200; //描いた線に長さを返す
	   }
}

