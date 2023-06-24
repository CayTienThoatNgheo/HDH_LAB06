#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include "FIFO.h"

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define RESET "\x1B[0m"
#define BOLD "\x1B[1m" 

void pause_program();
void input_program(int *nhap_hoac_random_input);
void random_arr(int so_phan_tu, char *mang_input);
void out_arr(int so_phan_tu, char *mang_input);
void input_pageframe(int *so_frame);
void choose_algorithm(int *chon_giai_thuat);
void init_matrix(int so_phan_tu, int so_frame, char(*mang_thay_trang)[*]);
void out_matrix(int so_phan_tu, int so_frame, char (*mang_thay_trang)[*]);
int mark_page_fault(int so_phan_tu, int so_frame, int *so_loi_trang, int chon_giai_thuat, char *mang_input, char (*mang_thay_trang)[*]);
void add_page(int so_phan_tu, int so_frame, int hang_can_thay, int cot_can_thay, char phan_tu_thay, char (*mang_thay_trang)[*]);
void output(int so_phan_tu, int so_frame, int so_loi_trang, char *mang_input, char (*mang_thay_trang)[*]);

int main(int argc, char* argv[])
{
    int input, numberOfPages, pageFrames, choose, pageErrors;
    char arr[100];
    char arr2[100][100];
    input_program(&input);

    pageErrors = 0;

    switch(input)
    {
        case 1:
            {
                numberOfPages = 11;
                char default_arr[] = {2, 1, 5, 2, 1 ,2, 6, 0, 0, 0, 7};
                memcpy(arr, default_arr, sizeof(default_arr));
                break;
            }

        default:
            {
                printf("Please input number of pages: ");
                scanf("%d", &numberOfPages);
                random_arr(numberOfPages, arr);
            }
    }

    input_pageframe(&pageFrames);
    choose_algorithm(&choose);
    
    init_matrix(pageFrames, numberOfPages, arr2);
    mark_page_fault(numberOfPages, pageFrames, &pageErrors, choose, arr, arr2);

    output(numberOfPages, pageFrames, pageErrors, arr, arr2);

    return 0;
}

void pause_program()
{
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    getchar();
}

void input_program(int *input)
{
    do
    {
        //system("clear");
        printf("%s--- Page Replacement algorithm ---%s\n",BOLD,RESET);
        printf("1. Default referenced sequence\n");
        printf("2. Manual input sequence\n");
        printf("You choose: ");
        scanf(" %d", input);

        if (*input < 1 || *input > 2)
        {
            printf("Invalid input. Please try again!");
            pause_program();
        }
    } while (*input < 1 || *input > 2);

}

void random_arr(int numberOfPages, char *arr)
{
    srand(time(NULL));
    for (int i = 0; i < numberOfPages; i++)
    {
        arr[i] = rand() % 10;
    }
}

void out_arr(int numberOfPages, char *arr)
{
    for (int i = 0; i < numberOfPages; i++)
    {
        printf("%d\t",arr[i]);
    }
    printf("\n");
}

void input_pageframe(int *pageframes)
{
    do
    {
        //system("clear");
        printf("%s--- Page Replacement algorithm ---%s\n",BOLD,RESET);
        printf("Input page frames: ");
        scanf(" %d", pageframes);

        if (*pageframes < 1)
        {
            printf("Invalid input. Please try again!");
            pause_program();
        }
    } while(*pageframes < 1);
}

void choose_algorithm(int *choose)
{
    do
    {
        //system("clear");
        printf("%s--- Page Replacement algorithm ---%s\n",BOLD,RESET);
        printf("1. FIFO algorithm\n");
        printf("2. OPT algorithm\n");
        printf("3. LRU algorithm\n");
        printf("You choose ");
        scanf(" %d", choose);

        if (*choose < 1 || *choose > 3)
        {
            printf("Invalid input. Please try again!");
            pause_program();
        }
    } while(*choose < 1 || *choose > 3);
}

void init_matrix(int numberOfPages, int pageFrames, char (*arr)[numberOfPages])
{
    for (int i = 0; i < numberOfPages; i++)
    {
        for (int j = 0; j < pageFrames + 1; j++)
        {
            arr[j][i] = ' ';
        }
        printf("\n");
    }
}

void out_matrix(int numberOfPages, int pageFrames, char (*arr)[numberOfPages])
{
    for (int i = 0; i < pageFrames + 1; i++)
    {
        for (int j = 0; j < numberOfPages; j++)
        {
            printf("%d\t", arr[i][j]);
        }
        printf("\n");
    }
}

int mark_page_fault(int numberOfPages, int pageFrames, int *pageErrors, int choose, char *arr, char (*arr2)[numberOfPages])
{
    int x = MIN(numberOfPages, pageFrames);
    for (int i = 0; i < x; i++)
    for (int j = 0; j < x; j++)
    {
        if(i == j)
        {
            arr2[i][i] = arr[i];
            arr2[pageFrames + 1][i] = '*';
            *pageErrors++;
        }
        if(j < i)
        {
            arr2[j][i] = arr2[j][i-1];
        }
    }

    int pos = -1;
    int do_change = 1;
    for (int i = x; i < numberOfPages; i++)
    {
        for (int j = 0; j < pageFrames; j++)
        {
    
            if (arr[i] == arr2[j][i])
                do_change = 0;
        }
        if (!do_change)
        {
            for (int j = 0; j < pageFrames; j++)
            {
                arr2[j][i] = arr2[j][i-1];
            }
            do_change = 1;
            continue;
        }
        switch (choose)
        {
        case 1:
        {
            fifo(&pos, pageFrames);
            add_page(numberOfPages, pageFrames, pos, i, arr[i], arr2);
            break;
        }
        case 2:
        {
            
            add_page(numberOfPages, pageFrames, pos, i, arr[i], arr2);
            break;
        }
        default:
        {
            
            add_page(numberOfPages, pageFrames, pos, i, arr[i], arr2);
            break;
        }
        }
        pageErrors++;
    }
}

void add_page(int numberOfPages, int pageFrames, int pos, int column, char arr, char (*arr2)[numberOfPages])
{
    for (int i = 0; i < pageFrames; i++)
    {
        arr2[i][column] = arr2[i][column - 1];
    }
    arr2[pos][column] = arr;
}

void output(int numberOfPages, int pageFrames, int pageErrors, char *arr, char (*arr2)[numberOfPages])
{
    //system("clear");
    printf("%s--- Page Replacement algorithm ---%s\n",BOLD,RESET);
    out_arr(numberOfPages, arr);
    out_matrix(numberOfPages, pageFrames, arr2);
    printf("Number of Page Fault: %d", pageErrors);
}