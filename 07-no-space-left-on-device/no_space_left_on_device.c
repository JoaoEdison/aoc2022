#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

/*fazer com que os tamanhos sejam dinâmicos e a implementação eficiente*/

struct vetor {
	struct direc *arr[100];
	short end;
};

struct file {
	long size;
	char name[500];
};

struct vfile {
	struct file arr[100];
	short end;
};

struct direc {
	long total;
	char name[500];
	struct vetor dirs;
	struct vfile files;
} root;

long res, needed;
long that = LONG_MAX;

main()
{
	void makedir(), print_tree();
	struct direc *cd();
	long compute_size();
	char command[500], name[500], type[500];
	struct direc *curr;
	
	if (getchar() == EOF)
		return 0;
	
	scanf(" cd /\n$ ");
	root.total = 0;
	root.dirs.end = 1;
	root.dirs.arr[0] = NULL;
	strcpy(root.name, "/");
	curr = &root;
	while (scanf("%s", command) != EOF) {
		if (strcmp(command, "ls") == 0) {
			getchar();
			for (;;) {
				if (scanf("%s ", type) == EOF)
					goto end_main;
				if (strcmp(type, "$") == 0)
					goto end_it;
				scanf("%s\n", name);
				if (strcmp(type, "dir") == 0)
					makedir(name, curr);
				else {
					strcpy(curr->files.arr[curr->files.end].name, name);
					curr->total += curr->files.arr[curr->files.end++].size = atol(type);
				}
			}
		} else if (strcmp(command, "cd") == 0) {
			scanf(" %s\n", name);
			curr = cd(name, curr);
		}
		getchar(), getchar();
		end_it:;
	}
	end_main:
	needed = 30000000 - 70000000 + compute_size(&root);
	print_tree(&root, 0);
	printf("%ld\n%ld\n", res, that);
}

struct direc *cd(name, curr)
char name[];
struct direc *curr;
{
	int i;
	
	if (strcmp(name, "..") == 0)
		return *curr->dirs.arr;

	for (i=1; i < curr->dirs.end; i++)
		if (strcmp(curr->dirs.arr[i]->name, name) == 0)
			return curr->dirs.arr[i];
}

void makedir(name, curr)
char name[];
struct direc *curr;
{
	struct direc *ptrst;

	ptrst = (struct direc *) malloc(sizeof(struct direc));
	curr->dirs.arr[curr->dirs.end++] = ptrst;
	ptrst->total = 0;
	strcpy(ptrst->name, name);
	ptrst->dirs.arr[0] = curr;
	ptrst->dirs.end = 1;
	ptrst->files.end = 0;
}

long compute_size(curr)
struct direc *curr;
{
	int i;

	for (i=1; i < curr->dirs.end; i++)
		curr->total += compute_size(curr->dirs.arr[i]);
	return curr->total;
}


void print_tree(curr, n)
struct direc *curr;
{
	int i;
	/* char levels[5000]; */

	/* for (i=0; i < n; i++) */
	/* 	levels[i] = ' '; */
	/* levels[i] = '\0'; */

	/* printf("%s- %s (dir) |%ld|\n", levels, curr->name, curr->total); */
	
	/*parte 1:*/
	if (curr->total <= 100000)
		res += curr->total;
	/*parte 2:*/
	if (curr->total >= needed && curr->total < that)
		that = curr->total;

	/* for (i=n; i < n+3; i++) */
	/* 	levels[i] = ' '; */
	/* levels[i] = '\0'; */

	/* for (i=0; i < curr->files.end; i++) */
		/* printf("%s- %s (file, size=%ld)\n", levels, curr->files.arr[i].name, curr->files.arr[i].size); */

	for (i=1; i < curr->dirs.end; i++)
		print_tree(curr->dirs.arr[i], n+3);
}
