// enroll_s 구조체 선언
typedef struct enroll_s { 
    int id_num; // id_num 생성
    int course_num; // course_num 생성
    struct enroll_s *left; // 왼쪽(left) 주소
    struct enroll_s *right; //오른쪽(right) 주소
} *enroll_sp;

// 크기 지정
enroll_sp courses[35];
enroll_sp student[500];


