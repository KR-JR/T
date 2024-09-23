import java.util.Scanner;

public class prj2_2 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        
        System.out.println("숫자 입력: ");
        int number = scanner.nextInt();
        int cal = (number / 10); // 10의 자리 확인
        int result = number - (cal * 10);
        
        System.out.println("결과: " + result);

        scanner.close();

    
    
    }
}
