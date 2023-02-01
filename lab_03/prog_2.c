#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <stdint.h>

#define MAX_TURN_LEN 1000
#define T1_START 0
#define T1_END 4
#define T2_START 1
#define T2_END 7

#define OK 0
#define ERROR_READING 1
#define ERROR_COMAND 2
#define ERROR_MEMORY 3
#define ERROR_NUM 4
#define ERROR_OVERFLOW 5
#define ERROR_TURN 6
#define EMPTY_TURN 7

#define TURN_LEN 100
#define REQUEST_LEN 1000
#define P_2 25

typedef struct
{
    double time;
    int type;
    int value;
}elem_t;

typedef struct
{
    elem_t *arr;
    elem_t *head;
    elem_t *tail;
    int size;
    int max_size;
} array_turn_t;

typedef struct node_turn
{
    elem_t data;
    struct node_turn *next;
} node_turn_t;

typedef struct
{
    node_turn_t *head;
    node_turn_t *tail;
    size_t size;
} list_turn_t;

typedef struct
{
    node_turn_t **arr;
    size_t size;
} arr_free_areas_t;


int push_turn_list(list_turn_t *turn, elem_t el, size_t max_size)
{
    if (turn->size >= max_size)
        return ERROR_OVERFLOW;
    node_turn_t *new;
    new = calloc(1, sizeof(node_turn_t));
    //printf("cringe");
    if (new == NULL)
        return ERROR_MEMORY;
    new->data = el;
    new->next = NULL;

    if (turn->head == NULL)
    {
        turn->head = new;
        turn->tail = turn->head;
    }
    else
    {
        turn->tail->next = new;
        turn->tail = new;
    }
    turn->size++;

    return OK;
}

int pop_turn_list(list_turn_t *turn, elem_t *el)
{
    if (turn->size == 0)
        return EMPTY_TURN;
    *el = turn->head->data;
    node_turn_t *next;
    next = turn->head->next;
    free(turn->head);
    turn->head = next;
    turn->size--;
    return OK;
}

elem_t create_task(int type)
{
    elem_t el;
    el.type = type;
    if (type == 1)
        el.time = (T1_END - T1_START) *(double) rand()/RAND_MAX + T1_START;
    else if (type == 2)
        el.time = (T2_END - T2_START) * (double) rand()/ RAND_MAX + T2_START;
    return el;
}

void fill_turn_first(list_turn_t *turn)
{
    elem_t el;
    for (int i = 0; i < TURN_LEN; i++)
    {
        el = create_task(1);
        push_turn_list(turn, el, TURN_LEN);
    }
}


void push_el_bcs_probility(list_turn_t *first, list_turn_t *second)
{
    int p = rand() % 100;
    elem_t el;
    if (p <= P_2)
    {
        el = create_task(2);
        push_turn_list(second, el, TURN_LEN);
    }
    else if (p > P_2)
    {
        el = create_task(1);
        push_turn_list(first, el, TURN_LEN);
    }
}


void check_hundred_task(int oa_2_out, list_turn_t *t_1, list_turn_t *t_2, int avglen_1, int avglen_2, int oa_1_out)
{
    if (oa_2_out % 100 == 0)
    {
        int p = rand() % 100;
        int f = 0, s = 0;
        if (p < P_2)
            s = 1;
        else
            f = 1;
    }
}


int main(void)
{
    list_turn_t turn_1;
    list_turn_t turn_2;

    turn_1.size = 0;
    turn_1.head = NULL;
    turn_1.tail = NULL;
    fill_turn_first(&turn_1);

    turn_2.size = 0;
    turn_2.head = NULL;
    turn_2.tail = NULL;

    int oa_2_out = 0, oa_1_out = 0;

    double oa_1_time = 0, oa_1_el_time = 0, oa_1_hold_time = 0;
    double oa_2_time = 0, oa_2_hold_time = 0, oa_2_el_time = 0;
    int avg_len_1 = 0, avg_len_2 = 0;

    elem_t el_turn_1, el_turn_2;

    while (oa_2_out < REQUEST_LEN)
    {
        avg_len_1 += turn_1.size;

        if (turn_1.size == 0 && turn_2.size != 0)
        {
            pop_turn_list(&turn_2, &el_turn_2);
            oa_2_out++;
            oa_2_time += el_turn_2.time;
            oa_2_el_time = el_turn_2.time;
            oa_1_hold_time += el_turn_2.time;
            push_el_bcs_probility(&turn_1, &turn_2);
            continue;
        }
        else if (turn_1.size)
        {
            pop_turn_list(&turn_1, &el_turn_1);
            oa_1_out++;
            oa_1_time += el_turn_1.time;
            oa_1_el_time = el_turn_1.time;
        }

        if (turn_2.size == 0)
        {
            oa_2_hold_time += el_turn_1.time;
            push_el_bcs_probility(&turn_1, &turn_2);
            continue;
        }
        oa_2_el_time = turn_2.head->data.time;

        if (oa_1_el_time >= oa_2_el_time)
        {
            while (oa_1_el_time > 0)
            {
                if (oa_1_el_time >= oa_2_el_time)
                {
                    oa_1_el_time -= oa_2_el_time;
                    avg_len_2 += turn_2.size;
                    oa_2_time += turn_2.head->data.time;

                    pop_turn_list(&turn_2, &el_turn_2);
                    oa_2_out++;

                    elem_t el = create_task(1);
                    push_turn_list(&turn_1, el, TURN_LEN);
                    check_hundred_task(oa_2_out, &turn_1, &turn_2, avg_len_1, avg_len_2, oa_1_out);

                    if (turn_2.size == 0)
                    {
                        oa_2_hold_time += oa_1_el_time;
                        push_el_bcs_probility(&turn_1, &turn_2);
                        break;
                    }
                    oa_2_el_time = turn_2.head->data.time;
                }
                else
                {
                    oa_2_time += oa_1_el_time;
                    turn_2.head->data.time -= oa_1_el_time;
                    push_el_bcs_probility(&turn_1, &turn_2);
                    break;
                }
            }
        }
        else if (oa_1_el_time < oa_2_el_time)
        {
            oa_2_time += oa_1_el_time;
            turn_2.head->data.time -= oa_1_el_time;
            push_el_bcs_probility(&turn_1, &turn_2);
        }

    }
    double t_2 = ((double)(T2_END - T2_START) / 2 + T2_START) * 1000.0;
    double t_1 = ((double)(T1_END - T1_START) / 2 + T1_START) * (1000.0 * (100.0 / P_2));
    int  p = 1000.0 * (100.0 / P_2);
    return 0;
}
