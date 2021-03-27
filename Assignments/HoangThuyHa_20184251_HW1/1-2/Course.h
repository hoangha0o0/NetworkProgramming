/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */

#ifndef __Course_h__
#define __Course_h__

#include <string.h>

#define MAXL_NAME 20
#define STD 100

typedef struct _course {
    char subjectID[MAXL_NAME+1];
    char subject[MAXL_NAME+1];
    double fractionForMidterm;
    double fractionForFinal;
    int semester;
    int studentCount;
} course;

course new_course(char subID[], char sub[], double f, int sem, int cnt) {
    course newC;

    strcpy(newC.subjectID, subID);
    strcpy(newC.subject, sub);
    newC.fractionForMidterm = f;
    newC.fractionForFinal = STD - f;
    newC.semester = sem;
    newC.studentCount = cnt;

    return newC;
}

#endif