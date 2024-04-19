#include <stdio.h>
#include <stdlib.h> 
#include <string.h>

#define MAX_EXPRESSION_SIZE 100

typedef char element;
typedef struct Stacktype {
    element* data;
    int capacity;
    int top;
} StackType;

void init(StackType* sptr) {
    sptr->data = malloc(MAX_EXPRESSION_SIZE * sizeof(element));
    sptr->capacity = MAX_EXPRESSION_SIZE;
    sptr->top = -1;
}

void free_stack(StackType* sptr) {
    free(sptr->data);
}

int is_full(StackType* sptr) {
    return (sptr->top == (sptr->capacity - 1));
}

int is_empty(StackType* sptr) {
    return (sptr->top == -1);
}

void push(StackType* sptr, element item) {
    if (is_full(sptr)) {
        fprintf(stderr, "Stack is full\n");
        return;
    }
    sptr->data[++(sptr->top)] = item;
}

element pop(StackType* sptr) {
    if (is_empty(sptr)) {
        fprintf(stderr, "Stack is empty\n");
        return '\0';  
    }
    return sptr->data[(sptr->top)--];
}

element peek(StackType* sptr) {
    if (is_empty(sptr)) {
        fprintf(stderr, "Stack is empty\n");
        return '\0';  
    }
    return sptr->data[sptr->top];
}

int eval(char* exp) {
    int op1, op2, value, i = 0;
    int len = strlen(exp);
    char ch;
    StackType s;

    init(&s);
    for (i = 0; i < len; i++) {
        printf("expression: %c\n", exp[i]);
        ch = exp[i];
        if ((ch == '+') || (ch == '-')
            || (ch == '*') || (ch == '/')) {
            op1 = pop(&s);
            op2 = pop(&s);
            switch (ch) {
            case '+': push(&s, op2 + op1); break;
            case '-':push(&s, op2 - op1); break;
            case '*':push(&s, op2 * op1); break;
            case '/':push(&s, op2 / op1); break;
            default:;
            }
        }
        else if ((ch >= '0') && (ch <= '9')) {
            value = ch - '0';
            push(&s, value);
        }
    }
    return pop(&s);
}


int prec(char op) {
    switch (op) {
    case '(': case ')': return 0;
    case '+': case '-': return 1;
    case '*': case '/': return 2;
    default: return -1;
    }
}

void infix_to_postfix(char infix[], char postfix[]) {
    int i, j = 0;
    char ch, top_op;
    int len = strlen(infix);
    StackType s;

    init(&s);
    for (i = 0; i < len; i++) {
        ch = infix[i];
        switch (ch) {
        case '+': case '-': case '*': case '/':
            while (!is_empty(&s) && (prec(ch) <= prec(peek(&s)))) {
                postfix[j++] = pop(&s);
                
            }
            push(&s, ch);
            break;
        case '(':
            push(&s, ch);
            break;
        case ')':
            top_op = pop(&s);
            while (top_op != '(') {
                postfix[j++] = top_op;
               
                top_op = pop(&s);
            }
            break;
        default:
            do {
                postfix[j++] = ch;
                ch = infix[++i];
            } while (ch >= '0' && ch <= '9'); 
            i--;  
            break;
        }
    }
    while (!is_empty(&s)) {
        postfix[j++] = pop(&s);
    }
    postfix[j] = '\0';
    free_stack(&s);
}

int main() {
    char infixExpression[MAX_EXPRESSION_SIZE];
    char postfixExpression[MAX_EXPRESSION_SIZE];  
    int choice;
    int result;

    while (1) {
        printf("다음과 같은 메뉴로 동작하는 프로그램입니다:\n");
        printf("1. 중위식을 입력받음\n");
        printf("2. 중위식을 후위식으로 변환\n");
        printf("3. 후위식을 계산\n");
        printf("4. 종료\n");
        printf("메뉴를 선택하세요: ");

        scanf_s("%d", &choice);
        getchar(); 

        switch (choice) {
        case 1:
            printf("중위식을 입력하세요: ");
            fgets(infixExpression, sizeof(infixExpression), stdin);
            infixExpression[strcspn(infixExpression, "\n")] = 0; 
            printf("입력된 중위식: %s\n", infixExpression);
            break;
        case 2:
            infix_to_postfix(infixExpression, postfixExpression);
            printf("후위식 수식: %s\n", postfixExpression);
            break;
        case 3:
            result = eval(postfixExpression);  
            printf("계산 결과: %d\n", result);
            break;
        case 4:
            printf("프로그램을 종료합니다.\n");
            exit(0);
        default:
            printf("잘못된 메뉴 선택입니다. 다시 선택하세요.\n");
            break;
        }
    }
    return 0;
}