import java.util.Scanner;

public class Ex2_4{
    public static void main(String[] args) {
        int min;
        Scanner scanner = new Scanner(System.in);
        System.out.print("정수 3개를 입력하시오: ");
        int a = scanner.nextInt();
        int b = scanner.nextInt();
        int c = scanner.nextInt();

        min = a < b ? a : b;
        min = min < c ? min : c;
        
        System.out.println("가장 작은 수는 : " + min);
        scanner.close();
    }
}