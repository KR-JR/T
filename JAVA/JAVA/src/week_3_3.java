import java.util.Scanner;

public class week_3_3 {
    public static void main(String[] args) {
        int[] unit = {50000, 10000, 1000, 100, 10, 1}; // 환산할 돈의 종류
        Scanner scanner = new Scanner(System.in);
        System.out.print("금액을 입력하세요 >> ");
        int money = scanner.nextInt();

        for (int i = 0; i < unit.length; i++) {
            int count = money / unit[i]; // 해당 화폐의 개수 계산
            if (count > 0) {
                System.out.println(unit[i] + "원 짜리 : " + count + "개");
            }
            money = money % unit[i]; // 남은 금액 계산
        }
        scanner.close(); // Scanner 닫기
    }
}
