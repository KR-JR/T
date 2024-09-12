import java.util.Scanner;

public class week_2_5 {
    public static void main(String[] args) {
        Scanner scanner = new Scanner(System.in);
        System.out.print("연산식을 입력하세요 >>");
        int op1 = scanner.nextInt();
        String op = scanner.next();
        int op2 = scanner.nextInt();
        switch (op) {
            case "+":
                System.out.println(op1 + " + " + op2 + "의 계산 결과는 " + (op1 + op2));
                break;
            case "-":
                System.out.println(op1 + " - " + op2 + "의 계산 결과는 " + (op1 - op2));
                break;
            case "*":
                System.out.println(op1 + " * " + op2 + "의 계산 결과는 " + (op1 * op2));
                break;
            case "/":
                System.out.println(op1 + " / " + op2 + "의 계산 결과는 " + (op1 / op2));
                break;
        }
        scanner.close();
    }    
}
