#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define INT_T 1
#define PTR_T 0
struct listIt {
	union {
		struct list *list;
		short item;
	} item;
	char what;
};

struct list {
	struct listIt arr[10];
	short end;
};

struct {
	struct list *arr[20];
	int end;
} stack, stack2;
#define PUSH(new) stack.arr[stack.end++] = new
#define POP stack.end--
#define TOP stack.end-1 >= 0? stack.arr[stack.end-1] : stack.arr[stack.end]

char flag, is_less;

struct {
	struct list *arr[500];
	short end;
} lists;

void list_less();
#define GREATER 0
#define LESS 1
#define EQUAL 2
#define COMPARE(x, y) flag = 1, is_less = EQUAL, list_less(x, y)

/*
PARTE 1:
13
5580
PARTE 2:
140
26200
*/
main(argc, argv)
char *argv[];
{
	struct list *get_list(), *create();
	void free_list(), quick_sort();
	struct list *l1, *l2, *aux;
	int total, ix;
	char c;
	
	if (argc < 2) {
		fprintf(stderr, "Informe a parte.\n");
		return 1;
	}
	total = lists.end = 0;
	ix = 1;
	for (;;) {
		if (*argv[1] == '1') {
			l1 = get_list(), l2 = get_list();
			COMPARE(l1, l2);
			if (is_less == LESS)
				total += ix;
			ix++;
			free_list(l1), free_list(l2);
		} else {
			lists.arr[lists.end++] = get_list();
			lists.arr[lists.end++] = get_list();
		}
		if ((c = getchar()) == EOF)
			break;
	}
	if (*argv[1] != '1') {
		l1 = (struct list *) malloc(sizeof(struct list));
		l2 = (struct list *) malloc(sizeof(struct list));
		l2->end = l1->end = 0;
		aux = lists.arr[lists.end++] = l1;
		l1 = create(l1);
		l1->arr[l1->end].what = INT_T;
		l1->arr[l1->end++].item.item = 2;
		l1 = aux;
		aux = lists.arr[lists.end++] = l2;
		l2 = create(l2);
		l2->arr[l2->end].what = INT_T;
		l2->arr[l2->end++].item.item = 6;
		l2 = aux;
		quick_sort(0, lists.end);
		total = (1 + binary(l1)) * (1 + binary(l2));
	}
	printf("%d\n", total);
}

struct list *get_list()
{
	struct list *create();
	struct list *curr;
	char buf[3], c;
	int ec;

	curr = NULL;
	stack.end = 0;
	while ((c = getchar()) != '\n') {
		beg:
		if (c == '[') {
			if (curr == NULL) {
				curr = (struct list *) malloc (sizeof(struct list));
				curr->end = 0;
			} else
				curr = create(curr);
			PUSH(curr);
		} else if (c == ']')
			POP, curr = TOP;
		else if (isdigit(c)) {
			ec = 0;
			buf[ec] = c;
			while(isdigit(c = getchar()))
				buf[++ec] = c;
			buf[++ec] = '\0';
			curr->arr[curr->end].what = INT_T;
			curr->arr[curr->end++].item.item = atoi(buf);
			goto beg;
		}
	}
	return TOP;
}

struct list *create(ptrl)
struct list *ptrl;
{
	ptrl->arr[ptrl->end].what = PTR_T;
	ptrl->arr[ptrl->end].item.list = (struct list *) malloc(sizeof(struct list));
	ptrl->arr[ptrl->end++].item.list->end = 0;
	return ptrl->arr[ptrl->end-1].item.list;
}

void convert(item)
struct listIt *item;
{
	int temp;

	temp = item->item.item;
	item->item.list = (struct list *) malloc(sizeof(struct list));
	item->what = PTR_T;
	item->item.list->arr[0].item.item = temp;
	item->item.list->arr[0].what = INT_T;
	item->item.list->end = 1;
}

void list_less(l1, l2)
struct list *l1, *l2;
{
	int i;

	for (i=0; i < l1->end; i++) {
		if (flag == 0)
			goto end;
		if (i == l2->end) {
			flag = 0;
			is_less = GREATER;
			goto end;
		}
		if (l1->arr[i].what == INT_T && l2->arr[i].what == INT_T) {
			if (l1->arr[i].item.item < l2->arr[i].item.item) {
				flag = 0;
				is_less = LESS;
				goto end;
			} else if (l1->arr[i].item.item > l2->arr[i].item.item) {
				flag = 0;
				is_less = GREATER;
				goto end;
			}
		} else if (l1->arr[i].what == PTR_T && l2->arr[i].what == PTR_T)
			list_less(l1->arr[i].item.list, l2->arr[i].item.list);
		else if (l1->arr[i].what == PTR_T) {
			convert(&l2->arr[i]);
			list_less(l1->arr[i].item.list, l2->arr[i].item.list);
		} else {
			convert(&l1->arr[i]);
			list_less(l1->arr[i].item.list, l2->arr[i].item.list);
		}
	}
	if (i < l2->end && flag) {
		flag = 0;
		is_less = LESS;
	}
	end:;
}

void quick_sort(begin, end)
{
        int i, j;
        struct list *pivot, *aux;

        i = begin;
        j = end - 1;
        pivot = lists.arr[(begin + end) / 2];
	while (i <= j) {
		for (; COMPARE(lists.arr[i], pivot), is_less == LESS && i < end; i++);
		for (; COMPARE(pivot, lists.arr[j]), is_less == LESS && j > begin; j--);
		if (i <= j) {
			aux = lists.arr[i];
			lists.arr[i++] = lists.arr[j];
			lists.arr[j--] = aux;
                }
        }
        if (j > begin)
                quick_sort(begin, j + 1);
        if (i < end)
                quick_sort(i, end);
}

binary(x)
struct list *x;
{
        int low = 0;
        int high = lists.end - 1;
        int mid;

        while (low <= high) {
                mid = (low + high) / 2;
		COMPARE(x, lists.arr[mid]);
                if (is_less == LESS)
                        high = mid - 1;
                else if (is_less == GREATER)
                        low = mid + 1;
                else
                        return mid;
        }
        return -1;
}

void free_list(curr)
struct list *curr;
{
	int i;

	for (i=0; i < curr->end; i++)
		if (curr->arr[i].what != INT_T) {
			free_list(curr->arr[i].item.list);
			free(curr->arr[i].item.list);
		}
}
