#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define Course_count 8
typedef struct
{
    char Code[5];
    double grade;
    double credit;
}Course;

char** split_line(char *line) {
    char **splitted = malloc(6 * sizeof(char *));
    for (int i = 0; i < 6; i++) {
        splitted[i] = malloc(100 * sizeof(char));
    }

    char *token = strtok(line, ",");
    int i = 0;
    while (token != NULL && i < 6) {
        strcpy(splitted[i], token);
        i++;
        token = strtok(NULL, ",");
    }

    splitted[i] = '\0';
    return splitted;
}


int get_subject_number(char *code, char code_list[Course_count][5]){
    for(int i = 0; i < Course_count; i++){
        if(strncmp(code_list[i], code, strlen(code_list[i]) - 1) == 0)
            return i;
    }
    return -1;
}

void save_result(char *file_name,char code_list[Course_count][5] ,Course *student_grade, double sum_grade, double sum_credit){
    FILE* outFile = fopen(file_name,"w");

    fprintf(outFile,"Average Grade from Transcript.csv\n");
    fprintf(outFile,"Course,Credit,Average Grade\n");
    for(int i = 0; i < Course_count;i++){
        fprintf(outFile,"%s",code_list[i]);
        fprintf(outFile,",%.2lf,%.2lf\n", student_grade[i].grade/student_grade[i].credit,student_grade[i].credit);
    }
    fprintf(outFile,"GPAX");
    fprintf(outFile,",%.2lf,%.2lf",sum_credit,sum_grade/sum_credit );
}

int main(void){
    Course student_grade[Course_count];
    char Code[Course_count][5] = {"MTH", "SCI", "TH", "SOC", "ART", "PE", "ENG", "GEN"};

    //initialize
    for(int i = 0; i < Course_count; i++){
        strcpy(student_grade[i].Code, Code[i]);
        student_grade[i].grade = 0;
        student_grade[i].credit = 0;
    }


    FILE* infile;
    infile = fopen("Transcript.csv", "r");

    if(infile == NULL){
        perror("Error opening Input file\n");
        return 1;
    }

    char line[100]; 
    fgets(line, sizeof(line), infile);

    while (fgets(line,sizeof(line),infile))
    {
        char **token;
        int code_num;
        
        //normal line
        if(line[0] == ',' && line[1] != ','){
            token = split_line(line);
            code_num = get_subject_number(token[0], Code);
            student_grade[code_num].credit += atof(token[2]);
            student_grade[code_num].grade += atof(token[3]) * atof(token[2]);
        }
        //Grade_term line handle
        else if(line[0] != ','){
            token = split_line(line);
            code_num = get_subject_number(token[1], Code);

            student_grade[code_num].credit += atof(token[3]);
            student_grade[code_num].grade += atof(token[4]) * atof(token[3]);
        }
        //empty line && Column name line handle
        else{
            continue;
        }
    }

    
    double sum_grade = 0;
    double sum_credit = 0;
    for(int i = 0; i < Course_count;i++){
        printf("Course: %s\n", Code[i]);
        printf("Avg Grade: %.2lf Credit: %.2lf\n\n", student_grade[i].grade/student_grade[i].credit, student_grade[i].credit);
        sum_grade += student_grade[i].grade;
        sum_credit += student_grade[i].credit;
    }

    printf("GPAX: %.2lf Credit:%.2lf",sum_grade/sum_credit, sum_credit);
    
    save_result("Output.csv",Code,student_grade,sum_grade,sum_credit);

    return 0;
}
