/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */

#ifndef __student_course_course_h__
#define __student_course_course_h__

#include <string.h>

#define MAXL_NAME 20

typedef struct _student_course {
    int MSSV;
    char surname[MAXL_NAME+1];
    char name[MAXL_NAME+1];
    double midtermScore;
    double finaltermScore;
    char totalScore_c;
    double totalScore; // total score in form of number
} student_course;

student_course new_student_course(int MSSV, char sur[], char name[], double mid, double fi, double total) {
    student_course newS; 

    newS.MSSV = MSSV;
    strcpy(newS.surname, sur);
    strcpy(newS.name, name);
    newS.midtermScore = mid;
    newS.finaltermScore = fi;
    newS.totalScore = total; 

    return newS;
} 

char convertScoreFromNumberToChar(double totalScore) {
    if(totalScore >= 8.5) return 'A';
    if(totalScore >= 7.0) return 'B';
    if(totalScore >= 5.5) return 'C';
    if(totalScore >= 4.0) return 'D';
    return 'F';
}

void print_student_course(student_course a) {
    printf("S|%d|%-20s|%-10s| %f | %f | %c=%lf |\n", a.MSSV, a.surname, a.name, a.midtermScore, a.finaltermScore,
                                                convertScoreFromNumberToChar(a.totalScore), a.totalScore);
}

int MSSVCmp(student_course a, student_course b) {
    /*
    compare MSSV of 2 student_courses: a and b
    return -1 if a < b
    return 0 if a == b
    return +1 if a > b
    */
   if(a.MSSV < b.MSSV) return -1;
   if(a.MSSV > b.MSSV) return +1;
   return 0;
}

int scoreCmp(student_course a, student_course b) {
    /*
    compare the total score of 2 student_courses: a and b
    return -1 if a < b
    return 0 if a == b
    return +1 if a > b
    */
   if(a.totalScore < b.totalScore) return -1;
   if(a.totalScore > b.totalScore) return +1;
   return MSSVCmp(a, b);
}

int NameCmp(student_course a, student_course b) {
    /*
    compare name of 2 student_courses: a and b
    return -1 if a < b
    return 0 if a == b
    return +1 if a > b
    */
   return strcmp(a.name, b.name);
}

#endif