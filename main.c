// 구조체 맴버들의 크기
#define MAX_NAME        20
#define MAX_PHONE       20
#define MAX_ADDRESS     100

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct book_t {
    struct book_t *next;
    unsigned long index;        // 항목의 index
    char name[MAX_NAME];        // 이름
    char phone[MAX_PHONE];      // 전화번호
    char address[MAX_ADDRESS];  // 주소
};

struct book_t *head, *tail;
unsigned long num_books;                     // 주소록 항목의 수
unsigned long last_index;                    // 주소록 항목의 마지막 index
const char *fnbook = "addressbook.dat";      // 데이터를 저장시 파일 이름

/* 사용자 인터페이스 함수 */
void menu_main();                       // 주메뉴 출력
void menu_add();                        // 항목 추가 메뉴
void menu_search();                     // 항목 검사 메뉴
/* 주소록 관리 함수 */
void init_book();                       // linked list 초기화
void clear_book();                      // linked list 메모리 해제
int add_book(struct book_t data);       // 항목 추가
int delete_book(int index);             // 항목 삭제
void view_all_book();                   // 전체 주소록 출력
void view_book(struct book_t *data);    // 하나의 항목 출력
int read_book();                        // 파일에서 데이터 읽기
int write_book();                       // 파일에 데이터 저장
int search_name(char *name);            // 이름 검색
int search_phone(char *phone);          // 전화번호 검색
int search_address(char *address);      // 주소 검색

int main()
{
    menu_main();
    return 0;
}

void menu_main()
{
    int choice;
    while (1) {
        printf("Choose Task\n");
        printf("1. Add address\n");
        printf("2. View all address book\n");
        printf("3. Search\n");
        printf("4. Save Current data\n");
        printf("5. Quit\n\n");
        printf("Input: ");
        scanf("%d", &choice);
        getchar();                                 // 입력 버퍼 \n 삭제

        switch(choice) {
            case 1:
                menu_add();
                break;
            case 2:
                view_all_book();
                break;
            case 3:
                menu_search();
                break;
            case 4:
                write_book();
                break;
            case 5:
                return;
            default:
                break;                
        }
    }
}

void menu_add()
{
    struct book_t data;
    data.index = last_index + 1;
    // 사용자에게 입력을 받을 때까지 루프를 돔
    do {
        printf("Input new data\n");
        printf("1. Name: ");
        fgets(data.name, MAX_NAME, stdin);
        printf("2. Phone Nunber: ");
        fgets(data.phone, MAX_PHONE, stdin);
        printf("3. Address: ");
        fgets(data.address, MAX_ADDRESS, stdin);
    } while(strlen(data.name) <= 0 || strlen(data.phone) <= 0 || strlen(data.address) <= 0);

    // linked list에 새로운 데이터 추가
    if (add_book(data)) {
        printf("Adding Data Success!\n");
        last_index++;
        num_books++;
    }
    else {
        printf("Adding Data Fail!\n");
    }
}

void menu_search()
{
    int choice;
    char buffer[MAX_ADDRESS];
    int result;

    while (1)
    {
        do {
            printf("What are you looking for?\n");
            printf("1. Name\n");
            printf("2. Phone Number\n");
            printf("3. Address\n");
            printf("4. Quit\n");
            printf("Input: ");
            scanf("%d", &choice);
            getchar();
        } while(choice < 1 || choice > 4);

        switch(choice) {
            case 1:
                printf("What name are you looking for?\n");
                printf("Input: ");
                fgets(buffer, MAX_ADDRESS, stdin);
                result = search_name(buffer);
                break;
            case 2:
                printf("What phone number are you looking for?\n");
                printf("Input: ");
                fgets(buffer, MAX_ADDRESS, stdin);
                result = search_phone(buffer);
                break;
            case 3:
                printf("What address are you looking for?\n");
                printf("Input: ");
                fgets(buffer, MAX_ADDRESS, stdin);
                result = search_address(buffer);
                break;
            case 4:
                return;
            default:
                break;
            
            /* 찾은 결과가 존재하면 항목 수를 출력 */
            if (result)
                printf("Found %d results.\n\n", result);
            else
                printf("Not Found!\n\n");
        }
    }
    
}

void init_book()
{
    num_books = 0;
    last_index = 0;
    clear_book();
}

void clear_book()
{
    struct book_t *book_i;

    while(head != NULL) {
        book_i = head;
        head = head->next;
        free(book_i);
    }
    head = NULL;
    tail = NULL;
}

int add_book(struct book_t data)
{
    struct book_t *book_i;

    if (data.index == 0)
        return 0;

    if ((book_i = (struct book_t *) malloc (sizeof(struct book_t))) == NULL) {
        printf("Not enough memory!\n");
        return 0;
    }

    // 새로운 노드에 값 저장
    book_i->index = data.index;
    strcpy(book_i->name, data.name);
    strcpy(book_i->phone, data.phone);
    strcpy(book_i->address, data.address);
    book_i->next = NULL;

    // 현재 리스트가 비어있을 경우
    if (tail == NULL) {
        head = tail = book_i;
    }
    else {
        tail->next = book_i;
        tail = book_i;
    }
    return 1;
}

int delete_book(int index)
{
    return 1;
}

void view_all_book()
{
    struct book_t *book_i;

    if (head == NULL) {
        printf("No Data!\n");
        return;
    }

    book_i = head;
    
    while (book_i != NULL) {
        view_book(book_i);
        book_i = book_i->next;
    }

    printf("Total Addresses: %ld\n\n", num_books);
}

void view_book(struct book_t *data)
{
    printf("1. Name: %s\n", data->name);
    printf("2. Phone: %s\n", data->phone);
    printf("3. Address: %s\n", data->address);
    printf("Press any key.\n");
    getchar();
    printf("\n\n");
}

int read_book()
{
    struct book_t data;
    FILE *fp;

    if ((fp = fopen(fnbook, "fb")) == NULL) {
        printf("Data Reading Failed!\n");
        return 0;
    }

    while (!feof(fp)) {
        data.index = 0;

        if (fread(&data, sizeof(struct book_t), 1, fp)) {
            add_book(data);
            num_books++;
        }
    }

    fclose(fp);
    last_index = data.index;
    
    return 1;
}

int write_book()
{
    struct book_t *book_i;
    FILE *fp;

    if ((fp = fopen(fnbook, "wb")) == NULL) {
        printf("Writing Data Failed!\n");
        return 0;
    }

    book_i = head;
    while (book_i != NULL) {
        fwrite(book_i, sizeof(struct book_t), 1, fp);
        book_i = book_i->next;
    }

    fclose(fp);

    printf("Writing Data Success!\n\n");

    return 1;
}

int search_name(char *name)    // 이름 검색
{
    struct book_t *book_i;
    int num = 0;
    
    book_i = head;
    
    while (book_i != NULL) {
        if (strstr(book_i->name, name)) {
            view_book(book_i);
            num++;
        }
        book_i = book_i->next;
    }
    
    return num;
}

int search_phone(char *phone)
{
    struct book_t *book_i;
    int num = 0;
    
    book_i = head;
    
    while(book_i != NULL) {
        if (strstr(book_i->phone, phone)) {
            view_book(book_i);
            num++;
        }
        book_i = book_i->next;
    }
    
    return num;
}

int search_address(char *address)
{
    struct book_t *book_i;
    int num = 0;
    
    book_i = head;
    
    while(book_i != NULL) {
        if (strstr(book_i->address, address)) {
            view_book(book_i);
            num++;
        }
        book_i = book_i->next;
    }
    return num;
}