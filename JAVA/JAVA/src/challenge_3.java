import java.util.Arrays;
import java.util.Scanner;

public class challenge_3 {
    public static void main(String[] args) {
        // 복권 당첨 번호 생성 (중복되지 않는 6개 숫자)
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

        // 생성된 복권 번호 정렬
        Arrays.sort(winningNumbers);

        // Debug
        // System.out.println(Arrays.toString(winningNumbers));



        // 사용자로부터 중복되지 않는 복권 번호 입력 받기
        Scanner scanner = new Scanner(System.in);
        int[] userNumbers = new int[6];
        int i = 0;

        while (i < 6) {
            try {
                System.out.print("복권 번호를 입력하세요 (1부터 45 사이의 숫자, 중복 없이) : ");
                
                // 숫자인지 확인하기 위해 nextInt() 대신 next()로 먼저 입력받고 확인
                String input = scanner.next();
                
                // 입력값이 숫자인지 확인
                int num = Integer.parseInt(input);

                // 숫자가 1~45 범위를 벗어나면 예외 발생
                if (num < 1 || num > 45) {
                    throw new Exception("잘못된 숫자를 입력하셨습니다."); // 예외 발생
                }

                // 중복 검사
                boolean isDuplicate = false;
                for (int j = 0; j < i; j++) {
                    if (userNumbers[j] == num) {
                        isDuplicate = true;
                        break;
                    }
                }

                // 올바른 입력값 처리
                if (isDuplicate) {
                    System.out.println("이미 입력한 숫자입니다.");
                } else {
                    userNumbers[i] = num;
                    i++; // 올바른 입력이었을 때만 i 증가
                }

            } catch (Exception e) {
                System.out.println(e.getMessage()); // 잘못된 입력일 경우 예외 메시지 출력
            }
        }

        // 복권 숫자 출력
        Arrays.sort(userNumbers);
        System.out.println("입력하신 복권 숫자 : " + Arrays.toString(userNumbers));
        System.out.println("랜덤한 복권 숫자 : " + Arrays.toString(winningNumbers));
    
        // 일치 결과 확인 및 결과 출력
        if(Arrays.toString(winningNumbers) == Arrays.toString(userNumbers)) {
            System.out.println("복권과 입력하신 숫자가 일치합니다");
        }
        else{
            System.out.println("복권과 입력하신 숫자가 일치하지 않습니다");
        }


        scanner.close();
    }
}
