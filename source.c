#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define LOWL_OK 1
#define LOWL_FAIL 0
#define LOWL_SUCCESS 0
#define BOUNDARY_REACHED 1

typedef struct OWN{
    float data;
    struct OWN *next;
}OWN;

typedef struct{
    OWN *beg;
    OWN *cur;
}LOWL;

LOWL *lowl_create_empty(void)
{
    LOWL *l;

    if((l=malloc(sizeof(LOWL)))==NULL)
    {
        return NULL;
    }

    l->beg=NULL;
    l->cur=NULL;

    return l;
}

char lowl_cur_step_left(LOWL *list)
{
	OWN *prev;
	
	if(list->beg == list->cur)
    {
		return BOUNDARY_REACHED;        
    }
		
	prev = list->beg;
	
	while(prev != list->cur)
    {
		if(prev->next == list->cur)
        {
			list->cur = prev;
			return LOWL_SUCCESS;
		}
		prev = prev->next;	
	}
}

char lowl_cur_step_right(LOWL *list)
{
	if(list->cur->next != NULL)
    {
		list->cur = list->cur->next;
		return LOWL_SUCCESS;
	}
    else
    {
		return BOUNDARY_REACHED;        
    }
}

OWN *lowl_insert_right(LOWL* list, float val)
{	
	OWN *l;
	
	if((l = malloc(sizeof(OWN))) == NULL)
    {
		return NULL;        
    }
	
	l->data = val;
	
	if(list->beg == NULL && list->cur == NULL)
    {
        list->beg = l;
        list->cur = l;
		
		return list->cur;
	}
	
	l->next = list->cur->next;
	list->cur->next = l;
	
	return l;
}

LOWL *lowl_create_random(unsigned int size)
{
	int i;
    float val;
	LOWL *l;

	l = lowl_create_empty();
	
	for(i = 0; i < size; i++)
    {
        val = rand();
		l->cur = lowl_insert_right(l,val);
	}
	
	return l;
}

char lowl_delete(LOWL* list)
{
	OWN *l;
	
	if(list->cur == NULL)
    {
		return LOWL_FAIL;        
    }

	l = list->cur;
	
	if(l->next != NULL)
    {
		free(list->cur);
		list->cur = l->next;
		
		return LOWL_OK;
	}
	
	free(list->cur);
	
	lowl_cur_step_left(list);
	
	free(l);
	
	if(l != NULL)
    {
		return LOWL_FAIL;        
    }
	
	return LOWL_OK;
}

void lowl_destroy(LOWL *list)
{
	while(list->cur != NULL)
    {
		lowl_delete(list);
	}
	
	free(list);
}

void lowl_print(LOWL *list)
{
	OWN *c;

    if(list->cur==NULL)
    {
        printf("NULL\n");
    }
    else
    {
    	c = list->cur;
	    list->cur = list->beg;
	
	    do
        {
		    if(c == list->cur)
            {
			    printf("-> ");            
            }
		
		    printf("%f\n", list->cur->data);
		    lowl_cur_step_right(list);
	    }
        while(list->cur->next != NULL);

        if(c == list->cur && list->cur!=list->beg)
        {
		    printf("-> ");
    	    printf("%f\n", list->cur->data);           
        }
    }
}

LOWL *lowl_devide(LOWL *list)
{
    LOWL *listB;
    float val=0.0;

    listB = lowl_create_empty();

    //toto som si len vybral situaciu kedy je kurzor na zaciatku
    //(vybral som si kde chcem aby sa to delilo, pre testy)
    list->cur=list->beg;

    if(list->cur->next == NULL)
    {
        return listB;
    }
    do
    {
        val=list->cur->next->data;
		listB->cur = lowl_insert_right(listB,val);
        lowl_cur_step_right(list);
    }
    while(list->cur->next != NULL);

    lowl_cur_step_right(list);

    do
    {
        list->cur->next=NULL;
        lowl_cur_step_left(list);
    } while (list->cur->next->data!=listB->beg->data);

    list->cur->next=NULL;

    return listB;
}

void main()
{
	srand(time(0));
	
	LOWL *list;
	
	list = lowl_create_random(3);

    lowl_print(list);
	
	lowl_print(lowl_devide(list));

    lowl_print(list);
}