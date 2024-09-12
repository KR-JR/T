import java.util.Scanner;

public class week_2_8 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);

        System.out.print("1 ~ 10 사이: ");
        int number = scanner.nextInt();
        for(int i = 1; i < 11; i++) {
            if(i == number) {
                continue;
            }
            System.out.print(i + " ");

        }
        scanner.close();
    }    

}
