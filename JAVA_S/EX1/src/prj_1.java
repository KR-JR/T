public class prj_1 {
    public static void main(String[] args) {
        int class_m = 27; // 학급 인원들
        int das = 12; // 1다스
        int pos = das * 5; // 색연필 개수
        int result_a; // 색연필 수
        int result_b; // 나머지
        result_a = pos / class_m;
        result_b = pos % class_m; // 결과 할당
    
        System.out.println("명당 나눠줄 수 있는 색연필 수: " + result_a);
        System.out.println("나머지: " + result_b); 
    }
}
