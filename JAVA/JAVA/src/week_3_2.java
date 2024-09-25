import java.util.Scanner;

public class week_3_2 {
    public static void main(String[] args) {
        String course[] = {"객체지향", "프로그래밍", "알고리즘", "자료구조", "컴퓨터구조"};
        char score[] = {'A', 'B', 'B', 'C', 'D'};
        Scanner scanner = new Scanner(System.in);

        while (true) {
            System.out.print("강의 이름을 입력하세요. >>");
            String T = scanner.next().trim(); // 입력값의 공백 제거

            if (T.equals("종료")) {
                System.out.println("종료되었습니다.");
                break;
            }

            boolean found = false; // 입력된 과목을 찾았는지 확인하는 변수
            for (int i = 0; i < course.length; i++) {
                if (T.equals(course[i])) { 
                    System.out.println(course[i] + "의 학점은 " + score[i]);
                    found = true;
                    break;
                }
            }

            if (!found) {
                System.out.println("존재하지 않는 강의입니다.");
            }
        }
        scanner.close(); // Scanner 닫기
    }
}
