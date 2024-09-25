import java.util.Arrays;
import java.util.Scanner;

public class challenge_3 {
    public static void main(String[] args) {
        // 로또 당첨 번호 생성 (중복되지 않는 6개 숫자)
        int winningNumbers[] = new int[6];
        int count = 0;

        while (count < 6) {
            int randomNumber = (int) (Math.random() * 45) + 1; // 1부터 45까지의 무작위 숫자 생성
            boolean isDuplicate = false; // 중복확인 기본값

            // 중복 확인
            for (int i = 0; i < count; i++) {
                if (winningNumbers[i] == randomNumber) {
                    isDuplicate = true;
                    break;
                }
            }

            // 중복이 아니면 배열에 추가
            if (!isDuplicate) {
                winningNumbers[count] = randomNumber;
                count++;
            }
        }

        // 생성된 로또 번호 정렬
        Arrays.sort(winningNumbers);

        // 사용자로부터 중복되지 않는 로또 번호 입력 받기
        Scanner scanner = new Scanner(System.in);
        int userNumbers[] = new int[6]; // 사용자 숫자

        for (int i = 0; i < 6; i++){
            System.out.println("로또 번호를 입력하세요 (1부터 45 사이의 숫자, 중복 없이 : )");
            userNumbers[i] = scanner.nextInt();
        }
        Arrays.sort(userNumbers);
        System.out.println(Arrays.toString(userNumbers));
        
    }
}
