public class week_2_10 {
    public static void main(String[] args) {
        for (int i = 1; i < 10; i++) { 
            for (int l = 9; l > 0; l--) { 
                System.out.print(l + " X " + i + " = " + (l * i) + "\t");
            }
            System.out.println(); 
        }
    }
}
