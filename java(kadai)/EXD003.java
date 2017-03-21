public class EXD003 {
   public static void main(String[] args)    {
      try{
            for(int i = 10; i >= 0; i--){
            Thread.sleep(1000); //1000ミリ秒停止
            if(i>0){
               System.out.println("あと"+i+"秒");
            }else{
               System.out.println("明けまして、おめでとうノシ");
            }
         }
         for(int i = 1; i <= 10; i++){   //経過表示
         Thread.sleep(1000);
         System.out.println(i+" 秒経過");
         }
      }catch(InterruptedException e){} 
   }
}
