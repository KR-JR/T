import java.util.Scanner;

public class week_2_9 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        while (true) {
            System.out.print("숫자를 입력하세요: ");
            int number = scanner.nextInt();
            
            if (number == 10) {
                System.out.println("프로그램을 종료합니다.");
                break;  // 반복문 종료
            }

            System.out.println("입력한 숫자: " + number);
        }

        scanner.close();
    }
}
