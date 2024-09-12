import java.util.Scanner;

public class week_2_3 {
    public static void main(String[] args) {
        double height, weight, bmi;

        Scanner scanner = new Scanner(System.in);
        System.out.printf("몸무게를 입략하세여.(kg) >> ");
        weight = scanner.nextDouble();

        System.out.printf("키를 입력하세요.(cm) >> ");
        height = scanner.nextDouble();

        bmi = weight / ((height / 100) * (height / 100));
        
        
        if(bmi < 19) {
            System.out.printf("BMI 지수 : %.2f, 판정결과 : 저체중", bmi);
        }
        else if(bmi <= 23) {
            System.out.printf("BMI 지수 : %.2f, 판정결과 : 정상", bmi);
        }
        else if(bmi <= 29) {
            System.out.printf("BMI 지수 : %.2f, 판정결과 : 과체중", bmi);
        }
        else if(bmi <= 30) {
            System.out.printf("BMI 지수 : %.2f, 판정결과 : 비만", bmi);
        }
        scanner.close();
    }    
}
