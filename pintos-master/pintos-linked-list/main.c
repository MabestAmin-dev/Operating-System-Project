#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct student {
	char *name;
	struct list_elem elem;
};


void insert (struct list *student_list) {
	char *str = malloc(sizeof(char)*50);
	printf("Please input the name of the student:\n");
	scanf("%s", str);
	struct student *newstudent = malloc(sizeof(struct student));
	newstudent->name = str;
	list_push_back(student_list, &newstudent->elem);
}

void delete (struct list *student_list) {
	if(list_empty(student_list)){
		printf("List empty\n");
		return;
	}
	char *str = malloc(sizeof(char)*30);
	printf("Please input the name of the student you want to delete:\n");
	scanf("%s", str);
	struct list_elem * current = list_front(student_list);
	while (current != list_tail(student_list))
    {
		struct student *currentstudent = list_entry(current, struct student, elem);
		char *name = currentstudent->name;
		if(strcmp(str, name)== 0){
			list_remove(current);
			free(currentstudent->name);
			free(currentstudent);
			free(str);
			return;
		}
		current = list_next(current);
		
    }
	free(str);

}

void list (struct list *student_list) {
	if(list_empty(student_list)){
		printf("List empty\n");
		return;
	}
	struct list_elem * current = list_front(student_list);
	printf("Current list of students:\n");
	while(current != list_tail(student_list)){
		struct student *currentstudent = list_entry(current, struct student, elem);
		char *name = currentstudent->name;
		printf("%s\n", name);
		current = list_next(current);
	}
}

void quit (struct list *student_list) {
	if(list_empty(student_list)){
		printf("List empty\n");
		return;
	}
	struct list_elem *current = list_front(student_list);
	while(current != list_tail(student_list)){
		struct list_elem *next = list_remove(current);
		struct student *currentstudent = list_entry(current, struct student, elem);
		char *name = currentstudent->name;
		free(currentstudent->name);
		free(currentstudent);
		current = next;
	}
}

int main() {
	struct list student_list;
	list_init (&student_list);
	int opt;

	do {
		printf("Menu:\n");
		printf("1 - Insert student\n");
		printf("2 - Delete student\n");
		printf("3 - List students\n");
		printf("4 - Exit\n");
		scanf("%d", &opt);
		switch (opt) {
			case 1:
				{
					insert(&student_list);
					break;
				}
			case 2:
				{
					delete(&student_list);
					break;
				}
			case 3:
				{
					list(&student_list);
					break;
				}

			case 4:
				{
					quit(&student_list);
					return 0;
					break;
				}
			default:
				{
					printf("Quit? (1/0):\n");
					scanf("%d", &opt);
					if (opt)
						quit(&student_list);
					break;
				}
		}
	} while(1);
		
	return 0;
}
