/* 
 * @copyright (c) 2020, ICT, Hanoi University of Technology
 * @author HoangThuy Ha
 * @version 1.0
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "StudentsList_course.h"
#include "Course.h"
#include "Student_course.h"

course cou;
studentList_course stuList_Cou;

#define MAXL_LINE 100
#define MAXL_FILENAME 20

void read(char filename[]) {
    FILE *file = fopen(filename, "r");
    if(file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    char buff[MAXL_LINE];
    char skip[MAXL_LINE];

    fgets(buff, sizeof(buff), file); buff[strlen(buff) - 1] = '\0';    
    sscanf(buff, "%[^|]|%[^\0]", skip, cou.subjectID);
    
    
    fgets(buff, sizeof(buff), file); buff[strlen(buff) - 1] = '\0';
    sscanf(buff, "%[^|]|%[^\0]", skip, cou.subject);

    fgets(buff, sizeof(buff), file); buff[strlen(buff) - 1] = '\0';
    sscanf(buff, "%[^|]|%lf|%lf", skip, &cou.fractionForMidterm, &cou.fractionForFinal);

    fgets(buff, sizeof(buff), file); buff[strlen(buff) - 1] = '\0';
    sscanf(buff, "%[^|]|%d", skip, &cou.semester);

    fgets(buff, sizeof(buff), file); buff[strlen(buff) - 1] = '\0';
    sscanf(buff, "%[^|]|%d", skip, &cou.studentCount);

    stuList_Cou = NULL;
    student_course a;
    for(int i = 0; i < cou.studentCount; i++) {
        if(fgets(buff, sizeof(buff), file) != NULL) {
            buff[strlen(buff) - 1] = '\0';
            sscanf(buff, "%[^|]|%d|%[^|]|%[^|]| %lf | %lf | %c |", skip, &a.MSSV, a.surname, a.name, &a.midtermScore, &a.finaltermScore, &a.totalScore_c);
            a.totalScore = cou.fractionForMidterm*a.midtermScore / 100
                          +cou.fractionForFinal*a.finaltermScore / 100;
            stuList_Cou = AVL_insert(stuList_Cou, a);
        }
        else {
            printf("\nWARNING: The number of student is not equal to StudentCount !!!\n");
            break;
        }
    }

    // print_student_course(AVL_minNode(stuList_Cou)->data);
    // print_student_course(AVL_maxNode(stuList_Cou)->data);
    fclose(file);
}

void fprint_studentList_course(FILE* file, studentList_course root) {
    if(root == NULL)
        return;
    fprint_studentList_course(file, root->left);
    student_course a = root->data;
    fprintf(file, "S|%d|%-20s|%-10s| %f | %f | %c |\n", a.MSSV, a.surname, a.name, a.midtermScore, 
                                                            a.finaltermScore, a.totalScore_c);
    fprint_studentList_course(file, root->right);
}
void write(char filename[]) {
    FILE* file = fopen(filename, "w");
    if(file == NULL) {
        printf("Error! Could not open file\n");
        exit(-1);
    }
    fprintf(file, "SubjectID|%s\n", cou.subjectID);
    fprintf(file, "Subject|%s\n", cou.subject);
    fprintf(file, "F|%lf|%lf\n", cou.fractionForMidterm, cou.fractionForFinal);
    fprintf(file, "Semester|%d\n", cou.semester);
    fprintf(file, "StudentCount|%d\n", cou.studentCount);
    
    fprint_studentList_course(file, stuList_Cou);
    fclose(file);
}

int menu() {
    printf("Learning Management System\n");
    printf("-------------------------------------\n");
    printf("\t1.\tAdd a new score board\n");
    printf("\t2.\tAdd score\n");
    printf("\t3.\tRemove score\n");
    printf("\t4.\tSearch score\n");
    printf("\t5.\tDisplay score board and score report\n");
    printf("Your choice (1-5, other to quit): ");
    int option;
    scanf("%d", &option);
    return option;
}

void AddNewScoreBoard() {
    printf("Enter the information below\n");
    printf("-----------------------------------\n");
    printf("SubjectID: ");
    fflush(stdin);
    scanf("%[^\n]", cou.subjectID);
    printf("Subject: ");
    fflush(stdin);
    scanf("%[^\n]", cou.subject);
    cou.subject[strlen(cou.subject) - 1] = '\0';
    printf("F: ");
    fflush(stdin);
    scanf("%lf %lf", &cou.fractionForMidterm, &cou.fractionForFinal);
    
    printf("Semester: ");
    fflush(stdin);
    scanf("%d", &cou.semester);
    
    printf("StudentCount: ");
    fflush(stdin);
    scanf("%d", &cou.studentCount);
    printf("-----------------------------------\n\n");

    printf("You've done. Here is the result\n");
    printf("-----------------------------------\n");
    printf("SubjectID|%s\n", cou.subjectID);
    printf("Subject|%s\n", cou.subject);
    printf("F|%lf|%lf\n", cou.fractionForMidterm, cou.fractionForFinal);
    printf("Semester|%d\n", cou.semester);
    printf("StudentCount|%d\n", cou.studentCount);
    printf("-----------------------------------\n\n");

    char filename[MAXL_FILENAME];
    char buff[MAXL_FILENAME];
    strcpy(filename, cou.subjectID);
    strcat(filename, "_");
    strcat(filename, itoa(cou.semester, buff, 10));
    strcat(filename, ".txt");
    printf("%s\n", filename);

    write(filename);
}

void AddScore() {
    char subjectID[MAXL_NAME];
    int semester;
    printf("Enter the information below\n");
    printf("-----------------------------------\n");
    printf("SubjectID: ");
    fflush(stdin);
    scanf("%[^\n]", subjectID);
    printf("Semester: ");
    fflush(stdin);
    scanf("%d", &semester);
    printf("-----------------------------------\n\n");
    
    char filename[MAXL_FILENAME];
    char buff[MAXL_FILENAME];
    strcpy(filename, subjectID);
    strcat(filename, "_");
    strcat(filename, itoa(semester, buff, 10));
    strcat(filename, ".txt");
    // printf("%s\n", filename);
    
    read(filename);

    student_course a;
    printf("Enter the information of a student\n");
    printf("-----------------------------------\n");
    printf("MSSV: ");
    fflush(stdin);
    scanf("%d", &a.MSSV);
    printf("Surname: ");
    fflush(stdin);
    scanf("%[^\n]", a.surname);
    printf("name: ");
    fflush(stdin);
    scanf("%s", a.name);
    printf("Midterm: ");
    fflush(stdin);
    scanf("%lf", &a.midtermScore);
    printf("Finalterm: ");
    fflush(stdin);
    scanf("%lf", &a.finaltermScore);
    printf("Total score: ");
    fflush(stdin);
    scanf("%c", &a.totalScore_c);
    printf("-----------------------------------\n\n");

    stuList_Cou = AVL_insert(stuList_Cou, a);

    write(filename);
}

studentList_course find_MSSV(studentList_course root, int MSSV) {
    if(root == NULL)
        return NULL;
    if(root->data.MSSV == MSSV)
        return root;

    studentList_course node = find_MSSV(root->left, MSSV);
    if(node == NULL)
        node =  find_MSSV(root->right, MSSV);
    
    return node;
}
void RemoveScore() {
    char subjectID[MAXL_NAME];
    int semester;
    printf("Enter the information below\n");
    printf("-----------------------------------\n");
    printf("SubjectID: ");
    fflush(stdin);
    scanf("%[^\n]", subjectID);
    printf("Semester: ");
    fflush(stdin);
    scanf("%d", &semester);
    printf("-----------------------------------\n\n");
    
    char filename[MAXL_FILENAME];
    char buff[MAXL_FILENAME];
    strcpy(filename, subjectID);
    strcat(filename, "_");
    strcat(filename, itoa(semester, buff, 10));
    strcat(filename, ".txt");
    // printf("%s\n", filename);
    
    read(filename);

    int MSSV;
    printf("Enter the information of a student\n");
    printf("-----------------------------------\n");
    printf("MSSV: ");
    fflush(stdin);
    scanf("%d", &MSSV);
    printf("-----------------------------------\n\n");

    studentList_course node = find_MSSV(stuList_Cou, MSSV);
    // print_student_course(data);
    if(node != NULL)
        stuList_Cou = AVL_deleteNode(stuList_Cou, node->data);
    
    write(filename);
    return;
}

void SearchScore() {
    char subjectID[MAXL_NAME];
    int semester;
    printf("Enter the information below\n");
    printf("-----------------------------------\n");
    printf("SubjectID: ");
    fflush(stdin);
    scanf("%[^\n]", subjectID);
    printf("Semester: ");
    fflush(stdin);
    scanf("%d", &semester);
    printf("-----------------------------------\n\n");
    
    char filename[MAXL_FILENAME];
    char buff[MAXL_FILENAME];
    strcpy(filename, subjectID);
    strcat(filename, "_");
    strcat(filename, itoa(semester, buff, 10));
    strcat(filename, ".txt");
    // printf("%s\n", filename);
    
    read(filename);

    int MSSV;
    printf("Enter the information of a student\n");
    printf("-----------------------------------\n");
    printf("MSSV: ");
    fflush(stdin);
    scanf("%d", &MSSV);
    printf("-----------------------------------\n\n");

    printf("The score:\n");
    
    studentList_course node = find_MSSV(stuList_Cou, MSSV);
    if(node != NULL)
        print_student_course(node->data);
    else    
        printf("\nWARNING: Student ID is not available\n");
}


void Display_studentList_course(studentList_course root) {
    if(root == NULL)
        return;
    Display_studentList_course(root->left);
    student_course a = root->data;
    printf("S|%d|%-20s|%-10s| %f | %f | %c |\n", a.MSSV, a.surname, a.name, a.midtermScore, 
                                                            a.finaltermScore, a.totalScore_c);
    Display_studentList_course(root->right);
}
void DisplayScoreBoard() {
    printf("SCORE BOARD\n");
    printf("-------------------------------------\n");
    printf("SubjectID|%s\n", cou.subjectID);
    printf("Subject|%s\n", cou.subject);
    printf("F|%lf|%lf\n", cou.fractionForMidterm, cou.fractionForFinal);
    printf("Semester|%d\n", cou.semester);
    printf("StudentCount|%d\n", cou.studentCount);
    
    Display_studentList_course(stuList_Cou);
    printf("-------------------------------------\n\n");
}
double average(studentList_course root) {
    if(root == NULL)
        return 0;
    return root->data.totalScore/cou.studentCount 
            + average(root->left) + average(root->right);
} 
int countScore(studentList_course root, char score) {
    if(root == NULL)
        return 0;
    return ((root->data.totalScore_c == score) ? 1 : 0)
            + countScore(root->left, score) + countScore(root->right, score);
}
void DisplayScoreReport() {
    student_course a;
    printf("SCORE REPORT\n");
    printf("-------------------------------------\n");
    a = AVL_maxNode(stuList_Cou)->data;
    printf("The student with the highest mark is: %s %s\n", a.surname, a.name);
    a = AVL_minNode(stuList_Cou)->data;
    printf("The student with the lowest mark is: %s %s\n", a.surname, a.name);
    printf("The average mark is: %lf\n", average(stuList_Cou));
    printf("\n");
    printf("A histogram of the subject IT4062 is: \n");
    printf("A:");
    for(int i = 0; i < countScore(stuList_Cou, 'A'); i++)
        printf("*"); printf("\n");
    printf("B:");
    for(int i = 0; i < countScore(stuList_Cou, 'B'); i++)
        printf("*"); printf("\n");
    printf("C:");
    for(int i = 0; i < countScore(stuList_Cou, 'C'); i++)
        printf("*"); printf("\n");
    printf("D:");
    for(int i = 0; i < countScore(stuList_Cou, 'D'); i++)
        printf("*"); printf("\n");
    printf("F:");
    for(int i = 0; i < countScore(stuList_Cou, 'F'); i++)
        printf("*"); printf("\n");
    printf("-------------------------------------\n\n");  
}
void DisplayScoreBoard_ScoreReport() {
    char subjectID[MAXL_NAME];
    int semester;
    printf("Enter the information below\n");
    printf("-----------------------------------\n");
    printf("SubjectID: ");
    fflush(stdin);
    scanf("%[^\n]", subjectID);
    printf("Semester: ");
    fflush(stdin);
    scanf("%d", &semester);
    printf("-----------------------------------\n\n");
    
    char filename[MAXL_FILENAME];
    char buff[MAXL_FILENAME];
    strcpy(filename, subjectID);
    strcat(filename, "_");
    strcat(filename, itoa(semester, buff, 10));
    strcat(filename, ".txt");
    // printf("%s\n", filename);

    read(filename);
    DisplayScoreBoard();
    DisplayScoreReport();
}

int main(int argc, char const * argv[]) {
    stuList_Cou = NULL;
    int option;
    do {
        system("pause");
        system("cls");
        option = menu();
        if(option < 1 || option > 5)
            break;
 
        printf("option = %d\n", option);
        system("pause");
        system("cls");
        switch (option) {
        case 1:
            AddNewScoreBoard();           
            break;
        case 2:
            AddScore();
            break;
        case 3:
            RemoveScore();
            break;
        case 4:
            SearchScore();
            break;
        case 5:
            DisplayScoreBoard_ScoreReport();
            break;
        default:
            break;
        }           
    } while(1);
    return 0;
}