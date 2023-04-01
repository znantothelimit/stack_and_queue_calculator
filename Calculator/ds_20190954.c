#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>

typedef double Element; // ������ ��� �ڷ��� ��ȯ �����ϵ��� Element ����

typedef struct LinkedNode { // ���Ḯ��Ʈ�� ��� ����
	Element data; // data����(���Ŀ� ����ִ� �ǿ����� or ������ ����)
	struct LinkedNode* link; // ���� ��Ҹ� ����Ű�� ������ link ����
}Node;

Node* top = NULL; // ���� �� �� ��� ������ ����(���� ��ġ) �� NULL �ʱ�ȭ (�������̱� ����)
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
		Qerror("ť ���� ����\n");

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
		Qerror("ť ���� ����\n");
	return front->data;
}

void destroy_queue()
{
	while (is_Qempty() == 0)
		dequeue();
}

int Qcheck_matching(char expr[])
{
	int i = 0, prev; // �Լ��� �μ� expr[]���ڿ��� ��� �ϳ��ϳ� �б� ���� int�� ���� i=0 �ʱ�ȭ
	char ch; // ��� ���Կ� char ���� ch����

	init_queue();

	while (expr[i] != '\0') { // �Լ��� �μ� expr[]���ڿ��� ��� ������ ���������� �ݺ� 
		ch = expr[i++]; // ch�� expr���ڿ� ��� ���� �� �������� expr���ڿ� ��� ��� �б�

		if (ch == '[' || ch == '(' || ch == '{') // ���� ���� ��ȣ�� ���
			enqueue(ch); // �ϴ� ť������

		else if (ch == ']' || ch == ')' || ch == '}') { // ������ ��ȣ�� ���� ���
			if (is_Qempty())
				return 2; // ���� 2 ���� : �����ʰ�ȣ���� ���͹���

			prev = dequeue(); // ���� ����X��� ���� �� �� ����� ��ȣ prev�� ���� 

			if ((ch == '[' && prev != ']')

				|| (ch == '(' && prev != ')')

				|| (ch == '{' && prev != '}')) { // ���� ��ȣ�� ���� ���� �ʴ´ٸ�

				return 3; // ���� 3 ����
			}
		}
	}
}

 /*ť�� �̿��Ͽ� ����ǥ��������� ����� �Լ�*/
void Qinfix_to_postfix(char expr[], char postexpr[])
{
	int i = 0, j = 0;
	char op; // op���� ť�� ����� �����ڰ� pop�Ǿ� ���ԵǾ� �켱������ �񱳵ȴ�.

	init_queue();

	while (expr[i] != '\0') { // ����ǥ����� ���ڿ��� ��Ҹ� ���� �޴´�. ��� ���(if, else if, else)�� i++�� ���ԵǾ� �����Ƿ� �ݺ����� �����ų �� �ִ�.
		if (expr[i] >= '0' && expr[i] <= '9') // ���� �ش� ��Ұ� ���ڶ��
		{
			do
				postexpr[j++] = expr[i++]; // �ϴ� postexpr[]���ڿ��� ����, 
			while (expr[i] >= '0' && expr[i] <= '9'); // �ٵ� ���� �ڿ� ���ڰ� ���� 2�ڸ� �� �̻��� ���� �ǹǷ� �ٽ� postexor[]���ڿ��� �����Ѵ�. 
			postexpr[j++] = ' '; // �ǿ����� ������ �����ϱ� ���� ���� �Է�
		}

		else if (expr[i] == '.') // ���� �ش� ��Ұ� '.'�̶�� �̴� ���� �ڿ� ��'.'��(ex. 2.4�� '.'�� ���� ��Ҵ� ���� 2) �Ǽ�(�Ҽ�) ǥ���� �ǹ��ϹǷ�
		{
			j--; // �������� ���еǾ� ���� postexpr[]�ȿ� �� ���� ����� ���ڰ� ����� 2.4�� ���� �ϳ��� �ǿ������̹Ƿ� ���� ���� ���� j--;
			postexpr[j++] = '.'; // ���� �ڸ��� '.'�� �־��ش�
			i++;
		}

		else if (expr[i] == '(')
			enqueue(expr[i++]);// expr[i]�� ���� ���ʰ�ȣ '('�� ��� ���� �ȿ� �ϴ� ����

		else if (expr[i] == ')') {
			while (is_Qempty() == 0) {
				op = dequeue(); // expr[i]�� ���� �����ʰ�ȣ ')'�� ��� ť �ȿ� ����Ǿ� �ִ� ���� dequeue �� op�� ����

				if (op == '(') break; // ��� ���ÿ��� ���鼭 ���� �ȿ� ���� �� �־��� '('�� ���� ��� ���� () ¦�� ã�ұ� ����.

				else
				{
					postexpr[j++] = op; // '('������ ���� ������ op�� ���Ե� ���� postexpr�� ����(�״�� ��µǴ� �Ͱ� ���� �ǹ�)
					postexpr[j++] = ' '; // �������� ������ (��)������ ����
				}
			}
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // char�� ���� c�� ���Ե� ���� ��Ģ�������� ��
			while (is_Qempty() == 0) {

				op = Qpeek(); // ���� �ȿ� ����Ǿ� �ִ� ���� peek �� op�� ����(top ���� �����ǵ���)

				if (precedence(expr[i]) <= precedence(op)) {
					postexpr[j++] = op; // op�� ���Ե� ������(���� ��)�� expr[i]�� �����ں��� �켱���� ���� ��� postexpr�� ����
					postexpr[j++] = ' '; // �������� ������ (��)������ ����
					dequeue(); 
				}
				else break;
			}
			enqueue(expr[i++]); // expr[]���ڿ����� �о���� ��Ģ�����ڴ� ���þ����� ����Ǿ� ��� �񱳵Ǿ� �ᱹ postexpr[j]�� ���Եǵ��� ��
		}
		else
			i++; // ��� ��쿡 i++�� �����ν� �ݺ������� ���ѷ����� ���� �ʵ��� �Ѵ�.
	}

	while (is_Qempty() == 0) // ť�� �� �ִ� ��� �����ڵ��� ���ÿ��� ����.
	{
		postexpr[j++] = dequeue();
		postexpr[j++] = ' '; 
	}
	postexpr[j - 1] = '\0'; // ����ǥ����� �� �������� '\0' �߰��Ͽ� ���ڿ��� ���� �˸� �������� j++�� �Ǿ����Ƿ� j-1��°�� �������� ��.
}

double Qcalc_postfix(char expr[])
{
	int i = 0;

	double val, val1, val2; // �ǿ����ڰ� �Ҽ��̹Ƿ�, ����� �Ҽ��� �� �����Ƿ� double�� ����

	init_queue();

	while (expr[i] != '\0') {
		if (expr[i] >= '0' && expr[i] <= '9') { // 0����ũ�� 9���� ���� �ǿ������ϰ��
			val = 0; // ���� ���ڸ��� �̻��� ���� ���� ����Ͽ� 0���� �ʱ�ȭ
			do {
				val = val * 10 + expr[i] - '0'; // ó�� do while �ݺ��� ���ư� ���� �ش� ���� ���� double�� �״�� ����, do���� ����Ǵ� Ƚ���� �ڸ����� �ȴ�.
				i++;
			} while (expr[i] >= '0' && expr[i] <= '9'); // �������� ���ڰ� ������ ��� n�ڸ���ŭ 10�� �����־� �ڸ�������
			val1 = 1; // ���� �Ҽ����� ���� ��츦 ����Ͽ� 1�� �ʱ�ȭ
			if (expr[i] == '.') {
				i++; // '.'�� ���� ������ ���� ������ �Ҽ��� �����̹Ƿ� ���� ��Ұ� �Ҽ��� ó���� �� �ֵ��� ������� Ž�� ���� i++;
				do {
					val1 *= 0.1; // do���� ���ư� ������ 10�� ���̳ʽ� n�¾� �þ
					val = val + (expr[i] - '0') * val1; // '.'�� ���� �����ǰ��� �Ҽ������� �ѱ�. do���� ����Ǵ� Ƚ���� �Ҽ��� n�ڸ����� �ȴ�.
					i++;
				} while (expr[i] >= '0' && expr[i] <= '9');
			}
			enqueue(val); // n�ڸ���, �Ҽ��� n�ڸ��� ó���� ��� ���� �ǿ����� ���� ���ÿ� push
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // �������� ���
			val2 = dequeue(); // ���� �� �� �ǿ����� dequeue
			val1 = dequeue(); // �� ���� �ǿ����� dequeue
			switch (expr[i]) { // �Ͽ� �� �������� ��츦 ���þȿ� enqueue
			case '+': enqueue(val1 + val2); break;
			case '-': enqueue(val1 - val2); break;
			case '*': enqueue(val1 * val2); break;
			case '/': enqueue(val1 / val2); break;
			}
			i++;
		}
		else // ���� �ǿ����ڿ� �����ڸ� ������ ������ ������ ��쿡�� �ٷ� ������ҷ� �Ѿ Ž���ǵ��� �Ѵ�.
			i++;
	}
	return dequeue(); // �ᱹ�� �������� dequeue()�Ǵ� ���� ������ ���� �ǹǷ� return
}

/*error ��� �Լ�*/
void error(char* str)
{
	printf("%s\n", str);
	exit(1);
}

/*���� �ʱ�ȭ �Լ�*/
void init_stack() { top = NULL; }

/*���� ���� Ȯ�� �Լ�, ���� �� ���� NULL�̸� �����̹Ƿ� �ȵ�(�� ��� 1 ��ȯ)*/
int is_empty() { return top == NULL; }

/*���� ��ȭ Ȯ�� �Լ�, �迭 �̿�ÿ��� �ʿ������� ������ ��� �� �ʿ�X*/
// int is_full() { return top == MAX_STACK_SIZE - 1; }

/*���� �ȿ� ��Ұ� �󸶳� �ִ��� Ȯ��*/
int size() {
	Node* p; 
	int count = 0;
	for (p = top; p != NULL; p = p->link) // p�� top���� �����ؼ� ���� �ϴܱ��� ���鼭 ���� ��带 ����ų������ count 1�� ���� �� NULL��(���� �� ��) ��ȯ �� �ݺ��� stop
		count++;
	return count; // ���� ��带 ����Ų Ƚ�� = ������ ũ�� return
}

/*���ÿ� ��� push*/
void push(Element e) // ��� e�� ���ڷ� �޾� ���ÿ� push
{
	Node* p = (Node*)malloc(sizeof(Node)); // ���ο� �����͸� push�Ҷ� �ʿ��� ���ο� ��带 ���� �Ҵ�, �� �� ������ �ڷ����� ���߾��ָ鼭 Node�� ũ��� �޸� �Ҵ�
	
	if (p == NULL) exit(1); // ���� p�� NULL�� ����ų ��� ����(NULL ���� ����Ű�� �����Ϳ� data ������� �� ���� �߻�)

	p->data = e; // data������ �Լ��� �μ��� ���� ���� ������ or �ǿ������� e push
	p->link = top; // �����̹Ƿ� push�� ��尡 ������ top�� �ǹǷ� link���� top�� ����
	top = p; // top�� ���� ����Ű�� ���� top�� push�� ���ο� top������ ����
}

/*���ÿ� ��� pop*/
Element pop()
{
	if (is_empty()) // ���� ������¶�� ���� ���� ����
		error("���� ���� ����\n");

	Node* p;
	Element e; // ��ȯ���� ���� e ����

	p = top; // pop �� �� top�� ��ĭ ������ �������� ���� p�� ����(top���� ����Ǿ�� �ϹǷ�) 
	top = p->link; // top�� ���Ӱ� top�� �� ��Ҹ� ����Ű���� ��
	e = p->data; // data���� e�� ��
	free(p); // p�� ��õ��� ��������Ƿ� �޸� ����
	return e;
}

/*���ÿ� ��� peek(pop�� �޸� ���� ���� ��� ����)*/
Element peek()
{
	if (is_empty()) // ���� ������¶�� ���� ���� ����
		error("���� ���� ����\n");

	return top->data; // ���� �� �� �� ��ȯ�� ��Ŵ
}

/*���� ���ִ� �Լ�*/
void destroy_stack() { while (is_empty() == 0) pop(); } // ���� ���°� �� ������ pop()�ϸ� pop() �Լ� �ȿ��� free()�Ǿ��ֱ� ������ ������ ������

/*���Ŀ��� ���� ���ִ� �Լ� (���Ͽ��� �о�� ���Ŀ� ������ ���к��ϰ� ���� ��� ������ �ǿ�����, ������ ��ο� �ش����� �����Ƿ� ���ذ� �ȴ�.) */
void remove_blank(char expr[]) // �μ��� ���⸦ ���� ���ڿ��� �޴´�.
{
	int i = 0;

	for (i = 0; expr[i]; i++) // �μ��� ���� ���ڿ��� expr�� ũ�⸦ �𸣹Ƿ� 
	{
		if (expr[i] == '\0') // ���ڿ��� ���� '\0'�� ������ break
			break;
		else if (isspace(expr[i])) // ctype.h�� ���ǵǾ� �ִ� isspace()�Լ��� ���� expr�� �� ��Ұ� ���������� Ȯ���ϰ� �����̸� ���� ���� ���� ������ �� ĭ�� ��ܿ�
			strcpy(&expr[i], &expr[i + 1]);// string.h�� ���ǵǾ� �ִ� strcpy()�Լ� �̿� / �Լ��� ���ڴ� ���� i��°, i+1��° ��ҷ� �����ϴ� ���ڿ� �̸�(= �ּ�)�� �־������ν� ���� ������ ��Ҹ� ���ܿ´�.
	}
}

/*��ȣ�˻� �Լ�, �μ��� ���ڿ��� �޾� ���ǿ� ���� ��ȣ�� ������ ������ 0��ȯ, ���� �߻� �� ���ǿ� ���� int�� ���� ��ȯ*/
int check_matching(char expr[])
{
	int i = 0, prev; // �Լ��� �μ� expr[]���ڿ��� ��� �ϳ��ϳ� �б� ���� int�� ���� i=0 �ʱ�ȭ
	char ch; // ��� ���Կ� char ���� ch����

	init_stack();

	while (expr[i] != '\0') { // �Լ��� �μ� expr[]���ڿ��� ��� ������ ���������� �ݺ� 
		ch = expr[i++]; // ch�� expr���ڿ� ��� ���� �� �������� expr���ڿ� ��� ��� �б�

		if (ch == '[' || ch == '(' || ch == '{') // ���� ���� ��ȣ�� ���
			push(ch); // �ϴ� ���þ�����

		else if (ch == ']' || ch == ')' || ch == '}') { // ������ ��ȣ�� ���� ���
			if (is_empty())
				return 2; // ���� 2 ���� : �����ʰ�ȣ���� ���͹���

			prev = pop(); // ���� ����X��� ���� �� �� ����� ��ȣ prev�� ���� 

			if ((ch == '[' && prev != ']')

				|| (ch == '(' && prev != ')')

				|| (ch == '{' && prev != '}')) { // ���� ��ȣ�� ���� ���� �ʴ´ٸ�

				return 3; // ���� 3 ����
			}
		}
	}

	if (is_empty() == 0) return 1; // ���� ������°� �ƴ϶�� ���� 1 ����(��ȣ ���� ��� �������� ����)

	return 0; // ���� 1, 2, 3 ��� ���� �� ���� �����Ƿ� 0 ��ȯ
}

/*������ �켱������ ��ȯ�ϴ� �Լ��� ��ȯ�Ǵ� ���ڰ� ũ�� �켱���� ����*/
int precedence(char op)
{
	switch (op) {
	case '(': case ')': return 0;

	case '+': case '-': return 1;

	case '*': case '/': return 2;
	}

	return -1;
}

/*����ǥ����� -> ����ǥ��������� ��ȯ�ϴ� �Լ�
	����ǥ����� ���ڿ� expr[]�� ��ȯ�� ����ǥ����� postexpr[] ���ڿ��� �μ��� ����
	expr[] ���ڿ� ��Ҹ� ��� ��ĵ�Ͽ� ����ǥ����Ŀ� ���߾� postexpr[]�� ��Ұ� �ϳ��� ������ ��
	�ǿ�����(����)�� ���� �� �ٷ� ����ǥ����� postexpr[]���ڿ��� ����ǵ��� ��
	���ڰ� 2�ڸ��� �̻��� ����, �Ǽ��� ���(�Ҽ��� ���)�� ���� �� �����Ƿ�, �� �����ڿ� �ǿ����ڴ� �������ν� ���е� �� �ֵ��� �Ѵ�.
	<�� ��, ����ǥ������� �� �����ڿ� �ǿ����ڴ� ������ �������� ���еȴ�!>
	������(��ȣ)�� ���� �� ���� ����ؾ� �ϹǷ� ���� ��ȣ��� ���ÿ� �����ϰ�, ������ ��ȣ��� ¦�� �´� ���� �����ڸ� ã�� ������ ���ÿ��� ���� ���ִ� ��Ģ�����ڸ� pop�Ͽ� ����ǥ����� postexpr[]���ڿ��� �� �� �ֵ��� ��.
	������(��Ģ������)�� ���� �� ���� �ȿ� ���ִ� �����ڸ� �����ϸ鼭 ���ϱ� ���� peek�Ѵ�.
	precedence()�Լ��� ����Ͽ� ���� �ȿ� ���ִ� �����ڿ� ����ǥ����Ŀ��� ���� ������ ���� �켱������ ���Ѵ�.
	���� ���� �ȿ� ���� ���ִ� �����ڰ� �켱������ ���ų� ���ٸ� postexpr[]���ڿ��� �� �� �ֵ��� �ϰ�, �� ���� pop�Ͽ� ���ÿ��� ���ش�.
	���� ���� �ȿ� ���� ���ִ� �������� �켱������ ���ٸ� ���þ����� ��Ģ�����ڰ� ������ ��.
	�� ���� ���ÿ� ���ִ� �����ڸ� ��� postexpr[]���ڿ��� ������ �Ѵ�.
*/
void infix_to_postfix(char expr[], char postexpr[]) 
{
	int i = 0, j = 0;
	char op; // op���� ���ÿ� ����� �����ڰ� pop�Ǿ� ���ԵǾ� �켱������ �񱳵ȴ�.

	init_stack();

	while (expr[i] != '\0') { // ����ǥ����� ���ڿ��� ��Ҹ� ���� �޴´�. ��� ���(if, else if, else)�� i++�� ���ԵǾ� �����Ƿ� �ݺ����� �����ų �� �ִ�.
		if (expr[i] >= '0' && expr[i] <= '9') // ���� �ش� ��Ұ� ���ڶ��
		{
			do
				postexpr[j++] = expr[i++]; // �ϴ� postexpr[]���ڿ��� ����, 
			while (expr[i] >= '0' && expr[i] <= '9'); // �ٵ� ���� �ڿ� ���ڰ� ���� 2�ڸ� �� �̻��� ���� �ǹǷ� �ٽ� postexor[]���ڿ��� �����Ѵ�. 
			postexpr[j++] = ' '; // �ǿ����� ������ �����ϱ� ���� ���� �Է�
		}

		else if (expr[i] == '.') // ���� �ش� ��Ұ� '.'�̶�� �̴� ���� �ڿ� ��'.'��(ex. 2.4�� '.'�� ���� ��Ҵ� ���� 2) �Ǽ�(�Ҽ�) ǥ���� �ǹ��ϹǷ�
		{
			j--; // �������� ���еǾ� ���� postexpr[]�ȿ� �� ���� ����� ���ڰ� ����� 2.4�� ���� �ϳ��� �ǿ������̹Ƿ� ���� ���� ���� j--;
			postexpr[j++] = '.'; // ���� �ڸ��� '.'�� �־��ش�
			i++;
		}

		else if (expr[i] == '(')
			push(expr[i++]);// expr[i]�� ���� ���ʰ�ȣ '('�� ��� ���� �ȿ� �ϴ� ����

		else if (expr[i] == ')') {
			while (is_empty() == 0) {
				op = pop(); // expr[i]�� ���� �����ʰ�ȣ ')'�� ��� ���� �ȿ� ����Ǿ� �ִ� ���� pop �� op�� ����

				if (op == '(') break; // ��� ���ÿ��� ���鼭 ���� �ȿ� ���� �� �־��� '('�� ���� ��� ���� () ¦�� ã�ұ� ����.

				else
				{
					postexpr[j++] = op; // '('������ ���� ������ op�� ���Ե� ����(���ÿ��� pop�� ����) postexpr�� ����(�״�� ��µǴ� �Ͱ� ���� �ǹ�)
					postexpr[j++] = ' '; // �������� ������ (��)������ ����
				}
			}
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // char�� ���� c�� ���Ե� ���� ��Ģ�������� ��
			while (is_empty() == 0) {

				op = peek(); // ���� �ȿ� ����Ǿ� �ִ� ���� peek �� op�� ����(top ���� �����ǵ���)

				if (precedence(expr[i]) <= precedence(op)) {
					postexpr[j++] = op; // op�� ���Ե� ������(���� ��)�� expr[i]�� �����ں��� �켱���� ���� ��� postexpr�� ����
					postexpr[j++] = ' '; // �������� ������ (��)������ ����
					pop(); // ���� pop�Ͽ� ���� top���� op ����
				}
				else break; 
			}
			push(expr[i++]); // expr[]���ڿ����� �о���� ��Ģ�����ڴ� ���þ����� ����Ǿ� ��� �񱳵Ǿ� �ᱹ postexpr[j]�� ���Եǵ��� ��
		}
		else
			i++; // ��� ��쿡 i++�� �����ν� �ݺ������� ���ѷ����� ���� �ʵ��� �Ѵ�.
	}

	while (is_empty() == 0) // ���ÿ� �� �ִ� ��� �����ڵ��� ���ÿ��� ����.
	{
		postexpr[j++] = pop(); // �����ð��� �ڵ��� �������� �ٷ� �����Լ����� ����� �� ����� printf("%c ", pop());�� ���� �ǹ��̸�,
		postexpr[j++] = ' '; // ������ ���� ���еǵ��� �Ѵ�
	}
	postexpr[j-1] = '\0'; // ����ǥ����� �� �������� '\0' �߰��Ͽ� ���ڿ��� ���� �˸� �������� j++�� �Ǿ����Ƿ� j-1��°�� �������� ��.
}

/*����ǥ����� ��� �Լ�
	�Լ��� �μ��� ���� ���ڿ� expr[]�� ��� ����ǥ������� ���ʿ��� ���������� ��ĵ
	�ǿ����� ���� �� �ٷ� ���� ����, ������ ���� �� pop �� ��(������ �ϳ��� �����Ǵ� �ǿ����� 2��)
	������ �� ���ÿ� �ٽ� push�Ͽ� ���ÿ� ������ ����
	���ڿ� expr[]�� ��� �о� ������ ����Ǹ� �� �� ���ÿ� push�Ͽ� ���ÿ��� ���갪��
	���갪 pop�Ͽ� ��ȯ��Ŵ
*/
double calc_postfix(char expr[])
{
	int i = 0;

	double val, val1, val2; // �ǿ����ڰ� �Ҽ��̹Ƿ�, ����� �Ҽ��� �� �����Ƿ� double�� ����

	init_stack();

	while (expr[i] != '\0') {
		if (expr[i] >= '0' && expr[i] <= '9') { // 0����ũ�� 9���� ���� �ǿ������ϰ��
			val = 0; // ���� ���ڸ��� �̻��� ���� ���� ����Ͽ� 0���� �ʱ�ȭ
			do {
				val = val * 10 + expr[i] - '0'; // ó�� do while �ݺ��� ���ư� ���� �ش� ���� ���� double�� �״�� ����, do���� ����Ǵ� Ƚ���� �ڸ����� �ȴ�.
				i++;
			} while (expr[i] >= '0' && expr[i] <= '9'); // �������� ���ڰ� ������ ��� n�ڸ���ŭ 10�� �����־� �ڸ�������
			val1 = 1; // ���� �Ҽ����� ���� ��츦 ����Ͽ� 1�� �ʱ�ȭ
			if (expr[i] == '.') {
				i++; // '.'�� ���� ������ ���� ������ �Ҽ��� �����̹Ƿ� ���� ��Ұ� �Ҽ��� ó���� �� �ֵ��� ������� Ž�� ���� i++;
				do {
					val1 *= 0.1; // do���� ���ư� ������ 10�� ���̳ʽ� n�¾� �þ
					val = val + (expr[i] - '0') * val1; // '.'�� ���� �����ǰ��� �Ҽ������� �ѱ�. do���� ����Ǵ� Ƚ���� �Ҽ��� n�ڸ����� �ȴ�.
					i++;
				} while (expr[i] >= '0' && expr[i] <= '9');
			}
			push(val); // n�ڸ���, �Ҽ��� n�ڸ��� ó���� ��� ���� �ǿ����� ���� ���ÿ� push
			i++;
		}

		else if (expr[i] == '+' || expr[i] == '-' || expr[i] == '*' || expr[i] == '/') { // �������� ���
			val2 = pop(); // ���� �� �� �ǿ����� pop
			val1 = pop(); // �� ���� �ǿ����� pop
			switch (expr[i]) { // �Ͽ� �� �������� ��츦 ���þȿ� push
			case '+': push(val1 + val2); break;
			case '-': push(val1 - val2); break;
			case '*': push(val1 * val2); break;
			case '/': push(val1 / val2); break;
			}
			i++;
		}
		else // ���� �ǿ����ڿ� �����ڸ� ������ ������ ������ ��쿡�� �ٷ� ������ҷ� �Ѿ Ž���ǵ��� �Ѵ�.
			i++;
	}
	return pop(); // �ᱹ�� �������� pop()�Ǵ� ���� ������ ���� �ǹǷ� return
}

int main(int argc, char* argv[]) // main�Լ��� �Ķ���� ���� / argc = ������ ������ ����, argv = �����Լ��� ���޵Ǵ� �������� ����
{
	FILE* fp; // ���� ������ ���� �� NULL �ʱ�ȭ
	char** expr; // ������ �󸶳� �Է¹����� �𸣹Ƿ� char�� ���������� ���� expr ����
	char** postexpr; // ����ǥ������� ��� ���ڿ� postexpr[80]���� ��, �� ���� �־������ν� �ʱ�ȭ
	int i, check, j = 0;;
	double val;

	if ((argc != 3)) // �����Լ��� ������ ������ 3���� �ƴ϶��(�������ϸ�, ���ϸ�, üũ��)
	{
		printf("exec equation check(0 or 1) \n");
		exit(1);
	}

	if((fp = fopen(argv[1], "rt"))==NULL) // ���� argv�� �Էµ� �� ��° ����� ���ϸ����� ������ ������ �ʾ��� ��� ���� �޽��� ��� �� �� ������ ������ ���
	{
		printf("���� ���� ����\n");
		printf("error = %d\n", errno); // errno.h�� ���ǵ� errno ����Ͽ� �� ������ �߻��ߴ��� ���, �ʱ� fopen�� ���� �߻��Ͽ� ���۸��� ���� �߰��� �ڵ�
		exit(1); // ���� ���� ���н� ���α׷� ���� <stdlib.h>�� ����
	}

	check = atoi(argv[2]); // ť�� ������ ������, ������ ������ ������ �� �� °�� �Է¹��� ���� int�� ����ȯ 

	expr = (char**)malloc(sizeof(char*) * 10); // ���� �� ��ü ������ ������ expr
	for (i = 0; i < 10; i++) 
		expr[i] = (char*)malloc(sizeof(char) * 256);  // ���� �� ���� ���پ� ����
	
	postexpr = (char**)malloc(sizeof(char*) * 10); // ���� ǥ�� ������ ������ expr
	for (i = 0; i < 10; i++) 
		postexpr[i] = (char*)malloc(sizeof(char) * 256); // ���� ǥ�� ���� ���Ͽ��� �о�� ���پ� ����

	i = 0; // �ݺ����� ���� i=0; �ʱ�ȭ
	while((fgets(expr[i], 256, fp)) != NULL) // �� �� �� ���Ϸκ��� �о��
	{
		remove_blank(expr[i]); // �������� �� ���� expr[i]�� ���پ� ��������
		i++;
	}


	if (check == 1) // 1(�������� ����ϱ�) �ɼ��̶�� �������� ���
	{
		for (j = i - 1; j >= 0; j--)
		{
			if (!check_matching(expr[j])) // ������ ��ȣ�˻縦 ����ϸ�
			{
				init_stack(); // ���� �ʱ�ȭ ��
				infix_to_postfix(expr[j], postexpr[j]); // ����ǥ��������� ��ȯ�Ͽ�
				printf(" Result = %lf\n", calc_postfix(postexpr[j])); // ���� �� ���
			}
			else
				printf("Checking Braces is failed \n");
		}
		// ���� destroy�Ͽ� �޸� Ȯ��
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

	// �޸� �Ҵ� ����
	for (i = 0; i < 10; i++)
	{
		free(expr[i]);
		free(postexpr[i]);
	}
	free(expr);
	free(postexpr);

	//���� �ݱ�
	fclose(fp);

	return 0;
}