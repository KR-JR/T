public class week3_1 {
    public static void main(String[] args) {
        double sum = 0;
        double num = 0;
        // for(int i = 0; i < 11; i ++) {
        //     sum += i;
        //     num = i;
        // }


        // double i = 0;
        // while(i < 11) {
        //     sum += i;
        //     num = i;
        //     i ++;
        // }

        double i = 0;
        do{
            sum += i;
            num = i;
            i ++;
        }
        while(i < 11);

        double result = sum / num;
        System.out.print(result);
    }
}
