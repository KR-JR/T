import java.util.Scanner;

public class week_2_2 {
    public static void main(String[] args) {
        
        int A, B;
        Scanner sc = new Scanner(System.in);
        A = sc.nextInt();
        B = sc.nextInt();

        if(A > B) {
            System.out.println(">");
        }
        if(A < B) {
            System.out.println("<");
        }
        if(A == B) {
            System.out.println("==");
        }
        sc.close();
    }
}
