public class week_3_1 {
    public static void main(String[] args) {
        int intArray[] = new int[5];
        int i = 0;
        while(true){
            intArray[i] = i+1;
            System.out.println("인덱스 " + i + ":" + intArray[i]);
            i ++;
            if(i == 5){
                break;
            }
        }

    }
}
