import java.util.Scanner;
public class Ex2_3 {
    public static void main(String[] args) {
        final double rate = 1310.0;
        Scanner scanner = new Scanner(System.in);
        System.out.print("원화를 입력하세요(단위 원)>>");
        int won = scanner.nextInt();
        double dollor = won/rate;

        System.out.printf("%d원은 $%.2f입니다.", won, dollor);
        scanner.close();
    }
}
