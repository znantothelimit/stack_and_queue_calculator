#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef double Element; // 스택의 요소 자료형 변환 용이하도록 Element 정의

typedef struct LinkedNode { // 연결리스트의 노드 선언
	Element data; // data영역(수식에 들어있는 피연산자 or 연산자 저장)
	struct LinkedNode* link; // 다음 요소를 가리키는 포인터 link 영역
}Node;

Node* top = NULL; // 스택 맨 위 헤드 포인터 선언(스택 위치) 및 NULL 초기화 (포인터이기 때문)
Node* front = NULL;
Node* rear = NULL;

void Qerror(char* str)
{
	fprintf(stderr, "%s\n", str);
	exit(1);
}

int is_Qempty() { return front == NULL; }

void init_queue() { front = rear = NULL; }

int Qsize()
{
	Node* p;
	int count = 0;
	for (p = front; p != NULL; p = p->link)
		count++;
	return count;
}

void enqueue(Element e)
{
	Node* p = (Node*)malloc(sizeof(Node));
	p->data = e;
	p->link = NULL;

	if (is_Qempty()) front = rear = p;
	else {

		rear->link = p;

		rear = p;

	}
}

Element dequeue()
{
	Node* p;
	Element e;

	if (is_Qempty())
		Qerror("큐 공백 에러\n");

	p = front;
	front = front->link;

	if (front == NULL) rear = NULL;
	e = p->data;
	free(p);

	return e;
}

Element Qpeek()
{
	if (is_Qempty())
		Qerror("큐 공백 에러\n");
	return front->data;
}

void destroy_queue()
{
	while (is_Qempty() == 0)
		dequeue();
}

int Qcheck_matching(char expr[])
{
	int i = 0, prev; // 함수의 인수 expr[]문자열의 요소 하나하나 읽기 위해 int형 변수 i=0 초기화
	char ch; // 요소 대입용 char 변수 ch선언

	init_queue();

	while (expr[i] != '\0') { // 함수의 인수 expr[]문자열의 요소 끝까지 읽을때까지 반복 
		ch = expr[i++]; // ch에 expr문자열 요소 대입 후 후증가로 expr문자열 요소 모두 읽기

		if (ch == '[' || ch == '(' || ch == '{') // 만약 왼쪽 괄호일 경우
			enqueue(ch); // 일단 큐안으로

		else if (ch == ']' || ch == ')' || ch == '}') { // 오른쪽 괄호를 만날 경우
			if (is_Qempty())
				return 2; // 조건 2 위반 : 오른쪽괄호부터 나와버림

			prev = dequeue(); // 스택 공백X라면 스택 맨 위 요소인 괄호 prev에 대입 

			if ((ch == '[' && prev != ']')

				|| (ch == '(' && prev != ')')

				|| (ch == '{' && prev != '}')) { // 만약 괄호의 쌍이 맞지 않는다면

				return 3; // 조건 3 위반
			}
		}
	}
}

 /*큐를 이용하여 후위표기수식으로 만드는 함수*/
void Qinfix_to_postfix(char expr[], char postexpr[])
{
	int i = 0, j = 0;
	char op; // op에는 큐에 저장될 연산자가 pop되어 대입되어 우선순위가 비교된다.

	init_queue();

	while (expr[i] != '\0') { // 중위표기수식 문자열의 요소를 전부 받는다. 모든 경우(if, else if, else)에 i++가 포함되어 있으므로 반복문을 종료시킬 수 있다.
		if (expr[i] >= '0' && expr[i] <= '9') // 만약 해당 요소가 숫자라면
		{
			do
				postexpr[j++] = expr[i++]; // 일단 postexpr[]문자열에 저장, 
			while (expr[i] >= '0' && expr[i] <= '9'); // 근데 숫자 뒤에 숫자가 오면 2자리 수 이상의 수가 되므로 다시 postexor[]문자열에 저장한다. 
			postexpr[j++] = ' '; // 피연산자 각각을 구분하기 위한 공백 입력
		}

		else if (expr[i] == '.') // 만약 해당 요소가 '.'이라면 이는 숫자 뒤에 들어간'.'의(ex. 2.4의 '.'의 선행 요소는 숫자 2) 실수(소수) 표현을 의미하므로
		{
			j--; // 공백으로 구분되어 버린 postexpr[]안에 들어간 선행 요소인 숫자가 사실은 2.4와 같이 하나의 피연산자이므로 공백 제거 위해 j--;
			postexpr[j++] = '.'; // 공백 자리에 '.'을 넣어준다
			i++;
		}

		else if (expr[i] == '(')
			enqueue(expr[i++]);// expr[i]의 값이 왼쪽괄호 '('일 경우 스택 안에 일단 저장

		else if (expr[i] == ')') {
			while (is_Qempty() == 0) {
				op = dequeue(); // expr[i]의 값이 오른쪽괄호 ')'일 경우 큐 안에 저장되어 있는 값을 dequeue 후 op에 대입

				if (op == '(') break; // 계속 스택에서 빼면서 스택 안에 먼저 들어가 있었던 '('가 나올 경우 멈춤 () 짝을 찾았기 때문.

				else
				{
					postexpr[j++] = op; // '('연산자 나올 때까지 op에 대입된 값을 postexpr에 저장(그대로 출력되는 것과 같은 의미)
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
				}
			}
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // char형 변수 c에 대입된 값이 사칙연산자일 때
			while (is_Qempty() == 0) {

				op = Qpeek(); // 스택 안에 저장되어 있는 값을 peek 후 op에 대입(top 값은 유지되도록)

				if (precedence(expr[i]) <= precedence(op)) {
					postexpr[j++] = op; // op에 대입된 연산자(스택 안)가 expr[i]의 연산자보다 우선순위 높을 경우 postexpr에 저장
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
					dequeue(); 
				}
				else break;
			}
			enqueue(expr[i++]); // expr[]문자열에서 읽어들인 사칙연산자는 스택안으로 저장되어 계속 비교되어 결국 postexpr[j]에 대입되도록 함
		}
		else
			i++; // 모든 경우에 i++을 함으로써 반복문에서 무한루프가 나지 않도록 한다.
	}

	while (is_Qempty() == 0) // 큐에 들어가 있던 모든 연산자들을 스택에서 뺀다.
	{
		postexpr[j++] = dequeue();
		postexpr[j++] = ' '; 
	}
	postexpr[j - 1] = '\0'; // 후위표기수식 맨 마지막에 '\0' 추가하여 문자열의 끝을 알림 마지막에 j++이 되었으므로 j-1번째가 마지막이 됨.
}

double Qcalc_postfix(char expr[])
{
	int i = 0;

	double val, val1, val2; // 피연산자값 소수이므로, 결과가 소수일 수 있으므로 double형 선언

	init_queue();

	while (expr[i] != '\0') {
		if (expr[i] >= '0' && expr[i] <= '9') { // 0보다크고 9보다 작은 피연산자일경우
			val = 0; // 만약 두자릿수 이상이 나올 때를 대비하여 0으로 초기화
			do {
				val = val * 10 + expr[i] - '0'; // 처음 do while 반복을 돌아갈 때는 해당 문자 값이 double형 그대로 저장, do문이 실행되는 횟수의 자릿수가 된다.
				i++;
			} while (expr[i] >= '0' && expr[i] <= '9'); // 다음에도 숫자가 나오는 경우 n자리만큼 10씩 곱해주어 자릿수맞춤
			val1 = 1; // 만약 소수값이 나올 경우를 대비하여 1로 초기화
			if (expr[i] == '.') {
				i++; // '.'이 나온 이후의 값은 무조건 소숫점 이하이므로 다음 요소가 소숫점 처리될 수 있도록 다음요소 탐색 위해 i++;
				do {
					val1 *= 0.1; // do문이 돌아갈 때마다 10의 마이너스 n승씩 늘어감
					val = val + (expr[i] - '0') * val1; // '.'이 나온 이후의값을 소숫점으로 넘김. do문이 실행되는 횟수가 소숫점 n자릿수가 된다.
					i++;
				} while (expr[i] >= '0' && expr[i] <= '9');
			}
			enqueue(val); // n자릿수, 소숫점 n자릿수 처리를 모두 해준 피연산자 값을 스택에 push
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // 연산자일 경우
			val2 = dequeue(); // 스택 맨 위 피연산자 dequeue
			val1 = dequeue(); // 그 다음 피연산자 dequeue
			switch (expr[i]) { // 하여 각 연산자의 경우를 스택안에 enqueue
			case '+': enqueue(val1 + val2); break;
			case '-': enqueue(val1 - val2); break;
			case '*': enqueue(val1 * val2); break;
			case '/': enqueue(val1 / val2); break;
			}
			i++;
		}
		else // 만약 피연산자와 연산자를 구분한 공백을 만났을 경우에는 바로 다음요소로 넘어가 탐색되도록 한다.
			i++;
	}
	return dequeue(); // 결국에 마지막에 dequeue()되는 값은 수식의 값이 되므로 return
}

/*error 출력 함수*/
void error(char* str)
{
	printf("%s\n", str);
	exit(1);
}

/*스택 초기화 함수*/
void init_stack() { top = NULL; }

/*스택 공백 확인 함수, 스택 맨 위가 NULL이면 공백이므로 안됨(이 경우 1 반환)*/
int is_empty() { return top == NULL; }

/*스택 포화 확인 함수, 배열 이용시에는 필요하지만 포인터 사용 시 필요X*/
// int is_full() { return top == MAX_STACK_SIZE - 1; }

/*스택 안에 요소가 얼마나 있는지 확인*/
int size() {
	Node* p; 
	int count = 0;
	for (p = top; p != NULL; p = p->link) // p가 top부터 시작해서 스택 하단까지 가면서 다음 노드를 가리킬때마다 count 1씩 증가 및 NULL값(스택 맨 위) 반환 시 반복문 stop
		count++;
	return count; // 다음 노드를 가리킨 횟수 = 스택의 크기 return
}

/*스택에 요소 push*/
void push(Element e) // 요소 e를 인자로 받아 스택에 push
{
	Node* p = (Node*)malloc(sizeof(Node)); // 새로운 데이터를 push할때 필요한 새로운 노드를 동적 할당, 이 때 포인터 자료형을 맞추어주면서 Node의 크기로 메모리 할당
	
	if (p == NULL) exit(1); // 만약 p가 NULL을 가리킬 경우 종료(NULL 값을 가리키는 포인터에 data 집어넣을 시 오류 발생)

	p->data = e; // data영역에 함수의 인수로 받은 수식 연산자 or 피연산자인 e push
	p->link = top; // 스택이므로 push된 노드가 스택의 top이 되므로 link값에 top값 대입
	top = p; // top이 원래 가리키던 이전 top을 push된 새로운 top값으로 대입
}

/*스택에 요소 pop*/
Element pop()
{
	if (is_empty()) // 만약 공백상태라면 스택 공백 에러
		error("스택 공백 에러\n");

	Node* p;
	Element e; // 반환받을 값인 e 선언

	p = top; // pop 할 때 top값 한칸 밑으로 내려가기 전에 p에 대입(top값이 변경되어야 하므로) 
	top = p->link; // top이 새롭게 top이 될 요소를 가리키도록 함
	e = p->data; // data값은 e가 됨
	free(p); // p는 잠시동안 사용했으므로 메모리 해제
	return e;
}

/*스택에 요소 peek(pop과 달리 스택 맨위 요소 유지)*/
Element peek()
{
	if (is_empty()) // 만약 공백상태라면 스택 공백 에러
		error("스택 공백 에러\n");

	return top->data; // 스택 맨 위 값 반환만 시킴
}

/*스택 없애는 함수*/
void destroy_stack() { while (is_empty() == 0) pop(); } // 공백 상태가 될 때까지 pop()하면 pop() 함수 안에는 free()되어있기 때문에 스택이 없어짐

/*수식에서 띄어쓰기 빼주는 함수 (파일에서 읽어올 수식에 공백이 무분별하게 있을 경우 공백은 피연산자, 연산자 모두에 해당하지 않으므로 방해가 된다.) */
void remove_blank(char expr[]) // 인수로 띄어쓰기를 없앨 문자열을 받는다.
{
	int i = 0;

	for (i = 0; expr[i]; i++) // 인수로 받을 문자열인 expr의 크기를 모르므로 
	{
		if (expr[i] == '\0') // 문자열의 끝인 '\0'를 만나면 break
			break;
		else if (isspace(expr[i])) // ctype.h에 정의되어 있는 isspace()함수를 통해 expr의 각 요소가 공백인지를 확인하고 공백이면 공백 뒤의 값을 앞으로 한 칸씩 당겨옴
			strcpy(&expr[i], &expr[i + 1]);// string.h에 정의되어 있는 strcpy()함수 이용 / 함수의 인자는 각각 i번째, i+1번째 요소로 시작하는 문자열 이름(= 주소)을 넣어줌으로써 공백 이후의 요소를 땡겨온다.
	}
}

/*괄호검사 함수, 인수로 문자열을 받아 조건에 따라 괄호에 오류가 없으면 0반환, 오류 발생 시 조건에 따른 int형 숫자 반환*/
int check_matching(char expr[])
{
	int i = 0, prev; // 함수의 인수 expr[]문자열의 요소 하나하나 읽기 위해 int형 변수 i=0 초기화
	char ch; // 요소 대입용 char 변수 ch선언

	init_stack();

	while (expr[i] != '\0') { // 함수의 인수 expr[]문자열의 요소 끝까지 읽을때까지 반복 
		ch = expr[i++]; // ch에 expr문자열 요소 대입 후 후증가로 expr문자열 요소 모두 읽기

		if (ch == '[' || ch == '(' || ch == '{') // 만약 왼쪽 괄호일 경우
			push(ch); // 일단 스택안으로

		else if (ch == ']' || ch == ')' || ch == '}') { // 오른쪽 괄호를 만날 경우
			if (is_empty())
				return 2; // 조건 2 위반 : 오른쪽괄호부터 나와버림

			prev = pop(); // 스택 공백X라면 스택 맨 위 요소인 괄호 prev에 대입 

			if ((ch == '[' && prev != ']')

				|| (ch == '(' && prev != ')')

				|| (ch == '{' && prev != '}')) { // 만약 괄호의 쌍이 맞지 않는다면

				return 3; // 조건 3 위반
			}
		}
	}

	if (is_empty() == 0) return 1; // 스택 공백상태가 아니라면 조건 1 위반(괄호 쌍이 모두 지어지지 않음)

	return 0; // 조건 1, 2, 3 모두 충족 시 오류 없으므로 0 반환
}

/*연산자 우선순위를 반환하는 함수로 반환되는 숫자가 크면 우선순위 높음*/
int precedence(char op)
{
	switch (op) {
	case '(': case ')': return 0;

	case '+': case '-': return 1;

	case '*': case '/': return 2;
	}

	return -1;
}

/*중위표기수식 -> 후위표기수식으로 변환하는 함수
	중위표기수식 문자열 expr[]과 변환된 후위표기수식 postexpr[] 문자열을 인수로 받음
	expr[] 문자열 요소를 모두 스캔하여 후위표기수식에 맞추어 postexpr[]에 요소가 하나씩 들어가도록 함
	피연산자(숫자)를 만날 시 바로 후위표기수식 postexpr[]문자열에 저장되도록 함
	숫자가 2자리수 이상인 경우와, 실수인 경우(소수인 경우)가 있을 수 있으므로, 각 연산자와 피연산자는 공백으로써 구분될 수 있도록 한다.
	<이 때, 후위표기수식의 각 연산자와 피연산자는 무조건 공백으로 구분된다!>
	연산자(괄호)를 만날 시 먼저 계산해야 하므로 왼쪽 괄호라면 스택에 저장하고, 오른쪽 괄호라면 짝이 맞는 왼쪽 연산자를 찾을 때까지 스택에서 먼저 들어가있던 사칙연산자를 pop하여 후위표기수식 postexpr[]문자열에 들어갈 수 있도록 함.
	연산자(사칙연산자)를 만날 시 스택 안에 들어가있는 연산자를 유지하면서 비교하기 위해 peek한다.
	precedence()함수를 사용하여 스택 안에 들어가있던 연산자와 중위표기수식에서 만난 연산자 간의 우선순위를 비교한다.
	만약 스택 안에 먼저 들어가있던 연산자가 우선순위가 높거나 같다면 postexpr[]문자열에 들어갈 수 있도록 하고, 이 값을 pop하여 스택에서 없앤다.
	만약 스택 안에 먼저 들어가있던 연산자의 우선순위가 낮다면 스택안으로 사칙연산자가 들어가도록 함.
	그 이후 스택에 들어가있던 연산자를 모두 postexpr[]문자열로 들어가도록 한다.
*/
void infix_to_postfix(char expr[], char postexpr[]) 
{
	int i = 0, j = 0;
	char op; // op에는 스택에 저장될 연산자가 pop되어 대입되어 우선순위가 비교된다.

	init_stack();

	while (expr[i] != '\0') { // 중위표기수식 문자열의 요소를 전부 받는다. 모든 경우(if, else if, else)에 i++가 포함되어 있으므로 반복문을 종료시킬 수 있다.
		if (expr[i] >= '0' && expr[i] <= '9') // 만약 해당 요소가 숫자라면
		{
			do
				postexpr[j++] = expr[i++]; // 일단 postexpr[]문자열에 저장, 
			while (expr[i] >= '0' && expr[i] <= '9'); // 근데 숫자 뒤에 숫자가 오면 2자리 수 이상의 수가 되므로 다시 postexor[]문자열에 저장한다. 
			postexpr[j++] = ' '; // 피연산자 각각을 구분하기 위한 공백 입력
		}

		else if (expr[i] == '.') // 만약 해당 요소가 '.'이라면 이는 숫자 뒤에 들어간'.'의(ex. 2.4의 '.'의 선행 요소는 숫자 2) 실수(소수) 표현을 의미하므로
		{
			j--; // 공백으로 구분되어 버린 postexpr[]안에 들어간 선행 요소인 숫자가 사실은 2.4와 같이 하나의 피연산자이므로 공백 제거 위해 j--;
			postexpr[j++] = '.'; // 공백 자리에 '.'을 넣어준다
			i++;
		}

		else if (expr[i] == '(')
			push(expr[i++]);// expr[i]의 값이 왼쪽괄호 '('일 경우 스택 안에 일단 저장

		else if (expr[i] == ')') {
			while (is_empty() == 0) {
				op = pop(); // expr[i]의 값이 오른쪽괄호 ')'일 경우 스택 안에 저장되어 있는 값을 pop 후 op에 대입

				if (op == '(') break; // 계속 스택에서 빼면서 스택 안에 먼저 들어가 있었던 '('가 나올 경우 멈춤 () 짝을 찾았기 때문.

				else
				{
					postexpr[j++] = op; // '('연산자 나올 때까지 op에 대입된 값을(스택에서 pop된 값을) postexpr에 저장(그대로 출력되는 것과 같은 의미)
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
				}
			}
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // char형 변수 c에 대입된 값이 사칙연산자일 때
			while (is_empty() == 0) {

				op = peek(); // 스택 안에 저장되어 있는 값을 peek 후 op에 대입(top 값은 유지되도록)

				if (precedence(expr[i]) <= precedence(op)) {
					postexpr[j++] = op; // op에 대입된 연산자(스택 안)가 expr[i]의 연산자보다 우선순위 높을 경우 postexpr에 저장
					postexpr[j++] = ' '; // 공백으로 각각의 (피)연산자 구분
					pop(); // 이후 pop하여 스택 top에서 op 제거
				}
				else break; 
			}
			push(expr[i++]); // expr[]문자열에서 읽어들인 사칙연산자는 스택안으로 저장되어 계속 비교되어 결국 postexpr[j]에 대입되도록 함
		}
		else
			i++; // 모든 경우에 i++을 함으로써 반복문에서 무한루프가 나지 않도록 한다.
	}

	while (is_empty() == 0) // 스택에 들어가 있던 모든 연산자들을 스택에서 뺀다.
	{
		postexpr[j++] = pop(); // 수업시간에 코딩한 예제에서 바로 메인함수에서 출력할 때 사용한 printf("%c ", pop());과 같은 의미이며,
		postexpr[j++] = ' '; // 공백을 통해 구분되도록 한다
	}
	postexpr[j-1] = '\0'; // 후위표기수식 맨 마지막에 '\0' 추가하여 문자열의 끝을 알림 마지막에 j++이 되었으므로 j-1번째가 마지막이 됨.
}

/*후위표기수식 계산 함수
	함수의 인수로 받은 문자열 expr[]에 담긴 후위표기수식을 왼쪽에서 오른쪽으로 스캔
	피연산자 만날 시 바로 스택 저장, 연산자 만날 시 pop 두 번(연산자 하나에 대응되는 피연산자 2개)
	연산한 후 스택에 다시 push하여 스택에 연산결과 넣음
	문자열 expr[]을 모두 읽어 연산이 종료되면 그 값 스택에 push하여 스택에는 연산값만
	연산값 pop하여 반환시킴
*/
double calc_postfix(char expr[])
{
	int i = 0;

	double val, val1, val2; // 피연산자값 소수이므로, 결과가 소수일 수 있으므로 double형 선언

	init_stack();

	while (expr[i] != '\0') {
		if (expr[i] >= '0' && expr[i] <= '9') { // 0보다크고 9보다 작은 피연산자일경우
			val = 0; // 만약 두자릿수 이상이 나올 때를 대비하여 0으로 초기화
			do {
				val = val * 10 + expr[i] - '0'; // 처음 do while 반복을 돌아갈 때는 해당 문자 값이 double형 그대로 저장, do문이 실행되는 횟수의 자릿수가 된다.
				i++;
			} while (expr[i] >= '0' && expr[i] <= '9'); // 다음에도 숫자가 나오는 경우 n자리만큼 10씩 곱해주어 자릿수맞춤
			val1 = 1; // 만약 소수값이 나올 경우를 대비하여 1로 초기화
			if (expr[i] == '.') {
				i++; // '.'이 나온 이후의 값은 무조건 소숫점 이하이므로 다음 요소가 소숫점 처리될 수 있도록 다음요소 탐색 위해 i++;
				do {
					val1 *= 0.1; // do문이 돌아갈 때마다 10의 마이너스 n승씩 늘어감
					val = val + (expr[i] - '0') * val1; // '.'이 나온 이후의값을 소숫점으로 넘김. do문이 실행되는 횟수가 소숫점 n자릿수가 된다.
					i++;
				} while (expr[i] >= '0' && expr[i] <= '9');
			}
			push(val); // n자릿수, 소숫점 n자릿수 처리를 모두 해준 피연산자 값을 스택에 push
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // 연산자일 경우
			val2 = pop(); // 스택 맨 위 피연산자 pop
			val1 = pop(); // 그 다음 피연산자 pop
			switch (expr[i]) { // 하여 각 연산자의 경우를 스택안에 push
			case '+': push(val1 + val2); break;
			case '-': push(val1 - val2); break;
			case '*': push(val1 * val2); break;
			case '/': push(val1 / val2); break;
			}
			i++;
		}
		else // 만약 피연산자와 연산자를 구분한 공백을 만났을 경우에는 바로 다음요소로 넘어가 탐색되도록 한다.
			i++;
	}
	return pop(); // 결국에 마지막에 pop()되는 값은 수식의 값이 되므로 return
}

int main(int argc, char* argv[]) // main함수에 파라미터 전달 / argc = 전달할 정보의 개수, argv = 메인함수에 전달되는 실질적인 정보
{
	FILE* fp; // 파일 포인터 선언 및 NULL 초기화
	char** expr; // 수식을 얼마나 입력받을지 모르므로 char형 이중포인터 변수 expr 선언
	char** postexpr; // 후위표기수식이 담길 문자열 postexpr[80]선언 후, 널 문자 넣어줌으로써 초기화
	int i, check, j = 0;;
	double val;

	if ((argc != 3)) // 메인함수에 전달할 정보가 3개가 아니라면(실행파일명, 파일명, 체크값)
	{
		printf("exec equation check(0 or 1) \n");
		exit(1);
	}

	if((fp = fopen(argv[1], "rt"))==NULL) // 만약 argv에 입력된 두 번째 요소인 파일명으로 파일이 열리지 않았을 경우 오류 메시지 출력 및 왜 오류가 떴는지 출력
	{
		printf("파일 열기 실패\n");
		printf("error = %d\n", errno); // errno.h에 정의된 errno 사용하여 왜 오류가 발생했는지 출력, 초기 fopen시 오류 발생하여 구글링을 통해 추가한 코드
		exit(1); // 파일 열기 실패시 프로그램 종료 <stdlib.h>에 정의
	}

	check = atoi(argv[2]); // 큐를 선택할 것인지, 스택을 선택할 것인지 새 번 째로 입력받은 값을 int형 형변환 

	expr = (char**)malloc(sizeof(char*) * 10); // 파일 내 전체 수식을 저장할 expr
	for (i = 0; i < 10; i++) 
		expr[i] = (char*)malloc(sizeof(char) * 256);  // 파일 내 수식 한줄씩 저장
	
	postexpr = (char**)malloc(sizeof(char*) * 10); // 후위 표기 수식을 저장할 expr
	for (i = 0; i < 10; i++) 
		postexpr[i] = (char*)malloc(sizeof(char) * 256); // 후위 표기 수식 파일에서 읽어온 한줄씩 저장

	i = 0; // 반복문을 위한 i=0; 초기화
	while((fgets(expr[i], 256, fp)) != NULL) // 한 줄 씩 파일로부터 읽어옴
	{
		remove_blank(expr[i]); // 공백제거 된 값이 expr[i]에 한줄씩 들어오도록
		i++;
	}


	if (check == 1) // 1(스택으로 계산하기) 옵션이라면 스택으로 계산
	{
		for (j = i - 1; j >= 0; j--)
		{
			if (!check_matching(expr[j])) // 수식이 괄호검사를 통과하면
			{
				init_stack(); // 스택 초기화 후
				infix_to_postfix(expr[j], postexpr[j]); // 후위표기수식으로 변환하여
				printf(" Result = %lf\n", calc_postfix(postexpr[j])); // 계산된 값 출력
			}
			else
				printf("Checking Braces is failed \n");
		}
		// 스택 destroy하여 메모리 확보
		destroy_stack();
	}
	else if (check == 0)
	{
		for (j = i - 1; j >= 0; j--)
		{
			if (!Qcheck_matching(expr[j]))
			{
				init_queue();
				Qinfix_to_postfix(expr[j], postexpr[j]);
				printf(" Result = %lf\n", Qcalc_postfix(postexpr[j]));
			}
			else
				printf("Checking Braces is failed \n");
		}
		destroy_queue();
	}

	// 메모리 할당 해제
	for (i = 0; i < 10; i++)
	{
		free(expr[i]);
		free(postexpr[i]);
	}
	free(expr);
	free(postexpr);

	//파일 닫기
	fclose(fp);

	return 0;
}